#pragma once
#ifndef VOX_LIB_MATH_H
#define VOX_LIB_MATH_H

#include <cmath>
#include <map>
#include <vector>

#include "../vox/generic.h"
#include "../vox/syntax.h"
#include "../vox/token.h"
#include "../vox/instruction.h"

namespace VoxMath
{

    extern std::map<std::string, Generic::Function> functions;

    std::vector<Token> ExecuteSeedRandom(Instruction instruction);
    std::vector<Token> ExecuteRandom(Instruction instruction);

    std::vector<Token> ExecuteCos(Instruction instruction);
    std::vector<Token> ExecuteSin(Instruction instruction);
    std::vector<Token> ExecuteTan(Instruction instruction);
    std::vector<Token> ExecuteACos(Instruction instruction);
    std::vector<Token> ExecuteASin(Instruction instruction);
    std::vector<Token> ExecuteATan(Instruction instruction);
    std::vector<Token> ExecuteATan2(Instruction instruction);

    std::vector<Token> ExecuteCosH(Instruction instruction);
    std::vector<Token> ExecuteSinH(Instruction instruction);
    std::vector<Token> ExecuteTanH(Instruction instruction);
    std::vector<Token> ExecuteACosH(Instruction instruction);
    std::vector<Token> ExecuteASinH(Instruction instruction);
    std::vector<Token> ExecuteATanH(Instruction instruction);

    std::vector<Token> ExecuteExp(Instruction instruction);
    std::vector<Token> ExecuteLog(Instruction instruction);
    std::vector<Token> ExecuteSqrt(Instruction instruction);
    std::vector<Token> ExecuteCbrt(Instruction instruction);

    std::vector<Token> ExecuteCeil(Instruction instruction);
    std::vector<Token> ExecuteFloor(Instruction instruction);

}

#endif // VOX_LIB_MATH_H