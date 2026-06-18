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
# include <signal.h>

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
# define ERR_PASSWDMISMATCH "Password incorrect\r\n"
# define ERR_NICKNAMEINUSE "Nickname is already in use\r\n"
# define ERR_NEEDMOREPARAMS "Not enough parameters\r\n"
# define ERR_ERRONEUSNICKNAME "Erroneous nickname\r\n"

class Server
{

	struct Registration {

		std::string	_nickname;
		std::string	_username;
		std::string	_pass;
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

		static bool					_signal;

	public:
        // Constructors & Destructor
        Server();									// Default
        Server(const Server &src);					// Copy
        ~Server();									// Destructor

        // Operators
        Server &operator=(const Server &src); // Copy Assignment
		
		// Server loop
		bool	init(); // Creates socket, bind, listen
		void	run();	// Accept clients in a loop;
		void	newConnection(int fd, struct sockaddr_in address, socklen_t addr_size);
		void	userMessage(int fd, const std::string &msg, ssize_t bytes);
		void	disconnect(int fd);
		void	parseMessage(int fd);
		void	handleCommand(int fd, const std::string &line);
		void	registerUser(int fd);
		void	closeServer();	// Clean up

		// Getters && Setters
		void	setPassword(char *pass);
		void	setPort(int port);
		int		getPort(void) const;
		void	getMessage(std::string &line, char *buffer, int i);
		void	getUserConfig(std::string &line, char *buffer, int i);
		const std::vector<Channel> &getChannel(void) const;
		
		// Helpers
		void	sendError(int fd, int code, const std::string target, const std::string &msg);
		void	welcomeUser(int i);
		bool	isValidNickname( std::string &str);
		bool	isValidUsername( std::string &str);

		// CMDS
		void	exec_cmd(std::string line, int index);
		void	kick(std::vector<std::string>& av, int index);
		void	mode(std::vector<std::string>& av, int index);
		void	join(std::vector<std::string>& av, int index);
		void	part(std::vector<std::string>& av, int index);		
		void	topic(std::vector<std::string>& av, int index);
		void	invite(std::vector<std::string>& av, int index);
		void	privmsg(std::vector<std::string>& av, int index);
		int		check_cmd(std::string &line, std::vector<std::string>& av);

		// DEL
		void print_everything(void);

		std::string usersPreChannelFormated(std::string name);


		bool isChannel(std::string name);
		void splitString(std::string line, std::vector<std::string>& av, char delim);

		int getChannelIndex(const std::string name) const;
		int getUserIndex(const std::string name) const;
		bool doesUserExist(const std::string name) const;
		std::string craftStringSpaces(const std::string str, int times);
		std::string getClientInfo(int index);

		void broadcastMessage(Channel &channel, std::string message, int index, int flag);


		static int getSignal(void);
		static void setSignal(int signal);
};

// Stream Operator Overload
std::ostream &operator<<(std::ostream &o, const Server &i);

#endif
