#include "../inc/Server.hpp"

bool Server::_signal = false;

/*
** ------------------------------- CONSTRUCTORS --------------------------------
*/

Server::Server() : _sockfd(-1), _port(0)
{
	User server(_sockfd,"server", "server");
	_users.push_back(server);
}

Server::Server(const Server &src)
{
    *this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() 
{

}

/*
** --------------------------------- OVERLOADS ---------------------------------
*/

Server &Server::operator=(const Server &src)
{
    if (this != &src) {
        // Copy attributes here
		_sockfd = src._sockfd ;
		_port = src._port;
		_password = src._password;
		_addr = src._addr;
		_pending = src._pending;
		_reg = src._reg;
		_channels = src._channels;
		_fds = src._fds;
		_channels = src._channels;
		_users = src._users;
    }
    return *this;
}

/*
** --------------------------------- METHODS ---------------------------------
*/

bool	Server::init()
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
	{	
		std::cout << "Error: Socket creation failed" << std::endl;
		return false;
	}
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET; // IPV4 PROTOCOL
	_addr.sin_port = htons(_port); // Converts port number to network byte order 
	_addr.sin_addr.s_addr = INADDR_ANY; // Listens on all available IP'S

	int sock_state = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &sock_state, sizeof(sock_state)) < 0
		|| fcntl(_sockfd, F_SETFL, O_NONBLOCK) < 0) // Setting options for socket (Reuse IP's) and Non-block mode
	{
		std::cout << "Couldn't set options for socket" << std::endl;
		return false;
	}
	if (bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) < 0)
	{
	    std::cout << "Error: Bind failed" << std::endl;
	    return false;
	}
	if (listen(_sockfd, 5) < 0) // Queue up to 5 pending connections
	{
		std::cout << "Error: Listen failed" << std::endl;
		return false;
	}
	std::cout << "	Server initialized!" << std::endl << std::endl;
	std::cout << "      ╔════════════════════════╗" << std::endl;
	std::cout << "      ║    IRC SERVER READY    ║" << std::endl;
	std::cout << "      ║  Listening on port " << _port << "║" << std::endl;
	std::cout << "      ╚════════════════════════╝" << std::endl;
	return true;
}

void	Server::setPort(int port)
{
	_port = port;
}

int		Server::getPort(void) const
{
	return (_port); 
}
 /*
 	Creates new client_fd and default user, adds them to the respective vectors.
 */  
void	Server::newConnection(int fd, struct sockaddr_in address, socklen_t addr_size)
{
	struct pollfd client;
	client.fd = fd;
	client.events = POLLIN;
	client.revents = 0;
	_fds.push_back(client);
	User user(fd, "", "", address, addr_size);
	_users.push_back(user);

	std::cout << "[CONNECT] fd = "<< fd << std::endl;
	return ;
}
/*
	Uses the data, stored in user buffer, to parse the login credentials. Uses RFC protocol to also register user in Hexchat Client format.
*/
void	Server::registerUser(int i)
{
	std::string	cmdline = _users[i].recvBuf;
	size_t find = cmdline.find("\r\n");

	if (find == std::string::npos)
		find = cmdline.find("\n"); // If using ncat with -C option, client only sends "\n" 
	if (find == std::string::npos)
		return;

	std::string line = cmdline.substr(0, find);
	size_t erase_len = find + 1;
	if (find > 0 && cmdline[find - 1] == '\r')
		find--;
	line = cmdline.substr(0, find);
	_users[i].recvBuf.erase(0, erase_len);

	std::istringstream ss(line);
	std::string	cmd, value;
	ss >> cmd >> value;

	if (cmd == "CAP")
	{
		if (value == "LS")
			return ;
		else if (value == "END")
			return ;
	}
	if (cmd == "PASS")
	{
		if (value != _password)
		{
			sendMessage(_fds[i].fd, 464, "*", ERR_PASSWDMISMATCH);
			disconnect(i);
			return ;
		}
		_reg[i]._pass = value;
		return ;
	}
	if (_reg[i]._pass.empty())
		return ;
	if (cmd == "NICK")
	{
		if (!isValidNickname(value))
		{
			sendMessage(_fds[i].fd, 432, value, ERR_ERRONEUSNICKNAME);
			return;
		}
		if (doesUserExist(value) == true)
		{
			sendMessage(_fds[i].fd, 433, value, ERR_NICKNAMEINUSE);
			return;
		}
		_reg[i]._nickname = value;
	}
	if (cmd == "USER")
	{
		std::string user = value;
		std::string mode, unused, realname;
		ss >> mode >> unused;
		std::getline(ss >> std::ws, realname);
		if (user.empty() || mode.empty() || unused.empty() || realname.empty())
		{
			sendMessage(_fds[i].fd, 461, "*", ERR_NEEDMOREPARAMS);
			return;
		}
		if (realname[0] == ':')
			realname = realname.substr(1);
		if (realname.empty())
		{
			sendMessage(_fds[i].fd, 461, "*", ERR_NEEDMOREPARAMS);
			return;
		}
		if (!isValidUsername(user))
		{
			sendMessage(_fds[i].fd, 432, user, ERR_ERRONEUSNICKNAME);
			return;
		}
		_reg[i]._username = user;
	}
	if (!_reg[i]._pass.empty() && !_reg[i]._username.empty() && !_reg[i]._nickname.empty())
	{
		_users[i].setUsername(_reg[i]._username);
		_users[i].setNickname(_reg[i]._nickname);
		_users[i].Register();
		welcomeUser(i);
		_reg.erase(i);
		_users[i].recvBuf.clear();
	}
}


