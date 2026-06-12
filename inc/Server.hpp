#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <map>
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
# include <fcntl.h>
# include "User.hpp"

// NEW
# include <poll.h>
# include <vector>
# include <sstream>
//# include "Commands.hpp"
# include "Channel.hpp"

# define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel\r\n"
# define INV_USAGE "Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel\r\n"
# define JOIN_USAGE "Usage: JOIN <channel>, joins the channel\r\n"
# define NOT_OPERATOR ": You are not the channel operator\r\n"


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

class Server
{

	struct Registration {

		std::string	_nickname;
		std::string	_username;
		bool		has_nick;
		bool		has_user;
	};

	private: 

		int							_sockfd;
		int							_port;
		std::string					_password;
		struct sockaddr_in			_addr;
//		std::map<int, User>			_users;
		std::map<int, std::string>	_pending;
		std::map<int, Registration>	_reg;
		//std::map<int, Channel>	_channels;


		std::vector<struct pollfd>	_fds;
		std::vector<Channel>		_channels;
		std::vector<User>			_users;

	public:
        // Constructors & Destructor
        Server();									// Default
        Server(const Server &src);					// Copy
        ~Server();									// Destructor

        // Operators
        Server &operator=(const Server &src); // Copy Assignment
		bool	init(); // Creates socket, bind, listen
		void	run();	// Accept clients in a loop;
		void	newConnection(int fd);
		void	userMessage(int fd, const std::string &msg, ssize_t bytes);
		void	disconnect(int fd);
		void	parseMessage(int fd);
		void	handleCommand(int fd, const std::string &line);
		void	registerUser(int fd);
		void	closeServer();	// Clean up


		void	setPassword(char *pass);

		void	setPort(int port);
		int		getPort(void) const;

		void	getMessage(std::string &line, char *buffer, int i);
		void	getUserConfig(std::string &line, char *buffer, int i);


		const std::vector<Channel> &getChannel(void) const;

		// CMDS
		void	exec_cmd(std::string line, int index);
		int		check_cmd(std::string &line, std::vector<std::string>& av);
		
		void	kick(std::vector<std::string>& av, int index);
		void	invite(std::vector<std::string>& av, int index);
		void	topic(std::vector<std::string>& av, int index);
		void	mode(void);
		void	join(std::vector<std::string>& av, int index);
		void	privmsg(std::vector<std::string>& av, int index);

		// DEL
		void print_everything(void);

		std::string usersPreChannelFormated(std::string name);


		bool isChannel(std::string name);
		void splitString(std::string line, std::vector<std::string>& av, char delim);

		int getChannelIndex(const std::string name) const;
		int getUserIndex(const std::string name) const;
		bool doesUserExist(const std::string name) const;
		std::string craftStringSpaces(const std::string str);
		std::string getClientInfo(int index);

};

// Stream Operator Overload
std::ostream &operator<<(std::ostream &o, const Server &i);

#endif
