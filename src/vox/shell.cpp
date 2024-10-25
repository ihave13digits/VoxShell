#include <iostream>
#include "shell.h"

Shell::Shell()
{
    //
}



std::string Shell::GetColorString(std::string text, int R, int G, int B)
{
    return "\x1b[38;2;"+std::to_string(R)+";"+std::to_string(G)+";"+std::to_string(B)+"m"+text+"\x1b[0m";
}

void Shell::PrintShellCall(std::string call, std::string args)
{
    if (output_debug) { std::cout<<GetColorString("Shell::"+call+"(",0,255,255)+GetColorString(args,0,255,0)+GetColorString(")",0,255,255)<<std::endl; }
}

void Shell::PrintShellHint(std::string call, std::string hint)
{
    if (output_debug) { std::cout<<GetColorString("Shell::"+call+"() -> ",255,255,0)+GetColorString(hint,255,127,0)<<std::endl; }
}

void Shell::PrintShellWarning(std::string text)
{
    if (output_debug) { std::cout<<GetColorString("Warning: ",255,255,0)+GetColorString(text,255,255,255)<<std::endl; }
}

void Shell::PrintShellError(std::string text)
{
    if (output_debug) { std::cout<<GetColorString("Error: ",255,0,0)+GetColorString(text,255,255,0)<<std::endl; }
}



void Shell::PrintTokens(std::vector<Token> tokens)
{
    //std::cout << "----------------------------------------------------------------" << std::endl;
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::cout << GetColorString("(", 0,212,0) << GetColorString(tokens[i].GetValue(), 255,255,255);
        std::cout << GetColorString(") Type[", 0,212,0);
        std::cout << GetColorString(SyntaxType::keys[tokens[i].GetType()], 255,255,255);
        std::cout << GetColorString("] Name[", 0,212,0);
        std::cout << GetColorString(tokens[i].GetName(), 255,255,255);
        std::cout << GetColorString("] @ Index:", 0,212,0);
        std::cout << GetColorString(std::to_string(tokens[i].GetIndex()), 255,255,255) << std::endl;
    }
    std::cout << std::endl;
    //std::cout << "----------------------------------------------------------------" << std::endl;
}

