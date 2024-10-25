#pragma once
#ifndef SYNTAX_H
#define SYNTAX_H

#include <string>
#include <vector>

namespace SyntaxGlobal
{
    extern std::string empty_block;
    extern std::string repeat_block;
    extern std::string end_repeat_block;

    extern std::string blank_instruction;
    extern std::string unsolved_problem;

    extern std::string source_is_direct;

    extern std::string function_exists;
}



namespace SyntaxType
{
    enum STATES
    {
        TYPE_UNKNOWN,
        TYPE_BOOLEAN,
        TYPE_INTEGER,
        TYPE_DECIMAL,
        TYPE_STRING,
        TYPE_SYNTAX,
        TYPE_OPERATOR,
        TYPE_BUILT_IN,
        TYPE_RETURN,
        TYPE_INVALID,
        TYPE_ERROR_STRING_LOGIC_BOOLEAN,
        TYPE_ERROR_STRING_LOGIC_INTEGER,
        TYPE_ERROR_STRING_LOGIC_DECIMAL,
        TYPE_ERROR_MISMATCHED_QUOTE,
        TYPE_ERROR_MISMATCHED_QUOTES,
        TYPE_ERROR_MISMATCHED_PARENTHESES,
        TYPE_ERROR_MISMATCHED_SET_VALUE,
        TYPE_ERROR_MISSING_ARGUMENTS,
        TYPE_ERROR_STACK_LIMIT_REACHED,
        TYPE_ERROR_UNINTELLIGIBLE_INPUT,
    };

    extern std::vector<std::string> keys;
}

namespace Syntax
{
    enum STATES
    {
        SYNTAX_SPACE,
        SYNTAX_ESCAPE,
        SYNTAX_QUOTES,
        SYNTAX_QUOTE,
        SYNTAX_COMMA,
        SYNTAX_BLOCK_L,
        SYNTAX_BLOCK_R,
        SYNTAX_END,
    };
    
    extern std::vector<std::string> keys;
}

namespace ReturnType
{
    enum STATES
    {
        RETURN_VOID,
        RETURN_BOOLEAN,
        RETURN_INTEGER,
        RETURN_DECIMAL,
        RETURN_STRING,
    };
    
    extern std::vector<std::string> keys;
}

namespace Operator
{
    enum STATES
    {
        OPERATOR_PAR_L,
        OPERATOR_PAR_R,
        OPERATOR_POW,
        OPERATOR_MUL,
        OPERATOR_DIV,
        OPERATOR_MOD,
        OPERATOR_ADD,
        OPERATOR_SUB,
        OPERATOR_SET,
        OPERATOR_NOT,
        OPERATOR_LESSER,
        OPERATOR_GREATER,
        OPERATOR_LOGIC_AND,
        OPERATOR_LOGIC_OR,
        OPERATOR_AND,
        OPERATOR_OR,
        OPERATOR_LESSER_EQUAL,
        OPERATOR_GREATER_EQUAL,
        OPERATOR_NOT_EQUAL,
        OPERATOR_EQUALS,
    };

    extern std::vector<std::string> keys;
}

namespace MathState
{
    enum STATES
    {
        MATH_PAR,
        MATH_POW,
        MATH_M_D,
        MATH_A_S,
        MATH_LOG,
    };

    extern std::vector<std::string> keys;
}

namespace BlockState
{
    enum STATES
    {
        BLOCK_WAITING,
        BLOCK_REPEATING,
        BLOCK_COMPUTING,
        BLOCK_COMPLETE,
    };
    
    extern std::vector<std::string> keys;
}

namespace Boolean
{
    enum STATES
    {
        FALSE,
        TRUE
    };

    extern std::vector<std::string> keys;
    extern std::vector<std::string> alias;
}

#endif // SYNTAX_H