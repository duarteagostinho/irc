#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton()
#include <string.h>
#include <stdlib.h>
#include <netdb.h> // NI_MAXHOST
#include <unistd.h>

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

	sockaddr_in client;
	socklen_t client_size = sizeof(client);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	client_socket = accept(listening, (sockaddr *)&client, &client_size);
	if (client_socket == -1)
	{
		std::cerr << "-error: Failed to accept connection\n";
		exit (6);
	}
	
	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	char buffer[4096];
	std::string test;
	while (true)
	{
		// Clear buffer
		memset(buffer, 0, 4096);

		// Wait for a message
		int bytes_recv = recv(client_socket, buffer, 4095, 0);
		std::cout << "read: " << bytes_recv << std::endl;
		if (bytes_recv == -1)
		{
			std::cerr << "-error: Connection issue\n";
			break ;
		}
		if (bytes_recv == 0)
		{
			std::cout << "Client disconnected\n";
			break ;
		}

		buffer[bytes_recv] = 0;
		
		std::cout << "buff is: " << buffer << std::endl;
		
		test.append(buffer);
		size_t find = test.find("\r\n");
		if (find != std::string::npos)
		{
			std::cout << "Test: " << test << std::endl;
			std::cout << "Function print: ";
			print_str(test.c_str());
			std::cout << std::endl;
			send(client_socket, test.c_str(), test.size(), 0);

			// Do stuff
			test.erase(find, 2);
			if (Commands::check_cmd(test) == -1)
				close(client_socket);



			// Reset string
			test.clear();
		}
	}
	close(listening);
	close(client_socket);
}

int main(int argc, char **argv)
{
	(void)argv;
	args_check(argc);

	socket_steps(argv + 1);
}