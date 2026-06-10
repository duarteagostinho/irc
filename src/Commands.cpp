/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/10 09:48:10 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

int Server::check_cmd(std::string line, std::vector<std::string>& av)
{
	// Clean the string from the carriage return
	std::string to_del = "\r\n";
	size_t pos = line.find(to_del);
	if (pos != line.npos)
		line.erase(pos, to_del.length());
	
	// Create an argv from the data
	std::stringstream split(line);
	std::string token;
	while (std::getline(split, token, ' '))
		av.push_back(token);
		
	// If no line is sent, return err
	if (av.empty())
		return (-1) ;
	
	// Check the first index for the chosen commands
	const size_t size = 5;
	std::string accepted[5] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN"};
	for (size_t i = 0; i < size; i++)
	{
		if (accepted[i] == av[0])
			return (i);
	}
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

	// std::cout << "size: " << _channels.size() << std::endl;
	// for (size_t i = 0; i < _channels.size(); i++)
	// 	std::cout << "channels: "  << _channels[i].getName() << std::endl;

	std::cout << "server line: " << line << std::endl;
	line.clear();
	return ;
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

				std::string response2 = "332 " + _users[index].getNickname() + " " + av[1] + " :Hello world\r\n";
				send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);

				std::string response3 = "353 " + _users[index].getNickname() + " = " + av[1] + ":+o" + _users[index].getNickname() + "\r\n";
				send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

				std::string response4 = "366 " + _users[index].getNickname() + " " + av[1] + ":End of /NAMES list\r\n"; 
				send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);

				// std::string response = "Now talking on " + av[1] + "\r\n";
				// if (_channels[i].getTopic() != "")
				// 	send(_fds[index].fd, _channels[i].printTopic().c_str(), _channels[i].printTopic().size() + 1, 0);
				// send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
			return ; // If there was a channel with that name, we either fail to enter or enter it
		}
	}
	// If channel doesn't exists, create one
	int x = _channels.size();
	_channels.push_back(Channel(av[1]));
	_channels[x].addUser(_users[index].getNickname(), 1);


	std::string response1 = ":chaud!vini@10.12.3.12 JOIN " + av[1] + "\r\n";
	send(_fds[index].fd, response1.c_str(), response1.size() + 1, 0);

	// std::string response2 = ":10.12.3.12 332 " + _users[index].getNickname() + " " + av[1] + " :Hello world\r\n";
	// send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);

	std::string response3 = ":ft_irc 353 " + _users[index].getNickname() + " = " + av[1] + " :@" + _users[index].getNickname() + "\r\n";
	send(_fds[index].fd, response3.c_str(), response3.size() + 1, 0);

	std::string response4 = ":ft_irc 366 " + _users[index].getNickname() + " " + av[1] + " :End of /NAMES list\r\n"; 
	send(_fds[index].fd, response4.c_str(), response4.size() + 1, 0);

	std::cout << "printing responses:\n" << response1 << response3 + response4 << std::endl;
	// std::string response2 = "Now talking on " + av[1] + "\r\n";
	// send(_fds[index].fd, response2.c_str(), response2.size() + 1, 0);
}

void Server::kick(std::vector<std::string>& av, int index)
{ 
	if (av.size() == 1) // Missing arguments
	{
		std::string response = KICK_USAGE;
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		return ;
	}
//	if (_nick[index].getChannel().isOperator())
//	if (_users[index].getChannel())




	if (av.size() == 1) // SEND BACK MESSAGE
		std::cout << KICK_USAGE << std::endl;
	else
	{
		if (index) // SEND BACK THIS MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
		


	}
	std::cout << "kick called\n";
}

void Server::invite(std::vector<std::string>& av, int index)
{ 
/*

/invite
Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel

/invite <nick>
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		 You've invited <nick> to #channel -> operator

		 You have been invited to #channel by <operator>			-> client

*/

	(void)index;
	if (av.size() == 1) // SEND BACK MESSAGE
		std::cout << INV_USAGE << std::endl;
	else
	{
//		if (!_channels.isOperator(_nick[index])) // SEND BACK THIS MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
//		else // BROADCAST THIS MESSAGE TO EVERYONE
		{
			std::cout << "You've invited <nick> to #channel" << std::endl; // SEND BACK TO SENDER
			std::cout << "You have been invited to #channel by <operator>" << std::endl; // SEND BACK TO CLIENT
			// ADD THE INVITED CLIENT TO THE OPERATOR LIST
		}
	}
}

