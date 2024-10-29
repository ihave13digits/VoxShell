#include <iostream>
#include "shell.h"

std::vector<Token> Shell::ParseBlocks(std::vector<Token> tokens)
{
    PrintShellCall("ParseBlocks", "");
    std::vector<Token> condensed;
    for (int i=0; i<int(tokens.size()); i++)
    {
        Token token = tokens[i];
        std::string value = token.GetValue();
        if      (value==Syntax::keys[Syntax::SYNTAX_BLOCK_L])
        {
            current_scope++;
        }
        else if (value==Syntax::keys[Syntax::SYNTAX_BLOCK_R])
        {
            current_scope--;
        }
        else
        {
            condensed.push_back(tokens.at(i));
        }
    }
    return condensed;
}

std::vector<Token> Shell::ParseQuotes(std::vector<Token> tokens)
{
    PrintShellCall("ParseQuotes", "");
    std::vector<Token> condensed;
    for (int i=0; i<int(tokens.size()); i++)
    {
        Token token = tokens[i];
        std::string value = token.GetValue();
        if (value==Syntax::keys[Syntax::SYNTAX_QUOTES])
        {
            std::string data = "";
            bool searching = true;
            i++;
            while (searching)
            {
                std::string _value = tokens[i].GetValue();
                if (i<int(tokens.size()))
                {
                    if (_value!=Syntax::keys[Syntax::SYNTAX_QUOTES]) { data+=_value; }
                    else { searching = false; break; }
                }
                if (i>=int(tokens.size())) { searching = false; break; }
                tokens.erase(tokens.begin()+i);
            }
            token.SetValue(data); token.SetType(SyntaxType::TYPE_STRING);
        }
        if (value!=Syntax::keys[Syntax::SYNTAX_SPACE]) { condensed.push_back(token); }
    }
    return condensed;
}

std::vector<Token> Shell::ParseVariables(std::vector<Token> tokens)
{
    PrintShellCall("ParseVariables", "");
    std::vector<Token> condensed;
    for (int i=0; i<int(tokens.size()); i++)
    {
        Token token = tokens.at(i);
        if (token.GetType()==SyntaxType::TYPE_UNKNOWN && VariableExists(token.GetValue()))
        {
            condensed.push_back(stack.GetVariable(token.GetValue(), current_scope));
        }
        else { condensed.push_back(token); }
    }
    return condensed;
}



inline int Shell::FirstEqualsIndex(std::vector<Token> tokens)
{
    int operation_index=-1;
    for (int i=0; i<int(tokens.size()); i++)
    {
        if (tokens[i].GetValue()==Operator::keys[Operator::OPERATOR_SET] && tokens[i].GetType()!=SyntaxType::TYPE_STRING) return i;
    }
    return operation_index;
}

