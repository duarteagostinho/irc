/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/18 19:17:27 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

int Server::check_cmd(std::string &line, std::vector<std::string>& av)
{
	// Clean the string from the carriage return
	std::string to_del = "\r\n";
	size_t pos = line.find(to_del);
	if (pos != line.npos)
		line.erase(pos, to_del.length());
	
	// Create an argv from the data
	splitString(line, av, ' ');
		
	// If no line is sent, return err
	if (av.empty())
		return (-1) ;
	
	// Check the first index for the chosen commands
	std::string accepted[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN", "PRIVMSG", "PART", "WHO"};
	const size_t size = sizeof(accepted) / sizeof(accepted[0]);
	std::cout << size << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		if (accepted[i] == av[0])
		{
			av[0] = line;
			return (i);
		}	
	}
	return (-1);
}

// MISSING: WHO
void Server::exec_cmd(std::string line, int index)
{
	std::vector<std::string> av;
	int cmds = check_cmd(line, av);
	for (size_t i = 0; i < av.size(); i++)
		std::cout << "[" << av[i] << "]" << std::endl;
	switch(cmds)
	{
		case 0:
			kick(av, index);
			break;
		case 1:
			invite(av, index);
			break;
		case 2:
			topic(av, index);
			break;
		case 3:
			mode(av, index);
			break;
		case 4:
			join(av, index);
			break;
		case 5:
			privmsg(av, index);
			break;
		case 6:
			part(av, index);
			break;
		case 7:
			who(av, index);
			break ;
		default: // HEXCHAT SENDS MODE <CHANNEL> AND WHO <CHANNEL> HANDLE IT
			std::string response = ":irc.server 421 " + _users[index].getNickname() + " :Unknown command\r\n";
//			sendMessage(_fds[index].fd, 421, _users[index].getNickname() + " " + av[0], ERR_UNKNOWNCOMMAND);
	}
	line.clear();
}

void Server::join(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel,{channel}
	// av[2] = pass,{pass}

	std::string pass;
	std::string response;
	std::vector<std::string> ch_av;
	std::vector<std::string> passwords;
	
	if (av.size() == 1) // Missing arguments
	{
		sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " JOIN", ERR_NEEDMOREPARAMS);
		return ;
	}
	splitString(av[1], ch_av, ',');
	if (av.size() > 2)
		splitString(av[2], passwords, ',');
	for (size_t i = 0; i < ch_av.size(); i++)
	{
		pass = "";
		if (ch_av[i][0] != '#')
			sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + ch_av[i], ERR_NOSUCHCHANNEL);
		else if (isChannel(ch_av[i])) // Valid channel, we either enther of fail to enter
		{
			int ch_i = getChannelIndex(ch_av[i]);

			if (passwords.size() > i)
				pass = passwords[i];
	
			if (!_channels[ch_i].hasInvite(_users[index].getNickname()))
			{
				if (_channels[ch_i].getKeyMode() && _channels[ch_i].getPass() != pass)
				{
					sendMessage(_fds[index].fd, 475, _users[index].getNickname() + " " + ch_av[i], ERR_BADCHANNELKEY);
					continue ;
				}
				else if (_channels[ch_i].getInviteMode())
				{
					sendMessage(_fds[index].fd, 473, _users[index].getNickname() + " " + ch_av[i], ERR_INVITEONLYCHAN);
					continue ;
				}
				else if (_channels[ch_i].getCount() >= _channels[ch_i].getMaxUsers() && _channels[ch_i].getLimitMode())
				{
					sendMessage(_fds[index].fd, 471, _users[index].getNickname() + " " + ch_av[i], ERR_CHANNELISFULL);
					continue ;
				}
			} // If invited, no pass or pass was correct, enter channel

			_channels[ch_i].addUser(_users[index].getNickname(), 0);
			_channels[ch_i].incrementCount();
			_channels[ch_i].rmInvite(_users[index].getNickname());

			response = getClientInfo(index) + " JOIN " + ch_av[i] + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1);

			if (_channels[ch_i].getTopic() != "")
			{
				response = ":irc.server 332 " + _users[index].getNickname() + " " + ch_av[i] + " :" + _channels[ch_i].getTopic() + "\r\n";
				broadcastMessage(_channels[ch_i], response, index, 1);

				response = ":irc.server 333 " + _users[index].getNickname() + " "
							+ ch_av[i] + " " + getClientInfo(index) + " " + _channels[ch_i].getTopicTime() + "\r\n";
				broadcastMessage(_channels[ch_i], response, index, 1);
			}
			sendMessage(_fds[index].fd, 353, _users[index].getNickname() + " = " + ch_av[i], _channels[ch_i].usersFormated());
			sendMessage(_fds[index].fd, 366, _users[index].getNickname() + " " + ch_av[i], RPL_ENDOFNAMES);
		}
		else // New channel creation
		{
			if (ch_av[i].size() == 1 || ch_av[i].size() > 30)
			{
				sendMessage(_fds[index].fd, 476, _users[index].getNickname() + " " + ch_av[i], ERR_BADCHANMASK);
				continue ;
			}
			int x = _channels.size();
			_channels.push_back(Channel(ch_av[i]));
			_channels[x].addUser(_users[index].getNickname(), 1);
			_channels[x].setChannelTime();
			
			response = getClientInfo(index) + " JOIN " + ch_av[i] + "\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);			
			sendMessage(_fds[index].fd, 353, _users[index].getNickname() + " = " + ch_av[i], "@" + _users[index].getNickname());			
			sendMessage(_fds[index].fd, 366, _users[index].getNickname() + " " + ch_av[i], "End of /NAMES list");
		}
	}
}

