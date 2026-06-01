#include "../Server.hpp"
#include <cstring>
#include <filesystem>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

/*
** ------------------------------- TO DELETE --------------------------------
*/

void print_str(const char *str)
{
	std::cout << "Function print: ";
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\r')
			std::cout << "'\\r'";
		else if (str[i] == '\n')
			std::cout << "'\\n'";
		else
			std::cout << str[i];
	}
	std::cout << std::endl;
}


/*
** ------------------------------- CONSTRUCTORS --------------------------------
*/

Server::Server()
{
    std::cout << "Default Constructor called" << std::endl;
}

Server::Server(int ac, char **av)
{
	if (ac != 3)
		throw std::runtime_error("Invalid number of arguments");
	_port = std::atoi(av[1]);
	_sockfd = -1;
	std::cout << "Server Constructor" << std::endl;

}

Server::Server(const Server &src) {
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

Server &Server::operator=(const Server &src) {
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

bool	Server::init()
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
	{	
		std::cout << "Error: Socket creation failed" << std::endl;
		return 1;
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
	return true;
}

void	Server::run()
{
	fd_set master, readfds;
	FD_ZERO(&master);
	FD_SET(_sockfd, &master);
	int				max_fd =_sockfd;
	while (true)
	{
		readfds = master;
		int ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if (ready < 0)
		{
			// handle EINTR & continue;
		}
		for (int fd = 0;fd <= max_fd && ready > 0; ++fd)
		{
			if (!FD_ISSET(fd, &readfds))
				continue;
			if (fd == _sockfd)
			{
				int client_fd = accept(_sockfd, NULL, NULL);
				if (client_fd < 0)
				{
					std::cout << "Error: accepting client_fd" << std::endl;
					continue;
				}
				FD_SET(client_fd, &master);
				if (client_fd > max_fd)
					max_fd = client_fd;
				acceptClient(client_fd);
				close(client_fd);
			}
		}
	}
	
}

/*
 * TODO: change this from a loop to a func for each client
*/

void	Server::acceptClient(int fd)
{
	char buf[4097];
	std::string str;
	ssize_t		bytes_rcvd = 0;

	while (true)
	{
		memset(buf, 0, 4097);
		bytes_rcvd = recv(fd, buf, 4096, 0);
		if (bytes_rcvd < 0)
		{
			std::cout << "Error: recv() failed" << std::endl;
			continue ;
		}
		if (bytes_rcvd == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break ;
		}
		buf[bytes_rcvd] = '\0';

		str.append(buf);
		size_t find = str.find("\r\n");
		if (find != std::string::npos)
		{
			std::cout << "Test: " << str << std::endl;
			print_str(str.c_str());
			
			if (send(fd , str.c_str(), str.size(), 0) < 0)
			{
				std::cout << "Error: send() failed" << std::endl;
				break;
			}	

			// Do stuff
			str.erase(find, 2);
			Server::exec_cmd(str);

			// Reset string
			str.clear();
		}
		std::cout << "Received: " << buf << std::endl;
	}
}
