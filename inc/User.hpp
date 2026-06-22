#ifndef USER_HPP
# define USER_HPP

# include <string>
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

		// Getter
		int					getFd() const;
		std::string			getAddress(void);
		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		bool				getRegistration(void) const;

		// Setter
		void				setNickname(std::string nick);
		void				setUsername(std::string user);

		// Utils
		void				Register(void);
};

#endif /* ************************************************************ USER_H */
