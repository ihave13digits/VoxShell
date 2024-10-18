#include <iostream>
#include "shell.h"

Shell::Shell()
{
    //
}



bool Shell::IsUserEngaged()
{
    return user_engaged;
}

void Shell::SetUserEngaged(bool value)
{
    user_engaged = value;
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



void Shell::DeleteVariable(std::string name)
{
    stack.DeleteVariable(name);
}

void Shell::ClearStack()
{
    stack = Block(0);
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



Token Shell::TokenizeSegment(std::string segment, int index)
{
    int type = GetTokenType(segment);
    return Token(index, type, segment);
}

std::vector<Token> Shell::TokenizeLine(std::string line)
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
    tokens = ParseQuotes(tokens);
    tokens = ParseMath(tokens);
    tokens = ParseEquals(tokens);
    tokens = ParseVariables(tokens);
    tokens = ParseMath(tokens);
    tokens = ParseEquals(tokens);
    tokens = ParseVariables(tokens);
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

/*
std::vector<Instruction> Shell::GenerateInstructions(std::vector<Token> tokens)
{
    std::vector<Instruction> instructions;
    std::string state = "";
    for (int i=0; i<int(tokens.size()); i++)
    {
        bool check_builtins = true;
        std::string value = tokens[i].GetValue();
        if (tokens[i].GetName()!="") { stack.PushVariable(tokens[i]); check_builtins=false; }
        if (check_builtins)
        {
            if (FunctionExists(value)) { state = value; }
        }
        if (value==Syntax::keys[Syntax::SYNTAX_END])
        {
            std::vector<Token> _tokens;
            for (int d=0; d<i; d++)
            {
                if (tokens.at(0).GetValue()!=Syntax::keys[Syntax::SYNTAX_END]) { _tokens.push_back(tokens.at(0)); }
                tokens.erase(tokens.begin());
            }
            if (HasFunction(_tokens))
            {
                instructions.push_back(Instruction(functions[state].GetReturnType(), functions[state].GetArgumentCount(), state, _tokens));
            }
            //PrintTokens(_tokens);
        }
    }
    return instructions;
}
*/
///*
std::vector<Instruction> Shell::GenerateInstructions(std::vector<Token> tokens)
{
    std::vector<Instruction> instructions;
    std::string state = "";
    bool computing = true;
    while (computing)
    {
        //PrintTokens(tokens);
        std::string state = "";
        int call_index=-1, left_index=-1, right_index=-1;
        for (int i=0; i<int(tokens.size()); i++)
        {
            std::string value = tokens.at(i).GetValue();
            //if (tokens[i].GetName()!="") { state = "Variable"; call_index = i; break; }
            //if (tokens[i].GetName()!="" && tokens[i].GetType()==SyntaxType::TYPE_UNKNOWN) { state = "Variable"; call_index = i; break; }
            if (value==Operator::keys[Operator::OPERATOR_PAR_L]) { left_index = i; }
            else if (value==Operator::keys[Operator::OPERATOR_PAR_R]) { right_index = i; break; }
            else if (FunctionExists(value)) { state = "Function"; call_index = i; }
            else if (value==Syntax::keys[Syntax::SYNTAX_END]) { break; }
        }
        /*if (state=="Variable")
        {
            stack.PushVariable(tokens.at(call_index));
            std::string var_name = tokens.at(call_index).GetName();
            tokens.erase(tokens.begin()+call_index);
        }*/
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
            instructions.push_back(Instruction(functions[call_name].GetReturnType(), functions[call_name].GetArgumentCount(), call_name, _tokens));
        }
        else { computing = false; break; }
    }
    //for (int i=0; i<int(instructions.size()); i++) { PrintTokens(instructions[i].GetArguments()); }
    //std::cout <<"END\n"<<std::endl;
    return instructions;
}
//*/

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
    std::cout << "Scope:" << stack.GetSize() << " Blocks:" << stack.GetBlocks().size() << " Variables:" << stack.GetVariables().size() << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
}
