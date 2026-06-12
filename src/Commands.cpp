/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/12 17:46:24 by vloureir         ###   ########.fr       */
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
	std::string accepted[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN", "PRIVMSG"};
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
			mode();
			break;
		case 4:
			join(av, index);
			break;
		case 5:
			privmsg(av, index);
			break;
		default:
			std::cout << "Invalid Command" << std::endl; // 421 chaud  :Unknown command
	}
	line.clear();
}

void Server::join(std::vector<std::string>& av, int index)
{
	if (av.size() == 1) // Missing arguments
	{
		std::string response = JOIN_USAGE;
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		return ;
	}
	else if (av[1][0] != '#') // Missing correct server name format
	{
		std::string response = "** - " + av[1] + ": No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		return ;
	}
	for (size_t i = 0; i < _channels.size(); i++) // Seeing if the channel already exists
	{
		if (_channels[i].getName() == av[1])
		{
			// Checking invite status, if pass and if pass is correct
			if (!_channels[i].hasInvite(_users[index].getNickname()) && "" != _channels[i].getPass() && av[2] != _channels[i].getPass())
			{
				std::string response = "Cannot join " + av[1] + " (Requires keyword)\r\n";
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
			else // If invited, no pass or pass was correct, enter channel
			{
				_channels[i].addUser(_users[index].getNickname(), 0);

				std::string response1 = ":" + _users[index].getNickname() + " JOIN " + av[1] + "\r\n";
				send(_fds[index].fd, response1.c_str(), response1.size() + 1, 0);

				if (_channels[i].getTopic() != "")
				{
					std::string response2 = ":irc_server 332 " + _users[index].getNickname() + " " + av[1] + " :" + _channels[i].getTopic() + "\r\n";
					send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);

					response2 = ":irc_server 333 " + _users[index].getNickname() + " " + av[1] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528
					send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);
				}
				std::string response3 = ":irc_server 353 " + _users[index].getNickname() + " = " + av[1] + " :" + _channels[i].usersFormated() + "\r\n";
				send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

				std::string response4 = ":irc_server 366 " + _users[index].getNickname() + " " + av[1] + ":End of /NAMES list\r\n"; 
				send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);

			}
			return ; // If there was a channel with that name, we either fail to enter or enter it
		}
	}
	// If channel doesn't exists, create one
	int x = _channels.size();
	_channels.push_back(Channel(av[1]));
	_channels[x].addUser(_users[index].getNickname(), 1);

	std::string response1 = getClientInfo(index) + " JOIN " + av[1] + "\r\n";
	send(_fds[index].fd, response1.c_str(), response1.size() + 1, 0);

	std::string response3 = ":irc_server 353 " + _users[index].getNickname() + " = " + av[1] + " :@" + _users[index].getNickname() + "\r\n";
	send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

	std::string response4 = ":irc_server 366 " + _users[index].getNickname() + " " + av[1] + " :End of /NAMES list\r\n"; 
	send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);
}

// void Server::join(std::vector<std::string>& av, int index)
// {
// 	if (av.size() == 1) // Missing arguments
// 	{
// 		std::string response = ":irc_server 461 <nickname> JOIN :Not enough parameters\r\n";
// 		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
// 		return ;
// 	}
// 	else if (av[1][0] != '#') // Missing correct server name format
// 	{
// 		std::string response = ":irc_server 403 <nickname> av[1] :No such channel\r\n";
// 		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
// 		return ;
// 	}
// 	else
// 	{
// //		std::vector<std::string> channels; //
// //		std::vector<std::string> pass;


