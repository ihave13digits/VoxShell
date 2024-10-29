#include <iostream>
#include "block.h"
#include "syntax.h"

Block::Block(int _scope, int _block_index)
{
    SetScope(_scope, _scope);
    SetBlockIndex(_block_index, _scope);
}



bool Block::ShouldTraverse(int current_scope)
{
    return (current_scope>scope && int(blocks.size())>0);
}



int Block::GetSize(int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetSize(current_scope); }
    return stack.size();
}

int Block::GetScope(int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetScope(current_scope); }
    return scope;
}

void Block::SetScope(int _scope, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).SetScope(_scope, current_scope); }
    else { scope = _scope; }
}

int Block::GetBlockIndex(int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetBlockIndex(current_scope); }
    return block_index;
}

void Block::SetBlockIndex(int _block_index, int current_scope)
{
    if (ShouldTraverse(current_scope)) { SetBlockIndex(_block_index, current_scope); }
    else { block_index = _block_index; }
}



bool Block::VariableNameExists(std::string name, int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).VariableNameExists(name, current_scope); }
    return variables.count(name) > 0;
}

void Block::PushVariable(Token token, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).PushVariable(token, current_scope); }
    else
    {
        if (!VariableNameExists(token.GetName(), current_scope)) { variables.emplace(token.GetName(), token); }
        else { SetVariable(token.GetName(), token, current_scope); }
        //for (auto& it : variables) { std::cout << it.second.GetName() << ":" << it.second.GetValue() <<"; "; } std::cout<<std::endl;
    }
}

void Block::DeleteVariable(std::string name, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).DeleteVariable(name, current_scope); }
    else
    {
        if (!VariableNameExists(name, current_scope)) { return; }//std::__throw_logic_error(("Cannot delete variable: " + name + " No such variable!").c_str());
        variables.erase(name);
    }
}

Token Block::GetVariable(std::string name, int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetVariable(name, current_scope); }
    else
    {
        if (VariableNameExists(name, current_scope)) return variables[name];
        else return Token();
    }
}

void Block::SetVariable(std::string name, Token token, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).SetVariable(name, token, current_scope); }
    else
    {
        if (VariableNameExists(name, current_scope)) variables[name]=token;
        else std::__throw_logic_error(("Cannot set variable: " + name + " No such variable!").c_str());
    }
}

std::vector<Token> Block::GetVariables(int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetVariables(current_scope); }
    else
    {
        std::vector <Token> v;
        for (auto& it : variables)
        {
            v.push_back( it.second );
        }
        return v;
    }
}

void Block::SetVariables(std::unordered_map<std::string, Token> _variables, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).SetVariables(_variables, current_scope); }
    else { variables = _variables; }
}



std::vector<Block> Block::GetBlocks(int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetBlocks(current_scope); }
    return blocks;
}

void Block::SetBlocks(std::vector<Block> _blocks, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).SetBlocks(_blocks, current_scope); }
    else { blocks = _blocks; }
}

void Block::PushBlock(Block _block, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).PushBlock(_block, current_scope); }
    else { blocks.push_back(_block); }
}



void Block::PopFront(int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).PopFront(current_scope); }
    else //{ stack.erase(stack.begin()); }
    {
        instruction_index++;
    }
}

void Block::PushBack(Instruction instruction, int current_scope)
{
    if (ShouldTraverse(current_scope)) { blocks.at(block_index).PushBack(instruction, current_scope); }
    else { stack.push_back(instruction); }
}

Instruction Block::GetNextInstruction(int current_scope)
{
    if (ShouldTraverse(current_scope)) { return blocks.at(block_index).GetNextInstruction(current_scope); }
    if (stack.size()<=0) { return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)}); }
    Instruction instruction = stack.at(instruction_index);
    return instruction;
}