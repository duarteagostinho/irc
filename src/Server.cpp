#include "../inc/Server.hpp"
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <vector>

/*
** ------------------------------- CONSTRUCTORS --------------------------------
*/

Server::Server() : _sockfd(-1), _port(0)
{
    std::cout << "Default Constructor called" << std::endl;
	User server(_sockfd,"server", "server");
	_users.push_back(server);
}

Server::Server(const Server &src)
{
    std::cout << "Copy Constructor called" << std::endl;
    *this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() {
    std::cout << "Destructor called" << std::endl;
}

/*
** --------------------------------- OVERLOADS ---------------------------------
*/

Server &Server::operator=(const Server &src)
{
    if (this != &src) {
        // Copy attributes here
    }
    return *this;
}

/*
** --------------------------------- DEL ---------------------------------
*/

void Server::print_everything(void) // DEL
{
	std::cout << "|---------- INFO ----------|" << std::endl;
	if (_channels.size() == 0)
		std::cout << "No channel yet." << std::endl;
	for (size_t i = 0; i < _channels.size(); i++)
	{
		std::cout << "channel: " << _channels[i].getName() << std::endl;
		_channels[i].print_users();
	}
 std::cout << "|--------- USERS -----------|";
 for (size_t j = 0; j < _users.size();j++)
   std::cout << "User " << j << ": " << _users[j].getNickname() << std::endl;
	std::cout << std::endl << "|---------- END ----------|" << std::endl;
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

void	Server::newConnection(int fd)
{
	struct pollfd client;
	client.fd = fd;
	client.events = POLLIN;
	client.revents = 0;
	_fds.push_back(client);
	User user(fd, "", "");
	_users.push_back(user);

	std::cout << "[CONNECT] fd = "<< fd << std::endl;
	return ;
}

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
		std::cout << "[DEBUG] fd=" << i << ", cmd=" << cmd << ", value=" << value << std::endl;
		if (value != _password)
		{
			sendError(i, 464, "*", ERR_PASSWDMISMATCH);
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
		std::cout << "[DEBUG] fd=" << i << ", cmd=" << cmd << ", value=" << value << std::endl;
		if (nickInUse(value) == true)
		{
			sendError(i, 433, value, ERR_NICKNAMEINUSE);
			return;
		}
		_reg[i]._nickname = value;
	}
	if (cmd == "USER")
	{
		std::cout << "[DEBUG] fd=" << i << ", cmd=" << cmd << ", value=" << value << std::endl;
		_reg[i]._username = value;
	}
	if (!_reg[i]._pass.empty() && !_reg[i]._username.empty() && !_reg[i]._nickname.empty())
	{
		_users[i] = User(_fds[i].fd, _reg[i]._nickname, _reg[i]._username);
		_users[i].Register();
		welcomeUser(i);
		_reg.erase(i);
		_users[i].recvBuf.clear();

	}
}

void	Server::welcomeUser(int i)
{
		std::string welcome1 = ":server 001 " + _reg[i]._nickname + " :Welcome to ft_irc\r\n";
		std::string welcome2 = ":server 002 " + _reg[i]._nickname + " :Your host is server\r\n";
		std::string welcome3 = ":server 003 " + _reg[i]._nickname + " :This server was created 2026\r\n";
		std::string welcome4 = ":server 004 " + _reg[i]._nickname + " server 1.0 o o\r\n";

		send(_fds[i].fd, welcome1.c_str(), welcome1.size(), 0);
		send(_fds[i].fd, welcome2.c_str(), welcome2.size(), 0);
		send(_fds[i].fd, welcome3.c_str(), welcome3.size(), 0);
		send(_fds[i].fd, welcome4.c_str(), welcome4.size(), 0);
}

void	Server::disconnect(int i)
{
	std::cout << "[DISCONECT] fd = "<< _fds[i].fd << std::endl;
//	if (_users.find(_fds[i].fd) != _users.end())
//	{
	std::cout << "nick=" << _users[i].getNickname() << std::endl;
//		_users.erase(_fds[i].fd);
	_users.erase(_users.begin() + i);
//	}
	// else
	// 	_pending.erase(_fds[i].fd);
	close(_fds[i].fd);
	_fds.erase(_fds.begin() + i);
}

void Server::getMessage(std::string &line, char *buffer, int i)
{
	if (_users[i].getRegistration() == false)
	{
		_users[i].recvBuf.append(buffer);
    while (_users[i].getRegistration() == false)
    {
      std::string old = _users[i].recvBuf;
      registerUser(i);
      if (_users[i].recvBuf == old)
        break ;
    }
		return;
	}
	line.clear();
	line.append(buffer);
	size_t find = line.find("\r\n");
	if (find != std::string::npos)
	{
		std::string cmd_line = line.substr(0, find);	
		exec_cmd(line, i);
		line.erase(0, find + 2);
		find = line.find("\r\n");
	}
}

void	Server::setPassword(char *pass)
{
	_password = pass;
}

void	Server::sendError(int fd, int code, const std::string target, const std::string &msg)
{
	std::ostringstream ss;
	ss << ":server " << code << " " << target << " :" << msg << "\r\n";
	send(fd, ss.str().c_str(), ss.str().size(), 0);
}

bool	Server::nickInUse(std::string toCheck)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i].getUsername() == toCheck)
			return true;
	}
	return false;
}

void	Server::run()
{
	struct pollfd server;

	server.fd = _sockfd;
	server.events = POLLIN;
	server.revents = 0;
	_fds.push_back(server);

	std::string line;
	while (true)
	{
		line.clear();
		print_everything();
		if (poll(&_fds[0], _fds.size(), -1) == -1)
		{
			std::cerr << "-error: poll failure\n";
			exit (10);
		}

		for (size_t i = 0; i < _fds.size() ;++i)
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
					// Create user inside, after gathering the nick and username
					// newConnection(user_fd, user_socket, user_size);
					newConnection(user_fd);
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
					std::cout << "raw buf: " << buf << ", bytes: " << bytes << std::endl;
//					userMessage(_fds[i].fd, buf, bytes);
					getMessage(line, buf, i);
				}
			}
		}
	}
}

