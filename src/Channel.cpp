#include "../inc/Channel.hpp"

// DEL
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

// UTILS
bool Channel::isOperator(const std::string nick) const
{
	std::map<std::string, bool>::const_iterator it = _users_op.find(nick);

	if (it != _users_op.end() && it->second == true)
		return (true);
	return (false);
}

bool Channel::isUserOnChannel(const std::string nick) const
{
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

bool Channel::hasInvite(const std::string nick)
{
	std::vector<std::string>::iterator it = std::find(_invited.begin(), _invited.end(), nick);
	if (it != _invited.end())
	{
		_invited.erase(it);
		return (true);
	}
	return (false);
}

void Channel::rmUser(const std::string& nick)
{
	_users_op.erase(nick);
}

void Channel::addUser(const std::string& nick, bool flag)
{
	if (_users_op.find(nick) != _users_op.end())
		return ;
	_users_op.insert(std::make_pair(nick, flag));
}

void Channel::addOperator(const std::string& nick)
{
	std::map<std::string, bool>::iterator it = _users_op.find(nick);

	if (_users_op.find(nick) != _users_op.end())
		it->second = true;
}

void Channel::rmOperator(const std::string &nick)
{
	std::map<std::string, bool>::iterator it = _users_op.find(nick);

	if (_users_op.find(nick) != _users_op.end())
		it->second = false;
}

void Channel::addInvite(const std::string& nick)
{
	for (size_t i = 0; i < _invited.size(); i++)
	{
		if (_invited[i] == nick)
			return ;
	}
	_invited.push_back(nick);
}

void Channel::rmInvite(const std::string& nick)
{
	for (size_t i = 0; i < _invited.size(); i++)
	{
		if (_invited[i] == nick)
			_invited.erase(_invited.begin() + i);
	}
}

std::string Channel::printMode(void)
{
	// THIS IS SOOO TRASH
	std::string str = "+";

	if (getTopicMode() == true)
		str += "t";
	if (getInviteMode() == true)
		str += "i";
	if (getLimitMode() == true)
		str += "l";
	if (getKeyMode() == true)
		str += "k";
	if (getLimitMode() == true)
	{
		std::stringstream ss;

		ss << getMaxUsers();
		str += " " + ss.str();
	}
	if (getKeyMode() == true)
		str += " " + getPass();
	return (str);
}

// Orthodox Cannonical Form
Channel::Channel()
: _name(""), _topic(""), _pass(""), _creation(0), _new_topic(0), _inv_flag(false),
_topc_flag(true), _key_flag(false), _optr_flag(false), _lim_flag(false), _max_users(0)
{

}

Channel::Channel(std::string name)
: _name(name), _topic(""), _pass(""), _creation(0), _new_topic(0), _inv_flag(false),
_topc_flag(true), _key_flag(false), _optr_flag(false), _lim_flag(false), _max_users(0)
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
		_users_op = other._users_op;
        _invited = other._invited;
		_inv_flag = other._inv_flag;
		_topc_flag = other._topc_flag;
		_key_flag = other._key_flag;
		_optr_flag = other._optr_flag;
		_lim_flag = other._lim_flag;
		_creation = other._creation;
		_max_users = other._max_users;
    }
    return *this;
}

//Getters
const std::string Channel::getName() const 
{
    return (_name);
}

bool Channel::getInviteMode() const
{
	return (_inv_flag);
}

bool Channel::getTopicMode() const
{
	return (_topc_flag);
}

bool Channel::getKeyMode() const
{
	return (_key_flag);
}

bool Channel::getOperatorMode() const
{
	return (_optr_flag);
}

bool Channel::getLimitMode() const
{
	return (_lim_flag);
}

const std::string Channel::getTopic() const
{
    return (_topic);
}

const std::string Channel::getPass() const
{
    return (_pass);
}

std::string Channel::getChannelTime(void) const
{
	std::stringstream ss;

	ss << _creation;
	return (ss.str());
}

std::string Channel::getTopicTime(void) const
{
	std::stringstream ss;

	ss << _new_topic;
	return (ss.str());
}

std::string Channel::getTopicMaker(void) const
{
	return (_topic_creator);
}

int Channel::getMaxUsers(void) const
{
	return (_max_users);
}

// //Setters
void Channel::setTopic(const std::string topic)
{
	_topic = topic;
}

void Channel::setInviteMode(bool mode)
{
	_inv_flag = mode;
}

void Channel::setTopicMode(bool mode)
{
	_topc_flag = mode;
}

void Channel::setKeyMode(bool mode)
{
	_key_flag = mode;
}

void Channel::setOperatorMode(bool mode)
{
	_optr_flag = mode;
}

void Channel::setLimitMode(bool mode)
{
	_lim_flag = mode;
}


void Channel::setChannelTime(void)
{
	_creation = time(NULL);
}

void Channel::setTopicTime(void)
{
	_new_topic = time(NULL);
}

void Channel::setTopicMaker(const std::string &str)
{
	_topic_creator = str;
}

void Channel::setPass(const std::string &str)
{
	_pass = str;
}

void Channel::setMaxUsers(int &num)
{
	_max_users = num;
}