void Shell::PrintInstructions(std::vector<Instruction> instructions)
{
    for (int i=0; i<int(instructions.size()); i++)
    {
        std::cout << GetColorString("State:", 255,128,  0) << GetColorString(instructions.at(i).GetState(), 255,255,255);
        std::cout << GetColorString(" Type:", 255,128,  0) << GetColorString(ReturnType::keys[instructions.at(i).GetType()], 255,255,255);
        std::cout << GetColorString(" Args:", 255,128,  0) << GetColorString(std::to_string(instructions.at(i).GetArgC()), 255,255,255);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
///*
void Shell::PrintState()
{
    std::string title = "================================================================\n"
                        "                        - State Summary -                       \n"
                        "================================================================";
    std::cout << GetColorString(title, 0, 128, 128) << std::endl;
    std::vector<Token> stack_vars = stack.GetVariables(current_scope);
    std::vector<Instruction> stack_calls = stack.GetInstructions(current_scope);
    const int S = stack.GetBlockSize(current_scope);

    std::cout << GetColorString("Scope:",128,0,255) << GetColorString(std::to_string(current_scope), 255,255,255);
    std::cout << GetColorString(" Instruction:",128,0,255) << GetColorString(std::to_string(stack.GetInstructionIndex(current_scope)), 255,255,255);
    std::cout << GetColorString("/"+std::to_string(stack.GetSize(current_scope)), 255,255,255);
    std::cout << GetColorString(" Block:",128,0,255) << GetColorString(std::to_string(stack.GetBlockIndex(current_scope)), 255,255,255);
    std::cout << GetColorString("/"+std::to_string(S), 255,255,255);
    std::cout << GetColorString(" Variables:",128,0,255) << GetColorString(std::to_string(stack_vars.size()), 255,255,255);
    std::cout << GetColorString(" State:",128,0,255) << GetColorString(BlockState::keys[stack.GetState(current_scope)], 255,255,255);
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
            std::cout << GetColorString("    Scope:",128,0,255) << GetColorString(std::to_string(_scope), 255,255,255);
            std::cout << GetColorString(" Instruction:",128,0,255) << GetColorString(std::to_string(blocks.at(i).GetInstructionIndex(_scope)), 255,255,255);
            std::cout << GetColorString("/"+std::to_string(blocks.at(i).GetSize(_scope)), 255,255,255);
            std::cout << GetColorString(" Block:",128,0,255) << GetColorString(std::to_string(blocks.at(i).GetBlockIndex(_scope)), 255,255,255);
            std::cout << GetColorString("/"+std::to_string(blocks.at(i).GetBlockSize(_scope)), 255,255,255);
            std::cout << GetColorString(" Variables:",128,0,255) << GetColorString(std::to_string(_stack_vars.size()), 255,255,255);
            std::cout << GetColorString(" State:",128,0,255) << GetColorString(BlockState::keys[blocks.at(i).GetState(_scope)], 255,255,255);
            std::cout << std::endl;
            PrintTokens(_stack_vars);
            PrintInstructions(_stack_calls);
        }
    }
}
//*/
/*
void Shell::PrintState()
{
    const int SCOPE = current_scope;
    std::cout << "================================================================" << std::endl;
    std::cout << "==================== - Shell State Summary - ===================" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::vector<Block> blocks = stack.GetBlocks(SCOPE);
    std::vector<Token> stack_vars = stack.GetVariables(SCOPE);
    std::vector<Instruction> stack_calls = stack.GetInstructions(SCOPE);
    const int S = blocks.size();

    std::cout << "Scope:" << SCOPE;
    std::cout << " Instruction:" << stack.GetInstructionIndex();
    std::cout << "/" << stack.GetSize(SCOPE);
    std::cout << " Block:" << stack.GetBlockIndex(SCOPE);
    std::cout << "/" << S;
    std::cout << " Variables:"<< stack_vars.size();
    std::cout << " State:" << BlockState::keys[stack.GetState(SCOPE)];
    std::cout << std::endl;
    // Print Variables
    std::cout << "Variables: ";
    for (int i=0; i<int(stack_vars.size()); i++)
    {
        Token var = stack_vars.at(i);
        if (i>0) { std::cout<<", "; }
        std::cout<<var.GetName()<<":"<<var.GetValue();
    }
    std::cout << std::endl;
    // Print Calls
    std::cout << "Calls: ";
    for (int i=0; i<int(stack_calls.size()); i++)
    {
        Instruction call = stack_calls.at(i);
        if (i>0) { std::cout<<", "; }
        std::cout<<ReturnType::keys[call.GetType()]<<" "<<call.GetState()<<"(";
        std::vector<Token> args = call.GetArguments();
        for (int a=0; a<int(args.size()); a++)
        {
            Token arg = args.at(a);
            if (a>0) { std::cout<<", "; }
            std::cout<<arg.GetValue();
            std::string name = arg.GetName();
            if (name!="") { std::cout<<"("<<name<<")"; }
        }
        std::cout<<")";
    }
    std::cout << std::endl;

}
*/



void Shell::ForceExit(std::string message)
{
    PrintShellError(message);
    PrintShellHint("ForceExit", "Clearing Stack");
    SetUserEngaged(false);
    ClearStack();
    stack.SetState(BlockState::BLOCK_COMPLETE, 0);
}

void Shell::BypassStackCompleted()
{
    stack.SetState(BlockState::BLOCK_COMPUTING, 0);
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

void Shell::SetRepeatBlock(bool value, int _scope)
{
    PrintShellCall("SetRepeatBlock", Boolean::alias[int(value)]+" @ "+std::to_string(_scope));
    if (value) { stack.SetState(BlockState::BLOCK_REPEATING, _scope); }
    else       { stack.SetState(BlockState::BLOCK_COMPUTING, _scope); }
    stack.SetRepeatBlock(value, _scope);
}

void Shell::SetExpectingBlock(std::string state, int index)
{
    PrintShellCall("SetExpectingBlock", state);
    call_expecting_block = state;
    call_expecting_index = index;
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
    PrintShellCall("ClearStack", "");
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
    PrintShellCall("SetStackLimit", std::to_string(_stack_limit));
    stack_limit = _stack_limit;
}



int Shell::GetInstructionIndex()
{
    return stack.GetInstructionIndex(current_scope);
}

void Shell::SetInstructionIndex(int index)
{
    PrintShellCall("SetInstructionIndex", std::to_string(index));
    stack.SetInstructionIndex(index, current_scope);
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
