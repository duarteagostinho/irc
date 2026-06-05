/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:35:05 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/05 15:39:19 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
//# include <array>
# include <string>
# include <sstream>
# include <unistd.h>
# include <vector>

# define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel"
# define INV_USAGE "Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel"
# define JOIN_USAGE "Usage: JOIN <channel>, joins the channel"
# define NOT_OPERATOR ": You are not the channel operator"

class Server;

class Commands
{
public:
	void	exec_cmd(std::string data, int i);
	int		check_cmd(std::string data, std::vector<std::string>& av);
	
	void	kick(std::vector<std::string>& av, int op);
	void	invite(std::vector<std::string>& av, int op);
	void	topic(std::string data, int op);
	void	mode(void);
	void	join(std::vector<std::string>& av, int i);

	Commands();
	Commands(const Commands &other);
	Commands &operator=(const Commands &other);

	virtual ~Commands() = 0;

private:

};