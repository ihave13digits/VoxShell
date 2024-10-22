#include "syntax.h"

std::string SyntaxGlobal::blank_instruction = "__InstructionIsBlank__";
std::string SyntaxGlobal::empty_block = "__BlockIsEmpty__";
std::string SyntaxGlobal::unsolved_problem = "__UnsolvedProblem__";

std::string SyntaxGlobal::source_is_direct = "__SourceIsDirect__";

std::string SyntaxGlobal::function_exists = "__FunctionExists__";



std::vector<std::string> SyntaxType::keys =
{
    "Unknown",
    "Boolean",
    "Integer",
    "Decimal",
    "String",
    "Syntax",
    "Operator",
    "Built-In",
    "Return",
    "Invalid",
    "Error: String Logic With Boolean",
    "Error: String Logic With Integer",
    "Error: String Logic With Decimal",
    "Error: Mismatched Quote",
    "Error: Mismatched Quotes",
    "Error: Mismatched Parentheses",
    "Error: Mismatched Set Value",
    "Error: Missing Arguments",
    "Error: Stack Limit Reached",
    "Error: Unintelligible Input",
};

std::vector<std::string> Syntax::keys =
{
    " ",
    "\\",
    "\"",
    "'",
    ",",
    "{",
    "}",
    ";",
};

std::vector<std::string> ReturnType::keys =
{
    "void",
    "bool",
    "int",
    "float",
    "string",
};

std::vector<std::string> Operator::keys =
{
    "(",
    ")",
    "^",
    "*",
    "/",
    "%",
    "+",
    "-",
    "=",
    "!",
    "<",
    ">",
    "&",
    "|",
    "&&",
    "||",
    "<=",
    ">=",
    "!=",
    "==",
};

std::vector<std::string> MathState::keys =
{
    "()",
    "^^",
    "*/",
    "+-",
    "<>!=|&"
};

std::vector<std::string> BlockState::keys =
{
    "Block Waiting",
    "Block Repeating",
    "Block Computing",
    "Block Complete",
};

std::vector<std::string> Boolean::keys =
{
    "0",
    "1",
};

std::vector<std::string> Boolean::alias =
{
    "false",
    "true",
};
