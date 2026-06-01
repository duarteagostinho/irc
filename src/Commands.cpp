/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/01 14:05:17 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Commands.hpp"

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
		std::cout << "return: " << av[i] << std::endl;

	switch(i)
	{
		case 0:
			Commands::kick();
			break;
		case 1:
			Commands::invite();
			break;
		case 2:
			Commands::topic();
			break;
		case 3:
			Commands::mode();
			break;
		case 4:
			Commands::join();
			break;
		default:
			std::cout << "Invalid Command" << std::endl;
	}
	return ;
}


void Commands::kick(void)
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
	std::cout << "kick called\n";
}

void Commands::invite(void)
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

	std::cout << "invite called\n";
}

void Commands::topic(void)
{
/*

/topic
Topic for #channel is: <TOPIC HERE>

/topic <topic>
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		 <operator> has changed the topic to: <topic>				-> BROADCAST	

*/

	
	std::cout << "topic called\n";
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


void Commands::join(void)
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