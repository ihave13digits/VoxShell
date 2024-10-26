#include "vox_standard.h"

std::map<std::string, Generic::Function> VoxStandard::functions =
{
    {"delete",    Generic::Function(-1, ReturnType::RETURN_VOID, VoxStandard::ExecuteDelete)},
    {"echo",      Generic::Function(-1, ReturnType::RETURN_VOID, VoxStandard::ExecuteEcho)},
    {"echi",      Generic::Function(-1, ReturnType::RETURN_STRING, VoxStandard::ExecuteEchi)},
    {"eval",      Generic::Function( 1, ReturnType::RETURN_VOID, VoxStandard::ExecuteEval)},
    {"exit",      Generic::Function( 0, ReturnType::RETURN_VOID, VoxStandard::ExecuteExit)},
    {"for",       Generic::Function( 3, ReturnType::RETURN_VOID, VoxStandard::ExecuteFor)},
    {"if",        Generic::Function( 1, ReturnType::RETURN_VOID, VoxStandard::ExecuteIf)},
    {"include",   Generic::Function( 1, ReturnType::RETURN_VOID, VoxStandard::ExecuteInclude)},
    {"to_string", Generic::Function( 1, ReturnType::RETURN_STRING, VoxStandard::ExecuteToString)},
};

std::vector<Token> VoxStandard::ExecuteDelete(Instruction instruction)
{
    const int S = instruction.GetArguments().size();
    for (int i=0; i<S; i++)
    {
        std::string value = instruction.GetArgument(i).GetName();
        Vox::shell.DeleteVariable(value);
    }
    return {};
}

std::vector<Token> VoxStandard::ExecuteEcho(Instruction instruction)
{
    std::vector<Token> args = instruction.GetArguments();
    const int S = args.size();
    if (S==0)
    {
        std::cout << std::endl;
    }
    else if (S==1)
    {
        Token token = args.at(0);
        std::string value = token.GetValue();
        if      (token.GetType()==SyntaxType::TYPE_BOOLEAN)
        {
            if (Vox::shell.IsStringInteger(value) && value.size()==1) { std::cout << Boolean::alias[std::stoi(value)]; }
            else { std::cout << value; }
        }
        else if (token.GetType()==SyntaxType::TYPE_STRING)
        {
            std::string last_char = "";
            std::string value = token.GetValue();
            const int VS = value.size();
            for (int c=0; c<VS; c++)
            {
                std::string this_char=value.substr(c, 1);
                if (this_char=="\\" && c+1<VS)
                {
                    std::string next_char=value.substr(c+1, 1);
                    if      (next_char=="n") { std::cout << "\n"; }
                    else if (next_char=="t") { std::cout << "\t"; }
                    else if (next_char=="v") { std::cout << "\v"; }
                    else if (next_char=="f") { std::cout << "\f"; }
                    else if (next_char=="r") { std::cout << "\r"; }
                    else if (next_char=="b") { std::cout << "\b"; }
                    else if (next_char=="a") { std::cout << "\a"; }
                }
                else if (last_char!="\\") { std::cout << this_char; }
                last_char = this_char;
            }
        }
        else { std::cout << token.GetValue() << std::endl; }
        std::cout << std::endl;
    }
    else
    {
        for (int i=0; i<S-1; i++)
        {
            Token token = args.at(i);
            std::string value = token.GetValue();
            if      (token.GetType()==SyntaxType::TYPE_BOOLEAN)
            {
                if (Vox::shell.IsStringInteger(value) && value.size()==1) { std::cout << Boolean::alias[std::stoi(value)]; }
                else { std::cout << value; }
            }
            else if (token.GetType()==SyntaxType::TYPE_STRING)
            {
                const int VS = value.size();
                std::string last_char = "";
                for (int c=0; c<VS; c++)
                {
                    std::string this_char=value.substr(c, 1);
                    if (this_char=="\\" && c+1<VS)
                    {
                        std::string next_char=value.substr(c+1, 1);
                        if      (next_char=="n") { std::cout << "\n"; }
                        else if (next_char=="t") { std::cout << "\t"; }
                        else if (next_char=="v") { std::cout << "\v"; }
                        else if (next_char=="f") { std::cout << "\f"; }
                        else if (next_char=="r") { std::cout << "\r"; }
                        else if (next_char=="b") { std::cout << "\b"; }
                        else if (next_char=="a") { std::cout << "\a"; }
                    }
                    else if (last_char!="\\") { std::cout << this_char; }
                    last_char = this_char;
                }
            }
            else { std::cout << value; }
        }
        std::string end = args.at(S-1).GetValue();
        if (end=="\\n") { std::cout << std::endl; }
        else            { std::cout << end; }
    }
    return {};
}

