# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "../inc/Server.hpp"

void args_check(Server &server, int argc, char **argv)
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
	for (int i = 0; argv[1][i]; i++)
	{
		if (!std::isdigit(argv[1][i]))
		{
			std::cerr << "-error: Invalid port number\n-expected: ./ircserv <port> <password>\n";
			exit(3);
		}
		try
		{
//			server.setPort(std::stoi(argv[1]));
			char *end;
			server.setPort(std::strtol(argv[1], &end, 10));

			if (server.getPort() < 1024 || server.getPort() > 65535)
				throw(std::out_of_range(""));
		}
		catch (const std::out_of_range &e)
		{
			std::cerr << "-error: Invalid port number\n-expected: ./ircserv <port> <password>\n";
			exit(3);
		}
	}
}

int main(int argc, char **argv)
{
	Server server;

	args_check(server, argc, argv);
	server.setPassword(argv[2]);
	
    if (!server.init())
        return 1;
	// create a signal handler for ctrl c ?
    server.run();
    return 0;
}