void Server::kick(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel
	// av[2] = target
	// av[3] = reason
	
	std::string response;
	int ch_i = getChannelIndex(av[1]);
	
	if (av.size() < 3) // Missing arguments
		sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " KICK", ERR_NEEDMOREPARAMS);
	else if (!isChannel(av[1]))
		sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + av[1], ERR_NOSUCHCHANNEL);
	else if (!_channels[ch_i].isOperator(_users[index].getNickname()))
		sendMessage(_fds[index].fd, 482, _users[index].getNickname() + " " + av[1], ERR_CHANOPRIVSNEEDED);
	else if (!_channels[ch_i].isUserOnChannel(av[2]))
		sendMessage(_fds[index].fd, 401, _users[index].getNickname() + " " + av[2], ERR_NOSUCHNICK);
	else
	{
		std::string reason;
		if (av.size() > 3) // there is reason
			reason = craftStringSpaces(av[0], 3);
		else
			reason = _users[index].getNickname();
		response = getClientInfo(index) + " KICK " + av[1] + " " + av[2]  + " :" + reason + "\r\n";
		broadcastMessage(_channels[ch_i], response, index, 1);
		_channels[ch_i].rmUser(av[2]);
		_channels[ch_i].rmOperator(av[2]);
		_channels[ch_i].decrementCount();
	}
}

void Server::topic(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel,{channel}
	// av[2+] = rest of the message (NOT USED)
	
	std::string response;
	std::vector<std::string> ch_av;

	if (av.size() == 1) // Missing arguments
	{
		sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " TOPIC", ERR_NEEDMOREPARAMS);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], ch_av, ',');
	for (size_t i = 0; i < ch_av.size(); i++)
	{
		int ch_i = getChannelIndex(ch_av[i]);
		if (!isChannel(ch_av[i])) // could check for ch_i == -1 here, left this way cause it's easier to understand
			sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + ch_av[i], ERR_NOSUCHCHANNEL);
		else if (av.size() == 2) // Print topic
		{
			if (_channels[ch_i].getTopic() == "") // If Topic is not set
				sendMessage(_fds[index].fd, 331, _users[index].getNickname() + " " + ch_av[i], RPL_NOTOPIC);
			else
			{
				sendMessage(_fds[index].fd, 332, _users[index].getNickname() + " " + ch_av[i], _channels[ch_i].getTopic());				
				sendMessage(_fds[index].fd, 333, _users[index].getNickname() + " " + ch_av[i], _channels[ch_i].getTopicMaker() + " " + _channels[ch_i].getTopicTime());
			}
		}
		else if (!_channels[ch_i].isOperator(_users[index].getNickname()) && _channels[ch_i].getTopicMode())
			sendMessage(_fds[index].fd, 482, _users[index].getNickname() + " " + ch_av[i], ERR_CHANOPRIVSNEEDED);
		else
		{
			std::string new_topic = craftStringSpaces(av[0], 2);
			_channels[ch_i].setTopicTime();
			_channels[ch_i].setTopicMaker(getClientInfo(index));
			_channels[ch_i].setTopic(new_topic);
			response = getClientInfo(index) + " TOPIC " + av[1] + " :" + new_topic + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1);
		}
	}
}

