#include <iostream>
#include "block.h"
#include "syntax.h"

Block::Block(int _scope, int _block_index, std::vector<Instruction> _stack)
{
    scope = _scope;
    block_index = _block_index;
    stack = _stack;
}



bool Block::ShouldTraverse(int _scope)
{
    return (_scope>scope && GetBlockSize(scope)>block_index);
}

bool Block::HasWorkLeft(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).HasWorkLeft(_scope); }
    return (state==BlockState::BLOCK_COMPLETE);//(GetBlockSize(scope)<=block_index);
}



bool Block::GetRepeatBlock(int _scope)
{
    if (_scope==scope) { return repeat_block; }
    else if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetRepeatBlock(_scope); }
    return false;
}

void Block::SetRepeatBlock(bool value, int _scope)
{
    if (_scope==scope) { repeat_block = value; }
    else if (ShouldTraverse(_scope)) { blocks.at(block_index).SetRepeatBlock(value, _scope); }
}

int Block::GetSize(int _scope)
{
    if (_scope==scope) { return stack.size(); }
    else if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetSize(_scope); }
    return 0;
}

int Block::GetBlockSize(int _scope)
{
    if (_scope==scope) { return blocks.size(); }
    else if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetBlockSize(_scope); }
    return 0;
}

int Block::GetState(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetState(_scope); }
    return state;
}

void Block::SetState(int _state, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).SetState(_state, _scope); }
    else if (_scope==scope) { state = _state; }
}

int Block::GetScope(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetScope(_scope); }
    return scope;
}

void Block::SetScope(int new_scope, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).SetScope(new_scope, _scope); }
    else if (_scope==scope) { scope = new_scope; }
}

int Block::GetBlockIndex(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetBlockIndex(_scope); }
    return block_index;
}

void Block::SetBlockIndex(int _block_index, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).SetBlockIndex(_block_index, _scope); }
    if (_block_index>-1) { block_index = _block_index; }
}

int Block::GetInstructionIndex(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetInstructionIndex(_scope); }
    return instruction_index;
}

void Block::SetInstructionIndex(int _instruction_index, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).SetInstructionIndex(_instruction_index, _scope); }
    else if (_scope==scope) { instruction_index = _instruction_index; }
}

//void Block::EraseFront(int _scope)
//{
//    if (ShouldTraverse(_scope)) { blocks.at(block_index).EraseFront(_scope); }
//    else if (_scope==scope) { stack.erase(stack.begin()); }
//}

void Block::PopFront(int _scope)
{
    if (_scope==scope)
    {
        if (instruction_index<GetSize(scope) && block_index<GetBlockSize(scope)) { instruction_index++; }
        else if (instruction_index>=GetSize(scope) && block_index<GetBlockSize(scope))
        {
            if (repeat_block && blocks.at(block_index).GetState(_scope)==BlockState::BLOCK_COMPLETE)
            {
                std::cout<<"Repeating"<<std::endl;
                blocks.at(block_index).SetBlockIndex(0, _scope);
                blocks.at(block_index).SetInstructionIndex(0, _scope);
                blocks.at(block_index).SetState(BlockState::BLOCK_COMPUTING, _scope);
            }
            else
            {
                std::cout<<"Oof?"<<std::endl;
                //blocks.at(block_index).SetState(BlockState::BLOCK_COMPLETE, _scope);
                if (block_index<GetBlockSize(scope)) { block_index++; }
                if (block_index>=GetBlockSize(scope) && instruction_index>=GetSize(scope)) { state=BlockState::BLOCK_COMPLETE; }
            }
        }
    }
    else if (ShouldTraverse(_scope))
    {
        std::cout<<"Traversing"<<std::endl;
        blocks.at(block_index).PopFront(_scope);
    }
}

/*void Block::PopFront(int _scope)
{
    if (_scope==scope)
    {
        if (state==BlockState::BLOCK_COMPUTING) { instruction_index++; if (block_index>=GetBlockSize(scope) && instruction_index>=GetSize(scope)) { state=BlockState::BLOCK_COMPLETE; } }
        else if (state==BlockState::BLOCK_WAITING && repeat_block)
        {
            if (blocks.at(block_index).GetState(_scope)==BlockState::BLOCK_COMPLETE)
            {
                std::cout<<"Repeating"<<std::endl;
                blocks.at(block_index).SetBlockIndex(0, _scope);
                blocks.at(block_index).SetInstructionIndex(0, _scope);
                blocks.at(block_index).SetState(BlockState::BLOCK_COMPUTING, _scope);
            }
            else
            {
                std::cout<<"Finishing"<<std::endl;
                if (block_index<GetBlockSize(scope)) { block_index++; }
                if (block_index>=GetBlockSize(scope) && instruction_index>=GetSize(scope)) { state=BlockState::BLOCK_COMPLETE; }
            }
        }
    }
    else if (ShouldTraverse(_scope))
    {
        std::cout<<"Traversing"<<std::endl;
        blocks.at(block_index).PopFront(_scope);
    }
}*/

void Block::PushBack(Instruction instruction, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).PushBack(instruction, _scope); }
    else if (_scope==scope) { stack.push_back(instruction); }
}



bool Block::VariableNameExists(std::string name, int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).VariableNameExists(name, _scope); }
    return variables.count(name) > 0;
}

void Block::PushVariable(Token token, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).PushVariable(token, _scope); }
    else if (_scope==scope)
    {
        if      (!VariableNameExists(token.GetName(), _scope) && _scope==scope) { variables.emplace(token.GetName(), token); }
        else if ( VariableNameExists(token.GetName(), _scope) && _scope==scope) { SetVariable(token.GetName(), token, _scope); }
    }
    //for (auto& it : variables) { std::cout << it.second.GetName() << ":" << it.second.GetValue() <<"; "; } std::cout<<std::endl;
}

