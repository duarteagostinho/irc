#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unordered_map>

// FAKE CHANNEL FUNC
#define INT_MAX 2147483647

char getSign(std::string line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] != '+' && line[i] != '-')
			return (line[i]);
	}
	return (0);
}

int adv_peek(std::string str, size_t &j)
{
	size_t valid_j;

	while ((str[j] == '+' || str[j] == '-') && str[j])
	{
		valid_j = j;
		j++;
	}
	j = valid_j;
	if (str[j] == '+')
		return (1);
	return (-1);
}

void handleInvite(int flag)
{
	if (flag > 0)
	{
		std::cout << "getUserInfo() MODE <channel> +i" << std::endl;
		// channel.setInvMode(true);
	}
	else if (flag < 0)
	{
		std::cout << "getUserInfo() MODE <channel> -i" << std::endl;
		// channel.setInvMode(false);
	}
}

void handleTopic(int flag)
{
	if (flag > 0)
	{
		std::cout << "getUserInfo() MODE <channel> +t" << std::endl;
		// channel.setTopicMode(true);
	}
	else if (flag < 0)
	{
		std::cout << "getUserInfo() MODE <channel> -t" << std::endl;
		// channel.setTopicMode(false);
	}
}

void handleLimit(std::vector<std::string>const &av, const int &index, const int &sign, int &offset, int &flag)
{
	int result;
	char *end;

	if (flag)
		return;
	if (sign < 0) // remove doesn't need arguments
		std::cout << "getUserInfo() MODE <channel> -l" << std::endl;
	else
	{
		if (index + offset >= av.size())
		{
			std::cout << ":irc.server 461 <nick> MODE +l :Not enough parameters" << std::endl;
		}
		else // valid amount of arguments, check if it's a valid num
		{
			result = strtol(av[index + 1].c_str(), &end, 10);
			if (result > INT_MAX || result < 0 || *end)
			{
				std::cout << ":irc.server 461 <nick> MODE +l :Invalid parameters\n";
			}
			else
			{
				std::cout << "getUserInfo() MODE <channel> +l " << result << std::endl;
				// channel.limit = result;
				// channel.setLimitMode(true);
			}
			offset++;
		}
	}
	flag = 1;
}


void handleKey(std::vector<std::string>const &av, const int &index, const int &sign, int &offset, int &flag)
{
	std::string c;

	if (flag)
		return;

	std::string channel_key = "lwxxw";

	// int ch_i = getChannelIndex(av[1]);
	if (index + offset >= av.size())
	{
		(sign < 0) ? c = "-" : c = "+";
		std::cout << ":irc.server 461 <nick> MODE " + c + "k :Not enough parameters" << std::endl;
	}
	else
	{
		if (sign < 0) // remove password
		{
			if (av[index + offset] != channel_key) // incorrect password
			{
				std::cout << ":irc.server 467 <nick> <channel> :Channel key already set\n";
				offset++;
				return ;
			}
			std::cout << "getUserInfo() MODE <channel> -k " << av[index + offset] << std::endl; // broadcast
			channel_key = "";
			// channel.SetKeyMode(false);
			// channel.SetKet("");
		}
		else // add password
		{
			channel_key = av[index + offset];
			// channel.SetKeyMode(true);
			std::cout << "getUserInfo() MODE <channel> +k " << av[index + offset] << std::endl; // broadcast
		}
		offset++;
	}
	flag = 1;
}

void getOperatorData(std::unordered_map<std::string, int> &operators, std::vector<std::string> av, int i, int sign, int &offset, int index)
{
	auto it = operators.find(av[i + offset]);

	if (!index) // CHECK IF THE USER AV[I + OFFSET] IS ON THE CHANNEL
		return ;
		// 		std::cout << ":irc.server 401 _users[index].getNickname() av[i + offset] :No such nick\r\n";

		// //		std::string response = ":irc.server 401 " + _users[index].getNickname() + " " + av[i + offset] + " :No such nick\r\n";
		// //		send(_fds[index].fd, response.c_str(), response.size(), 0);
	else
	{
		if (it != operators.end())
		{
			it->second += sign;
		}
		else
		{
			operators.insert(std::pair<const std::string, int>(av[i + offset], sign));
			
		}
	}
	offset++;
}

