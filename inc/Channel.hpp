#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class Channel
{
    private:
        std::string                 _name;
        std::string                 _topic;
		std::string                 _pass;
        // Add any invited user to this vector, once it joins the channel, remove from here
        std::vector<std::string>    _invited;
        std::map<std::string, bool> _users_op;

		// bool	_inv_flag;	// set/remove invite only
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
		bool							getTopcMode() const;
	

	//Setters
        // void    setInvite(bool state);
        // void    setPassOnly(bool state);
        void    setTopic(const std::string topic);



	//Utils
		std::string         usersFormated(void);
        void                rmUser(std::string& nick);
		bool				hasInvite(std::string nick);
		void                rmOperator(std::string& nick);
		void                addOperator(std::string& nick);
		bool                isOperator(const std::string nick) const;
		void                addUser(const std::string& nick, bool flag);
		bool				isUserOnChannel(const std::string nick) const;

		void				addInvite(const std::string& nick);
		void				rmInvite(const std::string& nick);

		// DEL
        void print_users(void);
};

#endif

