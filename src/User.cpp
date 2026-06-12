#include "../inc/User.hpp"

// Methods
void	User::Register(void)
{
	_registered = true;
}

bool	User::getRegistration(void) const
{
	return _registered;
}

int	User::getFd() const 
{
	return _fd;
}

const std::string&	User::getNickname() const
{
	return _nickname;
}

const std::string&	User::getUsername() const
{
	return _username;
}


std::string User::getAddress(void)
{
	char *str = inet_ntoa(_address.sin_addr);
	return (str);
}


// Orthodox Cannonical Form
User::User()
: _fd(-1), _nickname(""), _username(""), _registered(false)
{

}

User::User(int fd, std::string nick, std::string name)
: _fd(fd), _nickname(nick), _username(name),_registered(false)
{

}

User::User(int fd, std::string nick, std::string name, struct sockaddr_in	addr, socklen_t size)
: _fd(fd), _nickname(nick), _username(name), _address(addr), _addr_size(size), _registered(false)
{

}

User::User(const User &other)
{
	*this = other;
}

User::~User()
{

}

User &User::operator=(const User &src) {
    if (this != &src) {
		this->_fd = src._fd;
    	this->_username = src._username;
		this->_nickname = src._nickname;
		this->_address = src._address;
		this->_addr_size = src._addr_size;
		this->_registered = src._registered;
    }
    return *this;
}

