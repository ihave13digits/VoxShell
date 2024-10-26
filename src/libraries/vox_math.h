#pragma once
#ifndef VOX_LIB_MATH_H
#define VOX_LIB_MATH_H

#include <cmath>
#include <vector>

#include "../vox/syntax.h"
#include "../vox/token.h"
#include "../vox/instruction.h"

namespace VoxMath
{

    std::vector<Token> ExecuteSeedRandom(Instruction instruction);
    std::vector<Token> ExecuteRandom(Instruction instruction);
    std::vector<Token> ExecuteCos(Instruction instruction);
    std::vector<Token> ExecuteSin(Instruction instruction);
    std::vector<Token> ExecuteTan(Instruction instruction);

}

#endif // VOX_LIB_MATH_H