void Server::invite(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = nickname
	// av[2] = channel
	
	std::string response;
	int ch_i = getChannelIndex(av[2]);

	if (av.size() == 1) // Missing arguments
		sendMessage(_fds[index].fd, 337, _users[index].getNickname(), RPL_ENDOFINVITELIST);
	else if (av.size() < 3)
		sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " INVITE", ERR_NEEDMOREPARAMS);
	else if (!doesUserExist(av[1]))
		sendMessage(_fds[index].fd, 401, _users[index].getNickname() + " " + av[2], ERR_NOSUCHNICK);
	else if (!isChannel(av[2]))
		sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + av[2], ERR_NOSUCHCHANNEL);
	else if (_channels[ch_i].isUserOnChannel(av[1]))
		sendMessage(_fds[index].fd, 443, _users[index].getNickname() + " " + av[1] + " " + av[2], ERR_USERONCHANNEL);
	else if (!_channels[ch_i].isOperator(_users[index].getNickname()))
		sendMessage(_fds[index].fd, 482, _users[index].getNickname() + " " + av[2], ERR_CHANOPRIVSNEEDED);
	else
	{
		_channels[ch_i].addInvite(av[1]);
		// Sender
		std::string sender_response = ":irc.server 341 " + _users[index].getNickname() + " " + av[1] + " " + av[2] + "\r\n";
		send(_fds[index].fd, sender_response.c_str(), sender_response.size(), 0);

		// User Invited
		std::string client_response = getClientInfo(index) + " INVITE " + av[1] + " " + av[2] + "\r\n";
		send(_fds[getUserIndex(av[1])].fd, client_response.c_str(), client_response.size(), 0);
	}
}

void	Server::privmsg(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = target,{target}
	// av[2+] = rest of the message (NOT USED)
	
	std::string response;
	std::vector<std::string> targets;

	if (av.size() < 3) // Missing arguments
	{
		std::string response;
		if (av.size() == 1)
			sendMessage(_fds[index].fd, 411, _users[index].getNickname(), ERR_NORECIPIENT);
		else
			sendMessage(_fds[index].fd, 412, _users[index].getNickname(), ERR_NOTEXTTOSEND);
		send(_fds[index].fd, response.c_str(), response.size(), 0);
		return ;
	}
	splitString(av[1], targets, ',');
	for (size_t i = 0; i < targets.size(); i++)
	{ 
		bool channel = isChannel(targets[i]);
		bool user = doesUserExist(targets[i]);
		
		if (!channel && !user) // Error for sure
		{
			if (targets[i][0] == '#')
				sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + targets[i], ERR_NOSUCHCHANNEL);
			else
				sendMessage(_fds[index].fd, 401, _users[index].getNickname() + " " + targets[i], ERR_NOSUCHNICK);
			continue ;
		}
		response = getClientInfo(index) + " PRIVMSG " + targets[i] + " :" + craftStringSpaces(av[0], 2) + "\r\n";
		if (user)
			send(_users[getUserIndex(targets[i])].getFd(), response.c_str(), response.size(), 0);		
		else if (targets.size() > 1 || !_channels[getChannelIndex(targets[i])].isUserOnChannel(_users[index].getNickname()))
			// Check if sender is in the channel or if there are multiple args
			sendMessage(_fds[index].fd, 404, _users[index].getNickname() + " " + targets[i], ERR_CANNOTSENDTOCHAN);
		else
			broadcastMessage(_channels[getChannelIndex(targets[i])], response, index, 0);
	}
}

