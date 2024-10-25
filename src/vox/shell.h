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

    std::string call_expecting_block = "";
    int call_expecting_index = -1;
    bool step_parsing = false;
    bool output_debug = false;
    bool user_engaged = true;
    int stack_limit = 4096;
    int current_scope = 0;
    Block stack = Block(current_scope, 0);
    std::vector<Token> output_buffer;
    
    std::map<std::string, Generic::Function> functions;

public:

    Shell();

    std::string GetColorString(std::string text, int R, int G, int B);
    void PrintShellCall(std::string call, std::string args);
    void PrintShellHint(std::string call, std::string hint);
    void PrintShellWarning(std::string text);
    void PrintShellError(std::string text);

    void ForceExit(std::string message);
    void BypassStackCompleted();

    bool IsUserEngaged();
    void SetUserEngaged(bool value);

    bool GetRepeatBlock();
    void SetRepeatBlock(bool value, int _scope);
    void SetExpectingBlock(std::string state, int index);

    bool VariableExists(std::string name);
    void DeleteVariable(std::string name);
    Token GetVariable(std::string name);
    void SetVariable(std::string name, Token value);
    void PushVariable(Token value);
    void ClearStack();
    Block GetStack();
    int GetStackLimit();
    void SetStackLimit(int _stack_limit);

    int GetInstructionIndex();
    void SetInstructionIndex(int index);

    int GetScope();
    void SetScope(int _scope);

    bool FunctionExists(std::string name);
    bool FunctionReturns(std::string name);
    bool HasFunction(std::vector<Token> tokens);
    void RegisterFunction(std::string name, Generic::Function f);

    bool IsBreakPoint(std::string character);
    int GetTokenType(std::string segment);

    bool IsStringInteger(std::string text);
    bool IsStringDecimal(std::string text);
    void PrintTokens(std::vector<Token> tokens);
    void PrintInstructions(std::vector<Instruction> instructions);
    void PrintState();

    Token TokenizeSegment(std::string segment, int index);
    std::vector<Token> TokenizeLine(std::string line);

    bool HasOperation(std::vector<Token> tokens);
    bool HasLogic(std::vector<Token> tokens);
    int FirstOperationIndex(std::vector<Token> tokens, int state);
    int FirstLogicIndex(std::vector<Token> tokens);
    int FirstParenthesesIndex(std::vector<Token> tokens);
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

    void EvaluateLine(std::string line);
    void Evaluate();

};

#endif // SHELL_H