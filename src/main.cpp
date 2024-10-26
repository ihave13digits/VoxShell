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
    // Register Libraries
    Vox::shell.RegisterLibrary(VoxStandard::functions);
    Vox::shell.RegisterLibrary(VoxMath::functions);
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