void Server::part(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel,{channel}
	// av[2+] = rest of the message (NOT USED)
	
	std::string response;
	std::vector<std::string> ch_av;

	if (av.size() == 1) // Missing arguments
	{
		sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " PART", ERR_NEEDMOREPARAMS);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], ch_av, ',');
	for (size_t i = 0; i < ch_av.size(); i++)
	{
		int ch_i = getChannelIndex(ch_av[i]);
		if (!isChannel(ch_av[i]))
			sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + ch_av[i], ERR_NOSUCHCHANNEL);
		else if (av.size() == 2) // Just leave
		{
			response = getClientInfo(index) + " PART " + ch_av[i] + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1);
		}
		else
		{
			response = getClientInfo(index) + " PART " + ch_av[i] + " :" + craftStringSpaces(av[0], 2) + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1);
		}
		_channels[ch_i].rmUser(_users[index].getNickname());
		_channels[ch_i].decrementCount();
	}
}

/*
	Simple split that stores on av the line splitted by the delim character.
*/
void Server::splitString(std::string line, std::vector<std::string>& av, char delim)
{
	std::stringstream split(line);
	std::string token;
	while (std::getline(split, token, delim))
		av.push_back(token);
}

/*
	Re create the string if the ':' character is in the message, to keep the space, otherwise returns last word.
*/
std::string Server::craftStringSpaces(std::string str, int times)
{
	std::stringstream ss(str);
	std::string ret;

	for (int i = 0; i < times; i++)
		ss >> ret;
	getline(ss >> std::ws, ret);
	if (ret[0] == ':')
		return (&ret[1]);

	ss.str("");
	ss.clear();
	ss << str;
	ss >> ret >> ret;
	while (std::getline(ss, ret, ' '))
		;
	return (ret);
}

/*
	Gives back a string in the format <nick>!<user>@ip.
*/
std::string Server::getClientInfo(int index)
{
	std::string ret = ":" + _users[index].getNickname() + "!" + _users[index].getUsername();
	ret = ret + "@" + _users[index].getAddress();
	return (ret);
}

/*
	Broadcast the message to all users on the channel, also sends it back to the sender if flag is on.
*/
void Server::broadcastMessage(Channel &channel, std::string message, int index, int flag)
{
	if (flag)
		send(_fds[index].fd, message.c_str(), message.size(), 0);
	for (size_t i = 1; i < _fds.size(); i++)
	{
		if (_fds[i].fd != _fds[index].fd && channel.isUserOnChannel(_users[i].getNickname()))
				send(_fds[i].fd, message.c_str(), message.size(), 0);
	}
}


/////////////////////////// MODE ///////////////////////////////


static int adv_peek(std::string str, size_t &j)
{
	size_t valid_j;

	while ((str[j] == '+' || str[j] == '-') && str[j])
	{
		valid_j = j;
		j++;
	}
	j = valid_j;
	if (str[j] == '+')
		return (1);
	return (-1);
}

void Server::handleInv(std::vector<std::string>& av, int index, int inv)
{
	int ch = getChannelIndex(av[1]);
	std::string response;
	
	if (inv > 0 && !_channels[ch].getInviteMode())
	{
		_channels[ch].setInviteMode(true);
		response = getClientInfo(index) + " MODE " + av[1] + " +i\r\n";
		broadcastMessage(_channels[ch], response, index, 1); // DO IT IN THE END !!!
	}
	else if (inv < 0 && _channels[ch].getInviteMode())
	{
		_channels[ch].setInviteMode(false);
		response = getClientInfo(index) + " MODE " + av[1] + " -i\r\n";
		broadcastMessage(_channels[ch], response, index, 1); // DO IT IN THE END !!!
	}
}

void Server::handleTopic(std::vector<std::string>& av, int index, int topic)
{
	int ch = getChannelIndex(av[1]);
	std::string response;
	
	if (topic > 0 && !_channels[ch].getTopicMode())
	{
		_channels[ch].setTopicMode(true);
		response = getClientInfo(index) + " MODE " + av[1] + " +t\r\n";
		broadcastMessage(_channels[ch], response, index, 1); // DO IT IN THE END !!!
	}
	else if (topic < 0 && _channels[ch].getTopicMode())
	{
		_channels[ch].setTopicMode(false);
		response = getClientInfo(index) + " MODE " + av[1] + " -t\r\n";
		broadcastMessage(_channels[ch], response, index, 1); // DO IT IN THE END !!!
	}
}