void Block::DeleteVariable(std::string name, int _scope)
{
    //if (!VariableNameExists(name, _scope)) { return; }
    if (ShouldTraverse(_scope)) { blocks.at(block_index).DeleteVariable(name, _scope); }
    else if (_scope==scope) { variables.erase(name); }
}

Token Block::GetVariable(std::string name, int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetVariable(name, _scope); }
    else if (_scope==scope && VariableNameExists(name, _scope)) { return variables[name]; }
    return Token();
}

void Block::SetVariable(std::string name, Token token, int _scope)
{
    if (ShouldTraverse(_scope) && VariableNameExists(name, _scope)) { blocks.at(block_index).SetVariable(name, token, _scope); }
    else if (_scope==scope && VariableNameExists(name, scope)) { variables[name]=token; }
}

std::vector<Token> Block::GetVariables(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetVariables(_scope); }
    std::vector <Token> v; for (auto& it : variables) { v.push_back( it.second ); }
    return v;
}

void Block::SetVariables(std::unordered_map<std::string, Token> _variables)
{
    variables = _variables;
}



std::vector<Block> Block::GetBlocks(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetBlocks(_scope); }
    return blocks;
}

void Block::SetBlocks(std::vector<Block> _blocks)
{
    blocks = _blocks;
}

void Block::PushBlock(Block _block, int _scope)
{
    if (ShouldTraverse(_scope)) { blocks.at(block_index).PushBlock(_block, _scope); }
    else if (_scope==scope) { blocks.push_back(_block); }
}



std::vector<Instruction> Block::GetInstructions(int _scope)
{
    if (ShouldTraverse(_scope)) { return blocks.at(block_index).GetInstructions(_scope); }
    else if (_scope==scope) { return stack; }
    /*
    else
    {
        std::cout<<"Block::GetInstructions() Failed";
        std::cout<<" S:"<<scope<<"/"<<_scope;
        std::cout<<" I:"<<instruction_index<<"/"<<stack.size();
        std::cout<<" B:"<<block_index<<"/"<<blocks.size();
        std::cout<<std::endl;
    }
    */
    return {};
}
/*
Instruction Block::GetNextInstruction(int _scope)
{
    if (ShouldTraverse(_scope))
    {
        std::cout<<"InstructionChoice1"<<std::endl;
        if      (blocks.at(block_index).GetState()==BlockState::BLOCK_REPEATING) { return blocks.at(block_index).GetNextInstruction(_scope); }
        else if (blocks.at(block_index).GetState()==BlockState::BLOCK_COMPUTING) { return blocks.at(block_index).GetNextInstruction(_scope); }
    }
    else if (_scope>scope && block_index<=GetBlockSize(scope) && state==BlockState::BLOCK_REPEATING)
    {
        std::cout<<"InstructionChoice2"<<std::endl;
        if      (blocks.at(block_index).GetState()==BlockState::BLOCK_REPEATING) { return blocks.at(block_index).GetNextInstruction(_scope); }
        else if (blocks.at(block_index).GetState()==BlockState::BLOCK_COMPUTING) { return blocks.at(block_index).GetNextInstruction(_scope); }
    }
    else if (_scope==scope && instruction_index<GetSize(scope))
    {
        std::cout<<"InstructionChoice3"<<std::endl; return stack.at(instruction_index);
    }
    else { std::cout<<"Block::GetNextInstruction() Failed"<<std::endl; }
    return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)});
}
*/
/*
Instruction Block::GetNextInstruction(int _scope)
{
    //std::cout<<BlockState::keys[state]<<"("<<_scope<<")...";
    if (state==BlockState::BLOCK_REPEATING)
    {
        if (GetBlockSize(scope)>block_index)
        {
            //std::cout<<"Traversing"<<std::endl;
            return blocks.at(block_index).GetNextInstruction(_scope);
        }
        else if (instruction_index<GetSize(scope))
        {
            //std::cout<<"Immediate"<<std::endl;
            return stack.at(instruction_index);
        }
        else
        {
            return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)});
        }
    }
    else
    {
        if (ShouldTraverse(_scope))
        {
            //std::cout<<"Traversing"<<std::endl;
            return blocks.at(block_index).GetNextInstruction(_scope);
        }
        else if (instruction_index<GetSize(scope))
        {
            //std::cout<<"Immediate"<<std::endl;
            return stack.at(instruction_index);
        }
        else
        {
            return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)});
        }
    }
    return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)});
}
*/

Instruction Block::GetNextInstruction(int _scope)
{
    //std::cout<<BlockState::keys[state]<<"("<<_scope<<")...";
    if (state==BlockState::BLOCK_WAITING)
    {
        if (GetBlockSize(_scope)>block_index && GetState(_scope)!=BlockState::BLOCK_COMPLETE)
        {
            //std::cout<<"Traversing"<<std::endl;
            return blocks.at(block_index).GetNextInstruction(_scope);
        }
        //else { std::cout<<"Uh oh..."<<std::endl; }
    }
    else
    {
        if (ShouldTraverse(_scope))
        {
            //std::cout<<"Traversing"<<std::endl;
            return blocks.at(block_index).GetNextInstruction(_scope);
        }
        else if (instruction_index<GetSize(scope))
        {
            //std::cout<<"Immediate"<<std::endl;
            return stack.at(instruction_index);
        }
    }
    return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)});
}