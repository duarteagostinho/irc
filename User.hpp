#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

class User
{
    private:
        const std::string _nick;//tem que ser unico
        const std::string _name;//not unique

    public:
    //Constructor
        User();
        User(std::string name, std::string nick);
        User(const User &other);
    //Destructor
        ~User();
    //Overload Assignment Operator
        User &operator=(const User &other);

    //Getters
        const std::string& GetName() const;
        const std::string& GetNick() const;
};

#endif