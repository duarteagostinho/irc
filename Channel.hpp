#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include "User.hpp"

class Channel
{
    private:
        bool        _inviteOnly;
        std::string _topic;
        bool        _passOnly;
        std::string _pass;
        int         _userLimit;
        std::vector<User> _users;

    public:
    //Constructors
        Channel();
        Channel(/*unknown*/);
        Channel(const Channel &other);
    //Destructor
        ~Channel();
    //Overload Assignment Operator
        Channel &operator=(const Channel &other);

    //Getters
        const bool        getInvite() const;
        const bool        getPassOnly() const;
        const std::string getTopic() const;
        const std::string getPass() const;
        const int         getUserLimit() const
        const int         getUserNb() const;

    //Setters
        void    setInvite(bool state);
        void    setPassOnly(bool state);
        void    setTopic(std::string topic);
        void    setPass(sd::String pass);
        void    setUserLmit(int limit);
        //to add users to the vector we can use push_back,
        //we need to do it this way vector.push_back(User(variables)),
        //but this doesnt check for duplicates 
    
    
};

#endif