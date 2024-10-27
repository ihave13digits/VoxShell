#include <iostream>
#include <cmath>
#include "vox/vox.h"
#include "vox/shell.h"
#include "time/timer.h"
#include "libraries/vox_math.h"
#include "libraries/vox_standard.h"



void Eval(int argc, char *argv[])
{
    // Create An Input Buffer For User Input
    std::string line = "";
    // Concatenate Arguments Into Input Buffer
    for (int i=1; i<argc; i++)
    {
        line += argv[i];
    }
    // Have VoxShell Evaluate The Input Buffer
    Vox::shell.Evaluate(line);
}

void Run()
{
    // Only While VoxShell Is Active Will We Collect Input
    while (Vox::shell.IsUserEngaged())
    {
        // Collect User Input
        std::string line = ""; std::getline(std::cin, line);
        // Create A Timer To Test Peformance (Obviously This Comes At A Slight Cost)
        Timer timer = Timer();
        // Have VoxShell Evaluate The Input
        Vox::shell.Evaluate(line);
        // Output Timer's Delta Time
        std::cout << "\nTook: "<< timer.Tick() << " seconds.\n" << std::endl;
    }
}



int main(int argc, char *argv[])
{
    // Register Libraries
    Vox::shell.RegisterLibrary(VoxStandard::functions);
    Vox::shell.RegisterLibrary(VoxMath::functions);
    // If The Program Is Launched With Arguments, We just Evaluate What's Passed In And Exit
    if (argc>1)
    {
        Eval(argc, argv);
    }
    // If The Program Is Launched Without Arguments, We Enter An Evaluation Loop
    else
    {
        Run();
    }
    return 0;
}