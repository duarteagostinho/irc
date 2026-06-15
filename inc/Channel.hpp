#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime>
#include <sstream>

class Channel
{
    private:
        std::string                 _name;
        std::string                 _topic;
		std::string                 _pass;
        // Add any invited user to this vector, once it joins the channel, remove from here
        std::vector<std::string>    _invited;
        std::map<std::string, bool> _users_op;
		time_t						_creation;
		time_t						_new_topic;
		std::string					_topic_creator;

		bool	_inv_flag;	// set/remove invite only
		bool	_topc_flag;	// set/remove topic restriction on operators
		bool	_key_flag;	// set/remove key
		bool	_optr_flag;	// set/remove operator
		bool	_lim_flag;	// set/remove user limit to channel


        
        public:
        
        Channel();
        Channel(std::string name);
        Channel(const Channel &other);
        ~Channel();
        Channel& operator=(const Channel &other);

    //Getters
		const std::string				getName() const;
        const std::string               getPass() const;
		const std::string               getTopic() const;
		bool							getLimMode() const;
		bool							getKeyMode() const;
		bool                      		getInvMode() const;
		bool							getOptrMode() const;
		bool							getTopicMode() const;
		std::string						getChannelTime(void) const;
		std::string						getTopicTime(void) const;
		std::string						getTopicMaker(void) const;

	//Setters
        // void    setInvite(bool state);
        // void    setPassOnly(bool state);
        void    setTopic(const std::string topic);

		void							setTopicTime(void);
		void							setChannelTime(void);
		void							setLimMode(bool mode);
		void							setKeyMode(bool mode);
		void                      		setInvMode(bool mode);
		void							setOptrMode(bool mode);
		void							setTopicMode(bool mode);
		void							setTopicMaker(const std::string &str);


	//Utils
		std::string         usersFormated(void);
        void                rmUser(const std::string& nick);
		bool				hasInvite(const std::string nick);
		void                rmOperator(const std::string& nick);
		void                addOperator(const std::string& nick);
		bool                isOperator(const std::string nick) const;
		void                addUser(const std::string& nick, bool flag);
		bool				isUserOnChannel(const std::string nick) const;

		void				addInvite(const std::string& nick);
		void				rmInvite(const std::string& nick);

		std::string			printMode(void);

		// DEL
        void print_users(void);
};

#endif

