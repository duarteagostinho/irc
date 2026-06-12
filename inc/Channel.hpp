#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include "User.hpp"
#include <algorithm>
#include <map>

typedef std::vector<std::pair<bool, std::string> > operator_user_pair;

class Channel
{
    private:
        std::string                 _name;
        std::string                 _topic;
		std::string                 _pass;
        operator_user_pair          opFlag_users;
        // Add any invited user to this vector, once it joins the channel, remove from here
        std::vector<std::string>    _invited;
        std::map<std::string, bool> _users_op;
        
        public:
        
        Channel();
        Channel(std::string name);
        Channel(const Channel &other);
        ~Channel();
        Channel& operator=(const Channel &other);

    //Getters
        
        bool                      getInvite() const;
        bool                      getPassOnly() const;
        const std::string               getTopic() const;
        const std::string               getPass() const;
        int                       getUserLimit() const;
        const std::string               getCreator() const;
        const std::vector<std::string>  getUsers() const;


        std::string         usersFormated(void);

        const std::string   getName() const;
        bool                isOperator(const std::string nick) const;

        void                addOperator(std::string& nick);
        void                rmOperator(std::string& nick);

        void                addUser(const std::string& nick, bool flag);
        void                rmUser(std::string& nick);

        const std::string   printTopic(void) const;


		bool				hasInvite(std::string nick);
		bool				isUserOnChannel(const std::string nick) const;

        void print_users(void);


    //Setters
        void    setInvite(bool state);
        void    setPassOnly(bool state);
        void    setTopic(std::string topic);
        void    setPass(std::string pass);
        void    setUserLmit(int limit);

//        void    addOperator(std::string& nick);
//        void    rmOperator(std::string& nick);
        //to add users to the vector we can use push_back,
        //we need to do it this way vector.push_back(User(variables)),
        //but this doesnt check for duplicates 
    
    
};

#endif


        // std::vector<std::string> _users;
        // std::vector<std::string> _operators;
        // bool                     _inviteOnly;
        // bool                     _topicOp;
        // std::string              _topic;
        // bool                     _passOnly;
        // std::string              _pass;
        // int                      _userLimit;
        // std::string              _creator;
