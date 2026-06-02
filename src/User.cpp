#include "../User.hpp"

User::User(): _nick("Void"), _name("Void") {}

User::User(std::string nick, std::string name): _nick(nick), _name(name)  {}

User::User(const User &other): _nick(other._nick), _name(other._name) {}

User::~User(){}

User &User::operator=(const User &other)
{
	(void)other;
    return *this;
}

const std::string& User::GetName() const {return _name;}

const std::string& User::GetNick() const {return _nick;}