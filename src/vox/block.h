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

    bool repeat_block = false;
    int state=BlockState::BLOCK_COMPUTING, scope, block_index=-1, instruction_index=0;
    std::unordered_map<std::string, Token> variables;
    std::vector<Instruction> stack;
    std::vector<Block> blocks;

public:

    Block(int _scope, int _block_index, std::vector<Instruction> _stack={});

    bool ShouldTraverse(int _scope);
    bool HasWorkLeft(int _scope);

    bool GetRepeatBlock(int _scope);
    void SetRepeatBlock(bool value, int _scope);

    int GetSize(int _scope);
    int GetBlockSize(int _scope);

    int GetState();
    void SetState(int _state);

    int GetScope(int _scope);
    void SetScope(int new_scope, int _scope);

    int GetBlockIndex();
    void SetBlockIndex(int _block_index);

    int GetInstructionIndex();
    void SetInstructionIndex(int _instruction_index);

    void PopFront(int _scope);
    void PushBack(Instruction instruction, int _scope);

    bool VariableNameExists(std::string name, int _scope);
    void PushVariable(Token token, int _scope);
    void DeleteVariable(std::string name, int _scope);
    Token GetVariable(std::string name, int _scope);
    void SetVariable(std::string name, Token token, int _scope);
    std::vector<Token> GetVariables(int _scope);
    void SetVariables(std::unordered_map<std::string, Token> _variables);
    
    std::vector<Block> GetBlocks(int _scope);
    void SetBlocks(std::vector<Block> _blocks);
    void PushBlock(Block _block, int _scope);
    
    std::vector<Instruction> GetInstructions(int _scope);
    Instruction GetNextInstruction(int _scope);

};

#endif // BLOCK_H