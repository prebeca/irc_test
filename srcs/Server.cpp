#include "Server.hpp"

bool g_exit = false;

static void handleSignal(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
		g_exit = true;
}

/**
 * @brief Construct a new Server object
 *
 * @param name name of the server
 * @param password password to connect to the server
 * @param port listenting port of the server, delault to 6667 (IRC default port)
 */
Server::Server(std::string name, std::string password, unsigned int port)
	: exit(false), name(name), password(password), port(port), cmd_handler(new CmdHandler())
{
}

Server::~Server()
{
	stop();
	delete cmd_handler;
}

Client *Server::addUser(Client &new_user)
{
	std::pair<std::map<int, Client *>::iterator, bool> p;
	std::pair<std::map<std::string, Client *>::iterator, bool> p1;

	p = this->user_by_fd.insert(std::make_pair(new_user.getFd(), &new_user));
	if (p.second == false)
		return (NULL);
	if (!new_user.getNickname().empty())
	{
		p1 = this->user_by_nick.insert(std::make_pair(utils::strToUpper(new_user.getNickname()), &new_user));
		if (p1.second == false)
		{
			this->user_by_fd.erase(new_user.getFd());
			return (NULL);
		}
	}
	return (p.first->second);
}

int Server::registerUser(Client &user)
{
	if (user.isRegistered())
		return (1);

	this->sendMsg(user.getFd(), Message(RPL_WELCOME(user.getNickname())));
	this->sendMsg(user.getFd(), Message(RPL_YOURHOST(user.getNickname(), SERVER_NAME, VERSION)));
	this->sendMsg(user.getFd(), Message(RPL_CREATED(user.getNickname(), "today")));
	this->sendMsg(user.getFd(), Message(RPL_MYINFO(user.getNickname(), SERVER_NAME, VERSION, "<usermods>", "<chanmodes>")));

	user.setRegistered(true);
	std::cout << COLOR_GREEN << user.getNickname() << " succesfully registered !" << COLOR_RESET << std::endl;
	return (0);
}

void Server::removeUser(Client &user)
{
	this->user_by_fd.erase(user.getFd());
	if (!user.getNickname().empty())
		this->user_by_nick.erase(utils::strToUpper(user.getNickname()));
}

void Server::removeUser(int user_fd)
{
	Client *user = getUser(user_fd);
	if (user)
	{
		this->user_by_fd.erase(user->getFd());
		if (!user->getNickname().empty())
			this->user_by_nick.erase(utils::strToUpper(user->getNickname()));
	}
}

void Server::removeUser(std::string nickname)
{
	Client *user = getUser(nickname);
	if (user)
	{
		this->user_by_fd.erase(user->getFd());
		if (!user->getNickname().empty())
			this->user_by_nick.erase(utils::strToUpper(user->getNickname()));
	}
}

void Server::removeClient(Client *user, int fd_index)
{
	if (!user)
		return;

	if (fd_index < 0)
		for (fd_index = 0; (size_t)fd_index < fd_lst.size(); ++fd_index)
			if (fd_lst[fd_index].fd == user->getFd())
				break;

	fd_lst.erase(fd_lst.begin() + fd_index);
	close(user->getFd());
	this->removeUser(*user);
	delete user;
}

Client *Server::getUser(int user_fd) const
{
	std::map<int, Client *>::const_iterator it;
	it = this->user_by_fd.find(user_fd);
	if (it == this->user_by_fd.end())
		return (NULL);
	return (it->second);
}

Client *Server::getUser(std::string nickname) const
{
	std::map<std::string, Client *>::const_iterator it;
	it = this->user_by_nick.find(utils::strToUpper(nickname));
	if (it == this->user_by_nick.end())
		return (NULL);
	return (it->second);
}

Channel *Server::addChannel(std::string name)
{
	Channel *chan = new Channel(name);

	std::pair<std::map<std::string, Channel *>::iterator, bool> p;
	p = this->chan_lst.insert(std::make_pair(name, chan));
	if (p.second == false)
		return (NULL);
	return (p.first->second);
}

int Server::removeChannel(std::string name)
{
	return (this->chan_lst.erase(name));
}

Channel *Server::getChannel(std::string name)
{
	std::map<std::string, Channel *>::iterator it;
	it = this->chan_lst.find(name);
	if (it == this->chan_lst.end())
		return (NULL);
	return (it->second);
}

const std::map<std::string, Channel *> &Server::getChannels() const
{
	return (this->chan_lst);
}

std::string Server::getPassword() const
{
	return (this->password);
}

std::string Server::getName() const
{
	return (this->name);
}

unsigned int Server::getPort() const
{
	return (this->port);
}