void handleOperator(std::unordered_map<std::string, int> &operators)
{
	for (auto it = operators.begin(); it != operators.end(); it++)
	{
		std::cout << "Name: " << it->first << ", op status: " << it->second << std::endl;
	}
}

void parse(std::vector<std::string> av)
{
	std::unordered_map<std::string, int> operators;


	if (av.size() < 3)
	{
		if (av.size() == 1)
			std::cout << ":irc.server 461 :Not enough parameters\n";
		else
		{
			if (av[1][0] != '#')
				std::cout << ":irc.server 502 :No such channel\n";
		}
		return ;
	}

	int sign, offset;
	int inv = 0;
	int topic = 0;
	int	limit_flag = 0;
	int	key_flag = 0;

	int op = 0;

	int index;
	for (size_t i = 2; i < av.size(); ) // at least size 3
	{
		offset = 1;
		sign = 1;

		for (size_t j = 0; av[i][j]; j++)
		{
			switch(av[i][j])
			{
				case '+':
					sign = adv_peek(av[i], j);
					break ;
				case '-':
					sign = adv_peek(av[i], j);
					break ;
				case 't':
					topic += (sign * av[i][j]);
					break ;
				case 'i':
					inv += (sign * av[i][j]);
					break ;
				case 'o':
					getOperatorData(operators, av, i, sign, offset, index);
					break ;
				case 'k':
					handleKey(av, i, sign, offset, key_flag);
					break ;
				case 'l':
					handleLimit(av, i, sign, offset, limit_flag);
					break ;
				case '\0':
					break ;
				default:
					std::cout << ":irc.server 472 <nick> ";
					std::cout << av[i][j];
					std::cout << " :is unknown mode char to me\n";
			}
// 			std::cout << "sign: " << sign << ", char: " << av[i][j] << std::endl;
// 			std::cout << "i: " << inv << std::endl;
// 			std::cout << "t: " << topic << std::endl;
//			std::cout << "k: " << key << std::endl;
// 			std::cout << "o: " << op << std::endl;
//			std::cout << "l: " << limit << std::endl;
		}
		// HERE I NEED TO CHECK IF LIMIT, OPERATOR OR KEY IS CALLED
		// ALSO NEED THE ORDER THEY ARE CALLED
		// CHECK IN THE NEXT ARGS FOR THE CORRECT ARGUMENT
		i += offset;


	}
	if (inv)
		handleInvite(inv);

	if (topic)
		handleTopic(topic);
	handleOperator(operators);


	// +i -i, +t -t, +o <nick> -o <nick>


	// HERE I CALL THE FUNCTION THAT PRINT THE MODES OF THE CHANNEL !!!!!
}


int main(int argc, char **argv)
{
	std::vector<std::string> av;

	for (int i = 0; argv[i]; i++)
		av.push_back(argv[i]);
	parse(av);
}

/* Types of flags

	Type A: Modes that add or remove an address to or from a list. These modes MUST always have a parameter when sent from the server to a client. A client MAY issue this type of mode without an argument to obtain the current contents of the list. The numerics used to retrieve contents of Type A modes depends on the specific mode. Also see the EXTBAN parameter.
		NONE
	
	Type B: Modes that change a setting on a channel. These modes MUST always have a parameter.
		o AND k
	
	Type C: Modes that change a setting on a channel. These modes MUST have a parameter when being set, and MUST NOT have a parameter when being unset.
		l
	
	Type D: Modes that change a setting on a channel. These modes MUST NOT have a parameter.
		i AND t

*/