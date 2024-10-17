#pragma once
#ifndef TOKEN_H
#define TOKEN_H
#include <string>

class Token
{

private:

    int error, index, type;
    std::string value, name;

public:

    Token(int _index=0, int _type=0, std::string _value="", std::string _name="");

    int GetError();
    void SetError(int _error);
    int GetType();
    void SetType(int _type);
    int GetIndex();
    void SetIndex(int _index);
    std::string GetValue();
    void SetValue(std::string _value);
    std::string GetName();
    void SetName(std::string _name);

};

#endif // TOKEN_H