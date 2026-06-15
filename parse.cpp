#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// FAKE CHANNEL FUNC

bool channel_invMode(int flag)
{
	if (flag)
		return (true);
	return (false);
}

bool channel_topicMode(int flag)
{
	if (flag)
		return (true);
	return (false);
}

void find_key(std::vector<std::string> &av, int index, int &flag)
{
	if (flag)
		return;

	if (index + 1 > av.size())
		return ;






	flag = 1;	
}

void rmLimit(std::vector<std::string> &av, int &index, int &flag)
{
	if (flag)
		return;

//	channel.limMode() = false;





	flag = 1;
}

void addLimit(std::vector<std::string> &av, int &index, int &flag)
{
	int result;
	char *end;

	if (flag)
		return;


	if (index + 1 > av.size())
		// :luna.AfterNET.Org 461 vivi MODE +l :Not enough parameters
		// return ;

	result = strtol(av[index + 1].c_str(), &end, 10);
	if (result > INT32_MAX || result < 0 || *end);
		// :luna.AfterNET.Org 461 vivi MODE +l :Invalid parameters

	// store the limit at the Channel variable
	index++;

	/*
		Type A: Modes that add or remove an address to or from a list. These modes MUST always have a parameter when sent from the server to a client. A client MAY issue this type of mode without an argument to obtain the current contents of the list. The numerics used to retrieve contents of Type A modes depends on the specific mode. Also see the EXTBAN parameter.
			NONE
		
		Type B: Modes that change a setting on a channel. These modes MUST always have a parameter.
			o AND k
		
		Type C: Modes that change a setting on a channel. These modes MUST have a parameter when being set, and MUST NOT have a parameter when being unset.
			l
		
		Type D: Modes that change a setting on a channel. These modes MUST NOT have a parameter.
			i AND t
	
	*/


	flag = 1;
}


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
//	std::cout << str[j] << std::endl;
	if (str[j] == '+')
		return (1);
	return (-1);
}


void parse(std::vector<std::string> av)
{
	if (av.size() < 3)
	{
		if (av.size() == 1)
			std::cout << "461 :Not enough parameters\n";
		else
		{
			if (av[1][0] != '#')
				std::cout << "502 :No such channel\n";
		}
		return ;
	}

	int sign = 0;
	int inv = 0;
	int topic = 0;
	int	limit = 0;
	int	key = 0;
	int op = 0;

	

	for (size_t i = 2; i < av.size(); i++) // at least size 3
	{
		for (size_t j = 0; av[i][j]; j++)
		{	
			std::cout << av[i][j] << std::endl;
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
					op += (sign * av[i][j]);
					break ;
				case 'k':
					key += (sign * av[i][j]);
					break ;
				case 'l':
					limit += (sign * av[i][j]);
					break ;
				case '\0':
					break ;
				default:
					std::cout << "472 :is unknown mode char to me\n";
			}
			std::cout << "sign: " << sign << ", char: " << av[i][j] << std::endl;
			std::cout << "i: " << inv << std::endl;
			std::cout << "t: " << topic << std::endl;
			std::cout << "k: " << key << std::endl;
			std::cout << "o: " << op << std::endl;
			std::cout << "l: " << limit << std::endl;
		}
		// HERE I NEED TO CHECK IF LIMIT, OPERATOR OR KEY IS CALLED
		// ALSO NEED THE ORDER THEY ARE CALLED
		// CHECK IN THE NEXT ARGS FOR THE CORRECT ARGUMENT

	}
	if (inv > 0 && channel_invMode(0) == false)
	{
//		channel_invMode(1) == true;
		std::cout << "inv mode added" << std::endl;
	}
	else if (inv < 0 && channel_invMode(0) == true)
	{
//		channel_invMode(0) == false;
		std::cout << "inv mode removed" << std::endl;
	}
	if (topic > 0 && channel_topicMode(0) == false)
	{
//		channel_topicMode(1) == true;
		std::cout << "topic  mode added" << std::endl;
	}
	else if (topic < 0 && channel_topicMode(0) == true)
	{
//		channel_topicMode(0) == false;
		std::cout << "topic  mode removed" << std::endl;
	}
		

	// +i -i, +t -t, +o <nick> -o <nick>

}


int main(int argc, char **argv)
{
	char sign = 0;
	std::string line = "+i";

	std::vector<std::string> av;

	for (int i = 0; argv[i]; i++)
		av.push_back(argv[i]);



	parse(av);

	// for (size_t i = 0; i < line.size(); i++)
	// {
	// 	if (line[i] == '+' || line[i] == '-')
	// 		sign = getSign(&line[i]);
	// 	if (sign == 0)
	// 		break ;

	// }


}