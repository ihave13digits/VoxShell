#include <iostream>
#include "shell.h"

Shell::Shell()
{
    //
}



inline bool Shell::IsUserEngaged()
{
    return user_engaged;
}

void Shell::SetUserEngaged(bool value)
{
    user_engaged = value;
}



bool Shell::VariableExists(std::string name)
{
    return (stack.VariableNameExists(name));
}

void Shell::DeleteVariable(std::string name)
{
    stack.DeleteVariable(name);
}

Token Shell::GetVariable(std::string name)
{
    return stack.GetVariable(name);
}

void Shell::SetVariable(std::string name, Token value)
{
    stack.SetVariable(name, value);
}

void Shell::PushVariable(Token value)
{
    stack.PushVariable(value);
}

void Shell::ClearStack()
{
    stack = Block(0, 0);
}

Block Shell::GetStack()
{
    return stack;
}

int Shell::GetStackLimit()
{
    return stack_limit;
}

void Shell::SetStackLimit(int _stack_limit)
{
    stack_limit = _stack_limit;
}



int Shell::GetScope()
{
    return current_scope;
}

void Shell::SetScope(int _scope)
{
    current_scope = _scope;
}



bool Shell::FunctionExists(std::string name)
{
    return (functions.count(name)>0);
}

bool Shell::FunctionReturns(std::string name)
{
    return (functions.at(name).GetReturnType()!=ReturnType::RETURN_VOID);
}

bool Shell::HasFunction(std::vector<Token> tokens)
{
    for (int i=0; i<int(tokens.size()); i++)
    {
        if (functions.count(tokens.at(i).GetValue())>0) { return true; }
    }
    return false;
}

void Shell::RegisterFunction(std::string name, Generic::Function f)
{
    if (functions.count(name)==0) { functions.emplace(name, f); }
    else { std::cout << SyntaxGlobal::function_exists << std::endl; }
}



bool Shell::IsBreakPoint(std::string character)
{
    for (int k=0; k<int(Syntax::keys.size()); k++)
    {
        if (character==Syntax::keys[k])
        {
            return true;
        }
    }
    for (int k=0; k<int(Operator::keys.size()); k++)
    {
        if (character==Operator::keys[k])
        {
            return true;
        }
    }
    return false;
}

int Shell::GetTokenType(std::string segment)
{
    if (segment.size()==0)
    {
        return SyntaxType::TYPE_INVALID;
    }
    if (segment.size()==1)
    {
        for (int k=0; k<int(Syntax::keys.size()); k++)
        {
            if (segment==Syntax::keys[k])
            {
                return SyntaxType::TYPE_SYNTAX;
            }
        }
        for (int k=0; k<int(Operator::keys.size()); k++)
        {
            if (segment==Operator::keys[k])
            {
                return SyntaxType::TYPE_OPERATOR;
            }
        }
    }
    if (FunctionExists(segment))
    {
        return SyntaxType::TYPE_BUILT_IN;
    }
    for (int k=0; k<int(ReturnType::keys.size()); k++)
    {
        if (segment==ReturnType::keys[k])
        {
            return SyntaxType::TYPE_RETURN;
        }
    }
    if      (IsStringInteger(segment)) { return SyntaxType::TYPE_INTEGER; }
    else if (IsStringDecimal(segment)) { return SyntaxType::TYPE_DECIMAL; }
    for (int k=0; k<int(Boolean::keys.size()); k++)
    {
        if (segment==Boolean::keys[k])
        {
            return SyntaxType::TYPE_BOOLEAN;
        }
        else if (segment==Boolean::alias[k])
        {
            return SyntaxType::TYPE_BOOLEAN;
        }
    }
    return SyntaxType::TYPE_UNKNOWN;
}



bool Shell::IsStringInteger(std::string text)
{
    bool is_valid = true;
    if (text.size()>1 && text.at(0)=='0') { return false; }
    for (int i=0; i<int(text.size()); i++)
    {
        char c = text.at(i);
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return is_valid;
}

bool Shell::IsStringDecimal(std::string text)
{
    bool is_valid = true;
    int decimal_count = 0;
    int decimal_index = text.size();
    for (int i=0; i<int(text.size()); i++)
    {
        char c = text.at(i);
        if (!std::isdigit(c))
        {
            if (i==0 && c=='-')
            {
                
            }
            else if (c=='.')
            {
                decimal_count++;
                if (decimal_count>1) { return false; }
                else { decimal_index = i; }
            }
            else
            {
                return false;
            }
        }
    }
    if (text.size()>1 && text.at(0)=='0' && decimal_index!=1) { return false; }
    return is_valid;
}



void Shell::PrintTokens(std::vector<Token> tokens)
{
    //std::cout << "----------------------------------------------------------------" << std::endl;
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::cout << "(" << tokens[i].GetValue() << ") Type[";
        std::cout << SyntaxType::keys[tokens[i].GetType()] << "] Name[";
        std::cout << tokens[i].GetName() << "] @ Index:";
        std::cout << tokens[i].GetIndex() << std::endl;
    }
    std::cout << std::endl;
    //std::cout << "----------------------------------------------------------------" << std::endl;
}

void Shell::PrintState()
{
    std::cout << "Scope:" << current_scope;
    std::cout << " Stack:" << stack.GetSize();
    std::cout << " Blocks:" << stack.GetBlocks().size();
    std::cout << " Variables:"<< stack.GetVariables().size();
    std::cout << std::endl;
}



inline Token Shell::TokenizeSegment(std::string segment, int index)
{
    int type = GetTokenType(segment);
    return Token(index, type, segment);
}

