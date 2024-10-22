#include "generic.h"



std::vector<Token> Generic::Func(Instruction instruction)
{
    return {Token()};
}



Generic::Function::Function(int _argc, int _type, VoxFunction _func)
{
    SetArgumentCount(_argc);
    SetReturnType(_type);
    SetFunction(_func);
}

int Generic::Function::GetArgumentCount()
{
    return argc;
}

void Generic::Function::SetArgumentCount(int _argc)
{
    argc = _argc;
}

int Generic::Function::GetReturnType()
{
    return type;
}

void Generic::Function::SetReturnType(int _type)
{
    type = _type;
}

void Generic::Function::SetFunction(Generic::VoxFunction _func)
{
    func = _func;
}

std::vector<Token> Generic::Function::Call(Instruction instruction)
{
    return func(instruction);
}