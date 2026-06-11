#include "../inc/Channel.hpp"

// DEL

// void Channel::print_users(void)
// {
//     std::cout << "user size: " << opFlag_users.size() << std::endl;

//     for (size_t i = 0; i < opFlag_users.size(); i++)
//     {
//         std::cout << "user: " << opFlag_users[i].second;
//         std::cout << ", op: ";
//         if (opFlag_users[i].first)
//             std::cout << "true";
//         else
//             std::cout << "false";
//         std::cout << std::endl;
//     }
// }

void Channel::print_users(void)
{
	for (std::map<std::string, bool>::iterator it = _users_op.begin(); it != _users_op.end(); it++)
	{
		std::cout << "user: " << it->first;
        std::cout << ", op: ";
        if (it->second)
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
		_users_op = other._users_op;
        _invited = other._invited;
    }
    return *this;
}

//Getters
const std::string Channel::getName() const 
{
    return _name;
}

// bool Channel::isOperator(const std::string name) const
// {

//     std::cout << "SIZE: " << opFlag_users.size() << "\n\n";
//     for (size_t i = 0; i < opFlag_users.size(); i++)
//     {
//         if (opFlag_users[i].second == name && opFlag_users[i].first == true)
//             return (true);
//     }
//     return (false);
// }









bool Channel::isOperator(const std::string nick) const
{
	std::map<std::string, bool>::const_iterator it = _users_op.find(nick);

	if (it != _users_op.end() && it->second == true)
		return (true);
	return (false);
}

bool Channel::isUserOnChannel(const std::string nick) const
{
	std::cout << "WTF IS GOING ON??" << std::endl;
	std::cout << nick << std::endl;

	if (_users_op.find(nick) != _users_op.end())
		return (true);
	return (false);
}












std::string Channel::usersFormated(void)
{
	std::string ret;

	for (std::map<std::string, bool>::const_iterator it = _users_op.begin(); it != _users_op.end(); it++)
	{
		if (it->second)
            ret += '@';
		ret += it->first + ' ';
	}
	return (ret);
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


// bool Channel::isUserOnChannel(const std::string nick) const
// {
//     (void)nick;
//     std::cout << "size: " << opFlag_users.size() << std::endl << std::endl;
// 	for (size_t i = 0; i < opFlag_users.size(); i++)
// 	{
// 		if (opFlag_users[i].second == nick)
// 			return (true);
// 	}

//     // for (operator_user_pair::iterator it = opFlag_users.begin(); it != opFlag_users.end(); it++){

//     //     std::cout << "FIRST: " << it->first << std::endl << "SECOND: " << it->second << std::endl;
//     //     if (it->second == nick)
//     //         return true;
//     // }
// 	return (false);
// }

// What was this for? Print users name list no?


void Channel::rmUser(std::string& nick)
{
    std::cout << "User removed from " << _name << std::endl; // BROADCAST TO CHANNEL !!!

	// std::map<std::string, bool>::const_iterator it = _users_op.find(nick);

	// if (_users_op.find(nick) != _users_op.end())
	// 	_users_op.erase(it);
	_users_op.erase(nick);
}

void Channel::addUser(const std::string& nick, bool flag)
{
	if (_users_op.find(nick) != _users_op.end())
		return ;
	_users_op.insert(std::make_pair(nick, flag));
}

void Channel::addOperator(std::string& nick)
{
	std::map<std::string, bool>::iterator it = _users_op.find(nick);

	if (_users_op.find(nick) != _users_op.end())
		it->second = true;

}

void Channel::rmOperator(std::string &nick)
{
	std::map<std::string, bool>::iterator it = _users_op.find(nick);

	if (_users_op.find(nick) != _users_op.end())
		it->second = false;
}
