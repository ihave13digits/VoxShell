#include "instruction.h"
#include "syntax.h"

Instruction::Instruction(int _type, int _argc, std::string _state, std::vector<Token> _args)
{
    SetType(_type);
    SetArgC(_argc);
    SetState(_state);
    args=_args;
    if (_argc<0) { argc=args.size(); }
}

std::vector<Token> Instruction::GetArguments()
{
    return args;
}

Token Instruction::GetArgument(int index)
{
    if (index>=argc && index>=int(args.size())) { return Token(0, 0, SyntaxGlobal::blank_instruction); }
    return args.at(index);
}

void Instruction::SetArgument(Token arg, int index)
{
    while (index>argc) { args.push_back(Token(0, 0, SyntaxGlobal::blank_instruction)); }
    args.at(index) = arg;
}

std::string Instruction::GetState()
{
    return state;
}

void Instruction::SetState(std::string _state)
{
    state = _state;
}

int Instruction::GetType()
{
    return type;
}

void Instruction::SetType(int _type)
{
    type = _type;
}

int Instruction::GetArgC()
{
    return argc;
}

void Instruction::SetArgC(int _argc)
{
    argc = _argc;
}