#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

class User
{
	private:
		int 				_fd;
		std::string 		_nickname;
		std::string 		_username;
		struct sockaddr_in	_address;
		socklen_t 			_addr_size;
		bool				_registered;

	public:

		std::string recvBuf;

		User();
		User(int fd, std::string nick, std::string name);
		User(int fd, std::string nick, std::string user, struct sockaddr_in	addr, socklen_t size);
		User(User const & src);
		~User();
		User &operator=(const User &src);


		int			getFd() const;
		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		void	setNickname(std::string nick);
		void	setUsername(std::string user);
		void	Register(void);
		bool	getRegistration(void) const;

		std::string getAddress(void);

};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif /* ************************************************************ USER_H */
