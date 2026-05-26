#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <vector>
# include "User.hpp"

/*                        STRUCTURE FOR SOCKET ADDR
    struct sockaddr_in {
        sa_family_t sin_family;         // Address family (AF_INET)
        in_port_t   sin_port;           // Port number in network byte order
        struct      in_addr sin_addr;   // IP address (32-bit)
};
*/


/*                      	   PROGRAM LOGIC
	Init
		socket()	  -- Create socket with file descriptor _sockfd
		bind()		  -- Attach to _port using _addr
		listen()	  -- Ready to accept connections
	Run Loop
		poll()        -- Waits for one of a set of file descriptors to become ready to perform I/O.
		accept()	  -- Wait for client, get new fd
		recv()		  -- Read IRC commands
		send()		  -- Send responses from Server
	Cleanup
		close()		  -- Clean up socket
*/

class Server {

	private: 
		
		int					_sockfd;
		int					_port;
		struct sockaddr_in	_addr;
		std::vector<User>	_users;

	public:
        // Constructors & Destructor
        Server();                                  // Default
		Server(int ac, char **av);
        Server(const Server &src);            // Copy
        ~Server();                                 // Destructor

        // Operators
        Server &operator=(const Server &src); // Copy Assignment
		bool	init(); // Creates socket, bind, listen
		void	run();	// Accept clients in a loop;
		void	acceptClient(int fd);	// Handle new connection
		void	closeServer();	// Clean up

};

// Stream Operator Overload
std::ostream &operator<<(std::ostream &o, const Server &i);

#endif
