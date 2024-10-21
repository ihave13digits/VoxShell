#include <iostream>
#include "shell.h"

Shell::Shell()
{
    //
}



bool Shell::IsUserEngaged()
{
    return user_engaged;
}

void Shell::SetUserEngaged(bool value)
{
    user_engaged = value;
}

bool Shell::GetRepeatBlock()
{
    return stack.GetRepeatBlock(current_scope);
}

void Shell::SetRepeatBlock(bool value)
{
    if (value) { stack.SetState(BlockState::BLOCK_WAITING); }
    else       { stack.SetState(BlockState::BLOCK_COMPUTING); }
    stack.SetRepeatBlock(value, current_scope);
}



bool Shell::VariableExists(std::string name)
{
    return (stack.VariableNameExists(name, current_scope));
}

void Shell::DeleteVariable(std::string name)
{
    stack.DeleteVariable(name, current_scope);
}

Token Shell::GetVariable(std::string name)
{
    return stack.GetVariable(name, current_scope);
}

void Shell::SetVariable(std::string name, Token value)
{
    stack.SetVariable(name, value, current_scope);
}

void Shell::PushVariable(Token value)
{
    stack.PushVariable(value, current_scope);
}

void Shell::ClearStack()
{
    stack = Block(0, 0);
}

Block Shell::GetStack()
{
    return stack;
}

int Shell::GetStackLimit()
{
    return stack_limit;
}

void Shell::SetStackLimit(int _stack_limit)
{
    stack_limit = _stack_limit;
}



int Shell::GetScope()
{
    return current_scope;
}

void Shell::SetScope(int _scope)
{
    current_scope = _scope;
}



bool Shell::FunctionExists(std::string name)
{
    return (functions.count(name)>0);
}

bool Shell::FunctionReturns(std::string name)
{
    return (functions.at(name).GetReturnType()!=ReturnType::RETURN_VOID);
}

bool Shell::HasFunction(std::vector<Token> tokens)
{
    for (int i=0; i<int(tokens.size()); i++)
    {
        if (functions.count(tokens.at(i).GetValue())>0) { return true; }
    }
    return false;
}

void Shell::RegisterFunction(std::string name, Generic::Function f)
{
    if (functions.count(name)==0) { functions.emplace(name, f); }
    else { std::cout << SyntaxGlobal::function_exists << std::endl; }
}



bool Shell::IsBreakPoint(std::string character)
{
    for (int k=0; k<int(Syntax::keys.size()); k++)
    {
        if (character==Syntax::keys[k])
        {
            return true;
        }
    }
    for (int k=0; k<int(Operator::keys.size()); k++)
    {
        if (character==Operator::keys[k])
        {
            return true;
        }
    }
    return false;
}

int Shell::GetTokenType(std::string segment)
{
    if (segment.size()==0)
    {
        return SyntaxType::TYPE_INVALID;
    }
    if (segment.size()==1)
    {
        for (int k=0; k<int(Syntax::keys.size()); k++)
        {
            if (segment==Syntax::keys[k])
            {
                return SyntaxType::TYPE_SYNTAX;
            }
        }
        for (int k=0; k<int(Operator::keys.size()); k++)
        {
            if (segment==Operator::keys[k])
            {
                return SyntaxType::TYPE_OPERATOR;
            }
        }
    }
    if (FunctionExists(segment))
    {
        return SyntaxType::TYPE_BUILT_IN;
    }
    for (int k=0; k<int(ReturnType::keys.size()); k++)
    {
        if (segment==ReturnType::keys[k])
        {
            return SyntaxType::TYPE_RETURN;
        }
    }
    if      (IsStringInteger(segment)) { return SyntaxType::TYPE_INTEGER; }
    else if (IsStringDecimal(segment)) { return SyntaxType::TYPE_DECIMAL; }
    for (int k=0; k<int(Boolean::keys.size()); k++)
    {
        if (segment==Boolean::keys[k])
        {
            return SyntaxType::TYPE_BOOLEAN;
        }
        else if (segment==Boolean::alias[k])
        {
            return SyntaxType::TYPE_BOOLEAN;
        }
    }
    return SyntaxType::TYPE_UNKNOWN;
}



