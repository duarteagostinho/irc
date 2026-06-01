/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:35:05 by vloureir          #+#    #+#             */
/*   Updated: 2026/06/01 16:06:58 by vloureir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <array>
# include <string>
# include <sstream>
# include <unistd.h>
# include <vector>

class Commands
{
public:
	void exec_cmd(std::string data);
	int check_cmd(std::string data, std::vector<std::string>& av);
	
	void kick(void);
	void invite(void);
	void topic(std::string data, int op);
	void mode(void);
	void join(std::vector<std::string>& av, int op);

	Commands();
	Commands(const Commands &other);
	Commands &operator=(const Commands &other);

	virtual ~Commands() = 0;

private:

};