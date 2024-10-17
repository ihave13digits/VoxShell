#include "block.h"
#include "syntax.h"

Block::Block(int _scope, std::vector<Instruction> _stack)
{
    SetScope(_scope);
    stack=_stack;
}

bool Block::VariableNameExists(std::string name)
{
    return variables.count(name) > 0;
}

int Block::Size()
{
    if (blocks.size()>0) { return blocks[0].Size(); }
    return stack.size();
}

int Block::GetScope()
{
    return scope;
}

void Block::SetScope(int _scope)
{
    scope = _scope;
}

void Block::PopFront()
{
    stack.erase(stack.begin());
}

void Block::PushBack(Instruction instruction)
{
    stack.push_back(instruction);
}

void Block::PushVariable(Token token)
{
    if (!VariableNameExists(token.GetName())) { variables.emplace(token.GetName(), token); }
    else { SetVariable(token.GetName(), token); }
}



Token Block::GetVariable(std::string name)
{
    if (VariableNameExists(name)) return variables[name];
    else return Token();
}

void Block::SetVariable(std::string name, Token token)
{
    if (VariableNameExists(name)) variables[name]=token;
    else std::__throw_logic_error(("Cannot set variable: " + name + " No such variable!").c_str());
}

void Block::DeleteVariable(std::string name)
{
    if (!VariableNameExists(name)) { return; }//std::__throw_logic_error(("Cannot delete variable: " + name + " No such variable!").c_str());
    variables.erase(name);
}

std::vector<Token> Block::GetVariables()
{
    std::vector <Token> v;

    for (auto& it : variables) 
    {
        v.push_back( it.second );
    }

    return v;
}

std::vector<Block> Block::GetBlocks()
{
    return blocks;
}

void Block::SetBlocks(std::vector<Block> _blocks)
{
    blocks = _blocks;
}

Instruction Block::GetNextInstruction()
{
    if (stack.size()<=0) { return Instruction(ReturnType::RETURN_VOID, 0, SyntaxGlobal::empty_block, {Token(0, 0, SyntaxGlobal::blank_instruction)}); }
    Instruction instruction = stack.at(0);
    return instruction;
}