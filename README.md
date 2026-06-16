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