#include "Channel.hpp"

Channel::Channel(): _name("Void"), _inviteOnly(false), _topic("Void"), _passOnly(false),
_pass("Void"), _userLimit(-1), _creator("Void"), _users(NULL), _operators(NULL) {}

Channel::Channel(std::string name, std::string& creator)
{
    _name = name;
    _inviteOnly = false;
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

const std::string Channel::getName() const {return _name;}

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
    //falta fazer search antes para saber se o user ja esta no channel
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
        //ainda nao sei como vamos lidar com os erros
    }
}

void Channel::rmUser(std::string& nick)
{
	size_t i = 0;
    for(i; i < _users.size(); i++)
    {
        if (_users[i] == _creator)
        {
            std::cout << "That User can't be removed from the channel " << _name << " has he is the creator" << std::endl;
            //ainda nao sei como vamos lidar com isto
        }
        if (_users[i] == nick)
        {
            _users.erase(_users.begin() + i);
            std::cout << "User removed from " << _name << std::endl;
            break ;
        }
    }
    if (i == _users.size())
    {
        //throw or ainda nao sei
        //isto no caso do user nao existir no channel
    }
}

void Channel::addOperator(std::string& nick)
{
    //falta fazer search antes para saber se o user ja esta no channel
    if (_userLimit == -1)
    {
        _operators.push_back(nick);
        std::cout << "User added has an operator to " << _name << std::endl;
    }
    else if (_userLimit > _operators.size())
    {
        _operators.push_back(nick);
        std::cout << "User added has an operator to " << _name << std::endl;
    }
    else
    {
        //ainda nao sei como vamos lidar com os erros
    }
}

void Channel::rmOperator(std::string &nick)
{
	size_t i = 0;
    //falta fazer search antes para saber se ja existe no channel como operator
    for(i; i < _operators.size(); i++)
    {
        if (_operators[i] == nick)
        {
            std::cout << "That User can't be removed from operator status has he is the creator of the channel " << _name << std::endl;
            //ainda nao sei como vamos lidar com isto
        }
        if (_operators[i] == nick)
        {
            _operators.erase(_operators.begin() + i);
            std::cout << "User removed has an operator from " << _name << std::endl;
            break ;
        }
    }
    if (i == _operators.size())
    {
        //throw or ainda nao sei
        //isto no caso do user nao existir no channel
    }
}