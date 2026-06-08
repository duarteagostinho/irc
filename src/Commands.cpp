/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/08 15:14:45 by vloureir         ###   ########.fr       */
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
			topic(line, index);
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
				std::string response = "Now talking on " + av[1] + "\r\n";
				if (_channels[i].getTopic() != "")
					send(_fds[index].fd, _channels[i].printTopic().c_str(), _channels[i].printTopic().size() + 1, 0);
				send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
			}
			return ; // If there was a channel with that name, we either fail to enter or enter it
		}
	}
	// If channel doesn't exists, create one
	int x = _channels.size();
	_channels.push_back(Channel(av[1]));
	_channels[x].addUser(_users[index].getNickname(), 1);
	
	std::string response = "Now talking on " + av[1] + "\r\n";
	send(_fds[index].fd, response.c_str(), response.size() + 1, 0);
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

void Server::topic(std::string data, int index)
{
/*

/topic
Topic for #channel is: <TOPIC HERE>

/topic <topic>
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		 <operator> has changed the topic to: <topic>	-> BROADCAST	

*/
	// (void)index;
	// if (av.size() == 1) // SEND BACK MESSAGE
	// 	std::cout << INV_USAGE << std::endl;
	// else



	std::string old_topic = ""; // THIS IS A PLACEHOLDER, TRADE FOR THE ACTUAL VARIABLE

	int i = 0;
	for ( ; data[i] != 'C'; i++)
		;
	i++;
	data = &data[i];

	if (!data[i] || (data[i] == ' ' && !data[i + 1]))
	{
		if (old_topic.empty())  // SEND BACK THIS MESSAGE
			std::cout << "#channel: No topic is set." << std::endl;
		else 
			std::cout << "Topic for #channel is: " << old_topic << std::endl;
	}
	else
	{
		if (index) // SEND BACK THIS MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
		else // BROADCAST THIS MESSAGE TO EVERYONE
			std::cout << "<nick> has changed the topic to: " << data << std::endl;
	}
	
	// TODO: update the topic variable of the channel
//	this->test._topic = data; // How to solve this shit ?
	old_topic = data;

	std::cout << "[" << data << "]" << std::endl;

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


/*

about the server message

'*' indicates the message is from the server
	otherwise it has the nickname before it

maybe could use a flag to indicate
1- send the mesage back to the sender
2- send the message back to everyone BUT the sender
3- broadcast the message

*/