Token Shell::SolveEquals(int operation_index, std::vector<Token> tokens)
{
    PrintShellCall("SolveEquals", "");
    Token v_name=tokens.at(operation_index-1), v_value=tokens.at(operation_index+1), v_type;
    if (operation_index>1) { v_type = tokens.at(operation_index-2); }
    int value_type=v_value.GetType(), type_type=v_type.GetType();
    int token_index = v_name.GetIndex();
    std::string type_value=v_type.GetValue();
    std::string val_name = v_value.GetValue(), var_name = v_name.GetValue(), _var_name = v_name.GetName();
    Token solved = Token(token_index, value_type, val_name, SyntaxGlobal::unsolved_problem);
    if (type_type==SyntaxType::TYPE_RETURN && !stack.VariableNameExists(var_name, current_scope) && !stack.VariableNameExists(_var_name, current_scope))
    {
        // Handle Boolean
        if (type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_BOOLEAN)
        {
            std::string bool_value = val_name;
            if      (bool_value==Boolean::alias[0]) { bool_value = Boolean::keys[0]; }
            else if (bool_value==Boolean::alias[1]) { bool_value = Boolean::keys[1]; }
            return Token(token_index, value_type, bool_value, var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_INTEGER)
        {
            std::string bool_value = val_name;
            int numeric_value = 0;
            if      (bool_value==Boolean::alias[0]) { numeric_value = 0; }
            else if (bool_value==Boolean::alias[1]) { numeric_value = 1; }
            else if (IsStringInteger(bool_value)) { numeric_value = std::stoi(bool_value); }
            if (numeric_value>1) { numeric_value=1; } else if (numeric_value<0) { numeric_value=0; }
            bool_value=Boolean::keys[numeric_value];
            return Token(token_index, value_type, bool_value, var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_DECIMAL)
        {
            PrintShellError("Can't Assign Decimal Value To Boolean Type");
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_STRING)
        {
            PrintShellError("Can't Assign String Value To Boolean Type");
        }
        // Handle Integer
        else if (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_INTEGER)
        {
            return Token(token_index, value_type, val_name, var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_DECIMAL)
        {
            return Token(token_index, SyntaxType::TYPE_INTEGER, std::to_string(int(std::stof(val_name))), var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_BOOLEAN)
        {
            PrintShellError("Can't Assign Boolean Value To Integer Type");
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_STRING)
        {
            PrintShellError("Can't Assign String Value To Integer Type");
        }
        // Handle Decimal
        else if (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_DECIMAL)
        {
            return Token(token_index, value_type, val_name, var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_INTEGER)
        {
            return Token(token_index, SyntaxType::TYPE_DECIMAL, std::to_string(float(std::stoi(val_name))), var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_BOOLEAN)
        {
            PrintShellError("Can't Assign Boolean Value To Float Type");
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_STRING)
        {
            PrintShellError("Can't Assign String Value To Float Type");
        }
        // Handle String
        else if (type_value==ReturnType::keys[ReturnType::RETURN_STRING] && value_type==SyntaxType::TYPE_STRING)
        {
            return Token(token_index, value_type, val_name, var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_STRING] && value_type==SyntaxType::TYPE_BOOLEAN)
        {
            PrintShellError("Can't Assign Boolean Value To String Type");
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_STRING] && value_type==SyntaxType::TYPE_INTEGER)
        {
            PrintShellError("Can't Assign Integer Value To String Type");
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_STRING] && value_type==SyntaxType::TYPE_DECIMAL)
        {
            PrintShellError("Can't Assign Float Value To String Type");
        }
        // Handle Variable (This may need checks)
        else if (value_type==SyntaxType::TYPE_UNKNOWN && stack.VariableNameExists(val_name, current_scope))
        {
            Token compare = stack.GetVariable(val_name, current_scope);
            return Token(token_index, compare.GetType(), compare.GetValue(), var_name);
        }
        // Handle Function Returns
        else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
        {
            Instruction instruction = GenerateInstructions(tokens).at(0);
            std::vector<Token> fr = functions[val_name].Call(instruction);
            if (fr.size()>0)
            {
                return Token(token_index, fr.at(0).GetType(), fr.at(0).GetValue(), var_name);
            }
        }
    }
    else
    {
        if (stack.VariableNameExists(var_name, current_scope) && tokens.at(0).GetType()!=SyntaxType::TYPE_RETURN)
        {
            Token compare = stack.GetVariable(var_name, current_scope);
            // Handle.. Wait What?  They Match?!
            if (compare.GetType()==v_value.GetType())
            {
                return Token(token_index, value_type, val_name, var_name);
            }
            // Handle Boolean
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                std::string bool_value = val_name;
                if      (bool_value==Boolean::alias[0]) { bool_value = Boolean::keys[0]; }
                else if (bool_value==Boolean::alias[1]) { bool_value = Boolean::keys[1]; }
                return Token(token_index, compare.GetType(), bool_value, compare.GetName());
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_INTEGER)
            {
                std::string bool_value = val_name;
                int numeric_value = 0;
                if      (bool_value==Boolean::alias[0]) { numeric_value = 0; }
                else if (bool_value==Boolean::alias[1]) { numeric_value = 1; }
                else if (IsStringInteger(bool_value)) { numeric_value = std::stoi(bool_value); }
                if (numeric_value>1) { numeric_value=1; } else if (numeric_value<0) { numeric_value=0; }
                bool_value=Boolean::keys[numeric_value];
                return Token(token_index, compare.GetType(), bool_value, var_name);
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_DECIMAL)
            {
                PrintShellError("Can't Assign Float Value To Boolean Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_STRING)
            {
                PrintShellError("Can't Assign String Value To Boolean Type");
            }
            // Handle Integer
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && value_type==SyntaxType::TYPE_DECIMAL)
            {
                return Token(token_index, value_type, std::to_string(int(std::stof(val_name))), var_name);
            }
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                PrintShellError("Can't Assign Boolean Value To Integer Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && value_type==SyntaxType::TYPE_STRING)
            {
                PrintShellError("Can't Assign String Value To Integer Type");
            }
            // Handle Decimal
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && value_type==SyntaxType::TYPE_INTEGER)
            {
                return Token(token_index, value_type, std::to_string(float(std::stoi(val_name))), var_name);
            }
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                PrintShellError("Can't Assign Boolean Value To Float Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && value_type==SyntaxType::TYPE_STRING)
            {
                PrintShellError("Can't Assign String Value To Float Type");
            }
            // Handle String
            else if (compare.GetType()==SyntaxType::TYPE_STRING && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                PrintShellError("Can't Assign Boolean Value To String Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_STRING && value_type==SyntaxType::TYPE_INTEGER)
            {
                PrintShellError("Can't Assign Float Value To String Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_STRING && value_type==SyntaxType::TYPE_DECIMAL)
            {
                PrintShellError("Can't Assign Float Value To String Type");
            }
            // Handle Function Returns
            else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
            {
                Instruction instruction = GenerateInstructions(tokens).at(0);
                std::vector<Token> fr = functions[val_name].Call(instruction);
                if (fr.size()>0)
                {
                    return Token(token_index, fr.at(0).GetType(), fr.at(0).GetValue(), var_name);
                }
            }
            else
            {
                PrintShellWarning("Couldn't Parse Variable");
            }
        }
        else if (stack.VariableNameExists(_var_name, current_scope) && tokens.at(0).GetType()!=SyntaxType::TYPE_RETURN)
        {
            Token compare = stack.GetVariable(_var_name, current_scope);
            // Handle.. Wait What?  They Match?!
            if (compare.GetType()==v_value.GetType())
            {
                return Token(token_index, value_type, val_name, _var_name);
            }
            // Handle Boolean
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                std::string bool_value = val_name;
                if      (bool_value==Boolean::alias[0]) { bool_value = Boolean::keys[0]; }
                else if (bool_value==Boolean::alias[1]) { bool_value = Boolean::keys[1]; }
                return Token(token_index, compare.GetType(), bool_value, compare.GetName());
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_INTEGER)
            {
                std::string bool_value = val_name;
                int numeric_value = 0;
                if      (bool_value==Boolean::alias[0]) { numeric_value = 0; }
                else if (bool_value==Boolean::alias[1]) { numeric_value = 1; }
                else if (IsStringInteger(bool_value)) { numeric_value = std::stoi(bool_value); }
                if (numeric_value>1) { numeric_value=1; } else if (numeric_value<0) { numeric_value=0; }
                bool_value=Boolean::keys[numeric_value];
                return Token(token_index, compare.GetType(), bool_value, _var_name);
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_DECIMAL)
            {
                PrintShellError("Can't Assign Float Value To Boolean Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && value_type==SyntaxType::TYPE_STRING)
            {
                PrintShellError("Can't Assign String Value To Boolean Type");
            }
            // Handle Integer
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && value_type==SyntaxType::TYPE_DECIMAL)
            {
                return Token(token_index, SyntaxType::TYPE_INTEGER, std::to_string(int(std::stof(val_name))), _var_name);
            }
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                PrintShellError("Can't Assign Boolean Value To Integer Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && value_type==SyntaxType::TYPE_STRING)
            {
                PrintShellError("Can't Assign String Value To Integer Type");
            }
            // Handle Decimal
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && value_type==SyntaxType::TYPE_INTEGER)
            {
                return Token(token_index, SyntaxType::TYPE_DECIMAL, std::to_string(float(std::stoi(val_name))), _var_name);
            }
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                PrintShellError("Can't Assign Boolean Value To Float Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && value_type==SyntaxType::TYPE_STRING)
            {
                PrintShellError("Can't Assign String Value To Float Type");
            }
            // Handle String
            else if (compare.GetType()==SyntaxType::TYPE_STRING && value_type==SyntaxType::TYPE_BOOLEAN)
            {
                PrintShellError("Can't Assign Boolean Value To String Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_STRING && value_type==SyntaxType::TYPE_INTEGER)
            {
                PrintShellError("Can't Assign Float Value To String Type");
            }
            else if (compare.GetType()==SyntaxType::TYPE_STRING && value_type==SyntaxType::TYPE_DECIMAL)
            {
                PrintShellError("Can't Assign Float Value To String Type");
            }
            // Handle Function Returns
            else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
            {
                Instruction instruction = GenerateInstructions(tokens).at(0);
                std::vector<Token> fr = functions[val_name].Call(instruction);
                if (fr.size()>0)
                {
                    return Token(token_index, fr.at(0).GetType(), fr.at(0).GetValue(), _var_name);
                }
            }
            else
            {
                PrintShellWarning("Couldn't Parse Variable");
            }
        }
        else
        {
            PrintShellError("Variable Already Exists");
        }
    }
    return solved;
}

std::vector<Token> Shell::ParseEquals(std::vector<Token> tokens)
{
    PrintShellCall("ParseEquals", "");
    std::vector<Token> condensed = tokens;
    int operation_index=FirstEqualsIndex(tokens);
    bool has_operation = (operation_index>-1); if (!has_operation) { return tokens; }
    bool computing = has_operation;
    if (condensed.size()>4) { condensed = ParseMath(condensed); }
    while (computing)
    {
        int operation_index=FirstEqualsIndex(condensed);
        if (operation_index>-1)
        {
            // Initially Setting The Variable
            if (operation_index==2 && operation_index+1<int(condensed.size()))
            {
                Token solved = SolveEquals(operation_index, condensed);
                if (solved.GetValue()!=SyntaxGlobal::unsolved_problem)
                {
                    // Replace Statement With Token (type new_variable = value)
                    for (int c=0; c<4; c++)
                    {
                        condensed.erase(condensed.begin()+(operation_index-2));
                    }
                    condensed.insert(condensed.begin()+(operation_index-2), solved);
                }
                else { computing=false; break; }
            }
            // Resetting The Variable
            else if (operation_index==1 && operation_index+1<int(condensed.size()))
            {
                Token solved = SolveEquals(operation_index, condensed);
                if (solved.GetValue()!=SyntaxGlobal::unsolved_problem)
                {
                    // Replace Statement With Token (existing_variable = value)
                    for (int c=0; c<3; c++)
                    {
                        condensed.erase(condensed.begin()+(operation_index-1));
                    }
                    condensed.insert(condensed.begin()+(operation_index-1), solved);
                }
                else { computing=false; break; }
            }
        }
        else { computing=false; break; }
    }
    // Finally, We Push The Variable/s
    if (has_operation)
    {
        for (int i=0; i<int(condensed.size()); i++)
        {
            std::string name = condensed.at(i).GetName();
            if (name!="" && name!=SyntaxGlobal::unsolved_problem)
            {
                PushVariable(condensed.at(i));
            }
        }
    }
    return condensed;
}



Token Shell::TokenizeSegment(std::string segment, int index)
{
    //PrintShellCall("TokenizeSegment", segment+" ("+std::to_string(index)+")");
    int type = GetTokenType(segment);
    return Token(index, type, segment);
}

std::vector<Token> Shell::TokenizeLine(std::string line)
{
    PrintShellCall("TokenizeLine", line);
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
    return tokens;
}



void Shell::ParseLine(std::string line)
{
    PrintShellCall("ParseLine", line);
    if (line.size()==0) { return; }
    std::vector<Token> tokens = TokenizeLine(line);
    std::vector<Instruction> instructions = GenerateInstructions(tokens);
    for (int i=0; i<int(instructions.size()); i++)
    {
        stack.PushBack(instructions[i], current_scope);
    }
}

void Shell::ParseScript(Script script)
{
    PrintShellCall("ParseScript", script.GetName());
    std::vector<Token> tokens;
    std::vector<std::string> lines = script.GetLines();
    for (int i=0; i<int(lines.size()); i++)
    {
        if (stack.GetSize(current_scope)<stack_limit)
        {
            std::string line = lines.at(i);
            if (line.size()>0) { ParseLine(line); }
        }
        else { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_STACK_LIMIT_REACHED] << std::endl; break; }
    }
}

std::vector<Instruction> Shell::GenerateInstructions(std::vector<Token> tokens)
{
    PrintShellCall("GenerateInstructions", "");
    //tokens = ParseBlocks(tokens);
    tokens = ParseQuotes(tokens);
    tokens = ParseVariables(tokens);
    std::vector<Instruction> instructions;
    std::string state = "";
    bool computing = true;
    while (computing)
    {
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
                if (FirstEqualsIndex(_tokens)>-1) { _tokens = ParseEquals(_tokens); }
                if (HasOperation(_tokens)) { _tokens = ParseMath(_tokens); }
                if (int(_tokens.size())==S) { break; }
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
    }
    while(int(tokens.size())>1)
    {
        const int S = tokens.size();
        if (FirstEqualsIndex(tokens)>-1) { tokens = ParseEquals(tokens); }
        if (HasOperation(tokens)) { tokens = ParseMath(tokens); }
        if (int(tokens.size())==S) { break; }
    }
    return instructions;
}

void Shell::Evaluate(std::string line)
{
    PrintShellCall("Evaluate", line);
    if (line.size()==0) { return; }
    int callback_count = 0;
    std::string separator = GetColorString("----------------------------------------------------------------", 0,128,128);
    std::cout << separator << std::endl;
    ParseLine(line);
    while (stack.GetSize(current_scope)>0)
    {
        if (!IsUserEngaged()) { ClearStack(); break; }
        Instruction instruction = stack.GetNextInstruction(current_scope);
        //PrintTokens(instruction.GetArguments());
        functions[instruction.GetState()].Call(instruction);
        stack.PopFront(current_scope);
        if (callback_count>stack_limit) { std::cout << SyntaxType::keys[SyntaxType::TYPE_ERROR_STACK_LIMIT_REACHED] << std::endl; break; }
        callback_count++;
    }
    std::cout << separator << std::endl;
    PrintState();
    std::cout << separator << std::endl;
}