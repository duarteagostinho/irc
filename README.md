*This project has been created as part of the 42 curriculim by duandrad, gufreire and vloureir.*

# Description:
	The IRC project has the goal of creating an IRC Server from scratch using c++. It is supposed to follow IRC protocol standards and it is supposed to ne able to connect to an IRC Client, such as, hexchat to test its functionality.

	This project aims to develop knowledgeand experience with network programing, concurrency and client-server architecture in c++. This project aims to instruct how internet standards and protocols enable communication between diferent computers, and real-time messaging systems using TCP/IP communication. The basic implementation includes user authentication, channel creation and management, private messaging and handling various commands, such as, USER, NICK, KICK, TOPIC, JOIN. In this project there is no server-to-server communication, only client-to-server communication.

 ## IRC:
     IRC (Internet Relay Chat) is a text-based protocol designed for real time instant messaging through TCP/IP connections. It follows a client-server model with the server as its backbone and connection point to all the different clients and other servers, thus forming an IRC network.

# Instruction:
   ## To start the Server:
		./ircserver <port> <password>
	
   ## To connect with nc:
		nc -C <server ip> <port>
		PASS <pasword>
		NICK <nickname>
		USER <username>
	
   ## To connect with Hexchat:
		ADD networks
		EDIT the created network
			edit the ip/port to the server ip and port
			select the 1st(Connect to selected server only) and 5th(Accept invalid SSL certificates) boxes
			Choose Nick name
			Choose Real name
			Choose User name
			Login method is Server password
			Input the correct password
			Character set is UTF-8(Unicode)
		Connect to created network

   ## Commands
		

# Resources:

 ## Usage:

 **CMDS**:

  - **KICK:** to remove a user from a channel

      - **EXAMPLE:**  #KICK ***user channel***

  - **INVITE:** to invite a user to a channel

      - **EXAMPLE:** #INVITE ***user channel***

  - **TOPIC:** to change or view the channel TOPIC

      - **EXAMPLE:** #TOPIC ** **

  - **MODE:** to set modes

    i+/- to set/remove Invite ONLY channel mode

    t+/- to set/remove TOPIC to channel operators

    k+/- to set/remove channel password requirement

    o+/- to give channel operator privilege to a user

    l+/- to set/remove user limit on a channel

      - **EXAMPLE:** #MODE i+ ***user channel***

 ## Articles:

  - https://www.rfc-editor.org/info/rfc1459/
  - 


//check first for "\r\n", if not just append to user.fd and continue
//if not, check for user.registration == false
	//if not just append as normal and exec command
/*
	Parses every message sent on the server. Checks for unregistered users before parsing what command has to be executed.
*/
void Server::getMessage(std::string &line, char *buffer, int i)
{
	if (_users[i].getRegistration() == false)
	{
		_users[i].recvBuf.append(buffer);
		while (_users[i].getRegistration() == false)
		{
			std::string old = _users[i].recvBuf;
			registerUser(i);
			if ((size_t)i >= _users.size() || _users[i].recvBuf == old)
				break ;
		}
		return;
	}
	line.append(buffer);
	size_t find = line.find("\r\n");
	if (find != std::string::npos)
	{
		std::string cmd_line = line.substr(0, find);
		exec_cmd(line, i);
		line.erase(0, find + 2);
		find = line.find("\r\n");
	}
	line.clear();
}

//verificar primeiro se o buffer tem "\r\n", depois verificar de que fd veio, e depois juntar a string do fd.user
//se o buffer tiver "\r\n" limpa se o buffer e a variavel do fd.user
//se o buffer NAO tiver "\r\n" limpa se apenas o buffer e faz se append na string fd.user

//primeiro verificar se o buffer tem "\r\n", se sim, append na string do user, executa se o command e limpa se o buffer e a string do user
//se o buffer nao tem "\r\n" append na string do user e limpar apenas o buffer
//PRoblema, como diferenciar de onde vem o buffer, ve se pelo fd e com esse iterador procurar o user

//acho que nao e preciso line ou apenas e preciso para fazer encontrar no buffer o "\r\n" se existir

void Server::getMessage(char *buffer, int i)
{
	std::cout << std::endl;
	//fazendo append primeiro
	_users[i].recvBuf.append(buffer);

	std::cout << _users[i].recvBuf << std::endl;
	std::cout << _users[i].recvBuf.size();
	std::cout << std::endl;

	size_t find = _users[i].recvBuf.find("\r\n");
	if (find != std::string::npos)
	{
		// // THIS LOOP IS JUST SENDING THE MESSAGE AND NICK BACK TO EACH OTHER CLIENT
		// for (size_t j = 1; j < _fds.size(); j++) // start at 1 to always ignore the listening socket
		// {
		// 	std::ostringstream ss;
		// 	std::string str;
		// 	ss << _users[i].getNickname() << ": " << _users[i].recvBuf << "\r\n";
		// 	str = ss.str();

		// 	if (_fds[i].fd != _fds[j].fd) // Send to every fd that is not mine
		// 		send(_fds[j].fd, str.c_str(), str.size() + 1, 0);
		// }
		// _users[i].recvBuf.erase(find, 2);
		exec_cmd(_users[i].recvBuf, i);
		_users[i].recvBuf.erase();
	}
	else
		return ;

//verificando primeiro se tem "\r\n"
	// std::string tmp;
	// tmp.append(buffer);
	// size_t find = tmp.find("\r\n");
	// if (find != std::string::npos)//isto significa que encontrou "\r\n"
	// {
	// 	//encontar o user pelo fd para dar append do buffer na string
	// 	_users[i].recvBuf.append(buffer);
	// 	exec_cmd(_users[i].recvBuf, i);
	// 	//limpar a string usada para executar
	// 	_users[i].recvBuf.erase();
	// }
	// else
	// {
	// 	//apenas dar append do buffer na string do user
	// 	_users[i].recvBuf.append(buffer);
	// }
}