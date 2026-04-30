#include "../client.hpp"

Client::Client(std::string name, std::string nick): _name(name), _nick(nick)
{

}

~Client::Client(){}

const std::string Client::GetName() const
{
    return _name;
}

const std::string Client::GetNick() const
{
    return _nick;
}