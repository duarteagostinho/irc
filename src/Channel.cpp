#include "Channel.hpp"

Channel::Channel(): _name("Void"), _inviteOnly(false), _topicOp(false) _topic("Void"), _passOnly(false),
_pass("Void"), _userLimit(-1), _creator("Void"), _users(NULL), _operators(NULL) {}

Channel::Channel(std::string name, std::string& creator)
{
    _name = name;
    _inviteOnly = false;
    _topicOp = false;
    _topic = "";
    _passOnly = false;
    _pass = "";
    _userLimit = -1;
    _creator = creator;
    _users.push_back(creator);
    _operators.push_back(creator);
}

Channel::Channel(const Channel &other)
{
    _name = other._name;
    _inviteOnly = other._inviteOnly;
    _topicOp = other._topicOp;
    _topic = other._topic;
    _passOnly = other._passOnly;
    _pass = other._pass;
    _userLimit = other._userLimit;
    _creator = other._creator;
    _users = other._users;
    _operators = other._operators;
}

Channel::~Channel() {}

Channel &Channel::operator=(const Channel &other)
{
    if (this != &other)
    {
        _name = other._name;
    	_inviteOnly = other._inviteOnly;
        _topicOp = other._topicOp;
    	_topic = other._topic;
    	_passOnly = other._passOnly;
    	_pass = other._pass;
    	_userLimit = other._userLimit;
    	_creator = other._creator;
    	_users = other._users;
    	_operators = other._operators;
    }
    return *this;
}

//Getters
const std::string Channel::getName() const {return _name;}

const bool Channel::getInvite() const {return _inviteOnly;}

const bool Channel::getPassOnly() const {return _passOnly;}

const std::string Channel::getTopic() const {return _topic;}

const std::string Channel::getPass() const {return _pass;}

const int Channel::getUserLimit() const {return _userLimit;}

const std::string Channel::getCreator() const {return _creator;}

const std::vector<std::string> Channel::getUsers() const {return _users;}

const std::vector<std::string> Channel::getOperators() const {return _operators;}


//Setters
void Channel::setInvite(bool state) {_inviteOnly = state;}

void Channel::setPassOnly(bool state) {_passOnly = state;}

void Channel::setTopic(std::string topic) {_topic = topic;}

void Channel::setPass(std::string pass) {_pass = pass;}

void Channel::setUserLmit(int limit) {_userLimit = limit;}

void Channel::addUser(std::string& nick)
{
    if ((size_t pos = _users.find(nick)) != std::string::npos)
    {
        if (_userLimit == -1)
        {
            _users.push_back(nick);
            std::cout << "User added to " << _name << std::endl;
        }
        else if (_userLimit > _users.size())
        {
            _users.push_back(nick);
            std::cout << "User added to " << _name << std::endl;
        }
        else
        {
            std::cout << "No space on the channel" << std::endl;
            return ;
        }
    }
    else
        return ;
}

void Channel::rmUser(std::string& nick)
{
    if ((size_t pos = _users.find(nick)) != std::string::npos)
    {
        if (_users[pos] == _creator)
        {
            std::cout << "That User can't be removed from the channel " << _name << " has he is the creator" << std::endl;
            return ;
        }
        else
        {
            _users.erase(_users.begin() + pos);
            std::cout << "User removed from " << _name << std::endl;
            return ;
        }
    }
}

void Channel::addOperator(std::string& nick)
{
    if ((size_t pos = _operators.find(nick)) != std::string::npos)
    {
        _operators.push_back(nick);
        std::cout << "User added has an operator to " << _name << std::endl;
    }
    else
        return ;
}

void Channel::rmOperator(std::string &nick)
{
    if (nick == _creator)
    {
        std::cout << "That User can't be removed from operator status has he is the creator of the channel " << _name << std::endl;
        return ;
    }
    if ((size_t pos = _operators.find(nick)) != std::string::npos)
    {
        _operators.erase(_operators.begin() + pos);
        std::cout << "User removed has an operator from " << _name << std::endl;
        return ;
    }
    else
        return ;
}