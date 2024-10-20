#include <iostream>
#include "shell.h"

std::vector<Token> Shell::ParseBlocks(std::vector<Token> tokens)
{
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
    std::vector<Token> condensed;
    for (int i=0; i<int(tokens.size()); i++)
    {
        Token token = tokens.at(i);
        if (token.GetType()==SyntaxType::TYPE_UNKNOWN && VariableExists(token.GetValue()))
        {
            condensed.push_back(stack.GetVariable(token.GetValue()));
        }
        else { condensed.push_back(token); }
    }
    return condensed;
}



int Shell::FirstEqualsIndex(std::vector<Token> tokens)
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
    Token v_name=tokens.at(operation_index-1), v_value=tokens.at(operation_index+1), v_type;
    if (operation_index>1) { v_type = tokens.at(operation_index-2); }
    Token solved = Token(v_name.GetIndex(), v_value.GetType(), v_value.GetValue(), SyntaxGlobal::unsolved_problem);
    int value_type=v_value.GetType(), type_type=v_type.GetType();
    int token_index = v_name.GetIndex();
    std::string type_value=v_type.GetValue();
    std::string val_name = v_value.GetValue(), var_name = v_name.GetValue();
    if (type_type==SyntaxType::TYPE_RETURN)
    {
        /*
        if ((type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_BOOLEAN) ||
            (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_INTEGER) ||
            (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_DECIMAL) ||
            (type_value==ReturnType::keys[ReturnType::RETURN_STRING]  && value_type==SyntaxType::TYPE_STRING))
        {
            return Token(token_index, value_type, v_value.GetValue(), var_name);
        }
        
        else */
        // Handle Boolean
        if (type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_BOOLEAN)
        {
            std::string bool_value = v_value.GetValue();
            if      (bool_value==Boolean::alias[0]) { bool_value = Boolean::keys[0]; }
            else if (bool_value==Boolean::alias[1]) { bool_value = Boolean::keys[1]; }
            return Token(token_index, value_type, bool_value, var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_INTEGER)
        {
            std::string bool_value = v_value.GetValue();
            int numeric_value = 0;
            if      (bool_value==Boolean::alias[0]) { numeric_value = 0; }
            else if (bool_value==Boolean::alias[1]) { numeric_value = 1; }
            else if (IsStringInteger(bool_value)) { numeric_value = std::stoi(bool_value); }
            if (numeric_value>1) { numeric_value=1; } else if (numeric_value<0) { numeric_value=0; }
            bool_value=Boolean::keys[numeric_value];
            return Token(token_index, value_type, bool_value, var_name);
        }
        // Handle Integer
        else if (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_INTEGER)
        {
            return Token(token_index, value_type, v_value.GetValue(), var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_DECIMAL)
        {
            return Token(token_index, value_type, std::to_string(int(std::stof(v_value.GetValue()))), var_name);
        }
        // Handle Decimal
        else if (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_DECIMAL)
        {
            return Token(token_index, value_type, v_value.GetValue(), var_name);
        }
        else if (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_INTEGER)
        {
            return Token(token_index, value_type, std::to_string(float(std::stoi(v_value.GetValue()))), var_name);
        }
        // Handle String
        else if (type_value==ReturnType::keys[ReturnType::RETURN_STRING] && value_type==SyntaxType::TYPE_STRING)
        {
            return Token(token_index, value_type, v_value.GetValue(), var_name);
        }
        // Handle Variable (This may need checks)
        else if (value_type==SyntaxType::TYPE_UNKNOWN && stack.VariableNameExists(val_name))
        {
            Token compare = stack.GetVariable(val_name);
            return Token(token_index, compare.GetType(), compare.GetValue(), var_name);
        }
        // Handle Function Returns
        else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
        {
            Instruction instruction = GenerateInstructions(tokens).at(0);
            std::vector<Token> fr = functions[v_value.GetValue()].Call(instruction);
            if (fr.size()>0)
            {
                return Token(token_index, fr.at(0).GetType(), fr.at(0).GetValue(), var_name);
            }
        }
    }
    else
    {
        if (stack.VariableNameExists(var_name))
        {
            Token compare = stack.GetVariable(var_name);
            // Handle.. Wait What?  They Match?!
            if (compare.GetType()==v_value.GetType())
            {
                return Token(token_index, value_type, v_value.GetValue(), var_name);
            }
            // Handle Boolean
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && v_value.GetType()==SyntaxType::TYPE_BOOLEAN)
            {
                std::string bool_value = v_value.GetValue();
                if      (bool_value==Boolean::alias[0]) { bool_value = Boolean::keys[0]; }
                else if (bool_value==Boolean::alias[1]) { bool_value = Boolean::keys[1]; }
                return Token(token_index, compare.GetType(), bool_value, compare.GetName());
            }
            else if (compare.GetType()==SyntaxType::TYPE_BOOLEAN && v_value.GetType()==SyntaxType::TYPE_INTEGER)
            {
                std::string bool_value = v_value.GetValue();
                int numeric_value = 0;
                if      (bool_value==Boolean::alias[0]) { numeric_value = 0; }
                else if (bool_value==Boolean::alias[1]) { numeric_value = 1; }
                else if (IsStringInteger(bool_value)) { numeric_value = std::stoi(bool_value); }
                if (numeric_value>1) { numeric_value=1; } else if (numeric_value<0) { numeric_value=0; }
                bool_value=Boolean::keys[numeric_value];
                return Token(token_index, compare.GetType(), bool_value, var_name);
            }
            // Handle Integer
            else if (compare.GetType()==SyntaxType::TYPE_INTEGER && v_value.GetType()==SyntaxType::TYPE_DECIMAL)
            {
                return Token(token_index, value_type, std::to_string(int(std::stof(v_value.GetValue()))), var_name);
            }
            // Handle Decimal
            else if (compare.GetType()==SyntaxType::TYPE_DECIMAL && v_value.GetType()==SyntaxType::TYPE_INTEGER)
            {
                return Token(token_index, value_type, std::to_string(float(std::stoi(v_value.GetValue()))), var_name);
            }
            // Handle String
            else if (compare.GetType()==SyntaxType::TYPE_STRING && v_value.GetType()==SyntaxType::TYPE_STRING)
            {
                return Token(token_index, compare.GetType(), v_value.GetValue(), var_name);
            }
            // Handle Function Returns
            else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
            {
                Instruction instruction = GenerateInstructions(tokens).at(0);
                std::vector<Token> fr = functions[v_value.GetValue()].Call(instruction);
                if (fr.size()>0)
                {
                    return Token(token_index, fr.at(0).GetType(), fr.at(0).GetValue(), var_name);
                }
            }
            /*
            else
            {
                std::cout<<"EqualsError.  Tokens:"<<std::endl;
                PrintTokens(tokens);
            }
            */
        }
        else
        {
            //TODO: These Are ALL SUS!!!
            // Handle Boolean
            if (value_type==SyntaxType::TYPE_BOOLEAN)
            {
                std::string bool_value = v_value.GetValue();
                if      (bool_value==Boolean::alias[0]) { bool_value = Boolean::keys[0]; }
                else if (bool_value==Boolean::alias[1]) { bool_value = Boolean::keys[1]; }
                return Token(token_index, v_name.GetType(), bool_value, v_name.GetName());
            }
            // Handle Integer
            else if (value_type==SyntaxType::TYPE_INTEGER)
            {
                return Token(token_index, value_type, v_value.GetValue(), v_name.GetName());
            }
            // Handle Decimal
            else if (value_type==SyntaxType::TYPE_DECIMAL)
            {
                return Token(token_index, value_type, v_value.GetValue(), v_name.GetName());
            }
            // Handle String
            else if (value_type==SyntaxType::TYPE_STRING)
            {
                return Token(token_index, value_type, v_value.GetValue(), v_name.GetName());
            }
            // Handle Variable
            else if (value_type==SyntaxType::TYPE_UNKNOWN && stack.VariableNameExists(val_name))
            {
                Token compare = stack.GetVariable(val_name);
                return Token(token_index, compare.GetType(), compare.GetValue(), val_name);
            }
            // Handle Function Returns
            else if (HasFunction(tokens))
            {
                Instruction instruction = GenerateInstructions(tokens).at(0);
                std::vector<Token> fr = functions[v_value.GetValue()].Call(instruction);
                if (fr.size()>0)
                {
                    return Token(token_index, fr.at(0).GetType(), fr.at(0).GetValue(), v_name.GetName());
                }
            }
            /*
            else
            {
                std::cout<<"EqualsError.  Tokens:"<<std::endl;
                PrintTokens(tokens);
            }
            */
        }
    }
    return solved;
}

