
PART #a
:vini!vini@740E9B.95C97E.C247D8.AE513.IP PART :#a



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

///////////////////////////////////////////////////			PRIVMSG		///////////////////////////////////////////////////



/*
	std::vector<std::string> targets;

	if (size == 1)
		411 chaud :No recipient given (PRIVMSG)
	else if (size == 2)
		412 chaud :No text to send
 

	splitString(av[1], targets, ',');

	for (size_t i = 0; i < targets.size(); i++)
	{
		bool channel = isChannel(targets[i];

		if ((targets.size() > 1 && channel == true) || not on channel)
			404 chaud #42 :Cannot send to channel
		else if (targets[i] dont exists)
			401 chaud user_1 :No such nick
		else if (channel == false)
			403 chaud #42 :No such channel
		else
			:user_5!user_5@1C72DB:D17C90:7D2707:482FF6:IP PRIVMSG chaud :hi hi // RECEIVER GETS THIS

	}

*/



/*

PRIVMSG
:Aurora.AfterNET.Org 411 chaud :No recipient given (PRIVMSG)
JOIN
:Aurora.AfterNET.Org 461 chaud JOIN :Not enough parameters
PRIVMSg #42
:Aurora.AfterNET.Org 412 chaud :No text to send
PRIVMSG #42 hi
:Aurora.AfterNET.Org 403 chaud #42 :No such channel
PRIVMSg #42,#11
:Aurora.AfterNET.Org 412 chaud :No text to send
PRIVMSG #42 hi
:Aurora.AfterNET.Org 404 chaud #42 :Cannot send to channel
JOIN #42
:chaud!vini@AN-EA7BE6BE.net.novis.pt JOIN #42
:Aurora.AfterNET.Org 353 chaud = #42 :chaud @user_5
:Aurora.AfterNET.Org 366 chaud #42 :End of /NAMES list.
PRIVMSG #42,#11 hello
:Aurora.AfterNET.Org 404 chaud #42 :Cannot send to channel
:Aurora.AfterNET.Org 404 chaud #11 :Cannot send to channel
PRIVMSG #42 hi
:No recipient given (PRIVMSG)
:Aurora.AfterNET.Org 421 chaud recipient :Unknown command
PRIVMSG user_5 hello
PRIVMSG user_5 good morning wake up
:user_5!user_5@1C72DB:D17C90:7D2707:482FF6:IP PRIVMSG chaud :hi hi
PRIVMSG useee hello there
:Aurora.AfterNET.Org 401 chaud useee :No such nick
^C
vloureir@Vinicius:~$ nc -C irc.afternet.org 6667
NOTICE * :*** Looking up your hostname
NOTICE * :*** Found your hostname
USER vini 0 * :vini
NICK vini
PING :1778091801
PONG 1778091801
NOTICE * :*** Checking your client version (sorry..)

PRIVMSG <nick> :VERSION
001 <nick> 


:luna.AfterNET.Org PRIVMSG vini :VERSION
:luna.AfterNET.Org 001 vini :Welcome to the AfterNET IRC Network, vini
:luna.AfterNET.Org 002 vini :Your host is luna.AfterNET.Org, running version u2.10.12.14+Nefarious(2.0.0)
:luna.AfterNET.Org 003 vini :This server was created Fri May 29 2026 at 23:21:43 UTC
:luna.AfterNET.Org 004 vini luna.AfterNET.Org u2.10.12.14+Nefarious(2.0.0) adgiknoqswxzBDHLNORWX abCcDdhikLlMmNnOopQRrSsTtvZz bhkLlov
:luna.AfterNET.Org 005 vini WHOX WALLCHOPS WALLHOPS WALLVOICES USERIP CPRIVMSG CNOTICE NAMESX UHNAMES SILENCE=25 WATCH=128 MODES=6 MAXCHANNELS=50 :are supported by this server
:luna.AfterNET.Org NOTICE vini :Highest connection count: 108 (107 clients)
:luna.AfterNET.Org 422 vini :MOTD File is missing
:luna.AfterNET.Org NOTICE vini :on 1 ca 1(6) ft 10(10)
:luna.AfterNET.Org 396 vini AN-EA7BE6BE.net.novis.pt :is now your hidden host
:vini!vini@AN-EA7BE6BE.net.novis.pt MODE vini +x
JOIN #42
:vini!vini@AN-EA7BE6BE.net.novis.pt JOIN #42
:luna.AfterNET.Org 353 vini = #42 :vini @user_5
:luna.AfterNET.Org 366 vini #42 :End of /NAMES list.
JOIN #11
:vini!vini@AN-EA7BE6BE.net.novis.pt JOIN #11
:luna.AfterNET.Org 353 vini = #11 :vini @user_5
:luna.AfterNET.Org 366 vini #11 :End of /NAMES list.
PRIVMSG #42,#11,user_5 :wake up
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel
:luna.AfterNET.Org 404 vini #11 :Cannot send to channel
PRIVMSG user_5,user_66 :wakey wakey
PRIVMSG #11 hi
PRIVMSG #42,#11 alo
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel
:luna.AfterNET.Org 404 vini #11 :Cannot send to channel
PRIVMSG #42 alo
PRIVMSG #42,user_5,user_66 hello there
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel

:luna.AfterNET.Org 005 vini MAXBANS=50 NICKLEN=30 MAXNICKLEN=30 TOPICLEN=250 AWAYLEN=250 KICKLEN=250 CHANNELLEN=100 MAXCHANNELLEN=200 CHANTYPES=# PREFIX=(ohv)@%+ STATUSMSG=@%+ BOT=B CHANMODES=be,k,Ll,aCcDdiMmNnOpQRrSsTtZz :are supported by this server
:luna.AfterNET.Org 005 vini CASEMAPPING=rfc1459 NETWORK=AfterNET MAXLIST=b:50,e:45 ELIST=CT EXCEPTS=e MAXEXCEPTS=45 EXTBANS=~,acjnqrmM :are supported by this server

:luna.AfterNET.Org 251 vini :There are 212 users and 93 invisible on 9 servers
:luna.AfterNET.Org 252 vini 23 :operator(s) online
:luna.AfterNET.Org 254 vini 181 :channels formed
:luna.AfterNET.Org 255 vini :I have 97 clients and 1 servers
:luna.AfterNET.Org 265 vini :Current local users: 97 Max: 258
:luna.AfterNET.Org 266 vini :Current global users: 305 Max: 568
:luna.AfterNET.Org NOTICE vini :Highest connection count: 108 (107 clients)
:luna.AfterNET.Org 422 vini :MOTD File is missing
:luna.AfterNET.Org NOTICE vini :on 1 ca 1(6) ft 10(10)
:luna.AfterNET.Org 396 vini AN-EA7BE6BE.net.novis.pt :is now your hidden host
:vini!vini@AN-EA7BE6BE.net.novis.pt MODE vini +x
JOIN #42
:vini!vini@AN-EA7BE6BE.net.novis.pt JOIN #42
:luna.AfterNET.Org 353 vini = #42 :vini @user_5
:luna.AfterNET.Org 366 vini #42 :End of /NAMES list.
JOIN #11
:vini!vini@AN-EA7BE6BE.net.novis.pt JOIN #11
:luna.AfterNET.Org 353 vini = #11 :vini @user_5
:luna.AfterNET.Org 366 vini #11 :End of /NAMES list.
PRIVMSG #42,#11,user_5 :wake up
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel
:luna.AfterNET.Org 404 vini #11 :Cannot send to channel
PRIVMSG user_5,user_66 :wakey wakey
PRIVMSG #11 hi
PRIVMSG #42,#11 alo
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel
:luna.AfterNET.Org 404 vini #11 :Cannot send to channel
PRIVMSG #42 alo
PRIVMSG #42,user_5,user_66 hello there
:luna.AfterNET.Org 404 vini #42 :Cannot send to channel

*/

	/*
		priv alo hi
			no such nick
		priv #alo hi
			no such channel
		
		is channel OR user
		
		response = func();
		if (user)
			send
		else if (target > 1)
			cannot send to channel
		else
			broadcast
			
	
	*/

	// splitString(av[1], targets, ',');

	// for (size_t i = 0; i < targets.size(); i++)
	// {
	// 	bool channel = isChannel(targets[i];

	// 	if ((targets.size() > 1 && channel == true) || not on channel)
	// 		404 chaud #42 :Cannot send to channel
	// 	else if (targets[i] dont exists)
	// 		401 chaud user_1 :No such nick
	// 	else if (channel == false)
	// 		403 chaud #42 :No such channel
	// 	else
	// 		:user_5!user_5@1C72DB:D17C90:7D2707:482FF6:IP PRIVMSG chaud :hi hi // RECEIVER GETS THIS


///////////////////////////////////////////////////			MODES		///////////////////////////////////////////////////
