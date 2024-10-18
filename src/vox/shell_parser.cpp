#include <iostream>
#include "shell.h"

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
        if (token.GetType()==SyntaxType::TYPE_UNKNOWN && stack.VariableNameExists(token.GetValue()))
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
        if (tokens[i].GetValue()==Operator::keys[Operator::OPERATOR_SET]) return i;
    }
    return operation_index;
}

Token Shell::SolveEquals(int operation_index, std::vector<Token> tokens)
{
    //PrintTokens(tokens);
    Token v_name=tokens.at(operation_index-1), v_value=tokens.at(operation_index+1), v_type;
    if (operation_index>1) { v_type = tokens.at(operation_index-2); }
    Token solved = Token(v_name.GetIndex(), v_value.GetType(), v_value.GetValue(), SyntaxGlobal::unsolved_problem);
    int value_type=v_value.GetType(), type_type=v_type.GetType();
    std::string type_value=v_type.GetValue();
    if (type_type==SyntaxType::TYPE_RETURN)
    {
        if ((type_value==ReturnType::keys[ReturnType::RETURN_BOOLEAN] && value_type==SyntaxType::TYPE_BOOLEAN) ||
            (type_value==ReturnType::keys[ReturnType::RETURN_INTEGER] && value_type==SyntaxType::TYPE_INTEGER) ||
            (type_value==ReturnType::keys[ReturnType::RETURN_DECIMAL] && value_type==SyntaxType::TYPE_DECIMAL) ||
            (type_value==ReturnType::keys[ReturnType::RETURN_STRING] && value_type==SyntaxType::TYPE_STRING))
        {
            return Token(v_name.GetIndex(), value_type, v_value.GetValue(), v_name.GetValue());
        }
        else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
        {
            Instruction instruction = GenerateInstructions(tokens).at(0);
            std::vector<Token> fr = functions[v_value.GetValue()].Call(instruction);
            //PrintTokens(fr);
            if (fr.size()>0)
            {
                return Token(v_name.GetIndex(), fr.at(0).GetType(), fr.at(0).GetValue(), v_name.GetValue());
            }
        }
    }
    else
    {
        std::string var_name = v_name.GetValue();
        if (stack.VariableNameExists(var_name))
        {
            Token compare = stack.GetVariable(var_name);
            if (compare.GetType()==v_value.GetType())
            {
                return Token(v_name.GetIndex(), value_type, v_value.GetValue(), v_name.GetValue());
            }
            else if (value_type==SyntaxType::TYPE_BUILT_IN && HasFunction(tokens))
            {
                Instruction instruction = GenerateInstructions(tokens).at(0);
                std::vector<Token> fr = functions[v_value.GetValue()].Call(instruction);
                //PrintTokens(fr);
                if (fr.size()>0)
                {
                    return Token(v_name.GetIndex(), fr.at(0).GetType(), fr.at(0).GetValue(), v_name.GetValue());
                }
            }
        }
    }
    return solved;
}

std::vector<Token> Shell::ParseEquals(std::vector<Token> tokens)
{
    std::vector<Token> condensed = tokens;
    bool computing = true;
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
        if (condensed.at(i).GetName()!="") { stack.PushVariable(condensed.at(i)); }
    }
    //PrintTokens(condensed);
    return condensed;
}