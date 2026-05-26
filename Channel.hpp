#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel
{
    private:
        bool        _inviteOnly;
        std::string _topic;
        bool        _passOnly;
        std::string _pass;
        int         _userLimit;

    public:
    //Constructors
        Channel();
        Channel(/*unknown*/);
        Channel(const Channel &other);
    //Destructor
        ~Channel();
    //Overload Assignment Operator
        Channel &operator=(const Channel &other);
    
    
};

#endif