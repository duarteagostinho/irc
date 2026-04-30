#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <bool.h>

class client
{
    private:
        const std::string _name;
        const std::string _nick;
        bool        _OPStatus;

    public:

        Client(std::string name, std::string nick);
        ~Client();

        const std::string GetName() const ;
        const std::string GetNick() const ;


};

#endif