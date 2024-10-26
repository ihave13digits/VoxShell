#include "vox_math.h"

std::map<std::string, Generic::Function> VoxMath::functions =
{
    {"seed",      Generic::Function( 1, ReturnType::RETURN_VOID, VoxMath::ExecuteSeedRandom)},
    {"rand",      Generic::Function( 0, ReturnType::RETURN_INTEGER, VoxMath::ExecuteRandom)},
    
    {"cos",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteCos)},
    {"sin",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteSin)},
    {"tan",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteTan)},
    {"acos",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteACos)},
    {"asin",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteASin)},
    {"atan",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteATan)},
    {"atan2",     Generic::Function( 2, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteATan2)},
    
    {"cosh",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteCosH)},
    {"sinh",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteSinH)},
    {"tanh",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteTanH)},
    {"acosh",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteACosH)},
    {"asinh",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteASinH)},
    {"atanh",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteATanH)},
    
    {"exp",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteExp)},
    {"log",       Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteLog)},
    {"sqrt",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteSqrt)},
    {"cbrt",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteCbrt)},
    
    {"ceil",      Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteCeil)},
    {"floor",     Generic::Function( 1, ReturnType::RETURN_DECIMAL, VoxMath::ExecuteFloor)},
};

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
    float tan_value = tan(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(tan_value))};
}

std::vector<Token> VoxMath::ExecuteACos(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float cos_value = acos(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(cos_value))};
}

std::vector<Token> VoxMath::ExecuteASin(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float asin_value = asin(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(asin_value))};
}

std::vector<Token> VoxMath::ExecuteATan(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float atan_value = atan(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(atan_value))};
}

std::vector<Token> VoxMath::ExecuteATan2(Instruction instruction)
{
    Token token1 = instruction.GetArgument(0);
    Token token2 = instruction.GetArgument(1);
    float value1 = 0.0;
    float value2 = 0.0;
    if      (token1.GetType()==SyntaxType::TYPE_INTEGER) { value1 = float(std::stoi(token1.GetValue())); }
    else if (token1.GetType()==SyntaxType::TYPE_DECIMAL) { value1 = std::stof(token1.GetValue()); }
    if      (token2.GetType()==SyntaxType::TYPE_INTEGER) { value2 = float(std::stoi(token2.GetValue())); }
    else if (token2.GetType()==SyntaxType::TYPE_DECIMAL) { value2 = std::stof(token2.GetValue()); }
    float atan2_value = atan2(value1, value2);
    return {Token(token1.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(atan2_value))};
}



std::vector<Token> VoxMath::ExecuteCosH(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float cosh_value = cosh(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(cosh_value))};
}

std::vector<Token> VoxMath::ExecuteSinH(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sinh_value = sinh(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sinh_value))};
}

std::vector<Token> VoxMath::ExecuteTanH(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float tanh_value = tanh(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(tanh_value))};
}

std::vector<Token> VoxMath::ExecuteACosH(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float acosh_value = acosh(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(acosh_value))};
}

std::vector<Token> VoxMath::ExecuteASinH(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float asinh_value = asinh(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(asinh_value))};
}

std::vector<Token> VoxMath::ExecuteATanH(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float atanh_value = atanh(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(atanh_value))};
}



std::vector<Token> VoxMath::ExecuteExp(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float exp_value = exp(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(exp_value))};
}

std::vector<Token> VoxMath::ExecuteLog(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float log_value = log(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(log_value))};
}

std::vector<Token> VoxMath::ExecuteSqrt(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float sqrt_value = sqrt(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(sqrt_value))};
}

std::vector<Token> VoxMath::ExecuteCbrt(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float cbrt_value = cbrt(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(cbrt_value))};
}



std::vector<Token> VoxMath::ExecuteCeil(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float ceil_value = ceil(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(ceil_value))};
}

std::vector<Token> VoxMath::ExecuteFloor(Instruction instruction)
{
    Token token = instruction.GetArgument(0);
    float value = 0.0;
    if      (token.GetType()==SyntaxType::TYPE_INTEGER) { value = float(std::stoi(token.GetValue())); }
    else if (token.GetType()==SyntaxType::TYPE_DECIMAL) { value = std::stof(token.GetValue()); }
    float floor_value = floor(value);
    return {Token(token.GetIndex(), SyntaxType::TYPE_DECIMAL, std::to_string(floor_value))};
}