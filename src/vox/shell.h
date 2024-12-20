#pragma once
#ifndef SHELL_H
#define SHELL_H
#include <string>
#include <vector>
#include <map>
#include "script.h"
#include "token.h"
#include "syntax.h"
#include "instruction.h"
#include "block.h"
#include "generic.h"



class Shell
{

private:

    bool user_engaged = true;
    bool output_warnings = true;
    bool output_errors = true;
    bool output_calls = false;
    int stack_limit = 4096;
    int current_scope = 0;
    Block stack = Block(current_scope, 0);
    std::vector<Token> output_buffer;
    
    std::map<std::string, Generic::Function> functions;

public:

    Shell();

    std::string GetColorString(std::string text, int R, int G, int B);
    void PrintShellWarning(std::string text);
    void PrintShellError(std::string text);
    void PrintShellCall(std::string call, std::string text);

    bool IsUserEngaged();
    void SetUserEngaged(bool value);

    bool VariableExists(std::string name);
    void DeleteVariable(std::string name);
    Token GetVariable(std::string name);
    void SetVariable(std::string name, Token value);
    void PushVariable(Token value);
    void ClearStack();
    Block GetStack();
    int GetStackLimit();
    void SetStackLimit(int _stack_limit);

    int GetScope();
    void SetScope(int _scope);

    bool FunctionExists(std::string name);
    bool FunctionReturns(std::string name);
    bool HasFunction(std::vector<Token> tokens);
    void RegisterFunction(std::string name, Generic::Function f);
    void RegisterLibrary(std::map<std::string, Generic::Function> _functions);

    bool IsBreakPoint(std::string character);
    int GetTokenType(std::string segment);

    bool IsStringInteger(std::string text);
    bool IsStringDecimal(std::string text);
    void PrintTokens(std::vector<Token> tokens);
    void PrintState();

    Token TokenizeSegment(std::string segment, int index);
    std::vector<Token> TokenizeLine(std::string line);

    bool HasOperation(std::vector<Token> tokens);
    bool HasLogic(std::vector<Token> tokens);
    int FirstOperationIndex(std::vector<Token> tokens, int state);
    int FirstLogicIndex(std::vector<Token> tokens);
    bool CanComputeToken(Token token);
    Token SolveUnary(Token a, Token o);
    Token SolveMath(Token a, Token b, Token o);
    std::vector<Token> ParseMathChunk(std::vector<Token> tokens);
    std::vector<Token> ParseMath(std::vector<Token> tokens);

    Token SolveEquals(int operation_index, std::vector<Token> tokens);
    std::vector<Token> ParseEquals(std::vector<Token> tokens);

    std::vector<Token> ParseQuotes(std::vector<Token> tokens);

    std::vector<Token> ParseBlocks(std::vector<Token> tokens);

    int FirstEqualsIndex(std::vector<Token> tokens);
    std::vector<Token> ParseVariables(std::vector<Token> tokens);

    void ParseLine(std::string line);
    void ParseScript(Script script);

    std::vector<Instruction> GenerateInstructions(std::vector<Token> tokens);

    void Evaluate(std::string line);

};

#endif // SHELL_H