inline std::vector<Token> Shell::TokenizeLine(std::string line)
{
    std::vector<Token> tokens;
    std::string segment = "";
    int index = 0;
    if (line.substr(line.size()-1, 1)!=Syntax::keys[Syntax::SYNTAX_END]) { line = line+Syntax::keys[Syntax::SYNTAX_END]; }// Ensures All Data Is Gathered
    for (int i=0; i<int(line.size()); i++)
    {
        std::string character = line.substr(i, 1);
        bool break_point = IsBreakPoint(character);
        // Syntax Told It To Pop Segment
        if (break_point)
        {
            Token token = TokenizeSegment(segment, index);
            if (token.GetType()!=SyntaxType::TYPE_INVALID) { tokens.push_back(token); }
            segment = "";
            // Gather Break Key As Separate Token
            segment += character;
            Token _token = TokenizeSegment(segment, i);
            if (_token.GetType()!=SyntaxType::TYPE_INVALID) { tokens.push_back(_token); }
            segment = "";
            index = i+1;
        }
        else { segment += character; }
    }
    //PrintTokens(tokens);
    return tokens;
}



void Shell::ParseLine(std::string line)
{
    if (line.size()==0) { return; }
    std::vector<Token> tokens = TokenizeLine(line);
    std::vector<Instruction> instructions = GenerateInstructions(tokens);
    for (int i=0; i<int(instructions.size()); i++)
    {
        stack.PushBack(instructions[i]);
    }
}

void Shell::ParseScript(Script script)
{
    std::vector<Token> tokens;
    std::vector<std::string> lines = script.GetLines();
    for (int i=0; i<int(lines.size()); i++)
    {
        if (stack.GetSize()<stack_limit)
        {
            ParseLine(lines[i]);
        }
        else { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_STACK_LIMIT_REACHED] << std::endl; break; }
    }
}

std::vector<Instruction> Shell::GenerateInstructions(std::vector<Token> tokens)
{
    //std::cout <<"FirstPass:"<<std::endl;
    //PrintState();
    //PrintTokens(tokens);
    tokens = ParseBlocks(tokens);
    tokens = ParseQuotes(tokens);
    tokens = ParseVariables(tokens);
    std::vector<Instruction> instructions;
    std::string state = "";
    bool computing = true;
    //std::cout <<"SecondPass:"<<std::endl;
    while (computing)
    {
        //PrintTokens(tokens);
        std::string state = "";
        int call_index=-1, left_index=-1, right_index=-1;
        for (int i=0; i<int(tokens.size()); i++)
        {
            std::string value = tokens.at(i).GetValue();
            if (value==Operator::keys[Operator::OPERATOR_PAR_L]) { left_index = i; }
            else if (value==Operator::keys[Operator::OPERATOR_PAR_R]) { right_index = i; break; }
            else if (FunctionExists(value)) { state = "Function"; call_index = i; }
            else if (value==Syntax::keys[Syntax::SYNTAX_END]) { break; }
        }
        if (state=="Function" && (call_index>-1 && left_index>call_index && right_index>left_index))
        {
            std::vector<Token> _tokens;
            std::string call_name = tokens.at(call_index).GetValue();
            for (int i=call_index; i<right_index; i++)
            {
                std::string value = tokens.at(left_index).GetValue();
                if (value==Operator::keys[Syntax::SYNTAX_COMMA])
                {
                    _tokens.push_back(tokens.at(left_index));
                }
                if (value!=Operator::keys[Operator::OPERATOR_PAR_L] && value!=Operator::keys[Operator::OPERATOR_PAR_R])
                {
                    _tokens.push_back(tokens.at(left_index));
                }
                tokens.erase(tokens.begin()+call_index);
            }
            tokens.erase(tokens.begin()+call_index);
            int argc = functions[call_name].GetArgumentCount();
            while(int(_tokens.size())>argc)
            {
                const int S = _tokens.size();
                _tokens = ParseEquals(_tokens); //if (int(_tokens.size())==S) { break; }
                _tokens = ParseMath(_tokens); if (int(_tokens.size())==S) { break; }
            }
            if (functions[call_name].GetReturnType()!=ReturnType::RETURN_VOID)
            {
                Instruction instruction = Instruction(functions[call_name].GetReturnType(), functions[call_name].GetArgumentCount(), call_name, _tokens);
                tokens.insert(tokens.begin()+call_index, functions[call_name].Call(instruction).at(0));
            }
            else
            {
                instructions.push_back(Instruction(functions[call_name].GetReturnType(), functions[call_name].GetArgumentCount(), call_name, _tokens));
            }
        }
        else
        {
            computing = false; break;
        }
        //PrintTokens(tokens);
    }
    //std::cout <<"Residual:"<<std::endl; PrintTokens(tokens);
    while(int(tokens.size())>1)
    {
        const int S = tokens.size();
        tokens = ParseEquals(tokens);
        tokens = ParseMath(tokens);
        if (int(tokens.size())==S) { break; }
    }
    return instructions;
}

void Shell::Evaluate(std::string line)
{
    int callback_count = 0;
    std::cout << "----------------------------------------------------------------" << std::endl;
    ParseLine(line);
    while (stack.GetSize()>0)
    {
        if (!IsUserEngaged()) { ClearStack(); break; }
        Instruction instruction = stack.GetNextInstruction();
        //PrintTokens(instruction.GetArguments());
        functions[instruction.GetState()].Call(instruction);
        stack.PopFront();
        if (callback_count>stack_limit) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_STACK_LIMIT_REACHED] << std::endl; break; }
        callback_count++;
    }
    std::cout << "----------------------------------------------------------------" << std::endl;
    PrintState();
    std::cout << "----------------------------------------------------------------" << std::endl;
}
