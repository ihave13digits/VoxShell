#include "token.h"

Token::Token(int _index, int _type, std::string _value, std::string _name)
{
    index = _index;
    type = _type;
    value = _value;
    name = _name;
}

int Token::GetError()
{
    return error;
}

void Token::SetError(int _error)
{
    error = _error;
}

int Token::GetType()
{
    return type;
}

void Token::SetType(int _type)
{
    type = _type;
}

int Token::GetIndex()
{
    return index;
}

void Token::SetIndex(int _index)
{
    index = _index;
}

std::string Token::GetValue()
{
    return value;
}

void Token::SetValue(std::string _value)
{
    value = _value;
}

std::string Token::GetName()
{
    return name;
}

void Token::SetName(std::string _name)
{
    name = _name;
}