#pragma once
#ifndef GENERIC_H
#define GENERIC_H

#include <string>
#include <vector>
#include <functional>

#include "syntax.h"
#include "token.h"
#include "instruction.h"

namespace Generic
{
    using func = std::function<std::vector<Token>(Instruction)>;
    
    std::vector<Token> Foo(Instruction);
    
    class Function
    {
    
    private:
    
        int argc, type;
        func foo;
    
    public:
    
        Function(int _argc=0, int _type=ReturnType::RETURN_VOID, func _foo=Foo);
        int GetArgumentCount();
        void SetArgumentCount(int _argc);
        int GetReturnType();
        void SetReturnType(int _type);
        void SetFunction(func _foo);
        std::vector<Token> Call(Instruction instruction);
    
    };
}

#endif // GENERIC_H