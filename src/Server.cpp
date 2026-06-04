#include "../inc/Server.hpp"
#include "../inc/User.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <cerrno>
#include <stdio.h>
#include <sstream>

void	Server::setPort(int port)
{
	_port = port;
}

int		Server::getPort(void) const
{
	return (_port); 
}

void	Server::setPassword(char *pass)
{
	_password = pass;
}

/*
** ------------------------------- CONSTRUCTORS --------------------------------
*/

Server::Server() : _sockfd(-1), _port(0)
{
    std::cout << "Default Constructor called" << std::endl;
	std::string name = "server";
	_nick.push_back(name);
}

Server::Server(const Server &src) : Commands(src)
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

std::ostream &operator<<(std::ostream &o, const Server &i)
{
    (void)i;
	o << "Type: Server";
    return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Server::getMessage(std::string &line, char *buffer, int i)
{
	line.append(buffer);
	size_t find = line.find("\r\n");
	if (find != std::string::npos)
	{
		// Do stuff
		line.erase(find, 2);

		// parse and execute commands here
		exec_cmd(line);


		// THIS LOOP IS JUST SENDING THE MESSAGE AND NICK BACK TO EACH OTHER CLIENT
		for (size_t j = 1; j < _fds.size(); j++) // start at 1 to always ignore the listening socket
		{
			std::ostringstream ss;
			std::string str;
			ss << _nick[i] << ": " << line << "\r\n";
			str = ss.str();

			if (_fds[i].fd != _fds[j].fd) // Send to every fd that is not mine
				send(_fds[j].fd, str.c_str(), str.size() + 1, 0);
		}



		// Reset string
		std::cout << "server line: " << line << std::endl;
		line.clear();
	}
}


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
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &sock_state, sizeof(sock_state)) < 0) // Setting options for socket (Reuse IP's)
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


// NEW
void	Server::run()
{
	struct pollfd server;

	server.fd = _sockfd;
	server.events = POLLIN;
	server.revents = 0;
	_fds.push_back(server);

	while (true)
	{
		if (poll(&_fds[0], _fds.size(), -1) == -1)
		{
			std::cerr << "-error: poll failure\n";
			exit (10);
		}
		for (size_t i = 0; i <= _fds.size() ;++i)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _sockfd)  // New connection
				{
					int user_fd = accept(_sockfd, NULL, NULL);
					if (user_fd < 0)
					{
						perror("accept()");
						continue;
					}
					// std::ostringstream ss;
					// ss << "USER#" << _fds[i].fd << ": ";
					// ss >> _nick[i];
					newConnection(user_fd);
					std::string new_nick = "USER#" + std::to_string(user_fd);
					_nick.push_back(new_nick);

					std::string welcome = "Welcome to IRC!\r\n";
					send(user_fd, welcome.c_str(), welcome.size(), 0);
				}
				else // Existing user message 
				{
					char buf[4096] = {};
					std::string line;
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
//					userMessage(_fds[i].fd, buf, bytes);
					getMessage(line, buf, i);
				}
			}
		}
	}
}




void	Server::newConnection(int fd)
{
	struct pollfd client;
	client.fd = fd;
	client.events = POLLIN;
	client.revents = 0;
	_fds.push_back(client);
	// if (fd > _maxFd)
	// 	_maxFd = fd;
	_pending[fd] = "";
	std::cout << "[CONNECT] fd = "<< fd << std::endl;
	return ;
}

void	Server::registerUser(int fd)
{
	std::istringstream ss(_pending[fd]);
	std::string		cmd, token;
	unsigned long 	pos;
	ss >> cmd >> token;

	if (_users.find(fd) != _users.end())
		return ;
	if ((pos = _pending[fd].find('\n')) == std::string::npos)
		return ;
	std::string line = _pending[fd].substr(0, pos);
	_pending[fd].erase(0, pos + 1);
	if (cmd == "NICK")
	{
		_reg[fd]._nickname = token;
		_reg[fd].has_nick = true;
	}
	if (cmd == "USER")
	{
		_reg[fd]._username = token;
		_reg[fd].has_user = true;
	}
	if (_reg[fd].has_nick && _reg[fd].has_user)
	{
		_users[fd] = User(fd, _reg[fd]._nickname,_reg[fd]._username);
		_users[fd].registered = true;
		send(fd, "Welcome to ft_irc!\n", 19, 0);
		_reg.erase(fd);
	}
	return ;
}

void	Server::userMessage(int fd, const std::string &msg, ssize_t bytes)
{
	if (_users.find(fd) == _users.end())
	{
		_pending[fd].append(msg);
		registerUser(fd);
	}
	else
	{
		_users[fd].recvBuf.append(msg, bytes);
		parseMessage(fd);
	}
}

// NEW
void	Server::disconnect(int i)
{
	std::cout << "[DISCONECT] fd = "<< _fds[i].fd << std::endl;
	if (_users.find(_fds[i].fd) != _users.end())
	{
		std::cout << "nick=" << _users[_fds[i].fd].getNickname() << std::endl;
		_users.erase(_fds[i].fd);
	}
	else
		_pending.erase(_fds[i].fd);
	close(_fds[i].fd);
	_fds.erase(_fds.begin() + i);
}

void Server::handleCommand(int fd, const std::string &line)
{
    std::istringstream ss(line);
    std::string cmd;
    ss >> cmd;

	std::cout << cmd << std::endl;

    if (cmd == "PING")
    {
        std::string token;
        ss >> token;
        std::string pong = "PONG " + token + "\r\n";
        send(fd, pong.c_str(), pong.size(), 0);
    }
    else if (cmd == "PRIVMSG")
    {
        std::string target, msg;
        ss >> target;
        std::getline(ss, msg);
        std::cout << "[PRIVMSG] " << _users[fd].getNickname()
                  << " → " << target << " :" << msg << std::endl;
    }
    else if (cmd == "JOIN")
    {
        std::string channel;
        ss >> channel;
        std::cout << "[JOIN] " << _users[fd].getNickname()
                  << " entrou em " << channel << std::endl;
    }
    else if (cmd == "QUIT")
    {
        std::cout << "[QUIT] " << _users[fd].getNickname()
                  << " desligou-se" << std::endl;
        disconnect(fd);
    }
    else
    {
        std::cout << "[UNKNOWN] fd=" << fd
                  << " cmd='" << cmd << "' line='" << line << "'" << std::endl;
    }
}

void	Server::parseMessage(int fd)
{
	std::string msg = _users[fd].recvBuf;
	size_t		pos;
	
	while((pos = msg.find('\n')) != std::string::npos)
	{
		std::string line = msg.substr(0, pos + 1);
		msg.erase(0, pos + 1);
		while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) 
			line.pop_back();
		handleCommand(fd, line);
	}
}