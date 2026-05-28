/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:37:18 by vloureir          #+#    #+#             */
/*   Updated: 2026/05/28 11:09:03 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

int Commands::check_cmd(std::string data)
{
	size_t i = 0;
	std::array<std::string, 4> accepted = {"KICK", "INVITE", "TOPIC", "MODE"};
	
	for ( ;i < accepted.size(); i++)
	{
		if (accepted[i] == data)
			break ;
	}
	switch(i)
	{
		case 0:
			Commands::kick();
			return -1;
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
	return 0;
}


void Commands::kick(void)
{ 
	std::cout << "kick called\n";
//	close(client_socket);
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