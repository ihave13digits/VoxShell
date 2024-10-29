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

void Shell::PrintShellWarning(std::string text)
{
    if (output_warnings) { std::cout<<GetColorString("Warning: ",255,255,0)+GetColorString(text,255,255,255)<<std::endl; }
}

void Shell::PrintShellError(std::string text)
{
    if (output_errors) { std::cout<<GetColorString("Error: ",255,0,0)+GetColorString(text,255,255,0)<<std::endl; SetUserEngaged(false); }
}

void Shell::PrintShellCall(std::string call, std::string text)
{
    if (output_calls)
    {
        const int R=0,G=255,B=64, _R=255,_G=255,_B=255, TR=64,TG=128,TB=0;
        std::cout << GetColorString("Shell",R,G,B)
            << GetColorString("::",_R,_G,_B)
            << GetColorString(call,R,G,B)
            << GetColorString("(",_R,_G,_B)
            << GetColorString(text,TR,TG,TB)
            << GetColorString(")",_R,_G,_B)
            << std::endl;
    }
}



void Shell::PrintTokens(std::vector<Token> tokens)
{
    const int R=0,G=128,B=64, _R=255,_G=255,_B=255;
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::cout << GetColorString("(", R,G,B)
            << GetColorString(tokens[i].GetValue(), _R,_G,_B)
            << GetColorString(") Type[", R,G,B)
            << GetColorString(SyntaxType::keys[tokens[i].GetType()], _R,_G,_B)
            << GetColorString("] Name[", R,G,B)
            << GetColorString(tokens[i].GetName(), _R,_G,_B)
            << GetColorString("] @ Index:", R,G,B)
            << GetColorString(std::to_string(tokens[i].GetIndex()), _R,_G,_B)
            << std::endl;
    }
    std::cout << std::endl;
}

void Shell::PrintState()
{
    const int R=64,G=0,B=128, _R=255,_G=255,_B=255;
    std::vector<Block> blocks = stack.GetBlocks(current_scope);
    std::vector<Token> vars = stack.GetVariables(current_scope);
    std::cout << GetColorString("Scope:", R,G,B)
        << GetColorString(std::to_string(current_scope), _R,_G,_B)
        << GetColorString(" Stack:", R,G,B)
        << GetColorString(std::to_string(stack.GetSize(current_scope)), _R,_G,_B)
        << GetColorString(" Blocks:", R,G,B)
        << GetColorString(std::to_string(blocks.size()), _R,_G,_B)
        << GetColorString(" Variables:", R,G,B)
        << GetColorString(std::to_string(vars.size()), _R,_G,_B)
        << std::endl;
    PrintTokens(vars);
}



bool Shell::IsUserEngaged()
{
    return user_engaged;
}

void Shell::SetUserEngaged(bool value)
{
    user_engaged = value;
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
    PrintShellCall("SetStackLimit", std::to_string(_stack_limit));
    stack_limit = _stack_limit;
}



int Shell::GetScope()
{
    return current_scope;
}

void Shell::SetScope(int _scope)
{
    PrintShellCall("SetScope", std::to_string(_scope));
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
    PrintShellCall("RegisterFunction", name);
    // Make Sure The Function Is Unique (Until Namespacing Is Implemented)
    if (functions.count(name)==0) { functions.emplace(name, f); }
    else { PrintShellError("Function '"+name+"' Already Exists"); }
}

void Shell::RegisterLibrary(std::map<std::string, Generic::Function> _functions)
{
    for (auto f : _functions)
    {
        RegisterFunction(f.first, f.second);
    }
}



bool Shell::IsBreakPoint(std::string character)
{
    // Check For Syntax Keys
    for (int k=0; k<int(Syntax::keys.size()); k++)
    {
        if (character==Syntax::keys[k])
        {
            return true;
        }
    }
    // Check For Operator Keys
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
    // If The Size Is One, It's Likely An Operator Or Generic Syntax Glyph
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
    // Check For Built-In Functions
    if (FunctionExists(segment))
    {
        return SyntaxType::TYPE_BUILT_IN;
    }
    // Check For Variable Setting
    for (int k=0; k<int(ReturnType::keys.size()); k++)
    {
        if (segment==ReturnType::keys[k])
        {
            return SyntaxType::TYPE_RETURN;
        }
    }
    // Check For Numeric Values
    if      (IsStringInteger(segment)) { return SyntaxType::TYPE_INTEGER; }
    else if (IsStringDecimal(segment)) { return SyntaxType::TYPE_DECIMAL; }
    // Lastly, We Check For Booleans, Since It Can Also Be Represented As An Integer
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
    // Early Return
    if (text.size()>1 && text.at(0)=='0') { return false; }
    // Loop Over Characters
    for (int i=0; i<int(text.size()); i++)
    {
        char c = text.at(i);
        // If The Character Isn't A Number, Let's Just Return Early
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
    // Loop Over Characters
    for (int i=0; i<int(text.size()); i++)
    {
        char c = text.at(i);
        // Check If The Character Isn't A number
        if (!std::isdigit(c))
        {
            // Count The Decimals To Validate Floating Point Number
            if (c=='.')
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
    // The < 1.0 Edge Case
    if (text.size()>1 && text.at(0)=='0' && decimal_index!=1) { return false; }
    return is_valid;
}
