/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloureir <vloureir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:35:05 by vloureir          #+#    #+#             */
/*   Updated: 2026/05/31 16:22:46 by vloureir         ###   ########.fr       */
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
	static void exec_cmd(std::string data);
	static int check_cmd(std::string data);
	
	static void kick(void);
	static void invite(void);
	static void topic(void);
	static void mode(void);

private:
	Commands();
	Commands(const Commands &other);
	~Commands();
	Commands &operator=(const Commands &other);
};