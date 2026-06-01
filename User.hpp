#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

class User
{
    private:
        std::string _name;//not unique
        std::string _nick;//tem que ser unico

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

    //Setters
        void    setNick(std::string nick);
        void    setName(std::string name);
};

#endif