void Server::handleLimit(std::vector<std::string>const &av, const int &i, const int &sign, int &offset, int &flag, int index)
{
	int result;
	char *end;
	std::string response;

	if (flag)
		return;
	int ch_i = getChannelIndex(av[1]);
	if (sign < 0 && _channels[ch_i].getLimitMode()) // remove doesn't need arguments
	{
		_channels[ch_i].setLimitMode(false);
		response = getClientInfo(index) + " MODE " + av[1] + " -l\r\n";
		broadcastMessage(_channels[ch_i], response, index, 1); // DO IT IN THE END !!!
	}
	else
	{
		if (static_cast<size_t>(i + offset) >= av.size())
			sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " MODE +l", ERR_NEEDMOREPARAMS);
		else // valid amount of arguments, check if it's a valid num
		{
			result = strtol(av[i + offset].c_str(), &end, 10);
			if (result > INT_MAX || result < 0 || *end)
				sendMessage(_fds[index].fd, 696, _users[index].getNickname() + " MODE +l", ERR_INVALIDMODEPARAM);
			else
			{
				std::stringstream ss;
				
				ss << result;
				response = getClientInfo(index) + " MODE " + av[1] + " +l " + ss.str() + "\r\n";
				broadcastMessage(_channels[ch_i], response, index, 1); // DO IT IN THE END !!!
				_channels[ch_i].setMaxUsers(result);
				_channels[ch_i].setLimitMode(true);
			}
			offset++;
		}
	}
	flag = 1;
}

void Server::handleKey(std::vector<std::string>const &av, const int &i, const int &sign, int &offset, int &flag, int index)
{
	std::string c;
	std::string response;
	int ch_i = getChannelIndex(av[1]);

	if (flag)
		return;
	if (static_cast<size_t>(i + offset) >= av.size())
	{
		(sign < 0) ? c = "-" : c = "+";
		sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " MODE " + c, ERR_NEEDMOREPARAMS);
	}
	else if (av[i + offset][0] == ':')
	{
		sendMessage(_fds[index].fd, 525, _users[index].getNickname() + " " + av[1], ERR_INVALIDKEY);
		offset = av.size() - i;
	}
	else
	{
		if (av[i + offset] != _channels[ch_i].getPass() && _channels[ch_i].getKeyMode()) // incorrect password
		{
			sendMessage(_fds[index].fd, 467, _users[index].getNickname() + " " + av[1], ERR_KEYSET);
			offset++;
			return ;
		}
		if (sign < 0) // remove password
		{
			response = getClientInfo(index) + " MODE " + av[1] + " -k " + av[i + offset] + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1); // DO IT IN THE END !!!
			_channels[ch_i].setKeyMode(false);
			_channels[ch_i].setPass("");
		}
		else // add password
		{
			_channels[ch_i].setKeyMode(true);
			_channels[ch_i].setPass(av[i + offset]);
			response = getClientInfo(index) + " MODE " + av[1] + " +k " + av[i + offset] + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1); // DO IT IN THE END !!!
		}
		offset++;
	}
	flag = 1;
}

void Server::getOperatorData(std::map<std::string, int> &operators, std::vector<std::string> av, int i, int sign, int &offset, int index)
{
	std::map<std::string, int>::iterator it = operators.find(av[i + offset]);
	int ch_i = getChannelIndex(av[1]);

	if (static_cast<size_t>(i + offset) >= av.size())
		return ;
	if (!_channels[ch_i].isUserOnChannel(av[i + offset])) // Check if user av[i + offset] is on the channel
		sendMessage(_fds[index].fd, 401, _users[index].getNickname() + " " + av[i + offset], ERR_NOSUCHNICK);
	else
	{
		if (it != operators.end())
			it->second += sign;
		else
			operators.insert(std::pair<const std::string, int>(av[i + offset], sign));
	}
	offset++;
}

void Server::handleOperator(std::map<std::string, int> &operators, std::vector<std::string> av, int index)
{
	std::string response;
	int ch_i = getChannelIndex(av[1]);
	
	for (std::map<std::string, int>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if (it->second > 0 && !_channels[ch_i].isOperator(it->first))
		{
			// Add User
			_channels[ch_i].addOperator(it->first);
			response = getClientInfo(index) + " MODE " + av[1] + " +o " + it->first + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1); // DO IT IN THE END !!!
		}
		else if (it->second < 0 && _channels[ch_i].isOperator(it->first))
		{
			// Remove User
			_channels[ch_i].rmOperator(it->first);
			response = getClientInfo(index) + " MODE " + av[1] + " -o " + it->first + "\r\n";
			broadcastMessage(_channels[ch_i], response, index, 1); // DO IT IN THE END !!!
		}
		std::cout << "Name: " << it->first << ", op status: " << it->second << std::endl;
	}
}

