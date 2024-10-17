#include "generic.h"



std::vector<Token> Generic::Foo(Instruction instruction)
{
    return {Token()};
}



Generic::Function::Function(int _argc, int _type, func _foo)
{
    SetArgumentCount(_argc);
    SetReturnType(_type);
    SetFunction(_foo);
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

void Generic::Function::SetFunction(Generic::func _foo)
{
    foo = _foo;
}

std::vector<Token> Generic::Function::Call(Instruction instruction)
{
    return foo(instruction);
}