std::vector<Token> Shell::ParseEquals(std::vector<Token> tokens)
{
    std::vector<Token> condensed = tokens;
    bool computing = true;
    if (condensed.size()>4) { condensed = ParseMath(condensed); }
    while (computing)
    {
        //PrintTokens(condensed);
        int operation_index=FirstEqualsIndex(condensed);
        if (operation_index>-1)
        {
            if (operation_index==2 && operation_index+1<int(condensed.size()))
            {
                Token solved = SolveEquals(operation_index, condensed);
                if (solved.GetValue()!=SyntaxGlobal::unsolved_problem)
                {
                    for (int c=0; c<4; c++)
                    {
                        condensed.erase(condensed.begin()+(operation_index-2));
                    }
                    condensed.insert(condensed.begin()+(operation_index-2), solved);
                }
                else { computing=false; break; }
            }
            else if (operation_index==1 && operation_index+1<int(condensed.size()))
            {
                Token solved = SolveEquals(operation_index, condensed);
                if (solved.GetValue()!=SyntaxGlobal::unsolved_problem)
                {
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
        //PrintTokens(condensed);
    }
    for (int i=0; i<int(condensed.size()); i++)
    {
        std::string name = condensed.at(i).GetName();
        if (name!="" && name!=SyntaxGlobal::unsolved_problem) { stack.PushVariable(condensed.at(i)); }
        /*else if (name==SyntaxGlobal::unsolved_problem)
        {
            std::cout<<"- ERROR START -"<<std::endl;
            PrintTokens(condensed);
            std::cout<<"- ERROR END -"<<std::endl;
            //stack.PushVariable(condensed.at(i));
        }
        */
    }
    //PrintTokens(condensed);
    return condensed;
}