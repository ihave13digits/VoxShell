#include "vox_math.h"

std::vector<Token> VoxMath::ExecuteSeedRandom(Instruction instruction)
{
    Token arg = instruction.GetArgument(0);
    if (arg.GetType()==SyntaxType::TYPE_INTEGER) { srand(std::stoi(arg.GetValue())); }
    else { srand(time(0)%100000); }
    return {};
}

std::vector<Token> VoxMath::ExecuteRandom(Instruction instruction)
{
    return {Token(instruction.GetArgument(0).GetIndex(), SyntaxType::TYPE_INTEGER, std::to_string(rand()))};
}

std::vector<Token> VoxMath::ExecuteCos(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float cos_value = cos(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(cos_value))};
}

std::vector<Token> VoxMath::ExecuteSin(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sin_value = sin(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sin_value))};
}

std::vector<Token> VoxMath::ExecuteTan(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sin_value = tan(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sin_value))};
}