void	Server::welcomeUser(int i)
{
		std::string nick = _reg[i]._nickname;
		std::string welcome1 = ":irc.server 001 " + nick + " :Welcome to ircserv, " + nick + "\r\n";
		std::string welcome2 = ":irc.server 002 " + nick + " :Your host is irc.server\r\n";
		std::string welcome3 = ":irc.server 003 " + nick + " :This server was created in 2026\r\n";
		std::string welcome4 = ":irc.server 004 " + nick + " irc.server v1.0 tilk o\r\n";

		send(_fds[i].fd, welcome1.c_str(), welcome1.size(), 0);
		send(_fds[i].fd, welcome2.c_str(), welcome2.size(), 0);
		send(_fds[i].fd, welcome3.c_str(), welcome3.size(), 0);
		send(_fds[i].fd, welcome4.c_str(), welcome4.size(), 0);

}

void	Server::disconnect(int index)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].isUserOnChannel(_users[index].getNickname()))
		{
			broadcastMessage(_channels[i], getClientInfo(index) + " QUIT :Quit: Leaving\r\n", index, 0);
			_channels[i].rmUser(_users[index].getNickname());
			_channels[i].decrementCount();
		}
	}
	if ((_users.begin() + index) != _users.end())
		_users.erase(_users.begin() + index);
	close(_fds[index].fd);
	_fds.erase(_fds.begin() + index);
}

/*
	Parses every message sent on the server. Checks for unregistered users before parsing what command has to be executed.
*/
void Server::getMessage(char *buffer, int i)
{
	_users[i].recvBuf.append(buffer);
	size_t find = _users[i].recvBuf.find("\r\n");
	if (find != std::string::npos)
	{
		if (_users[i].getRegistration() == false)
		{
			while (_users[i].getRegistration() == false)
			{
				std::string old = _users[i].recvBuf;
				registerUser(i);
				if ((size_t)i >= _users.size() || _users[i].recvBuf == old)
					break ;
			}
			return;
		}
		else
		{
			exec_cmd(_users[i].recvBuf, i);
			_users[i].recvBuf.erase();
		}
	}
	else
		return ;
}

void	Server::setPassword(char *pass)
{
	_password = pass;
}

void	Server::sendMessage(int fd, int code, const std::string target, const std::string &msg)
{
	std::ostringstream ss;
	ss << ":irc.server " << code << " " << target << " :" << msg << "\r\n";
	send(fd, ss.str().c_str(), ss.str().size(), 0);
}

void	Server::run()
{
	signal(SIGINT, Server::setSignal);
	struct pollfd server;

	server.fd = _sockfd;
	server.events = POLLIN;
	server.revents = 0;
	_fds.push_back(server);

//	std::string line;
	while (Server::getSignal() == false)
	{
		cleanChannels();
		if (poll(&_fds[0], _fds.size(), -1) == -1)
		{
			if (Server::getSignal() == false)
				std::cerr << "-error: poll failure\n";
			break ;
		}
		for (size_t i = 0; i < _fds.size();++i)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _sockfd)  // New connection
				{
					struct sockaddr_in user_socket;
					socklen_t user_size = sizeof(user_socket);
					int user_fd = accept(_sockfd, (struct sockaddr *)&user_socket, &user_size);
					if (user_fd < 0)
					{
						perror("accept()");
						continue;
					}
					if (fcntl(user_fd, F_SETFL, O_NONBLOCK) < 0)
					{
						perror("fcntl()");
						continue;
					}
					newConnection(user_fd, user_socket, user_size);
				}
				else // Existing user message 
				{
					char buf[4096] = {};
					ssize_t bytes = recv(_fds[i].fd, buf, 4095, 0);
					if (bytes < 0)
					{
						perror("recv()");
						continue;
					}
					if (bytes == 0)
					{
						disconnect(i);
						continue;
					}
					buf[bytes] = 0;
					getMessage(buf, i);
					if (i >= _fds.size())
						continue;
				}
			}
		}
	}
	for (size_t i = 0; i < _fds.size(); i++)
		close(_fds[i].fd);
}

bool Server::isChannel(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return (true);
	}
	return (false);
}

int Server::getChannelIndex(const std::string name) const
{
	for (size_t i = 0; i < _channels.size(); i ++)
	{
		if (_channels[i].getName() == name)
			return (i);
	}
	return (-1);
}

int Server::getUserIndex(const std::string name) const
{
	for (size_t i = 0; i < _users.size(); i ++)
	{
		if (_users[i].getNickname() == name)
			return (i);
	}
	return (-1);
}

bool Server::doesUserExist(const std::string name) const
{
	for (size_t i = 0; i < _users.size(); i ++)
	{
		if (_users[i].getNickname() == name)
			return (true);
	}
	return (false);
}

bool Server::isValidNickname(std::string &str) 
{
	if (str.empty())
		return false;
	if (str.size() > 30)
		return false;
	std::string forbidden = "#&!@:%+~,";
	for (size_t i = 0; i < str.size(); i++)
	{
		unsigned char c = str[i];
		if (c <= 32 || c > 126)
			return false;
		if (forbidden.find(c) != std::string::npos)
			return false;
	}
	return true;
}

bool Server::isValidUsername( std::string &str)
{
	if (str.empty())
		return false;
	if (str.size() > 30)
		return false;
	std::string forbidden = "#&!@:%+~";
	for (size_t i = 0; i < str.size(); i++)
	{
		unsigned char c = str[i];
		if (c <= 32 || c > 126)
			return false;
		if (forbidden.find(c) != std::string::npos)
			return false;
	}
	return true;
}

int Server::getSignal(void)
{
	return _signal;
}

void Server::setSignal(int signal)
{ 
	(void)signal;
	std::cout << std::endl;
	_signal = true;
}

void Server::cleanChannels(void)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getCount() == 0)
			_channels.erase(_channels.begin() + i);
	}
}
