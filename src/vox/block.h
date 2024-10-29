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

    int scope, instruction_index=0, block_index=0;
    std::unordered_map<std::string, Token> variables;
    std::vector<Instruction> stack;
    std::vector<Block> blocks;

public:

    Block(int _scope, int _block_index);

    bool ShouldTraverse(int current_scope);

    int GetSize(int current_scope);

    int GetScope(int current_scope);
    void SetScope(int _scope, int current_scope);

    int GetBlockIndex(int current_scope);
    void SetBlockIndex(int _block_index, int current_scope);

    bool VariableNameExists(std::string name, int current_scope);
    void PushVariable(Token token, int current_scope);
    void DeleteVariable(std::string name, int current_scope);
    Token GetVariable(std::string name, int current_scope);
    void SetVariable(std::string name, Token token, int current_scope);
    std::vector<Token> GetVariables(int current_scope);
    void SetVariables(std::unordered_map<std::string, Token> _variables, int current_scope);
    
    std::vector<Block> GetBlocks(int current_scope);
    void SetBlocks(std::vector<Block> _blocks, int current_scope);
    void PushBlock(Block _block, int current_scope);

    void PopFront(int current_scope);
    void PushBack(Instruction instruction, int current_scope);
    
    Instruction GetNextInstruction(int current_scope);

};

#endif // BLOCK_H