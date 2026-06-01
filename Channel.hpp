#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include "User.hpp"

class Channel
{
    private:
        std::string              _name;
        bool                     _inviteOnly;
        std::string              _topic;
        bool                     _passOnly;
        std::string              _pass;
        int                      _userLimit;
        std::string              _creator;
        std::vector<std::string> _users;
        std::vector<std::string> _operators;
        bool                     _inv;

    public:
    //Constructors
        Channel();
        Channel(std::string name, std::string& creator);
        Channel(const Channel &other);
    //Destructor
        ~Channel();
    //Overload Assignment Operator
        Channel &operator=(const Channel &other);

    //Getters
        std::string               getName() const;
        bool                      getInvite() const;
        bool                      getPassOnly() const;
        std::string               getTopic() const;
        std::string               getPass() const;
        int                       getUserLimit() const;
        std::string               getCreator() const;
        std::vector<std::string>  getUsers() const;
        std::vector<std::string>  getOperators() const;

    //Setters
        void    setInvite(bool state);
        void    setPassOnly(bool state);
        void    setTopic(std::string topic);
        void    setPass(std::string pass);
        void    setUserLmit(int limit);
        void    addUser(std::string& nick);
        void    rmUser(std::string& nick);
        void    addOperator(std::string& nick);
        void    rmOperator(std::string& nick);
        //to add users to the vector we can use push_back,
        //we need to do it this way vector.push_back(User(variables)),
        //but this doesnt check for duplicates 
    
    
};

#endif