/**
 * @brief create a socket and bind it on port
 *
 * @return return 0 or non null value if an error occured
 */
int Server::init()
{

	signal(SIGINT, handleSignal);
	signal(SIGQUIT, handleSignal);

	pollfd server_socket;
	server_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket.fd == -1)
		return (1);
	fcntl(server_socket.fd, F_SETFL, O_NONBLOCK);
	server_socket.events = POLLIN;

	fd_lst.push_back(server_socket);

	// pollfd serverCmd_input;
	// serverCmd_input.fd = STDIN_FILENO;
	// // fcntl(serverCmd_input.fd, F_SETFL, O_NONBLOCK);
	// serverCmd_input.events = POLLIN;

	// fd_lst.push_back(serverCmd_input);

	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port = htons(this->port);

	int on = 1;
	setsockopt(server_socket.fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

	if (bind(server_socket.fd, (sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
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
	this->init();

	if (listen(fd_lst.front().fd, SOMAXCONN) == -1)
	{
		std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
		return (1);
	}

	while (!g_exit)
	{
#ifdef DEBUG
		std::cout << COLOR_YELLOW << "Waiting for ready fds... " << COLOR_RESET << std::endl;
#endif // DEBUG
		int nfds_ready = poll(&fd_lst.front(), fd_lst.size(), -1);
		if (nfds_ready == -1)
		{
			std::cerr << COLOR_RED << std::strerror(errno) << COLOR_RESET << std::endl;
			return (1);
		}
		for (unsigned int i = 0; i < fd_lst.size(); ++i)
		{
			if (fd_lst[i].revents == 0)
				continue;
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

int Server::handleCmd(int user_fd, const Message &msg)
{
	Client *user = getUser(user_fd);
	if (user == NULL)
		return (1);

	const ACommand *cmd = cmd_handler->getCmd(msg.getArgv()[0]);
	if (cmd != NULL)
		return (cmd->execute(*this, *user, msg));
	return (1);
}

/**
 * @brief accept all queued connection
 */
int Server::receiveMsg(int fd, int index)
{
	Client *client;
	char buffer[BUFFER_SIZE + 1];
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
		std::cout << COLOR_GREEN << "Client quit: " << inet_ntoa(client->getSocket().sin_addr) << COLOR_RESET << std::endl;
		this->removeClient(client, index);
	}
	else
	{
		std::string str_buff(buffer);
		client->getBuffer() += str_buff;
		size_t cmd_end;
		while ((cmd_end = client->getBuffer().find("\r\n", 0)) != std::string::npos)
		{
			Message msg(client->getBuffer().substr(0, cmd_end + 2));

#ifdef DEBUG
			std::cout << COLOR_MAGENTA << "<- " << "[" << client->getNickname() << "]" << COLOR_RESET << msg.getRaw();
#endif // DEBUG

			client->getBuffer().erase(0, cmd_end + 2);

			if (handleCmd(fd, msg) == QUIT_RETURN)
				break;
		}
	}
	return (0);
}

int Server::sendMsg(int fd, const Message &msg) const
{
	send(fd, msg.getRaw().c_str(), msg.getRaw().size(), 0);
#ifdef DEBUG
	std::cout << COLOR_BLUE << "-> " << COLOR_RESET << msg.getRaw();
#endif // DEBUG
	return (0);
}

/**
 * @brief accept all queued connection
 */
int Server::acceptClient()
{
	int new_fd;
	sockaddr_in new_soket;
	socklen_t sok_len = 0;

	// accept all queued connection
	while ((new_fd = accept(fd_lst[0].fd, (sockaddr *)&new_soket, &sok_len)) != -1)
	{
		fd_lst.push_back(pollfd());
		fd_lst.back().fd = new_fd;
		fd_lst.back().events = POLLIN;

		Client *new_u = new Client(new_fd, new_soket);
		this->addUser(*new_u);

		std::cout << COLOR_GREEN << "New client: " << inet_ntoa(new_u->getSocket().sin_addr) << COLOR_RESET << std::endl;
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

	std::cout << COLOR_GREEN << "\b\bServer stopped. Good bye!\n"
			  << COLOR_RESET;

	close(this->fd_lst[0].fd);
	while (!this->user_by_fd.empty())
	{
		std::map<int, Client *>::iterator it;

		it = this->user_by_fd.begin();
		removeClient(it->second);
		this->user_by_fd.erase(it->first);
	}
	while (!this->chan_lst.empty())
	{
		std::map<std::string, Channel *>::iterator it;

		it = this->chan_lst.begin();
		delete it->second;
		this->chan_lst.erase(it->first);
	}
	return (0);
}