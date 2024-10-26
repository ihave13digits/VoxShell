#pragma once
#ifndef VOX_LIB_STANDARD_H
#define VOX_LIB_STANDARD_H

#include <iostream>
#include <vector>

#include "../vox/syntax.h"
#include "../vox/token.h"
#include "../vox/instruction.h"
#include "../vox/vox.h"

namespace VoxStandard
{

    extern std::map<std::string, Generic::Function> functions;

    std::vector<Token> ExecuteDelete(Instruction instruction);
    std::vector<Token> ExecuteEcho(Instruction instruction);
    std::vector<Token> ExecuteEchi(Instruction instruction);
    std::vector<Token> ExecuteEval(Instruction instruction);
    std::vector<Token> ExecuteExit(Instruction instruction);
    std::vector<Token> ExecuteFor(Instruction instruction);
    std::vector<Token> ExecuteIf(Instruction instruction);
    std::vector<Token> ExecuteInclude(Instruction instruction);
    std::vector<Token> ExecuteToString(Instruction instruction);

}

#endif // VOX_LIB_STANDARD_H