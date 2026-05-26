#include "../User.hpp"

User::User(std::string name, std::string nick): _name(name), _nick(nick)
{

}

User::~User(){}

const std::string User::GetName() const
{
    return _name;
}

const std::string User::GetNick() const
{
    return _nick;
}