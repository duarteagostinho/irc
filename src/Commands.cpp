/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/13 21:59:19 by vloureir         ###   ########.fr       */
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
	av[0] = line;
	return (-1);
}

// MISSING:   MODE, WHO, INVITE

void Server::exec_cmd(std::string line, int index)
{
	std::vector<std::string> av;
	int cmds = check_cmd(line, av);
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
			break ;
		default: // HEXCHAT SENDS MODE <CHANNEL> AND WHO <CHANNEL> HANDLE IT
			std::string response = ":irc.server 421 " + _users[index].getNickname() + " :Unknown command\r\n";
			// send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	line.clear();
}

// void Server::join(std::vector<std::string>& av, int index)
// {
// 	if (av.size() == 1) // Missing arguments
// 	{
// 		std::string response = JOIN_USAGE;
// 		send(_fds[index].fd, response.c_str(), response.size(), 0);
// 		return ;
// 	}
// 	else if (av[1][0] != '#') // Missing correct server name format
// 	{
// 		std::string response = "** - " + av[1] + ": No such channel\r\n";
// 		send(_fds[index].fd, response.c_str(), response.size(), 0);
// 		return ;
// 	}
// 	for (size_t i = 0; i < _channels.size(); i++) // Seeing if the channel already exists
// 	{
// 		if (_channels[i].getName() == av[1])
// 		{
// 			// Checking invite status, if pass and if pass is correct
// 			if (!_channels[i].hasInvite(_users[index].getNickname()) && "" != _channels[i].getPass() && av[2] != _channels[i].getPass())
// 			{
// 				std::string response = "Cannot join " + av[1] + " (Requires keyword)\r\n";
// 				send(_fds[index].fd, response.c_str(), response.size(), 0);
// 			}
// 			else  // If invited, no pass or pass was correct, enter channel
// 			{
// 				_channels[i].addUser(_users[index].getNickname(), 0);

// 				std::string response1 = ":" + _users[index].getNickname() + " JOIN " + av[1] + "\r\n";
// 				broadcastMessage(_channels[i], response1, index, 1);
// //				send(_fds[index].fd, response1.c_str(), response1.size(), 0);

// 				if (_channels[i].getTopic() != "")
// 				{
// 					std::string response2 = ":irc.server 332 " + _users[index].getNickname() + " " + av[1] + " :" + _channels[i].getTopic() + "\r\n";
// 					broadcastMessage(_channels[i], response2, index, 1);
// //					send(_fds[index].fd, response2.c_str(), response2.size(), 0);

// 					response2 = ":irc.server 333 " + _users[index].getNickname() + " " + av[1] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528
// 					broadcastMessage(_channels[i], response2, index, 1);
// //					send(_fds[index].fd, response2.c_str(), response2.size(), 0);
// 				}
// 				std::string response3 = ":irc.server 353 " + _users[index].getNickname() + " = " + av[1] + " :" + _channels[i].usersFormated() + "\r\n";
// 				broadcastMessage(_channels[i], response3, index, 1);
// //				send(_fds[index].fd, response3.c_str(), response3.size(), 0);

// 				std::string response4 = ":irc.server 366 " + _users[index].getNickname() + " " + av[1] + " :End of /NAMES list\r\n"; 
// 				broadcastMessage(_channels[i], response4, index, 1);
// //				send(_fds[index].fd, response4.c_str(), response4.size(), 0);
				
// 			}
// 			return ; // If there was a channel with that name, we either fail to enter or enter it
// 		}
// 	}
// 	// If channel doesn't exists, create one
// 	int x = _channels.size();
// 	_channels.push_back(Channel(av[1]));
// 	_channels[x].addUser(_users[index].getNickname(), 1);

// 	std::string response1 = getClientInfo(index) + " JOIN " + av[1] + "\r\n";
// 	send(_fds[index].fd, response1.c_str(), response1.size(), 0);

// 	std::string response3 = ":irc.server 353 " + _users[index].getNickname() + " = " + av[1] + " :@" + _users[index].getNickname() + "\r\n";
// 	send(_fds[index].fd, response3.c_str(), response3.size(), 0);

// 	std::string response4 = ":irc.server 366 " + _users[index].getNickname() + " " + av[1] + " :End of /NAMES list\r\n"; 
// 	send(_fds[index].fd, response4.c_str(), response4.size(), 0);
// }


