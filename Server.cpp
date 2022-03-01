#include "Server.hpp"

/**
 * @brief Construct a new Server object
 * 
 * @param name name of the server
 * @param password password to connect to the server
 * @param port listenting port of the server, delault to 6667 (IRC default port)
 */
Server::Server(std::string name, std::string password = "", unsigned int port = 6667)
: name(name), password(password), port(port)
{}

Server::~Server()
{
	stop();
}

/**
 * @brief setup and run the server
 * 
 * @return int 
 */
int Server::run()
{
	if (setup())
		return (1);
	if (start())
		return (1);
	return (0);
}

User*	Server::addUser(User& new_user)
{
	std::pair<std::map<int, User*>::iterator, bool> p;
	std::pair<std::map<std::string, User*>::iterator, bool> p1;

	p =	this->user_by_fd.insert(std::make_pair(new_user.fd, &new_user));
	if (p.second == false)
		return (NULL);
	p1 = this->user_by_nick.insert(std::make_pair(new_user.nickname, &new_user));
	if (p1.second == false)
	{
		this->user_by_fd.erase(new_user.fd);
		return (NULL);
	}
	return (p.first->second);
}

void	Server::removeUser(User& user)
{
	this->user_by_fd.erase(user.fd);
	this->user_by_nick.erase(user.nickname);
}

User*	Server::getUser(int user_fd) const
{
	std::map<int, User*>::const_iterator it;
	it = this->user_by_fd.find(user_fd);
	if (it == this->user_by_fd.end())
		return (NULL);
	return (it->second);
}
 
User*	Server::getUser(std::string nickname) const
{
	std::map<std::string, User*>::const_iterator it;
	it = this->user_by_nick.find(nickname);
	if (it == this->user_by_nick.end())
		return (NULL);
	return (it->second);
}
 

Channel* Server::createChannel(std::string name, User* creator)
{
	std::pair<std::map<std::string, Channel*>::iterator, bool> p;
	p = this->chan_lst.insert(std::make_pair(name, new Channel()));
	if (p.second == false)
		return (NULL);
	// TODO add creator to user list and chanop
	// p.first.addUser(creator);
	// p.first.addOpe(creator);
	return(p.first->second);
}
 
Channel* Server::getChannel(std::string name)
{
	std::map<std::string, Channel*>::iterator it;
	it = this->chan_lst.find(name);
	if (it == this->chan_lst.end())
		return (NULL);
	return (it->second);
}
 
std::string Server::getPassword() const
{
	return(this->password);
}
 
std::string Server::getName() const
{
	return(this->name);
}
 
unsigned int Server::getPort() const
{
	return(this->port);
}
 

/**
 * @brief create a socket and bind it on port
 * 
 * @return return 0 or non null value if an error occured 
 */
int Server::setup()
{

	pollfd server_socket;
	server_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket.fd == -1)
		return (1);
	fcntl(server_socket.fd, F_SETFL, O_NONBLOCK);
	server_socket.events = POLLIN;

	fd_lst.push_back(server_socket);

	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port = htons(this->port);

	int on = 1;
	setsockopt(server_socket.fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));

	if (bind(server_socket.fd, (sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
	{
		std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
		return (1);
	}

	std::cout << COLOR_GREEN << "Server is setup! Bind on " << inet_ntoa(sock_addr.sin_addr) << ":" << this->port << COLOR_RESET << std::endl;
	return (0);
}

/**
 * @brief start listening and handle packets
 * 
 * @return return 0 or non null value if an error occured 
 */
int Server::start()
{
	if (listen(fd_lst.front().fd, SOMAXCONN) == -1)
	{
		std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
		return (1);
	}

	while (true)
	{
		std::cout << COLOR_YELLOW << "Waiting for ready fds... " << COLOR_RESET << std::endl;
		int nfds_ready = poll(&fd_lst.front(), fd_lst.size(), -1);
		if (nfds_ready == -1)
		{
			std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
			return (1);
		}
		for (unsigned int i = 0; i < fd_lst.size(); ++i)
		{
			if (fd_lst[i].revents == 0)
				continue ;
			if (fd_lst[i].revents == POLLIN)
			{
				if (i == 0)
					acceptClient();
				else
					receiveMsg(fd_lst[i].fd, i);
			}
		}
	}

	return (0);
}

void Server::handleCmd(int usr_fd, const Message& msg)
{
	CmdHandler::const_iterator it = CmdHandler::cmd_lst.find(msg.getArgv()[0]);
	if (it != CmdHandler::cmd_lst.end())
		it->second(*this, usr_fd, msg);
	// else
	// 	TODO handle unknown cmd
}

/**
 * @brief accept all queued connection
 */
int Server::receiveMsg(int fd, int index)
{
	User*	client;
	char	buffer[BUFFER_SIZE + 1];
	memset(buffer, 0, BUFFER_SIZE + 1);

	client = this->getUser(fd);
	if (client == NULL)
		return (1);

	int recv_bytes = recv(fd, buffer, BUFFER_SIZE, 0);
	if (recv_bytes == -1)
	{
		std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
		return (1);
	}
	if (recv_bytes == 0)
	{
		std::cout << COLOR_GREEN << "[-> Client quit: " << inet_ntoa(client->socket.sin_addr) << COLOR_RESET << std::endl;
		close(fd);
		this->removeUser(*client);
		fd_lst.erase(fd_lst.begin() + index);
	}
	else
	{
		std::string str_buff(buffer);
		client->buffer += str_buff;
		size_t	cmd_end;
		while ((cmd_end = client->buffer.find("\r\n", 0)) != std::string::npos)
		{
			Message msg(client->buffer.substr(0, cmd_end + 2));
			
			std::cout << msg.getRaw() << std::endl;
			handleCmd(fd, msg);
			
			client->buffer.erase(0, cmd_end + 2);
		}
	}
	return (0);
}

/**
 * @brief accept all queued connection
 */
int Server::acceptClient()
{
	int			new_fd;
	sockaddr_in	new_soket;
	socklen_t	sok_len;
	User		new_u;

	// accept all queued connection
	while ((new_fd = accept(fd_lst[0].fd, (sockaddr*)&new_soket, &sok_len)) != -1)
	{
		fd_lst.push_back(pollfd());
		fd_lst.back().fd = new_fd;
		fd_lst.back().events = POLLIN;
		
		new_u.socket = new_soket;
		new_u.fd = new_fd;
		new_u.registered = false;
		this->addUser(*new User(new_u));

		std::cout << COLOR_GREEN << "[<- New client: " << inet_ntoa(new_u.socket.sin_addr) << COLOR_RESET << std::endl;
	}

	// if errno == EWOULDBLOCK it mean there is no more connection queued, else there is an error
	if (errno != EWOULDBLOCK)	
	{
		std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
		return (1);
	}

	return (0);
}

/**
 * @brief close every connection and exit
 * 
 * @return return 0 or non null value if an error occured
 */
int Server::stop()
{
	std::vector<pollfd>::iterator it;
	for (it = fd_lst.begin(); it != fd_lst.end(); ++it)
		close(it->fd);
	return (0);
}