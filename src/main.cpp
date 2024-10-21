#include <iostream>
#include <cmath>
#include "vox/shell.h"
#include "time/timer.h"

Shell shell = Shell();

//
// Standard Library
//

std::vector<Token> ExecuteDelete(Instruction instruction)
{
    const int S = instruction.GetArguments().size();
    for (int i=0; i<S; i++)
    {
        std::string value = instruction.GetArgument(i).GetName();
        shell.DeleteVariable(value);
    }
    return {};
}

std::vector<Token> ExecuteEcho(Instruction instruction)
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
            if (shell.IsStringInteger(value) && value.size()==1) { std::cout << Boolean::alias[std::stoi(value)]; }
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
                if (shell.IsStringInteger(value) && value.size()==1) { std::cout << Boolean::alias[std::stoi(value)]; }
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

std::vector<Token> ExecuteEchi(Instruction instruction)
{
    ExecuteEcho(instruction);
    std::string line = ""; std::getline(std::cin, line);
    return {Token(instruction.GetArgument(0).GetIndex(), SyntaxType::TYPE_STRING, line, "")};
}

std::vector<Token> ExecuteEval(Instruction instruction)
{
    //shell.PrintTokens(instruction.GetArguments());
    std::string value = instruction.GetArgument(0).GetValue();
    if (value==SyntaxGlobal::blank_instruction) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_UNINTELLIGIBLE_INPUT] << std::endl; return {Token()}; }
    shell.ParseLine(value);
    return {};
}

std::vector<Token> ExecuteExit(Instruction instruction)
{
    shell.SetUserEngaged(false);
    return {};
}

std::vector<Token> ExecuteFor(Instruction instruction)
{
    std::cout<<"For Called"<<std::endl;
    //shell.PrintTokens(instruction.GetArguments());
    Token iter = instruction.GetArgument(0);
    Token oper = instruction.GetArgument(1);
    Token comp = instruction.GetArgument(2);
    std::string var_name = iter.GetName();
    bool loop = false;
    Token replace;
    if (shell.VariableExists(var_name))
    {
        replace = shell.GetVariable(var_name);
        std::string value = replace.GetValue();
        if      (replace.GetType()==SyntaxType::TYPE_INTEGER)
        {
            if      (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER]       )
            {
                int v = std::stoi(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v< std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v< std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER]      )
            {
                int v = std::stoi(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v> std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v> std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER_EQUAL] )
            {
                int v = std::stoi(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v<=std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v<=std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER_EQUAL])
            {
                int v = std::stoi(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v>=std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v>=std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
        }
        else if (replace.GetType()==SyntaxType::TYPE_DECIMAL)
        {
            if      (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER]       )
            {
                float v = std::stof(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v< std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v< std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER]      )
            {
                float v = std::stof(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v> std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v> std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_LESSER_EQUAL] )
            {
                float v = std::stof(value); value = std::to_string(v+1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v<=std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v<=std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
            else if (oper.GetValue()==Operator::keys[Operator::OPERATOR_GREATER_EQUAL])
            {
                float v = std::stof(value); value = std::to_string(v-1);
                if      (comp.GetType()==SyntaxType::TYPE_INTEGER) { if (v>=std::stoi(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
                else if (comp.GetType()==SyntaxType::TYPE_DECIMAL) { if (v>=std::stof(comp.GetValue())) { replace.SetValue(value); shell.SetVariable(var_name, replace); loop=true; } }
            }
        }
    }
    shell.SetRepeatBlock(loop);
    return {};
}

std::vector<Token> ExecuteIf(Instruction instruction)
{
    std::string value = instruction.GetArgument(0).GetValue();
    return {};
}

std::vector<Token> ExecuteInclude(Instruction instruction)
{
    std::string value = instruction.GetArgument(0).GetValue();
    if (value==SyntaxGlobal::blank_instruction) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_MISSING_ARGUMENTS] << std::endl; return {Token()}; }
    Script script = Script();
    script.LoadFile("assets/scripts/"+value);
    shell.ParseScript(script);
    return {};
}

std::vector<Token> ExecuteToString(Instruction instruction)
{
    return {Token(instruction.GetArgument(0).GetIndex(), SyntaxType::TYPE_STRING, instruction.GetArgument(0).GetValue(), instruction.GetArgument(0).GetName())};
}



//
// Math Library
//

std::vector<Token> ExecuteSeedRandom(Instruction instruction)
{
    Token arg = instruction.GetArgument(0);
    if (arg.GetType()==SyntaxType::TYPE_INTEGER) { srand(std::stoi(arg.GetValue())); }
    else { srand(time(0)%100000); }
    return {};
}

std::vector<Token> ExecuteRandom(Instruction instruction)
{
    return {Token(instruction.GetArgument(0).GetIndex(), SyntaxType::TYPE_INTEGER, std::to_string(rand()))};
}

std::vector<Token> ExecuteCos(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float cos_value = cos(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(cos_value))};
}

std::vector<Token> ExecuteSin(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sin_value = sin(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sin_value))};
}

std::vector<Token> ExecuteTan(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sin_value = tan(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sin_value))};
}



void Eval(int argc, char *argv[])
{
    std::string line = "";
    for (int i=1; i<argc; i++)
    {
        line += argv[i];
    }
    shell.Evaluate(line);
}

void Run()
{
    while (shell.IsUserEngaged())
    {
        std::string line = ""; std::getline(std::cin, line);
        Timer timer = Timer();
        //time = timer.Tick();
        shell.BypassStackCompleted();
        shell.Evaluate(line);
        std::cout << "\nTook: "<< timer.Tick() << " seconds.\n" << std::endl;
    }
}



int main(int argc, char *argv[])
{
    // TODO: Move These To "Standard Library"
    shell.RegisterFunction("delete",    Generic::Function(-1, ReturnType::RETURN_VOID, ExecuteDelete));
    shell.RegisterFunction("echo",      Generic::Function(-1, ReturnType::RETURN_VOID, ExecuteEcho));
    shell.RegisterFunction("echi",      Generic::Function(-1, ReturnType::RETURN_STRING, ExecuteEchi));
    shell.RegisterFunction("eval",      Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteEval));
    shell.RegisterFunction("exit",      Generic::Function( 0, ReturnType::RETURN_VOID, ExecuteExit));
    shell.RegisterFunction("for",       Generic::Function( 3, ReturnType::RETURN_VOID, ExecuteFor));
    shell.RegisterFunction("if",        Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteIf));
    shell.RegisterFunction("include",   Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteInclude));
    shell.RegisterFunction("to_string", Generic::Function( 1, ReturnType::RETURN_STRING, ExecuteToString));
    // TODO: Move These To "Math Library"
    shell.RegisterFunction("seed",      Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteSeedRandom));
    shell.RegisterFunction("rand",      Generic::Function( 0, ReturnType::RETURN_INTEGER, ExecuteRandom));
    shell.RegisterFunction("cos",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, ExecuteCos));
    shell.RegisterFunction("sin",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, ExecuteSin));
    shell.RegisterFunction("tan",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, ExecuteTan));
    if (argc>1)
    {
        Eval(argc, argv);
    }
    else
    {
        Run();
    }
    return 0;
}