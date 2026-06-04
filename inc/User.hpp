#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>

class User
{
	private:
	
		int			_fd;
		std::string _nickname;
		std::string _username;

		std::string channel;

	public:

		std::string recvBuf;
		bool		registered;

		User();
		User(int fd, std::string nick, std::string user);
		User( User const & src );
		~User();

		User &operator=(const User &src);
		int			getFd() const {return _fd;}
		const std::string&	getNickname() const {return _nickname;}
		const std::string&	getUsername() const {return _username;}

};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif /* ************************************************************ USER_H */
