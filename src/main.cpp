#include <iostream>
#include <cmath>
#include "vox/vox.h"
#include "vox/shell.h"
#include "time/timer.h"
#include "libraries/vox_math.h"
#include "libraries/vox_standard.h"



void Eval(int argc, char *argv[])
{
    std::string line = "";
    for (int i=1; i<argc; i++)
    {
        line += argv[i];
    }
    Vox::shell.Evaluate(line);
}

void Run()
{
    while (Vox::shell.IsUserEngaged())
    {
        std::string line = ""; std::getline(std::cin, line);
        Timer timer = Timer();
        //time = timer.Tick();
        Vox::shell.Evaluate(line);
        std::cout << "\nTook: "<< timer.Tick() << " seconds.\n" << std::endl;
    }
}



int main(int argc, char *argv[])
{
    // Standard Library
    Vox::shell.RegisterFunction("delete",    Generic::Function(-1, ReturnType::RETURN_VOID, VoxStandard::ExecuteDelete));
    Vox::shell.RegisterFunction("echo",      Generic::Function(-1, ReturnType::RETURN_VOID, VoxStandard::ExecuteEcho));
    Vox::shell.RegisterFunction("echi",      Generic::Function(-1, ReturnType::RETURN_STRING, VoxStandard::ExecuteEchi));
    Vox::shell.RegisterFunction("eval",      Generic::Function( 1, ReturnType::RETURN_VOID, VoxStandard::ExecuteEval));
    Vox::shell.RegisterFunction("exit",      Generic::Function( 0, ReturnType::RETURN_VOID, VoxStandard::ExecuteExit));
    Vox::shell.RegisterFunction("for",       Generic::Function( 3, ReturnType::RETURN_VOID, VoxStandard::ExecuteFor));
    Vox::shell.RegisterFunction("if",        Generic::Function( 1, ReturnType::RETURN_VOID, VoxStandard::ExecuteIf));
    Vox::shell.RegisterFunction("include",   Generic::Function( 1, ReturnType::RETURN_VOID, VoxStandard::ExecuteInclude));
    Vox::shell.RegisterFunction("to_string", Generic::Function( 1, ReturnType::RETURN_STRING, VoxStandard::ExecuteToString));
    // Math Library
    Vox::shell.RegisterFunction("seed",      Generic::Function( 1, ReturnType::RETURN_VOID, VoxMath::ExecuteSeedRandom));
    Vox::shell.RegisterFunction("rand",      Generic::Function( 0, ReturnType::RETURN_INTEGER, VoxMath::ExecuteRandom));
    Vox::shell.RegisterFunction("cos",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteCos));
    Vox::shell.RegisterFunction("sin",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteSin));
    Vox::shell.RegisterFunction("tan",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteTan));
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