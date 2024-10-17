#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"
#include "syntax.h"
#include "instruction.h"

class Block
{

private:

    int scope;
    std::unordered_map<std::string, Token> variables;
    std::vector<Instruction> stack;
    std::vector<Block> blocks;

public:

    Block(int _scope, std::vector<Instruction> _stack={});

    bool VariableNameExists(std::string name);
    int Size();
    int GetScope();
    void SetScope(int _scope);
    void PopFront();
    void PushBack(Instruction instruction);
    void PushVariable(Token token);

    Token GetVariable(std::string name);
    void SetVariable(std::string name, Token token);
    void DeleteVariable(std::string name);
    std::vector<Token> GetVariables();
    void SetBlocks(std::vector<Block> _blocks);
    std::vector<Block> GetBlocks();
    Instruction GetNextInstruction();

};

#endif // BLOCK_H