// //		!isChannel(av[1])




		
// 	}


	
// 	for (size_t i = 0; i < _channels.size(); i++) // Seeing if the channel already exists
// 	{
// 		if (_channels[i].getName() == av[1])
// 		{
// 			// Checking invite status, if pass and if pass is correct
// 			if (!_channels[i].hasInvite(_users[index].getNickname()) && "" != _channels[i].getPass() && av[2] != _channels[i].getPass())
// 			{
// 				std::string response = "Cannot join " + av[1] + " (Requires keyword)\r\n";
// 				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
// 			}
// 			else // If invited, no pass or pass was correct, enter channel
// 			{
// 				_channels[i].addUser(_users[index].getNickname(), 0);
// 				std::string response1 = ":" + _users[index].getNickname() + " JOIN " + av[1] + "\r\n";
// 				send(_fds[index].fd, response1.c_str(), response1.size() + 1, 0);

// 				std::string response2 = "332 " + _users[index].getNickname() + " " + av[1] + " :Hello world\r\n";
// 				send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);

// 				std::string response3 = "353 " + _users[index].getNickname() + " = " + av[1] + ":+o" + _users[index].getNickname() + "\r\n";
// 				send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

// 				std::string response4 = "366 " + _users[index].getNickname() + " " + av[1] + ":End of /NAMES list\r\n"; 
// 				send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);

// 				// std::string response = "Now talking on " + av[1] + "\r\n";
// 				// if (_channels[i].getTopic() != "")
// 				// 	send(_fds[index].fd, _channels[i].printTopic().c_str(), _channels[i].printTopic().size() + 1, 0);
// 				// send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
// 			}
// 			return ; // If there was a channel with that name, we either fail to enter or enter it
// 		}
// 	}
// 	// If channel doesn't exists, create one
// 	int x = _channels.size();
// 	_channels.push_back(Channel(av[1]));
// 	_channels[x].addUser(_users[index].getNickname(), 1);


// 	std::string response1 = ":chaud!vini@10.12.3.12 JOIN " + av[1] + "\r\n";
// 	send(_fds[index].fd, response1.c_str(), response1.size() + 1, 0);

// 	// std::string response2 = ":10.12.3.12 332 " + _users[index].getNickname() + " " + av[1] + " :Hello world\r\n";
// 	// send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);

// 	std::string response3 = ":ft_irc 353 " + _users[index].getNickname() + " = " + av[1] + " :@" + _users[index].getNickname() + "\r\n";
// 	send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

// 	std::string response4 = ":ft_irc 366 " + _users[index].getNickname() + " " + av[1] + " :End of /NAMES list\r\n"; 
// 	send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);

// 	std::cout << "printing responses:\n" << response1 << response3 + response4 << std::endl;
// 	// std::string response2 = "Now talking on " + av[1] + "\r\n";
// 	// send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);
// }



