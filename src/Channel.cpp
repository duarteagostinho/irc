#include "Channel.hpp"

Channel::Channel(): _inviteOnly(false), _topic("Void"), _passOnly(false),
_pass("Void"), _userLimit(-1), _creator("Void"), _users(void), _operators(void) {}

Channel::Channel(bool inv, std::string topic, bool passO, std::string pass, int userL, std::string creator, std::vector<std::string> users, std::vector<std::string> op)
{
    _inv = inv;
    _topic = topic;
    _passOnly = passO;
    _pass = pass;
    _userLimit = userL;
    _creator = creator;
    _users = users;
    _operators = op;
}

Channel::Channel(const Channel &other)
{
    _inv = other.inv;
    _topic = other.topic;
    _passOnly = other.passO;
    _pass = other.pass;
    _userLimit = other.userL;
    _creator = other.creator;
    _users = other.users;
    _operators = other.op;
}

Channel::~Channel() {}

Channel &Channel::operator=(const Cannel &other)
{
    if (*this != other)
    {
        _inv = other.inv;
        _topic = other.topic;
        _passOnly = other.passO;
        _pass = other.pass;
        _userLimit = other.userL;
        _creator = other.creator;
        _users = other.users;
        _operators = other.op;
    }
    return *this;
}

const bool Channel::getInvite() const {return _inviteOnly;}

const bool Channel::getPassOnly() const {return _passOnly;}

const std::string Channel::getTopic() const {return _topic;}

const std::string Channel::getPass() const {return _pass;}

const int Channel::getUserLimit() const {return _userLimit;}

const std::string Channel::getCreator() const {return _creator;}

const int Channel::getUserLimit() const {return _userLimit;}

const std::vector<std::string> Channel::getUsers() const {return _users;}

const std::vector<std::string> Channel::getOperators() const {return _operators;}

void Channel::setInvite(bool state) {_inviteOnly = state;}

void Channel::setPassOnly(bool state) {_passOnly = state;}

void Channel::setTopic(std::string topic) {_topic = topic;}

void Channel::setPass(std::string pass) {_pass = pass;}

void Channel::setUserLmit(int limit) {_userLimit = limit;}

void Channel::addUser(std::string& nick)
{
    if (_userLimit > _users.size())
    {
        _users.pushback(nick);
    }
    else
    {

    }
}

void Channel::rmUser(std::string& nick)
{
    for(size_t i = 0; i < _users.size(), i++)
    {
        if (_users[i] == nick)
        {
            _users.erase(_users.begin() + i)
            break ;
        }
    }
    if (i == _users.size())
    {
        //throw or 
    }
}

void Channel::addOperator(std::string& nick)
{
    _operators.pushback(nick);
}

void Channel::rmOperator()
{

}