void Server::topic(std::vector<std::string>& av, int index)
{
	(void)av;
	(void)index;

	std::vector<std::string> channels;

	std::cout << "av size: " << av.size() << std::endl;
	if (av.size() == 1) // Missing arguments
	{
		std::string response = ":server 461 <nickname> TOPIC :Not enough parameters\r\n";
		send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		return ;
	}
	// Split the channels argument and loop through them to check every channel received
	splitCommas(av[1], channels);
	for (size_t i = 0; i < channels.size(); i++)
	{
		std::cout << "channel name: " << channels[i] << std::endl;
		
		if (!isChannel(channels[i]))
		{
			std::string response = ":server 403 <nickname> <channels[i]> :No such channel\r\n";
			send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
		}
		else if (av.size() == 2) // Print topic
		{
			if (!topic)
				std::cout << ":server 331 <nickname> <channels[i]> :No topic is set\r\n" << std::endl;
			else
			{
				std::cout << ":server 332 <nickname> <channels[i]> :<topic>\r\n" << std::endl;
				std::cout << ":server 333 <nickname> <channels[i]> <nick!user@ip> <timestamp>\r\n" << std::endl; // timestamp ex: 1781075528				
			}
		}
		else if (!channels[i].isOperator(_users[index].getNickname()))
		{
			std::cout << ":server 482 <nickname> <channels[i]> :You're not channel operator\r\n" << std::endl;
		}
		else
		{
				// 	if av[2][0] == ':'
				// 		merge all avs left into a string, string = new_topic
				// 	else
				// 		new_topic == last av[i]			

		}
	}
}



/*
	av[0] = TOPIC
	
	av[1] = <channel> 
	if (!av[1])
		:server 461 <nickname> TOPIC :Not enough parameters
	else
		if (find(av[1], ',')) // NEED TO SPLIT INTO MULTIPLE CHANNELS
	
	for each channel, try whats below
		
	if (!channel_name)
		:server 403 <nickname> <av[1]> :No such channel

	av[2] = <parameters>
	if !av[2]
		print topic

	(if we get here, check if <nickname> is operator on channel)
	if (!operator)
		:server 482 <nickname> <av[1]> :You're not channel operator
	else
		if av[2][0] == ':'
			merge all avs left into a string, string = new_topic
		else
			new_topic == last av[i]

*/

/*
	av[0] = TOPIC
	
	av[1] = <channel> 
	if (!av[1])
		:server 461 <nickname> TOPIC :Not enough parameters
	else
		if (find(av[1], ',')) // NEED TO SPLIT INTO MULTIPLE CHANNELS
	
	for each channel, try whats below
		
	if (!channel_name)
		:server 403 <nickname> <av[1]> :No such channel

	av[2] = <parameters>
	if !av[2]
		print topic

	(if we get here, check if <nickname> is operator on channel)
	if (!operator)
		:server 482 <nickname> <av[1]> :You're not channel operator
	else
		if av[2][0] == ':'
			merge all avs left into a string, string = new_topic
		else
			new_topic == last av[i]

*/

/*

	
	

	Will receive:
	TOPIC #42,#22 hello there


	If av[1] NOT channel
		SEND SERVER_NAME: No such channel (403)

	If av[1] == true and av[1+] == false
		SEND av[1++]: No such channel (for each invalid channel name) (403)

	if (client NOT on channel)
		SEND 442

	if (NOT TOPIC and on channel)
		SEND 331

	if (TOPIC and on channel)
		SEND 332
		SEND 333
*/

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




void Server::splitCommas(std::string line, std::vector<std::string>& av)
{	
	// Create an argv from the line
	std::stringstream split(line);
	std::string token;
	while (std::getline(split, token, ','))
		av.push_back(token);
}

void Server::mergeString(std::string &line)
{
	std::string tmp = line;

	




}