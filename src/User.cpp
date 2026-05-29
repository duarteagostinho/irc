#include "../User.hpp"

User::User(): _nick("Void"), _name("Void") {}

User::User(std::string name, std::string nick): _name(name), _nick(nick) {}

User:User(const User &other): _name(other._name), _nick(other._nick) {}

User::~User(){}

User &User::operator=(const User &other)
{
    if (*this != other)
    {
        _nick = other._nick;
        _name = other._name;
    }
    return *this;
}

const std::string& User::GetName() const {return _name;}

const std::string& User::GetNick() const {return _nick;}

void User::setNick(std::string nick): _nick(nick) {}

void User::setName(std::string name): _name(name) {}