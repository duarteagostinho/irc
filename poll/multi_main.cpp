#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()
#include <string.h>
#include <stdlib.h>
#include <netdb.h> // NI_MAXHOST
#include <unistd.h>
#include <sys/select.h>
#include <poll.h>


#include "Commands.hpp"

int client_socket = -1;

void args_check(int argc)
{
	if (argc < 3)
	{
		std::cerr << "-error: Missing arguments\n-expected: ./ircserv <port> <password>\n";
		exit (1);
	}
	else if (argc > 3)
	{
		std::cerr << "-error: Too many arguments\n-expected: ./ircserv <port> <password>\n";
		exit (2);
	}
}

void print_str(const char *str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\r')
			std::cout << "'\\r'";
		else if (str[i] == '\n')
			std::cout << "'\\n'";
		else
			std::cout << str[i];
	}
}

static void accept_data(std::vector<struct pollfd> &fds, int i, std::string &line, char *buffer, std::vector<std::string> &nick)
{
	// Wait for a message
	int bytes_recv = recv(fds[i].fd, buffer, 4095, 0);
	std::cout << "read: " << bytes_recv << std::endl;
	if (bytes_recv == -1)
	{
		std::cerr << "-error: Connection issue\n";
		return ;
	}
	if (bytes_recv == 0)
	{
		std::cout << "Client disconnected\n";
		std::cout << fds[i].fd << ": " << nick[i] << std::endl;
		fds.erase(fds.begin() + i);
		nick.erase(nick.begin() + i);
		return ;
	}
	buffer[bytes_recv] = 0;
	
//	std::cout << "buff is: " << buffer << std::endl;
	
	line.append(buffer);
	size_t find = line.find("\r\n");
	if (find != std::string::npos)
	{
		// std::cout << "Test: " << line << std::endl;
		// std::cout << "Function print: ";
//		print_str(line.c_str());
//		std::cout << std::endl;


		for (size_t j = 1; j < fds.size(); j++)
		{
			std::string tmp = nick[i] + ": " + line;
			send(fds[j].fd, tmp.c_str(), tmp.size(), 0);
		}

//		send(fds[i].fd, tmp.c_str(), tmp.size(), 0);
//		send(fds[i].fd, line.c_str(), line.size(), 0);

		// Do stuff
		line.erase(find, 2);

		// Reset string
		line.clear();
	}
}


void socket_steps(char **argv)
{
/*
	1 - Create a socket
	2 - Bind the socket
	3 - Put socket in listening mode
	4 - accept() / connect()
	5 - data transfer with recv() / send()
	6 - close socket()
*/

	// Create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == -1)
	{
		std::cerr << "-error: Failed to create socket\n";
		exit (3);
	}
	
	int opt = 1;
	if (setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 || fcntl(listening, F_SETFL, O_NONBLOCK) == -1)
	{
		printf("socket options failed\n");
		exit (1);
	}


	// Bind the socket
	struct sockaddr_in hint;

	hint.sin_family = AF_INET;
	hint.sin_port = htons(atoi(argv[0]));
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // THIS DOESNT WORK, CANT USE PTON, BUT I CAN USE NTOP !!!!!!!!!!!!!!!!!
	if (bind(listening, (struct sockaddr *)&hint, sizeof(hint)) == -1) // why bind(listening, AF_INET, &hint, sizeof(hint)) ??
	{
		std::cerr << "-error: Failed to bind\n";
		exit (4);
	}

	// Put socket in listening mode
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "-error: Listen initialization\n";
		exit (5);
	}
	//--- SERVER SHOULD BE INITIALIZED TO LISTENING FOR ANY CONNECTION ---//

	// Servers cans only do two things
	// 1- Accept a connection
	// 2- Receive a message

	std::vector<std::string> nick;
	std::string lol = "server";
	nick.push_back(lol);

	std::vector<struct pollfd> fds;
	struct pollfd server;

	server.fd = listening;
	server.events = POLLIN;
	server.revents = 0;

	fds.push_back(server);

	char buffer[4096];
	std::string line;
	
	while (true)
	{
		memset(buffer, 0, 4096);
		if (poll(&fds[0], fds.size(), -1) == -1)
		{
			std::cerr << "-error: poll failure\n";
			exit (10);
		}
		for (size_t i = 0; i < fds.size(); i++)
		{
			std::cout << "size: " << fds.size() <<  std::endl;
			if (fds[i].revents & POLLIN)
			{
				std::cout << "fd with information: " << i << std::endl;
				if (fds[i].fd == listening)
				{
					// New connection?
					std::cout << "New connection!" << std::endl;
					sockaddr_in client_addr;
					socklen_t client_size = sizeof(client_addr);
					int client_socket = accept(listening, (sockaddr *)&client_addr, &client_size);
					if (client_socket == -1)
					{
						std::cerr << "-error: Failed to accept connection\n";
						exit (6);
					}
					struct pollfd client;
					client.fd = client_socket;
					client.events = POLLIN;
					client.revents = 0;
					fds.push_back(client);
					send(client.fd, "NICK: ", 6, 0);
					recv(client.fd, buffer, 4095, 0);
					line = buffer;

					line.erase(line.end() - 2, line.end());
					nick.push_back(line);
					line.clear();
				}
				else
				{
					// Data receives from a valid fd
					std::cout << "New message!" << std::endl;
					accept_data(fds, i, line, buffer, nick);
				}
				fds[i].revents = 0;
			}
		}
	}
}

int main(int argc, char **argv)
{
	(void)argv;
	args_check(argc);

	Server server;

	socket_steps(argv + 1);
}