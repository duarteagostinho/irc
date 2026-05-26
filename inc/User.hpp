#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>

class User
{
	private:

	std::string _nickname;
	std::string _username;

	public:

		User();
		User(std::string nick, std::string user);
		User( User const & src );
		~User();

		User &		operator=( User const & rhs );

};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif /* ************************************************************ USER_H */