#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
//#include <iostream>
# include <vector>
# include <algorithm>
# include <map>
# include <ctime>
# include <sstream>
# include <limits.h>

class Channel
{
    private:
        std::string                 _name;
        std::string                 _topic;
		std::string                 _pass;
        std::vector<std::string>    _invited; // track invited users
        std::map<std::string, bool> _users_op;
		time_t						_creation;
		time_t						_new_topic;
		std::string					_topic_creator;
		int							_usersCount;
		int							_max_users;
		bool						_inv_flag;	// set/remove invite only
		bool						_topc_flag;	// set/remove topic restriction on operators
		bool						_key_flag;	// set/remove key
		bool						_optr_flag;	// set/remove operator
		bool						_lim_flag;	// set/remove user limit to channel

    public:
        Channel();
        Channel(std::string name);
        Channel(const Channel &other);
        ~Channel();
        Channel& operator=(const Channel &other);

    // Getters
		const std::string	getName() const;
        const std::string	getPass() const;
		const std::string	getTopic() const;	
		bool				getKeyMode() const;
		int					getCount(void) const;
		bool				getTopicMode() const;
		bool				getLimitMode() const;
		bool				getInviteMode() const;
		int					getMaxUsers(void) const;
		bool				getOperatorMode() const;
		std::string			getTopicTime(void) const;
		std::string			getTopicMaker(void) const;
		std::string			getChannelTime(void) const;

	// Setters
		void				setTopicTime(void);
		void				setChannelTime(void);
		void				setMaxUsers(int &num);
		void				setKeyMode(bool mode);
		void				setTopicMode(bool mode);
		void				setLimitMode(bool mode);
		void            	setInviteMode(bool mode);
		void				setOperatorMode(bool mode);
		void				setPass(const std::string &str);
		void				setTopic(const std::string topic);
		void				setTopicMaker(const std::string &str);

	// Utils
		std::string			usersFormated(void);
		void				incrementCount(void);
		void				decrementCount(void);
        void				rmUser(const std::string& nick);
		bool				hasInvite(const std::string nick);
		void				rmInvite(const std::string& nick);
		void				addInvite(const std::string& nick);
		void				rmOperator(const std::string& nick);
		void				addOperator(const std::string& nick);
		std::string			printMode(const std::string &nickname);
		bool				isOperator(const std::string nick) const;
		void				addUser(const std::string& nick, bool flag);
		bool				isUserOnChannel(const std::string nick) const;

		// DEL
        void print_users(void);
};

#endif