void Server::mode(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel
	// av[2] = mode flags,{mode flags}
	// av[3+] = target / pass

	int ch_i = getChannelIndex(av[1]);

	if (av.size() < 3) // Missing arguments or Print info
	{
		if (av.size() == 1 || av[1] == "")
			sendMessage(_fds[index].fd, 461, _users[index].getNickname() + " MODE", ERR_NEEDMOREPARAMS);
		else // size == 2
		{
			if (doesUserExist(av[1]))
				sendMessage(_fds[index].fd, 502, _users[index].getNickname(), ERR_USERSDONTMATCH);
			else if (isChannel(av[1])) // Send channel info
			{
				sendMessage(_fds[index].fd, 324, _users[index].getNickname()+ " " + av[1], _channels[ch_i].printMode(_users[index].getNickname()));
				sendMessage(_fds[index].fd, 329, _users[index].getNickname()+ " " + av[1], _channels[ch_i].getChannelTime());
			}
			else // Not a user nor a channel
				sendMessage(_fds[index].fd, 403, _users[index].getNickname(), ERR_NOSUCHCHANNEL);
		}
	}
	else if (!isChannel(av[1]))
		sendMessage(_fds[index].fd, 403, _users[index].getNickname() + " " + av[1], ERR_NOSUCHCHANNEL);
	else if (!_channels[ch_i].isUserOnChannel(_users[index].getNickname()))
		sendMessage(_fds[index].fd, 442, _users[index].getNickname() + " " + av[1], ERR_NOTONCHANNEL);
	else if (!_channels[ch_i].isOperator(_users[index].getNickname()))
		sendMessage(_fds[index].fd, 482, _users[index].getNickname() + " " + av[1], ERR_CHANOPRIVSNEEDED);
	else
	{
		int sign, offset;
		int inv = 0, topic = 0, limit_flag = 0, key_flag = 0;
		std::map<std::string, int> operators;

		for (size_t i = 2; i < av.size(); ) // at least size 3
		{
			// Need to reset the offset and sign every new string
			offset = 1;
			sign = 1;
			for (size_t j = 0; av[i][j]; j++)
			{
				switch(av[i][j])
				{
					case '+':
						sign = adv_peek(av[i], j);
						break ;
					case '-':
						sign = adv_peek(av[i], j);
						break ;
					case 't':
						topic += (sign * av[i][j]);
						break ;
					case 'i':
						inv += (sign * av[i][j]);
						break ;
					case 'o':
						getOperatorData(operators, av, i, sign, offset, index);
						break ;
					case 'k':
						handleKey(av, i, sign, offset, key_flag, index);
						break ;
					case 'l':
						handleLimit(av, i, sign, offset, limit_flag, index);
						break ;
					case '\0':
						break ;
					default:
						sendMessage(_fds[index].fd, 472, _users[index].getNickname() + " " + av[i][j], ERR_UNKNOWNMODE);
				}
			}
			i += offset;
		}
		if (inv)
			handleInv(av, index, inv);
		if (topic)
			handleTopic(av, index, topic);
		handleOperator(operators, av, index); // map: key: nick, value: 1 = add, 0 = nothing, -1 = remove;
	}
	// BROADCAST THIS
	// NEED TO TRACK IF THERE WERE ANY CHANGE OR NOT. PRINT ONLY IF CHANGES HAPPEN
	// THIS DOESNT WORK !
	// NEED TO BROADCAST ONLY THE CHANGES
	//	response = 	getClientInfo(index) + " MODE " + av[1] + " " + _channels[ch_i].printMode() + "\r\n";
}

void Server::who(std::vector<std::string>& av, int index)
{
	if (av.size() == 1)
		sendMessage(_fds[index].fd, 315, _users[index].getNickname(), "End of the /WHO List");
	else
		sendMessage(_fds[index].fd, 315, _users[index].getNickname() + " " + av[1], "End of the /WHO List");
}


/*
av[5]


av[0] -

av[1] +

av[2] 1245 (LIMIT) + hello (KEY) // Which comes first, stays first

av[3] jinx vini (OP)

*/