#include "../inc/User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User() : _fd(-1), _nickname(""), _username("") 
{
}

User::User(int fd, const std::string &nick, const std::string &user)
	: _fd(fd), _nickname(nick), _username(user)
{	
}

// User &User::operator=(const User &src) {
//     if (this != &src) {
//     	this->_username = src._username;
// 		this->_nickname = src._nickname;
// 		this->_fd = src._fd;
//     }
//     return *this;
// }
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
//
User::~User()
{
}

//
/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// User &				User::operator=( User const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }
//
// std::ostream &			operator<<( std::ostream & o, User const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }
//

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
