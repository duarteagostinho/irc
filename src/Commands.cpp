/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/11 18:28:36 by vloureir         ###   ########.fr       */
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

	// std::stringstream split(line);
	// std::string token;
	// while (std::getline(split, token, ' '))
	// 	av.push_back(token);
		
	// If no line is sent, return err
	if (av.empty())
		return (-1) ;
	
	// Check the first index for the chosen commands
	const size_t size = 5;
	std::string accepted[5] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN"};
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
		default:
			std::cout << "Invalid Command" << std::endl;
	}
//	std::cout << "server line: " << line << std::endl;
	line.clear();
//	return ;
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
					std::string response2 = ":server 332 " + _users[index].getNickname() + " " + av[1] + " :" + _channels[i].getTopic() + "\r\n";
					send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);

					response2 = ":server 333 " + _users[index].getNickname() + " " + av[1] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528
					send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);
				}
				std::string response3 = ":server 353 " + _users[index].getNickname() + " = " + av[1] + ":+o" + _users[index].getNickname() + "\r\n";
				send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

				std::string response4 = ":server 366 " + _users[index].getNickname() + " " + av[1] + ":End of /NAMES list\r\n"; 
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

	std::string response3 = ":server 353 " + _users[index].getNickname() + " = " + av[1] + " :@" + _users[index].getNickname() + "\r\n";
	send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

	std::string response4 = ":server 366 " + _users[index].getNickname() + " " + av[1] + " :End of /NAMES list\r\n"; 
	send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);
}

// void Server::join(std::vector<std::string>& av, int index)
// {
// 	if (av.size() == 1) // Missing arguments
// 	{
// 		std::string response = ":server 461 <nickname> JOIN :Not enough parameters\r\n";
// 		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
// 		return ;
// 	}
// 	else if (av[1][0] != '#') // Missing correct server name format
// 	{
// 		std::string response = ":server 403 <nickname> av[1] :No such channel\r\n";
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
	
	if (av.size() < 3) // Missing arguments
	{
		std::string response = ":server 461 <nickname> KICK :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!isChannel(av[1]))
	{
		std::string response = ":server 403 <nickname> av[1] :No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!_channels[getChannelIndex(av[1])].isOperator(_users[index].getNickname()))
	{
		std::string response = ":server 482 <nickname> av[1] :You're not channel operator\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!_channels[getChannelIndex(av[1])].isUserOnChannel(av[2]))
	{
		std::string response = ":server 401 <nickname> av[2] :No such nick\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else
	{
		std::string response = getClientInfo(index) + " KICK " + av[1] + " :" + craftStringSpaces(av[0]);
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
		std::string response = ":server 461 " + _users[index].getNickname() + " TOPIC :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitString(av[1], channels, ',');
	for (size_t i = 0; i < channels.size(); i++)
	{
//		std::cout << "channel name: " << channels[i] << std::endl;

		if (!isChannel(channels[i]))
		{
			std::string response = ":server 403 " + _users[index].getNickname() + " " + channels[i] + " :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		}
		else if (av.size() == 2) // Print topic
		{
			if (_channels[getChannelIndex(channels[i])].getTopic() == "")
			{
				std::string response =":server 331 " + _users[index].getNickname() + " " + channels[i] + " :No topic is set\r\n";
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
			else
			{
				std::string response = ":server 332 " + _users[index].getNickname() + " " + channels[i] + " :" + _channels[getChannelIndex(channels[i])].getTopic() + "\r\n";
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
				
				response = ":server 333 " + _users[index].getNickname() + " " + channels[i] + " " + getClientInfo(index) + " 0\r\n"; // timestamp ex: 1781075528	
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
		}
		else if (!_channels[getChannelIndex(channels[i])].isOperator(_users[index].getNickname()))
		{
			std::string response = ":server 482 " + _users[index].getNickname() + " " + channels[i] + " :You're not channel operator\r\n";
			send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		}
		else
		{
			std::string new_topic = craftStringSpaces(av[0]);
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
	std::cout << "INDEX: " << index << std::endl << "NICKNAME: " << _users[index].getNickname() << "\n\n" << _channels[0].opFlag_users.size() << "\n\n";
	if (av.size() == 1) // Missing arguments
	{
		std::string response = ":server 337 " + _users[index].getNickname() + " :End of Invite List\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (av.size() < 3)
	{
		std::string response = ":server 461 " + _users[index].getNickname() + " INVITE :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (doesUserExist(av[1]))
	{
		std::string response = ":server 401 " + _users[index].getNickname() + " " + av[2] +  " :No such nick\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (isChannel(av[2]))
	{
		std::string response = ":server 403 " + _users[index].getNickname() + " " + av[2] + " :No such channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	// CHECK IF USER IS ALREADY ON CHANNEL
	else if (_channels[getChannelIndex(av[2])].isUserOnChannel(av[1]))
	{
		// :luna.AfterNET.Org 443 jinx vini #42 :is already on channel
		std::string response = ":server 443 " + _users[index].getNickname() + " " + av[1] + " " + av[2] + " :is already on channel\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else if (!_channels[getChannelIndex(av[2])].isOperator(_users[index].getNickname()))
	{
		std::string response = ":server 482 " + _users[index].getNickname() + " " + av[2] + " :You're not channel operator\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
	}
	else
	{
		// :Aurora.AfterNET.Org 341 alo chaud #a // SENDER GETS THIS
		std::string sender_response = ":server 341" + _users[index].getNickname() + " " + av[1] + " " + av[2] + "\r\n";
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

std::string Server::craftStringSpaces(std::string str)
{
	std::stringstream ss(str);
	std::string ret;

	ss >> ret >> ret;
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
