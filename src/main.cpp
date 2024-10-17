#include <iostream>
#include <cmath>
#include "vox/shell.h"
#include "time/timer.h"

Shell shell = Shell();

std::vector<Token> ExecuteTest(Instruction instruction)
{
    std::string value = instruction.GetArgument(1).GetValue();
    std::cout << value << std::endl;
    return {};
}

std::vector<Token> ExecuteDelete(Instruction instruction)
{
    std::string value = instruction.GetArgument(1).GetName();
    shell.DeleteVariable(value);
    return {};
}

std::vector<Token> ExecuteEcho(Instruction instruction)
{
    //shell.PrintTokens(instruction.GetArguments());
    std::string value = instruction.GetArgument(1).GetValue();
    if (value=="") { std::cout << std::endl; return {Token()}; }
    std::string end = "";
    std::string output = "";
    std::string last_value = "";
    std::string current_value = "";
    int i = 1;
    while (current_value!=SyntaxGlobal::blank_instruction)
    {
        Token token = instruction.GetArgument(i);
        output += last_value;
        last_value = current_value;
        
        if      (token.GetType()==SyntaxType::TYPE_BOOLEAN) { current_value = Boolean::alias[std::stoi(token.GetValue())]; }
        /*else if (token.GetType()==SyntaxType::TYPE_UNKNOWN)
        {
            std::string var_name = token.GetName();
            if (var_name!="")
            {
                current_value = shell.GetStack().GetVariable(var_name).GetValue();
            }
        }*/
        else { current_value = token.GetValue(); }
        if (current_value!=SyntaxGlobal::blank_instruction) { end = current_value; }
        i++;
    }
    if (output=="") { std::cout << end << std::endl; }
    else
    {
        if (end!="\\n" && end!=SyntaxGlobal::blank_instruction)
        {
            std::cout << output << end;
        }
        else
        {
            std::cout << output << std::endl;
        }
    }
    return {};
}

std::vector<Token> ExecuteEval(Instruction instruction)
{
    std::string value = instruction.GetArgument(1).GetValue();
    if (value==SyntaxGlobal::blank_instruction) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_UNINTELLIGIBLE_INPUT] << std::endl; return {Token()}; }
    shell.ParseLine(value);
    return {};
}

std::vector<Token> ExecuteExit(Instruction instruction)
{
    //std::string value = instruction.GetArgument(1).GetName();
    //shell.ClearStack();
    shell.SetUserEngaged(false);
    return {};
}

std::vector<Token> ExecuteIf(Instruction instruction)
{
    std::string value = instruction.GetArgument(1).GetValue();
    return {};
}

std::vector<Token> ExecuteInclude(Instruction instruction)
{
    std::string value = instruction.GetArgument(1).GetValue();
    if (value==SyntaxGlobal::blank_instruction) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_MISSING_ARGUMENTS] << std::endl; return {Token()}; }
    Script script = Script();
    script.LoadFile("assets/scripts/"+value);
    shell.ParseScript(script);
    return {};
}

std::vector<Token> ExecuteSeedRandom(Instruction instruction)
{
    Token arg = instruction.GetArgument(1);
    if (arg.GetType()==SyntaxType::TYPE_INTEGER) { srand(std::stoi(arg.GetValue())); }
    else { srand(time(0)%100000); }
    return {};
}

std::vector<Token> ExecuteRandom(Instruction instruction)
{
    int value = rand();
    return {Token(instruction.GetArgument(1).GetIndex(), SyntaxType::TYPE_INTEGER, std::to_string(value))};
}

std::vector<Token> ExecuteCos(Instruction instruction)
{
    Token token = instruction.GetArgument(1);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float cos_value = cos(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(cos_value))};
}

std::vector<Token> ExecuteSin(Instruction instruction)
{
    Token token = instruction.GetArgument(1);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sin_value = sin(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sin_value))};
}

std::vector<Token> ExecuteTan(Instruction instruction)
{
    Token token = instruction.GetArgument(1);
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
        shell.Evaluate(line);
        std::cout << "\nTook: "<< timer.Tick() << " seconds.\n" << std::endl;
    }
}



int main(int argc, char *argv[])
{
    shell.RegisterFunction("test",    Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteTest));
    shell.RegisterFunction("delete",  Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteDelete));
    shell.RegisterFunction("echo",    Generic::Function(-1, ReturnType::RETURN_VOID, ExecuteEcho));
    shell.RegisterFunction("eval",    Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteEval));
    shell.RegisterFunction("exit",    Generic::Function( 0, ReturnType::RETURN_VOID, ExecuteExit));
    shell.RegisterFunction("if",      Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteIf));
    shell.RegisterFunction("include", Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteInclude));
    shell.RegisterFunction("seed",    Generic::Function( 1, ReturnType::RETURN_VOID, ExecuteSeedRandom));
    shell.RegisterFunction("rand",    Generic::Function( 0, ReturnType::RETURN_INTEGER, ExecuteRandom));
    shell.RegisterFunction("cos",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, ExecuteCos));
    shell.RegisterFunction("sin",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, ExecuteSin));
    shell.RegisterFunction("tan",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, ExecuteTan));
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