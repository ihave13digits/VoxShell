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
    using VoxFunction = std::function<std::vector<Token>(Instruction)>;
    
    std::vector<Token> Func(Instruction); // A Template To Soothe The Compiler For Function::Function
    
    class Function
    {
    
    private:
    
        int argc, type;
        VoxFunction func;
    
    public:
    
        Function(int _argc=0, int _type=ReturnType::RETURN_VOID, VoxFunction _func=Func);
        int GetArgumentCount();
        void SetArgumentCount(int _argc);
        int GetReturnType();
        void SetReturnType(int _type);
        void SetFunction(VoxFunction _func);
        std::vector<Token> Call(Instruction instruction);
    
    };
}

#endif // GENERIC_H