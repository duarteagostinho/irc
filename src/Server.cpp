#include "../inc/Server.hpp"

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



					// THIS WHOLE PACKAGE NEEDS TO BE IN THE USER REGISTRATION !!
					std::ostringstream ss;
					ss << user_fd;
					std::string new_nick = "USER#" + ss.str();
					User usr(user_fd, new_nick, new_nick);
					_users.push_back(usr);
					// THIS WHOLE PACKAGE NEEDS TO BE IN THE USER REGISTRATION !!
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

	std::cout << "[CONNECT] fd = "<< fd << std::endl;
	return ;
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
	std::cout << std::endl;
	// for (size_t i = 0; i < line.size(); i++)
	// 	std::cout << "line in: " << (int)line[i] << std::endl;
	line.append(buffer);
	size_t find = line.find("\r\n");
	if (find != std::string::npos)
	{
		// Do stuff
//		line.erase(find, 2);
		std::string msg = line.substr(0, find);
		// parse and execute commands here
		exec_cmd(line, i);
		

		// THIS LOOP IS JUST SENDING THE MESSAGE AND NICK BACK TO EACH OTHER CLIENT
		for (size_t j = 1; j < _fds.size(); j++) // start at 1 to always ignore the listening socket
		{
			std::ostringstream ss;
			std::string str;
			ss << _users[i].getNickname() << ": " << line << "\r\n";
			str = ss.str();

			if (_fds[i].fd != _fds[j].fd) // Send to every fd that is not mine
				send(_fds[j].fd, str.c_str(), str.size() + 1, 0);
		}
		// Reset string
		line.erase(0, find + 2);
	}
}

void	Server::setPassword(char *pass)
{
	_password = pass;
}
