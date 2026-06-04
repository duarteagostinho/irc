/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/04 07:12:00 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

int Commands::check_cmd(std::string data, std::vector<std::string>& av)
{
	// Clean the string from the carriage return
	std::string to_del = "\r\n";
	size_t pos = data.find(to_del);
	if (pos != data.npos)
		data.erase(pos, to_del.length());
	
	// Create an argv from the data
	std::stringstream split(data);
	std::string token;
	while (std::getline(split, token, ' '))
		av.push_back(token);
		
	// If no line is sent, return err
	if (av.empty())
		return (-1) ;
	
	// Check the first index for the chosen commands
	const size_t size = 5;
	std::array<std::string, size> accepted = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN"};
	for (size_t i = 0; i < accepted.size(); i++)
	{
		if (accepted[i] == av[0])
			return (i);
	}
	return (-1);
}

void Commands::exec_cmd(std::string data)
{
	std::vector<std::string> av;
	int i = Commands::check_cmd(data, av);

	for (size_t i = 0; i < av.size(); i++)
		std::cout << "inside exec_cmd: " << av[i] << std::endl;

	switch(i)
	{
		case 0:
			Commands::kick(av, 0);
			break;
		case 1:
			Commands::invite(av, 0);
			break;
		case 2:
			Commands::topic(data, 0);
			break;
		case 3:
			Commands::mode();
			break;
		case 4:
			Commands::join(av, 0);
			break;
		default:
			std::cout << "Invalid Command" << std::endl;
	}
	return ;
}


void Commands::kick(std::vector<std::string>& av, int op)
{ 
/*

/kick 
Usage: KICK <nick> [reason], kicks the nick from the current channel

/kick <nick>
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		IF (DONT_EXIST)
			*nothing happens*
		ELSE
			/kick chaud
			vloureir__ has kicked chaud from #mtg (vloureir__)			-> CHANNEL

			You have been kicked from #mtg by vloureir__ (vloureir__)	-> KICKED PERSON

			If no reason, operator name = reason.
			
*/

	if (av.size() == 1) // SEND BACK MESSAGE
		std::cout << KICK_USAGE << std::endl;
	else
	{
		if (!op) // SEND BACK THIS MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
		


	}
	std::cout << "kick called\n";
}

void Commands::invite(std::vector<std::string>& av, int op)
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

	if (av.size() == 1) // SEND BACK MESSAGE
		std::cout << INV_USAGE << std::endl;
	else
	{
		if (!op) // SEND BACK THIS MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
		else // BROADCAST THIS MESSAGE TO EVERYONE
		{
			std::cout << "You've invited <nick> to #channel" << std::endl; // SEND BACK TO SENDER
			std::cout << "You have been invited to #channel by <operator>" << std::endl; // SEND BACK TO CLIENT
			// ADD THE INVITED CLIENT TO THE OPERATOR LIST
		}
	}
}

void Commands::topic(std::string data, int op)
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
		if (!op) // SEND BACK THIS MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
		else // BROADCAST THIS MESSAGE TO EVERYONE
			std::cout << "<nick> has changed the topic to: " << data << std::endl;
	}
	
	// TODO: update the topic variable of the channel
//	this->test._topic = data; // How to solve this shit ?
	old_topic = data;

	std::cout << "[" << data << "]" << std::endl;

}

void Commands::mode(void)
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


void Commands::join(std::vector<std::string>& av, int key)
{
/*

/join
Usage: JOIN <channel>, joins the channel

/join alo
*nothing happens*

 /join #mychannel
	IF (keyword)
		 Cannot join #mtg (Requires keyword)
	ELSE
		 Now talking on #channel

IF INVITED, IGNORE THE KEYWORD

*/

	if (av.size() == 1) // SEND BACK MESSAGE
		std::cout << JOIN_USAGE << std::endl;
	else if (av[1][0] != '#')
		return ;
	else
	{
		if (!key) // SEND BACK MESSAGE
			std::cout << "#channel: You are not the channel operator" << std::endl;
		else // SEND BACK MESSAGE
			std::cout << "Now talking on #channel" << std::endl;
		
		// Need to actually add the person to the channel list


		// HOW TO DETERMINE IF THE PERSON WAS INVITED ???
		// CANT I JUST SET THE OP FLAG TO 1 ??
	}
	// Channel class could have a vec<string> invited
	// Saves each invited name in there, if the name is there
	// disable the key and remove name from the list


	std::cout << "join called\n";
}

Commands::Commands()
{

}

Commands::Commands(const Commands &other)
{
	(void)other;
}

Commands::~Commands()
{

}

Commands &Commands::operator=(const Commands &other)
{
	(void)other;
	return (*this);
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