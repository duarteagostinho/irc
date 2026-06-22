#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <map>
# include <cstdio>
# include <cstring>
# include <sstream>
# include <fcntl.h>
# include <signal.h>
# include <poll.h>
# include <vector>
# include "User.hpp"
# include "Channel.hpp"

# define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel\r\n"
# define INV_USAGE "Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel\r\n"
# define JOIN_USAGE "Usage: JOIN <channel>, joins the channel\r\n"

# define RPL_NOTOPIC "No topic is set"							// 331
# define RPL_ENDOFINVITELIST "End of Invite List"				// 337
# define RPL_ENDOFNAMES "End of /NAMES list"					// 366
# define ERR_NOSUCHNICK "No such nick"							// 401
# define ERR_NOSUCHCHANNEL "No such channel"					// 403
# define ERR_CANNOTSENDTOCHAN "Cannot send to channel"			// 404
# define ERR_NORECIPIENT "No recipient given (PRIVMSG)"			// 411
# define ERR_NOTEXTTOSEND "No text to send"						// 412
# define ERR_UNKNOWNCOMMAND "Unknown command"					// 421
# define ERR_ERRONEUSNICKNAME "Erroneous nickname"				// 432
# define ERR_NICKNAMEINUSE "Nickname is already in use"			// 433
# define ERR_NOTONCHANNEL "You're not on that channel"			// 442
# define ERR_USERONCHANNEL "is already on channel"				// 443
# define ERR_NEEDMOREPARAMS "Not enough parameters"				// 461
# define ERR_PASSWDMISMATCH "Password incorrect"				// 464
# define ERR_KEYSET "Channel key already set"  					// 467
# define ERR_CHANNELISFULL "Cannot join channel (+l)"			// 471
# define ERR_UNKNOWNMODE "is not a recognised channel mode"		// 472
# define ERR_INVITEONLYCHAN "Cannot join channel (+i)"			// 473
# define ERR_BADCHANNELKEY "Cannot join channel (+k)"			// 475
# define ERR_BADCHANMASK "Bad Channel name"						// 476
# define ERR_CHANOPRIVSNEEDED "You're not channel operator"		// 482
# define ERR_USERSDONTMATCH "Cant change mode for other users"	// 502
# define ERR_INVALIDKEY "Key is not well-formed"				// 525
# define ERR_INVALIDMODEPARAM "Invalid parameters" 				// 696

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
		std::map<int, std::string>	_pending;
		std::map<int, Registration>	_reg;
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
		bool		init(); // Creates socket, bind, listen
		void		run();	// Accept clients in a loop;
		void		newConnection(int fd, struct sockaddr_in address, socklen_t addr_size);
		void		disconnect(int fd);
		void		registerUser(int fd);

		// Getters && Setters
		void		setPassword(char *pass);
		void		setPort(int port);
		int			getPort(void) const;
		void		getMessage(char *buffer, int i);

		// Helpers
		void		welcomeUser(int i);
		void		cleanChannels(void);
		bool 		isValidNickname(std::string &str);
		bool		isValidUsername( std::string &str);
		void		sendMessage(int fd, int code, const std::string target, const std::string &msg);

		// Cmds
		void		exec_cmd(std::string line, int index);
		void		kick(std::vector<std::string>& av, int index);
		void		mode(std::vector<std::string>& av, int index);
		void		join(std::vector<std::string>& av, int index);
		void		part(std::vector<std::string>& av, int index);		
		void		topic(std::vector<std::string>& av, int index);
		void		invite(std::vector<std::string>& av, int index);
		void		privmsg(std::vector<std::string>& av, int index);
		int			check_cmd(std::string &line, std::vector<std::string>& av);

		// Mode
		void		handleInv(std::vector<std::string>& av, int index, int inv);
		void		handleTopic(std::vector<std::string>& av, int index, int inv);
		void		handleOperator(std::map<std::string, int> &operators, std::vector<std::string> av, int index);
		void		handleKey(std::vector<std::string>const &av, const int &i, const int &sign, int &offset, int &flag, int index);
		void		handleLimit(std::vector<std::string>const &av, const int &i, const int &sign, int &offset, int &flag, int index);
		void		getOperatorData(std::map<std::string, int> &operators, std::vector<std::string> av, int i, int sign, int &offset, int index);

		// Cmds Utils
		std::string	getClientInfo(int index);
		bool		isChannel(std::string name);
		int			getUserIndex(const std::string name) const;
		bool		doesUserExist(const std::string name) const;
		int			getChannelIndex(const std::string name) const;
		std::string	craftStringSpaces(const std::string str, int times);
		void		splitString(std::string line, std::vector<std::string>& av, char delim);
		void		broadcastMessage(Channel &channel, std::string message, int index, int flag);

		// Signals
		static int	getSignal(void);
		static void	setSignal(int signal);
};

#endif
