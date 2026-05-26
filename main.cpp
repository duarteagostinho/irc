# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "inc/Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <pass>" << std::endl;
        return 1;
    }

    Server server(argc, argv);
    if (!server.init())
        return 1;
    server.run();
    return 0;
}
