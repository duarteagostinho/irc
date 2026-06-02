# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "../inc/Server.hpp"

int main(int argc, char **argv)
{
    Server server(argc, argv);
    if (!server.init())
        return 1;
    server.run();
    return 0;
}