void Server::kick(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel
	// av[2] = target
	// av[3] = reason
	
	if (av.size() < 3) // Missing arguments
	{
		std::string response = ":irc_server 461 " + _users[index].getNickname() + " KICK :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!isChannel(av[1]))
	{
		std::string response = ":irc_server 403 " + _users[index].getNickname() + " " + av[1] + " :No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!_channels[getChannelIndex(av[1])].isOperator(_users[index].getNickname()))
	{
		std::string response = ":irc_server 482 " + _users[index].getNickname() + " " + av[1] + " :You're not channel operator\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!_channels[getChannelIndex(av[1])].isUserOnChannel(av[2]))
	{
		std::string response = ":irc_server 401 " + _users[index].getNickname() + " " + av[2] + " :No such nick\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else
	{
		std::string response = getClientInfo(index) + " KICK " + av[1] + " :" + craftStringSpaces(av[0], 3);
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		_channels[getChannelIndex(av[1])].rmUser(av[2]);
		_channels[getChannelIndex(av[1])].rmOperator(av[2]);
		// BROADCAST TO THE WHOLE CHANNEL THE NEW TOPIC
	}
}

void Server::topic(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = channel,{channel}
	// av[2+] = rest of the message (NOT USED)
	
	std::vector<std::string> channels;

	if (av.size() == 1) // Missing arguments
	{
		std::string response = ":irc_server 461 " + _users[index].getNickname() + " TOPIC :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], channels, ',');
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!isChannel(channels[i]))
		{
			std::string response = ":irc_server 403 " + _users[index].getNickname() + " " + channels[i] + " :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		}
		else if (av.size() == 2) // Print topic
		{
			if (_channels[getChannelIndex(channels[i])].getTopic() == "")
			{
				std::string response =":irc_server 331 " + _users[index].getNickname() + " " + channels[i] + " :No topic is set\r\n";
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
			else
			{
				std::string response = ":irc_server 332 " + _users[index].getNickname() + " " + channels[i] + " :" + _channels[getChannelIndex(channels[i])].getTopic() + "\r\n";
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
				
				response = ":irc_server 333 " + _users[index].getNickname() + " " + channels[i] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528	
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
		}
		else if (!_channels[getChannelIndex(channels[i])].isOperator(_users[index].getNickname()))
		{
			std::string response = ":irc_server 482 " + _users[index].getNickname() + " " + channels[i] + " :You're not channel operator\r\n";
			send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		}
		else
		{
			std::string new_topic = craftStringSpaces(av[0], 2);
			_channels[getChannelIndex(channels[i])].setTopic(new_topic);
//			std::cout << new_topic << std::endl;
			// BROADCAST TO THE WHOLE CHANNEL THE NEW TOPIC
			std::string response = getClientInfo(index) + " TOPIC " + av[1] + " :" + new_topic + "\r\n";
			send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		}
	}
}

void Server::invite(std::vector<std::string>& av, int index)
{
	// av[0] = whole line
	// av[1] = nickname
	// av[2] = channel
	
	if (av.size() == 1) // Missing arguments
	{
		std::string response = ":irc_server 337 " + _users[index].getNickname() + " :End of Invite List\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (av.size() < 3)
	{
		std::string response = ":irc_server 461 " + _users[index].getNickname() + " INVITE :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!doesUserExist(av[1]))
	{
		std::string response = ":irc_server 401 " + _users[index].getNickname() + " " + av[2] +  " :No such nick\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!isChannel(av[2]))
	{
		std::string response = ":irc_server 403 " + _users[index].getNickname() + " " + av[2] + " :No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	// CHECK IF USER IS ALREADY ON CHANNEL
	else if (_channels[getChannelIndex(av[2])].isUserOnChannel(av[1]))
	{
		// :luna.AfterNET.Org 443 jinx vini #42 :is already on channel
		std::string response = ":irc_server 443 " + _users[index].getNickname() + " " + av[1] + " " + av[2] + " :is already on channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!_channels[getChannelIndex(av[2])].isOperator(_users[index].getNickname()))
	{
		std::string response = ":irc_server 482 " + _users[index].getNickname() + " " + av[2] + " :You're not channel operator\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else
	{
		// :Aurora.AfterNET.Org 341 alo chaud #a // SENDER GETS THIS
		std::string sender_response = ":irc_server 341 " + _users[index].getNickname() + " " + av[1] + " " + av[2] + "\r\n";
		send(_fds[index].fd, sender_response.c_str(), sender_response.size() + 1, 0);

		// :chaud2!vloureir@81C240:F52884:7D2707:482FF6:IP INVITE vini #42 // RECEIVER GETS THIS
		std::string client_response = getClientInfo(index) + " INVITE " + av[1] + " " + av[2] + "\r\n";
		send(_fds[getUserIndex(av[1])].fd, client_response.c_str(), client_response.size() + 1, 0);
	}
}

void Server::mode(void)
{

/*
/mode
	 Channel #channel modes: +tink * (prints current modes)
/mode i
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		<operator> sets mode +i on #channel
*/
	std::cout << "mode called\n";
}

void Server::splitString(std::string line, std::vector<std::string>& av, char delim)
{
	// Create an argv from the line
	std::stringstream split(line);
	std::string token;
	while (std::getline(split, token, delim))
		av.push_back(token);
}

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

std::string Server::getClientInfo(int index)
{
	std::string ret = ":" + _users[index].getNickname() + "!" + _users[index].getUsername();
	ret = ret + "@" + _users[index].getAddress();
	return (ret);
}

void Server::broadcastMessage(std::string message, int index, int flag)
{
	(void)flag;
	(void)index;
	(void)message;
	
//	Need channel name as well or pass the channel variable
	for (size_t i = 0; i < _fds.size(); i++)
	{
	//	if (_fds[i].fd != _fds[j].fd) // Send to every fd that is not mine
		// 		send(_fds[j].fd, str.c_str(), str.size() + 1, 0);


	}
//	send()

	// for (size_t j = 1; j < _fds.size(); j++) // start at 1 to always ignore the listening socket
	// {
	// 	std::ostringstream ss;
	// 	std::string str;
	// 	ss << _users[i].getNickname() << ": " << line << "\r\n";
	// 	str = ss.str();

	// 	if (_fds[i].fd != _fds[j].fd) // Send to every fd that is not mine
	// 		send(_fds[j].fd, str.c_str(), str.size() + 1, 0);
	// }

}

void	Server::privmsg(std::vector<std::string>& av, int index)
{
	(void)av;
	(void)index;

	// av[0] = whole line
	// av[1] = target,{target}
	// av[2+] = rest of the message (NOT USED)

	std::vector<std::string> targets;

	if (av.size() == 1) // Missing arguments
	{
		std::string response = ":server 411 " + _users[index].getNickname() + " :No recipient given (PRIVMSG)\r\n";
		send(_fds[index].fd, response.c_str(), response.size(), 0);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], targets, ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (!isChannel(targets[i]))
		{
			std::string response = ":server 403 " + _users[index].getNickname() + " " + targets[i] + " :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size(), 0);
		}
		else if (av.size() == 2) // Print topic
		{
			if (_channels[getChannelIndex(targets[i])].getTopic() == "")
			{
				std::string response =":server 331 " + _users[index].getNickname() + " " + targets[i] + " :No topic is set\r\n";
				send(_fds[index].fd, response.c_str(), response.size(), 0);
			}
			else
			{
				std::string response = ":server 332 " + _users[index].getNickname() + " " + targets[i] + " :" + _channels[getChannelIndex(targets[i])].getTopic() + "\r\n";
				send(_fds[index].fd, response.c_str(), response.size(), 0);
				
				response = ":server 333 " + _users[index].getNickname() + " " + targets[i] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528	
				send(_fds[index].fd, response.c_str(), response.size(), 0);
			}
		}
		else
		{
			std::string msg = craftStringSpaces(av[0], 2);
//			std::cout << new_topic << std::endl;
			// BROADCAST TO THE WHOLE CHANNEL THE NEW TOPIC
			int ch_id = getChannelIndex(av[1]);
			for (size_t u = 0; u < _users.size(); u++)
			{
				if (_users[u].getNickname() == _users[index].getNickname())
					continue;
				if (_channels[ch_id].isUserOnChannel(_users[u].getNickname()))
				{
					std::string response = getClientInfo(index) + " PRIVMSG " + av[1] + " :" + msg + "\r\n";
					send(_fds[u].fd, response.c_str(), response.size(), 0);
				}
			}
		}
	}

}


// void	Server::privmsg(std::vector<std::string>& av, int index)
// {
// 	(void)av;
// 	(void)index;

// 	// av[0] = whole line
// 	// av[1] = target,{target}
// 	// av[2+] = rest of the message (NOT USED)

// 	std::vector<std::string> targets;

// 	if (av.size() == 1) // Missing arguments
// 	{
// 		std::string response = ":server 411 " + _users[index].getNickname() + " :No recipient given (PRIVMSG)\r\n";
// 		send(_fds[index].fd, response.c_str(), response.size(), 0);
// 		return ;
// 	}
// 	// Split the channels argument and loop through them to check every channel received
// 	splitString(av[1], targets, ',');
// 	for (size_t i = 0; i < targets.size(); i++)
// 	{
// 		if (!isChannel(targets[i]))
// 		{
// 			std::string response = ":server 403 " + _users[index].getNickname() + " " + targets[i] + " :No such channel\r\n";
// 			send(_fds[index].fd, response.c_str(), response.size(), 0);
// 		}
// 		else if (av.size() == 2) // Print topic
// 		{
// 			if (_channels[getChannelIndex(targets[i])].getTopic() == "")
// 			{
// 				std::string response =":server 331 " + _users[index].getNickname() + " " + targets[i] + " :No topic is set\r\n";
// 				send(_fds[index].fd, response.c_str(), response.size(), 0);
// 			}
// 			else
// 			{
// 				std::string response = ":server 332 " + _users[index].getNickname() + " " + targets[i] + " :" + _channels[getChannelIndex(targets[i])].getTopic() + "\r\n";
// 				send(_fds[index].fd, response.c_str(), response.size(), 0);
				
// 				response = ":server 333 " + _users[index].getNickname() + " " + targets[i] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528	
// 				send(_fds[index].fd, response.c_str(), response.size(), 0);
// 			}
// 		}
// 		else
// 		{
// 			std::string msg = craftStringSpaces(av[0], 2);
// //			std::cout << new_topic << std::endl;
// 			// BROADCAST TO THE WHOLE CHANNEL THE NEW TOPIC
// 			int ch_id = getChannelIndex(av[1]);
// 			for (size_t u = 0; u < _users.size(); u++)
// 			{
// 				if (_users[u].getNickname() == _users[index].getNickname())
// 					continue;
// 				if (_channels[ch_id].isUserOnChannel(_users[u].getNickname()))
// 				{
// 					std::string response = getClientInfo(index) + " PRIVMSG " + av[1] + " :" + msg + "\r\n";
// 					send(_fds[u].fd, response.c_str(), response.size(), 0);
// 				}
// 			}
// 		}
// 	}
// }

/*
	std::vector<std::string> targets;

	if (size == 1)
		411 chaud :No recipient given (PRIVMSG)
	else if (size == 2)
		412 chaud :No text to send
 

	splitString(av[1], targets, ',');

	for (size_t i = 0; i < targets.size(); i++)
	{
		bool channel = isChannel(targets[i];

		if ((targets.size() > 1 && channel == true) || not on channel)
			404 chaud #42 :Cannot send to channel
		else if (targets[i] dont exists)
			401 chaud user_1 :No such nick
		else if (channel == false)
			403 chaud #42 :No such channel
		else
			:user_5!user_5@1C72DB:D17C90:7D2707:482FF6:IP PRIVMSG chaud :hi hi // RECEIVER GETS THIS
	
	


	
	}

	

*/



/*

PRIVMSG
:Aurora.AfterNET.Org 411 chaud :No recipient given (PRIVMSG)
JOIN
:Aurora.AfterNET.Org 461 chaud JOIN :Not enough parameters
PRIVMSg #42
:Aurora.AfterNET.Org 412 chaud :No text to send
PRIVMSG #42 hi
:Aurora.AfterNET.Org 403 chaud #42 :No such channel
PRIVMSg #42,#11
:Aurora.AfterNET.Org 412 chaud :No text to send
PRIVMSG #42 hi
:Aurora.AfterNET.Org 404 chaud #42 :Cannot send to channel
JOIN #42
:chaud!vini@AN-EA7BE6BE.net.novis.pt JOIN #42
:Aurora.AfterNET.Org 353 chaud = #42 :chaud @user_5
:Aurora.AfterNET.Org 366 chaud #42 :End of /NAMES list.
PRIVMSG #42,#11 hello
:Aurora.AfterNET.Org 404 chaud #42 :Cannot send to channel
:Aurora.AfterNET.Org 404 chaud #11 :Cannot send to channel
PRIVMSG #42 hi
:No recipient given (PRIVMSG)
:Aurora.AfterNET.Org 421 chaud recipient :Unknown command
PRIVMSG user_5 hello
PRIVMSG user_5 good morning wake up
:user_5!user_5@1C72DB:D17C90:7D2707:482FF6:IP PRIVMSG chaud :hi hi
PRIVMSG useee hello there
:Aurora.AfterNET.Org 401 chaud useee :No such nick
^C
vloureir@Vinicius:~$ nc -C irc.afternet.org 6667
NOTICE * :*** Looking up your hostname
NOTICE * :*** Found your hostname
USER vini 0 * :vini
NICK vini
PING :1778091801
PONG 1778091801
NOTICE * :*** Checking your client version (sorry..)

PRIVMSG <nick> :VERSION
001 <nick> 


:luna.AfterNET.Org PRIVMSG vini :VERSION
:luna.AfterNET.Org 001 vini :Welcome to the AfterNET IRC Network, vini
:luna.AfterNET.Org 002 vini :Your host is luna.AfterNET.Org, running version u2.10.12.14+Nefarious(2.0.0)
:luna.AfterNET.Org 003 vini :This server was created Fri May 29 2026 at 23:21:43 UTC
:luna.AfterNET.Org 004 vini luna.AfterNET.Org u2.10.12.14+Nefarious(2.0.0) adgiknoqswxzBDHLNORWX abCcDdhikLlMmNnOopQRrSsTtvZz bhkLlov
:luna.AfterNET.Org 005 vini WHOX WALLCHOPS WALLHOPS WALLVOICES USERIP CPRIVMSG CNOTICE NAMESX UHNAMES SILENCE=25 WATCH=128 MODES=6 MAXCHANNELS=50 :are supported by this server
:luna.AfterNET.Org 005 vini MAXBANS=50 NICKLEN=30 MAXNICKLEN=30 TOPICLEN=250 AWAYLEN=250 KICKLEN=250 CHANNELLEN=100 MAXCHANNELLEN=200 CHANTYPES=# PREFIX=(ohv)@%+ STATUSMSG=@%+ BOT=B CHANMODES=be,k,Ll,aCcDdiMmNnOpQRrSsTtZz :are supported by this server
:luna.AfterNET.Org 005 vini CASEMAPPING=rfc1459 NETWORK=AfterNET MAXLIST=b:50,e:45 ELIST=CT EXCEPTS=e MAXEXCEPTS=45 EXTBANS=~,acjnqrmM :are supported by this server



:luna.AfterNET.Org 251 vini :There are 212 users and 93 invisible on 9 servers
:luna.AfterNET.Org 252 vini 23 :operator(s) online
:luna.AfterNET.Org 254 vini 181 :channels formed
:luna.AfterNET.Org 255 vini :I have 97 clients and 1 servers
:luna.AfterNET.Org 265 vini :Current local users: 97 Max: 258
:luna.AfterNET.Org 266 vini :Current global users: 305 Max: 568
:luna.AfterNET.Org NOTICE vini :Highest connection count: 108 (107 clients)
:luna.AfterNET.Org 422 vini :MOTD File is missing
:luna.AfterNET.Org NOTICE vini :on 1 ca 1(6) ft 10(10)
:luna.AfterNET.Org 396 vini AN-EA7BE6BE.net.novis.pt :is now your hidden host
:vini!vini@AN-EA7BE6BE.net.novis.pt MODE vini +x
JOIN #42
:vini!vini@AN-EA7BE6BE.net.novis.pt JOIN #42
:luna.AfterNET.Org 353 vini = #42 :vini @user_5
:luna.AfterNET.Org 366 vini #42 :End of /NAMES list.
JOIN #11
:vini!vini@AN-EA7BE6BE.net.novis.pt JOIN #11
:luna.AfterNET.Org 353 vini = #11 :vini @user_5
:luna.AfterNET.Org 366 vini #11 :End of /NAMES list.
PRIVMSG #42,#11,user_5 :wake up
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel
:luna.AfterNET.Org 404 vini #11 :Cannot send to channel
PRIVMSG user_5,user_66 :wakey wakey
PRIVMSG #11 hi
PRIVMSG #42,#11 alo
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel
:luna.AfterNET.Org 404 vini #11 :Cannot send to channel
PRIVMSG #42 alo
PRIVMSG #42,user_5,user_66 hello there
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel

*/