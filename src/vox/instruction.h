#pragma once
#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>
#include <vector>
#include "token.h"

class Instruction
{

private:

    int type, argc;
    std::string state;
    std::vector<Token> args;

public:

    Instruction(int _type=0, int _argc=0, std::string _state="", std::vector<Token> _args={});

    std::vector<Token> GetArguments();
    Token GetArgument(int index);
    void SetArgument(Token arg, int index);

    std::string GetState();
    void SetState(std::string _state);

    int GetType();
    void SetType(int _type);

    int GetArgC();
    void SetArgC(int _argc);

};

#endif // INSTRUCTION_H