#include "../inc/User.hpp"

User::User(): _fd(-1), _nickname("Void"), _username("Void") {}

User::User(int fd, std::string nick, std::string name): _fd(fd), _nickname(nick), _username(name)  {}

User::User(const User &other): _nickname(other._nickname), _username(other._username) {}

User::~User(){}

User &User::operator=(const User &src) {
    if (this != &src) {
    	this->_username = src._username;
		this->_nickname = src._nickname;
		this->_fd = src._fd;
    }
    return *this;
}