void Server::join(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel,{channel}
	// av[2] = pass,{pass}

	std::string pass;
	std::string response;
	std::vector<std::string> channels;
	std::vector<std::string> passwords;
	
	if (av.size() == 1) // Missing arguments
	{
		response = ":irc.server 461 " + _users[index].getNickname() + " JOIN :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
		return ;
	}
	splitString(av[1], channels, ',');
	if (av.size() > 2)
		splitString(av[2], passwords, ',');
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i][0] != '#')
		{
			response = ":irc.server 403 " + _users[index].getNickname() + " " + channels[i] + " :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
		}
		else if (isChannel(channels[i])) // Valid channel, we either enther of fail to enter
		{
			int c_index = getChannelIndex(channels[i]);

			if (passwords.size() > i) // DOES THIS WORK ?
				pass = passwords[i];
				
			if (_channels[c_index].getKeyMode() && _channels[c_index].getPass() != pass)
			{
				response = ":irc.server 475 " + _users[index].getNickname() + " " + channels[i] + " :Cannot join channel (+k)\r\n";
				send(_fds[index].fd, response.c_str(), response.size(), 0);
			}
			else if (!_channels[c_index].hasInvite(_users[index].getNickname()) && _channels[c_index].getInvMode())
			{
				response = ":irc.server 473 " + _users[index].getNickname() + " " + channels[i] + " :Cannot join channel (+i)\r\n";
				send(_fds[index].fd, response.c_str(), response.size(), 0);
			}
			else // If invited, no pass or pass was correct, enter channel
			{
				_channels[c_index].addUser(_users[index].getNickname(), 0);
				_channels[c_index].rmInvite(_users[index].getNickname());

				response = ":" + _users[index].getNickname() + " JOIN " + channels[i] + "\r\n";
				broadcastMessage(_channels[c_index], response, index, 1);

				if (_channels[c_index].getTopic() != "")
				{
					response = ":irc.server 332 " + _users[index].getNickname() + " " + channels[i] + " :" + _channels[c_index].getTopic() + "\r\n";
					broadcastMessage(_channels[c_index], response, index, 1);

					response = ":irc.server 333 " + _users[index].getNickname() + " " + channels[i] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528
					broadcastMessage(_channels[c_index], response, index, 1);
				}
				response = ":irc.server 353 " + _users[index].getNickname() + " = " + channels[i] + " :" + _channels[c_index].usersFormated() + "\r\n";
				broadcastMessage(_channels[c_index], response, index, 1);

				response = ":irc.server 366 " + _users[index].getNickname() + " " + channels[i] + " :End of /NAMES list\r\n"; 
				broadcastMessage(_channels[c_index], response, index, 1);
			}
		}
		else // New channel creation
		{
			int x = _channels.size();
			_channels.push_back(Channel(channels[i]));
			_channels[x].addUser(_users[index].getNickname(), 1);
			response = getClientInfo(index) + " JOIN " + channels[i] + "\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
			response = ":irc.server 353 " + _users[index].getNickname() + " = " + channels[i] + " :@" + _users[index].getNickname() + "\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
			response = ":irc.server 366 " + _users[index].getNickname() + " " + channels[i] + " :End of /NAMES list\r\n"; 
			send(_fds[index].fd, response.c_str(), response.size(), 0);
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
	
	if (av.size() < 3) // Missing arguments
	{
		response = ":irc.server 461 " + _users[index].getNickname() + " KICK :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (!isChannel(av[1]))
	{
		response = ":irc.server 403 " + _users[index].getNickname() + " " + av[1] + " :No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (!_channels[getChannelIndex(av[1])].isOperator(_users[index].getNickname()))
	{
		response = ":irc.server 482 " + _users[index].getNickname() + " " + av[1] + " :You're not channel operator\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (!_channels[getChannelIndex(av[1])].isUserOnChannel(av[2]))
	{
		response = ":irc.server 401 " + _users[index].getNickname() + " " + av[2] + " :No such nick\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else
	{
		// Reason is not working correctly if there is no reason
		response = getClientInfo(index) + " KICK " + av[1] + " " + av[2]  + " :" + craftStringSpaces(av[0], 3) + "\r\n";
		broadcastMessage(_channels[getChannelIndex(av[1])], response, index, 1);
		_channels[getChannelIndex(av[1])].rmUser(av[2]);
		_channels[getChannelIndex(av[1])].rmOperator(av[2]);
	}
}

void Server::topic(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel,{channel}
	// av[2+] = rest of the message (NOT USED)
	
	std::string response;
	std::vector<std::string> channels;

	if (av.size() == 1) // Missing arguments
	{
		response = ":irc.server 461 " + _users[index].getNickname() + " TOPIC :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], channels, ',');
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!isChannel(channels[i]))
		{
			response = ":irc.server 403 " + _users[index].getNickname() + " " + channels[i] + " :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
		}
		else if (av.size() == 2) // Print topic
		{
			if (_channels[getChannelIndex(channels[i])].getTopic() == "") // If Topic is not set
			{
				response =":irc.server 331 " + _users[index].getNickname() + " " + channels[i] + " :No topic is set\r\n";
				send(_fds[index].fd, response.c_str(), response.size(), 0);
			}
			else
			{
				response = ":irc.server 332 " + _users[index].getNickname() + " " + channels[i] + " :" + _channels[getChannelIndex(channels[i])].getTopic() + "\r\n";
				send(_fds[index].fd, response.c_str(), response.size(), 0);
				
				response = ":irc.server 333 " + _users[index].getNickname() + " " + channels[i] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528	
				send(_fds[index].fd, response.c_str(), response.size(), 0);
			}
		}
		else if (!_channels[getChannelIndex(channels[i])].isOperator(_users[index].getNickname()))
		{
			response = ":irc.server 482 " + _users[index].getNickname() + " " + channels[i] + " :You're not channel operator\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
		}
		else
		{
			// Need to store the time it happens here
			std::string new_topic = craftStringSpaces(av[0], 2);
			_channels[getChannelIndex(channels[i])].setTopic(new_topic);
			response = getClientInfo(index) + " TOPIC " + av[1] + " :" + new_topic + "\r\n";
			broadcastMessage(_channels[getChannelIndex(channels[i])], response, index, 1);
		}
	}
}

void Server::invite(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = nickname
	// av[2] = channel
	
	std::string response;

	if (av.size() == 1) // Missing arguments
	{
		response = ":irc.server 337 " + _users[index].getNickname() + " :End of Invite List\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (av.size() < 3)
	{
		response = ":irc.server 461 " + _users[index].getNickname() + " INVITE :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (!doesUserExist(av[1]))
	{
		response = ":irc.server 401 " + _users[index].getNickname() + " " + av[2] +  " :No such nick\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (!isChannel(av[2]))
	{
		response = ":irc.server 403 " + _users[index].getNickname() + " " + av[2] + " :No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (_channels[getChannelIndex(av[2])].isUserOnChannel(av[1]))
	{
		response = ":irc.server 443 " + _users[index].getNickname() + " " + av[1] + " " + av[2] + " :is already on channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else if (!_channels[getChannelIndex(av[2])].isOperator(_users[index].getNickname()))
	{
		response = ":irc.server 482 " + _users[index].getNickname() + " " + av[2] + " :You're not channel operator\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
	}
	else
	{
		_channels[getChannelIndex(av[2])].addInvite(av[1]);
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
	
	std::vector<std::string> targets;

	if (av.size() < 3) // Missing arguments
	{
		std::string response;
		if (av.size() == 1)
			response = ":server 411 " + _users[index].getNickname() + " :No recipient given (PRIVMSG)\r\n";
		else
			response = ":server 412 " + _users[index].getNickname() + " :No text to send\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
		return ;
	}
	
	std::string response;
	splitString(av[1], targets, ',');
	for (size_t i = 0; i < targets.size(); i++)
	{ 
		bool channel = isChannel(targets[i]);
		bool user = doesUserExist(targets[i]);
		
		if (!channel && !user)  // Error for sure
		{
			if (targets[i][0] == '#')
				response = ":server 403 " + _users[index].getNickname() + " " + targets[i] + " :No such channel\r\n";
			else
				response = ":server 401 " + _users[index].getNickname() + " " + targets[i] + " :No such nick\r\n";
			send(_users[index].getFd(), response.c_str(), response.size(), 0);
			continue ;
		}
		response = getClientInfo(index) + " PRIVMSG " + targets[i] + " :" + craftStringSpaces(av[0], 2) + "\r\n";
		if (user)
			send(_users[getUserIndex(targets[i])].getFd(), response.c_str(), response.size(), 0);		
		else if (targets.size() > 1 || !_channels[getChannelIndex(targets[i])].isUserOnChannel(_users[index].getNickname()))
		{	
			// Check if sender is in the channel or if there are multiple args
			response = ":server 404 " + _users[index].getNickname() + " " + targets[i] + " :Cannot send to channel\r\n";
			send(_users[index].getFd(), response.c_str(), response.size(), 0);
		}
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
	std::vector<std::string> channels;

	if (av.size() == 1) // Missing arguments
	{
		response = ":irc.server 461 " + _users[index].getNickname() + " PART :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], channels, ',');
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!isChannel(channels[i]))
		{
			response = ":irc.server 403 " + _users[index].getNickname() + " " + channels[i] + " :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
		}
		else if (av.size() == 2) // Just leave
		{
			response = getClientInfo(index) + " PART " + channels[i] + "\r\n";
			broadcastMessage(_channels[getChannelIndex(channels[i])], response, index, 1);
		}
		else
		{
			response = getClientInfo(index) + " PART " + av[1] + " :" + craftStringSpaces(av[0], 2) + "\r\n";
			broadcastMessage(_channels[getChannelIndex(channels[i])], response, index, 1);
		}
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

void Server::mode(std::vector<std::string>& av, int index)
{	
	// av[0] = whole line
	// av[1] = channel
	// av[2] = mode flags,{mode flags}
	// av[3+] = target / pass

	char c;
	std::string response;
	
	if (av.size() < 3) // Missing arguments or Print info
	{
		if (av.size() == 1)
		{
			response = ":irc.server 461 " + _users[index].getNickname() + " MODE :Not enough parameters\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
		}
		else // size == 2
		{
			if (!doesUserExist(av[1]))
				response = ":irc.server 502 vivi :Cant change mode for other users\r\n";
			else if (isChannel(av[1]))
			{
				response = 	":irc.server 324 vivi #b +tn";
				response += ":irc.server 329 vivi #b 1781365780";
			}
			else // Not a user nor a channel
				response = 	":irc.server 403 vivi #v :No such channel";
		}
	}

	// if l OR k OR o


	if (c != 'i' && c != 't' && c != 'k' && c != 'o' && c != 'l')
	{
		// ERR_UMODEUNKNOWNFLAG (501)
		// ERR_UNKNOWNMODE (472)
	}
	
	//	ERR_INVALIDKEY (525)
	// If someone tries to set an invalid key, send this flag


	/*
		MODE 
			:luna.AfterNET.Org 461 vivi MODE :Not enough parameters

		MODE #channel
			:luna.AfterNET.Org 324 vivi #b +tn
			:luna.AfterNET.Org 329 vivi #b 1781365780
			
		MODE !channel
			:luna.AfterNET.Org 403 vivi #v :No such channel

		MODE vini (EXISTING USER)
			:luna.AfterNET.Org 502 vivi :Cant change mode for other users

		MODE #42 +o vini
			:vivi!vini@AN-EA7BE6BE.net.novis.pt MODE #42 +o vini

		

	*/
	
/*
/mode
	Channel #channel modes: +tink * (prints current modes)
/mode i
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		<operator> sets mode +i on #channel
*/
}

/*
mode
	:Aurora.AfterNET.Org 461 vini MODE :Not enough parameters

mode #42
	:Aurora.AfterNET.Org 324 vini #3 +tnk 12
	:Aurora.AfterNET.Org 329 vini #3 1781372033


mode #42 k 123 (check if key is valid)
	:vini!vini@AN-EA7BE6BE.net.novis.pt MODE #3 +k 12

mode #42 -k 123
	:vini!vini@AN-EA7BE6BE.net.novis.pt MODE #3 -k 12

mode #42 -k (ignored)
	:Aurora.AfterNET.Org 461 vini MODE +k :Not enough parameters

mode #42 l
	:Aurora.AfterNET.Org 461 vini MODE :Not enough parameters

mode #42 l 2147483647 (max int) ?
	:vini!vini@AN-EA7BE6BE.net.novis.pt MODE #3 +l 2147483647

mode #42 o (IGNORED)

mode #42 o chuchu
	:Aurora.AfterNET.Org 401 vini chuchu :No such nick
	
mode #42 o vini

+o (BROADCAST)
:vini!vini@AN-EA7BE6BE.net.novis.pt MODE #3 +o vini

-o (BROADCAST)
:vini!vini@AN-EA7BE6BE.net.novis.pt MODE #3 -o vini




*/