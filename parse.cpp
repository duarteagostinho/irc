#include <iostream>
#include <string>
#include <vector>

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
	if (inv > 0 && channel.invMode() == false)
		channel.invMode() == true;
	else if (inv < 0 && channel.invMode() == true)
		channel.invMode() == false;

	if (topic > 0 && channel.topicMode() == false)
		channel.topicMode() == true;
	else if (topic < 0 && channel.topicMode() == true)
		channel.topicMode() == false;

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