std::vector<Token> VoxStandard::ExecuteEchi(Instruction instruction)
{
    ExecuteEcho(instruction);
    std::string line = ""; std::getline(std::cin, line);
    return {Token(instruction.GetArgument(0).GetIndex(), SyntaxType::TYPE_STRING, line, "")};
}

std::vector<Token> VoxStandard::ExecuteEval(Instruction instruction)
{
    //Vox::shell.PrintTokens(instruction.GetArguments());
    std::string value = instruction.GetArgument(0).GetValue();
    if (value==SyntaxGlobal::blank_instruction) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_UNINTELLIGIBLE_INPUT] << std::endl; return {Token()}; }
    Vox::shell.ParseLine(value);
    return {};
}

std::vector<Token> VoxStandard::ExecuteExit(Instruction instruction)
{
    Vox::shell.SetUserEngaged(false);
    return {};
}

std::vector<Token> VoxStandard::ExecuteFor(Instruction instruction)
{
    //Vox::shell.PrintTokens(instruction.GetArguments());
    Token iter = instruction.GetArgument(0);
    Token oper = instruction.GetArgument(1);
    Token comp = instruction.GetArgument(2);
    std::string var_name = iter.GetName();
    bool loop = false;
    Token replace;
    if (Vox::shell.VariableExists(var_name))
    {
        replace = Vox::shell.GetVariable(var_name);
        std::string value = replace.GetValue();
        if      (replace.GetType()==SyntaxType::TYPE_INTEGER)
        {
            if      (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER]       )
            {
                int v = std::stoi(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v< std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v< std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER]      )
            {
                int v = std::stoi(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v> std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v> std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER_EQUAL] )
            {
                int v = std::stoi(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v<=std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v<=std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER_EQUAL])
            {
                int v = std::stoi(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v>=std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v>=std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
        }
        else if (replace.GetType()==SyntaxType::TYPE_DECIMAL)
        {
            if      (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER]       )
            {
                float v = std::stof(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v< std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v< std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER]      )
            {
                float v = std::stof(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v> std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v> std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER_EQUAL] )
            {
                float v = std::stof(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v<=std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v<=std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER_EQUAL])
            {
                float v = std::stof(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v>=std::stoi(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v>=std::stof(comp.GetValue())) { replace.SetValue(value); Vox::shell.SetVariable(var_name, replace); loop=true; } }
            }
        }
    }
    if (loop)
    {
        // Figure Out Pushing Block To Stack
        
        
    }
    return {};
}

std::vector<Token> VoxStandard::ExecuteIf(Instruction instruction)
{
    std::string value = instruction.GetArgument(0).GetValue();
    return {};
}

std::vector<Token> VoxStandard::ExecuteInclude(Instruction instruction)
{
    std::string value = instruction.GetArgument(0).GetValue();
    if (value==SyntaxGlobal::blank_instruction) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_MISSING_ARGUMENTS] << std::endl; return {Token()}; }
    Script script = Script();
    script.LoadFile("assets/scripts/"+value);
    Vox::shell.ParseScript(script);
    return {};
}

std::vector<Token> VoxStandard::ExecuteToString(Instruction instruction)
{
    return {Token(instruction.GetArgument(0).GetIndex(), SyntaxType::TYPE_STRING, instruction.GetArgument(0).GetValue(), instruction.GetArgument(0).GetName())};
}