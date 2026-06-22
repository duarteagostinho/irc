# include "../inc/Server.hpp"

int args_check(Server &server, int argc, char **argv)
{
	if (argc < 3)
	{
		std::cerr << "-error: Missing arguments\n-expected: ./ircserv <port> <password>\n";
		return  (-1);
	}
	else if (argc > 3)
	{
		std::cerr << "-error: Too many arguments\n-expected: ./ircserv <port> <password>\n";
		return  (-1);
	}
	for (int i = 0; argv[1][i]; i++)
	{
		if (!std::isdigit(argv[1][i]))
		{
			std::cerr << "-error: Invalid port number\n-expected: ./ircserv <port> <password>\n";
			return (-1);
		}
		try
		{
			char *end;
			server.setPort(std::strtol(argv[1], &end, 10));

			if (server.getPort() < 1024 || server.getPort() > 65535)
				throw(std::out_of_range(""));
		}
		catch (const std::out_of_range &e)
		{
			std::cerr << "-error: Invalid port number\n-expected: ./ircserv <port> <password>\n";
			return (-1);
		}
	}
	return (0);
}

int main(int argc, char **argv)
{
	Server server;

	if (args_check(server, argc, argv) == -1 || !server.init())
		return (1);
	server.setPassword(argv[2]);
	server.run();

    return 0;
}

