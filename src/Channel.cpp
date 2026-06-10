#include "../inc/Channel.hpp"

// DEL

void Channel::print_users(void)
{
    std::cout << "user size: " << opFlag_users.size() << std::endl;

    for (size_t i = 0; i < opFlag_users.size(); i++)
    {
        std::cout << "user: " << opFlag_users[i].second;
        std::cout << ", op: ";
        if (opFlag_users[i].first)
            std::cout << "true";
        else
            std::cout << "false";
        std::cout << std::endl;
    }
}

// Orthodox Cannonical Form

Channel::Channel()
: _name(""), _topic(""), _pass("")
{

}

Channel::Channel(std::string name)
: _name(name), _topic(""), _pass("")
{

}

Channel::Channel(const Channel &other)
{
    *this = other;
}

Channel::~Channel()
{

}

Channel &Channel::operator=(const Channel &other)
{
    if (this != &other)
    {
        _name = other._name;
        _topic = other._topic;
		_pass = other._pass;
        opFlag_users = other.opFlag_users;
        _invited = other._invited;
    }
    return *this;
}

//Getters
const std::string Channel::getName() const 
{
    return _name;
}

bool Channel::isOperator(std::string name) const
{
    for (size_t i = 0; i < opFlag_users.size(); i++)
    {
        if (opFlag_users[i].second == name && opFlag_users[i].first == true)
            return (true);
    }
    return (false);
}

bool Channel::hasInvite(std::string nick)
{
	std::vector<std::string>::iterator it = std::find(_invited.begin(), _invited.end(), nick);
	if (it != _invited.end())
	{
		_invited.erase(it);
		return (true);
	}
	return (false);
}



// const bool Channel::getInvite() const {return _inviteOnly;}

// const bool Channel::getPassOnly() const {return _passOnly;}

const std::string Channel::printTopic(void) const
{
    if (_topic == "")
        return (getName() + " :No Topic is set.\r\n");
    else
        return ("Topic for " + getName() + " is: " + getTopic() + "\r\n");
}


const std::string Channel::getTopic() const
{
    return _topic;
}

const std::string Channel::getPass() const
{
    return _pass;
}

// const int Channel::getUserLimit() const {return _userLimit;}

// const std::string Channel::getCreator() const {return _creator;}

// const std::vector<std::string> Channel::getUsers() const {return _users;}




// //Setters
// void Channel::setInvite(bool state) {_inviteOnly = state;}

// void Channel::setPassOnly(bool state) {_passOnly = state;}

void Channel::setTopic(std::string topic) {_topic = topic;}

// void Channel::setPass(std::string pass) {_pass = pass;}

// void Channel::setUserLmit(int limit) {_userLimit = limit;}


bool Channel::isUserOnChannel(std::string nick)
{
	for (size_t i = 0; i < opFlag_users.size(); i++)
	{
		if (opFlag_users[i].second == nick)
			return (true);
	}
	return (false);
}

// What was this for? Print users name list no?
std::string Channel::usersFormated(void)
{
    std::string ret;

    for (size_t i = 0; i < opFlag_users.size(); i++)
    {
        
    }
    return (ret);
}

void Channel::rmUser(std::string& nick)
{
    size_t i = 0;
    for ( ; i < opFlag_users.size(); i++)
    {
        if (opFlag_users[i].second == nick)
            break ;
    }
    opFlag_users.erase(opFlag_users.begin() + i);
    std::cout << "User removed from " << _name << std::endl; // BROADCAST TO CHANNEL !!!
}

void Channel::addUser(const std::string& nick, int flag)
{
    std::pair<int, std::string> new_user;

    for (size_t i = 0; i < opFlag_users.size(); i++)
    {
        if (opFlag_users[i].second == nick)
            return ;
    }
    new_user.first = flag;
    new_user.second = nick;
    opFlag_users.push_back(new_user);
}

void Channel::addOperator(std::string& nick)
{
    for (size_t i = 0; i < opFlag_users.size(); i++)
    {
        if (opFlag_users[i].second == nick)
            opFlag_users[i].first = 1;
    }
}

void Channel::rmOperator(std::string &nick)
{
    for (size_t i = 0; i < opFlag_users.size(); i++)
    {
        if (opFlag_users[i].second == nick)
            opFlag_users[i].first = 0;
    }
}
