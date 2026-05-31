/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/05/31 16:26:02 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

int Commands::check_cmd(std::string data)
{
	// Clean the string from the carriage return
	std::string to_del = "\r\n";
	int pos = data.find(to_del);
	if (pos != data.npos)
		data.erase(pos, to_del.length());
	
	// Create an argv from the data
	std::stringstream split(data);
	std::string token;
	std::vector<std::string> av;
	while (std::getline(split, token, ' '))
		av.push_back(token);

	// Check the first index for the chosen commands
	std::array<std::string, 4> accepted = {"KICK", "INVITE", "TOPIC", "MODE"};
	for (size_t i ; i < accepted.size(); i++)
	{
		if (accepted[i] == av[0])
			return (i);
	}
	return (-1);
}

void Commands::exec_cmd(std::string data)
{
	int i = Commands::check_cmd(data);

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
		default:
			std::cout << "Invalid Command" << std::endl;
	}
	return ;
}


void Commands::kick(void)
{ 
	std::cout << "kick called\n";
}

void Commands::invite(void)
{ 
	std::cout << "invite called\n";
}

void Commands::topic(void)
{ 
	std::cout << "topic called\n";
}

void Commands::mode(void)
{ 
	std::cout << "mode called\n";
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