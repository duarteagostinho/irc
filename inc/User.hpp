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

	public:

		std::string recvBuf;
		bool		registered;

		User();
		User(int fd, const std::string &nick, const std::string &user);
		User( User const & src );
		~User();

		int			getFd() const {return _fd;}
		std::string	getNickname() const {return _nickname;}
		std::string	getUsername() const {return _username;}

};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif /* ************************************************************ USER_H */