bool Shell::IsStringInteger(std::string text)
{
    bool is_valid = true;
    if (text.size()>1 && text.at(0)=='0') { return false; }
    for (int i=0; i<int(text.size()); i++)
    {
        char c = text.at(i);
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return is_valid;
}

bool Shell::IsStringDecimal(std::string text)
{
    bool is_valid = true;
    int decimal_count = 0;
    int decimal_index = text.size();
    for (int i=0; i<int(text.size()); i++)
    {
        char c = text.at(i);
        if (!std::isdigit(c))
        {
            if (i==0 && c=='-')
            {
                
            }
            else if (c=='.')
            {
                decimal_count++;
                if (decimal_count>1) { return false; }
                else { decimal_index = i; }
            }
            else
            {
                return false;
            }
        }
    }
    if (text.size()>1 && text.at(0)=='0' && decimal_index!=1) { return false; }
    return is_valid;
}



void Shell::PrintTokens(std::vector<Token> tokens)
{
    //std::cout << "----------------------------------------------------------------" << std::endl;
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::cout << "(" << tokens[i].GetValue() << ") Type[";
        std::cout << SyntaxType::keys[tokens[i].GetType()] << "] Name[";
        std::cout << tokens[i].GetName() << "] @ Index:";
        std::cout << tokens[i].GetIndex() << std::endl;
    }
    std::cout << std::endl;
    //std::cout << "----------------------------------------------------------------" << std::endl;
}

void Shell::PrintInstructions(std::vector<Instruction> instructions)
{
    for (int i=0; i<int(instructions.size()); i++)
    {
        std::cout << "State:" << instructions.at(i).GetState();
        std::cout << " Type:" << ReturnType::keys[instructions.at(i).GetType()];
        std::cout << " Args:" << instructions.at(i).GetArgC();
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Shell::PrintState()
{
    std::vector<Token> stack_vars = stack.GetVariables(current_scope);
    std::vector<Instruction> stack_calls = stack.GetInstructions(0);
    const int S = stack.GetBlockSize(current_scope);

    std::cout << "Scope:" << current_scope;
    std::cout << " State:" << BlockState::keys[stack.GetState()];
    std::cout << " Instruction:" << stack.GetInstructionIndex();
    std::cout << "/" << stack.GetSize(current_scope);
    std::cout << " Block:" << stack.GetBlockIndex();
    std::cout << "/" << S;
    std::cout << " Variables:"<< stack_vars.size();
    std::cout << std::endl;
    PrintTokens(stack_vars);
    PrintInstructions(stack_calls);
    if (S>0)
    {
        std::vector<Block> blocks = stack.GetBlocks(current_scope);
        for (int i=0; i<S; i++)
        {
            int _scope = blocks.at(i).GetScope(current_scope);
            std::vector<Token> _stack_vars = blocks.at(i).GetVariables(_scope);
            std::vector<Instruction> _stack_calls = stack.GetInstructions(_scope);
            std::cout << "    Scope:" << _scope;
            std::cout << " State:" << BlockState::keys[blocks.at(i).GetState()];
            std::cout << " Instruction:" << blocks.at(i).GetInstructionIndex();
            std::cout << "/" << blocks.at(i).GetSize(_scope);
            std::cout << " Block:" << blocks.at(i).GetBlockIndex();
            std::cout << "/" << blocks.at(i).GetBlockSize(_scope);
            std::cout << " Variables:"<< _stack_vars.size();
            std::cout << std::endl;
            PrintTokens(_stack_vars);
            PrintInstructions(_stack_calls);
        }
    }
}