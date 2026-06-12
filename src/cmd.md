

///////////////////////////////////////////////////			TOPIC		///////////////////////////////////////////////////


/*
	av[0] = TOPIC
	
	av[1] = <channel> 
	if (!av[1])
		:server 461 <nickname> TOPIC :Not enough parameters
	else
		if (find(av[1], ',')) // NEED TO SPLIT INTO MULTIPLE CHANNELS
	
	for each channel, try whats below
		
	if (!channel_name)
		:server 403 <nickname> <av[1]> :No such channel

	av[2] = <parameters>
	if !av[2]
		print topic

	(if we get here, check if <nickname> is operator on channel)
	if (!operator)
		:server 482 <nickname> <av[1]> :You're not channel operator
	else
		if av[2][0] == ':'
			merge all avs left into a string, string = new_topic
		else
			new_topic == last av[i]

*/

/*
	av[0] = TOPIC
	
	av[1] = <channel> 
	if (!av[1])
		:server 461 <nickname> TOPIC :Not enough parameters
	else
		if (find(av[1], ',')) // NEED TO SPLIT INTO MULTIPLE CHANNELS
	
	for each channel, try whats below
		
	if (!channel_name)
		:server 403 <nickname> <av[1]> :No such channel

	av[2] = <parameters>
	if !av[2]
		print topic

	(if we get here, check if <nickname> is operator on channel)
	if (!operator)
		:server 482 <nickname> <av[1]> :You're not channel operator
	else
		if av[2][0] == ':'
			merge all avs left into a string, string = new_topic
		else
			new_topic == last av[i]

*/

/*

	Will receive:
	TOPIC #42,#22 hello there


	If av[1] NOT channel
		SEND SERVER_NAME: No such channel (403)

	If av[1] == true and av[1+] == false
		SEND av[1++]: No such channel (for each invalid channel name) (403)

	if (client NOT on channel)
		SEND 442

	if (NOT TOPIC and on channel)
		SEND 331

	if (TOPIC and on channel)
		SEND 332
		SEND 333
*/


///////////////////////////////////////////////////			KICK		///////////////////////////////////////////////////


/*
// KICK <channel> <nickname> <reason>
// Does not accept multiple channels, nor multiple users


	KICK chaud #a,#b
	:luna.AfterNET.Org 403 vivi chaud :No such channel
	:chaud!vloureir@81C240:F52884:7D2707:482FF6:IP PRIVMSG #a :hello
	KICK chaud #a
	:luna.AfterNET.Org 403 vivi chaud :No such channel
	KICK #a,#b chaud
	:luna.AfterNET.Org 403 vivi #a,#b :No such channel
	KICK #a chaud
	:vivi!vini@AN-EA7BE6BE.net.novis.pt KICK #a chaud :vivi
	:chaud!vloureir@81C240:F52884:7D2707:482FF6:IP JOIN #a
	KICK #a chaud,jinxjinx
	:luna.AfterNET.Org 401 vivi chaud,jinxjinx :No such nick
	KICK #a jinxjinx :I dont like you
	:vivi!vini@AN-EA7BE6BE.net.novis.pt KICK #a jinxjinx :I dont like you
	KICK #a chaud hello little one
	:vivi!vini@AN-EA7BE6BE.net.novis.pt KICK #a chaud :one

	KICK
	:luna.AfterNET.Org 461 vivi KICK :Not enough parameters
	KICK #a
	:luna.AfterNET.Org 461 vivi KICK :Not enough parameters
	KICK #b
	:luna.AfterNET.Org 461 vivi KICK :Not enough parameters
	KICK #b chaud
	:luna.AfterNET.Org 482 vivi #b :You're not channel operator
	KICK #a alo
	:luna.AfterNET.Org 401 vivi alo :No such nick
	KICK #bb chaud
	:luna.AfterNET.Org 403 vivi #bb :No such channel

*/


///////////////////////////////////////////////////			INVITE		///////////////////////////////////////////////////


/*

/invite
Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel

/invite <nick>
	IF (!operator)
		#channel :You're not channel operator
	ELSE
		 You've invited <nick> to #channel -> operator

		 You have been invited to #channel by <operator>			-> client

*/

/*
INVITE
:luna.AfterNET.Org 337 chaud :End of Invite List
JOIN #42
:chaud!vivi@AN-EA7BE6BE.net.novis.pt JOIN #42
:luna.AfterNET.Org 353 chaud = #42 :@chaud
:luna.AfterNET.Org 366 chaud #42 :End of /NAMES list.
INVITE
:luna.AfterNET.Org 337 chaud :End of Invite List
INVITE #42
:luna.AfterNET.Org 461 chaud INVITE :Not enough parameters
INVITE chaud
:luna.AfterNET.Org 461 chaud INVITE :Not enough parameters


INVITE #a chaud
:Aurora.AfterNET.Org 401 alo #a :No such nick
INVITE #a chaud
:Aurora.AfterNET.Org 401 alo #a :No such nick
INVITE chaud #a
:Aurora.AfterNET.Org 403 alo #a :No such channel
JOIN #a
:alo!vini@AN-EA7BE6BE.net.novis.pt JOIN #a
:Aurora.AfterNET.Org 353 alo = #a :@alo
:Aurora.AfterNET.Org 366 alo #a :End of /NAMES list.
INVITE chaud #a
:Aurora.AfterNET.Org 341 alo chaud #a
INVITE chaud,chaud2 #a
:Aurora.AfterNET.Org 401 alo chaud,chaud2 :No such nick
JOIN #b
:alo!vini@AN-EA7BE6BE.net.novis.pt JOIN #b
:Aurora.AfterNET.Org 353 alo = #b :@alo
:Aurora.AfterNET.Org 366 alo #b :End of /NAMES list.
INVITE chaud #a,#b
:Aurora.AfterNET.Org 403 alo #a,#b :No such channel
*/