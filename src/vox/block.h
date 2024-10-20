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

    int scope, block_index;
    std::unordered_map<std::string, Token> variables;
    std::vector<Instruction> stack;
    std::vector<Block> blocks;

public:

    Block(int _scope, int _block_index, std::vector<Instruction> _stack={});

    int GetSize();

    int GetScope();
    void SetScope(int _scope);

    int GetBlockIndex();
    void SetBlockIndex(int _block_index);

    void PopFront();
    void PushBack(Instruction instruction);

    bool VariableNameExists(std::string name);
    void PushVariable(Token token);
    void DeleteVariable(std::string name);
    Token GetVariable(std::string name);
    void SetVariable(std::string name, Token token);
    std::vector<Token> GetVariables();
    void SetVariables(std::unordered_map<std::string, Token> _variables);
    
    std::vector<Block> GetBlocks();
    void SetBlocks(std::vector<Block> _blocks);
    void PushBlock(Block _block);
    
    Instruction GetNextInstruction();

};

#endif // BLOCK_H