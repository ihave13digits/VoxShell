#include <iostream>
#include <cmath>
#include "shell.h"

bool Shell::HasOperation(std::vector<Token> tokens)
{
    // Loop Through Tokens
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::string value = tokens[i].GetValue();
        // Check For Operator
        if (value==Operator::keys[Operator::OPERATOR_ADD] ||
            value==Operator::keys[Operator::OPERATOR_SUB] ||
            value==Operator::keys[Operator::OPERATOR_MUL] ||
            value==Operator::keys[Operator::OPERATOR_DIV] ||
            value==Operator::keys[Operator::OPERATOR_MOD] ||
            value==Operator::keys[Operator::OPERATOR_POW])
        { return true; }
    }
    return HasLogic(tokens);
}

inline bool Shell::HasLogic(std::vector<Token> tokens)
{
    // Loop Through Tokens
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::string value = tokens[i].GetValue();
        // Check For Operator
        if (value==Operator::keys[Operator::OPERATOR_SET] ||
            value==Operator::keys[Operator::OPERATOR_NOT] ||
            value==Operator::keys[Operator::OPERATOR_LESSER] ||
            value==Operator::keys[Operator::OPERATOR_GREATER] ||
            value==Operator::keys[Operator::OPERATOR_LOGIC_AND] ||
            value==Operator::keys[Operator::OPERATOR_LOGIC_OR] ||
            value==Operator::keys[Operator::OPERATOR_AND] ||
            value==Operator::keys[Operator::OPERATOR_OR] ||
            value==Operator::keys[Operator::OPERATOR_LESSER_EQUAL] ||
            value==Operator::keys[Operator::OPERATOR_GREATER_EQUAL] ||
            value==Operator::keys[Operator::OPERATOR_NOT_EQUAL] ||
            value==Operator::keys[Operator::OPERATOR_EQUALS])
        { return true; }
    }
    return false;
}

inline int Shell::FirstOperationIndex(std::vector<Token> tokens, int state)
{
    if (state==MathState::MATH_LOG) { return FirstLogicIndex(tokens); }
    int operation_index=-1;
    // Loop Through Tokens
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::string value = tokens[i].GetValue();
        bool is_op=tokens[i].GetType()==SyntaxType::TYPE_OPERATOR, L=false, R=false;
        if (i>0 && is_op) { L=CanComputeToken(tokens[i-1]); } if (i<int(tokens.size()-1) && is_op) { R=CanComputeToken(tokens[i+1]); }
        // Check If It's A Valid Problem
        if ((L && R) || R)
        {
            // Check Addition / Subtraction
            if      (state==MathState::MATH_A_S &&
                (value==Operator::keys[Operator::OPERATOR_ADD] ||
                value==Operator::keys[Operator::OPERATOR_SUB]))
            { operation_index = i; break; }
            // Check Multiplication / Division / Modulo
            else if (state==MathState::MATH_M_D &&
                ((value==Operator::keys[Operator::OPERATOR_MUL] ||
                value==Operator::keys[Operator::OPERATOR_DIV]) ||
                value==Operator::keys[Operator::OPERATOR_MOD]))
            { operation_index = i; break; }
            // Check Exponents
            else if (state==MathState::MATH_POW &&
                (value==Operator::keys[Operator::OPERATOR_POW]))
            { operation_index = i; break; }
            // Check Logic
            else if (state>MathState::MATH_A_S &&
                (value==Operator::keys[Operator::OPERATOR_SET] ||
                value==Operator::keys[Operator::OPERATOR_NOT] ||
                value==Operator::keys[Operator::OPERATOR_LESSER] ||
                value==Operator::keys[Operator::OPERATOR_GREATER] ||
                value==Operator::keys[Operator::OPERATOR_LOGIC_AND] ||
                value==Operator::keys[Operator::OPERATOR_LOGIC_OR] ||
                value==Operator::keys[Operator::OPERATOR_AND] ||
                value==Operator::keys[Operator::OPERATOR_OR] ||
                value==Operator::keys[Operator::OPERATOR_LESSER_EQUAL] ||
                value==Operator::keys[Operator::OPERATOR_GREATER_EQUAL] ||
                value==Operator::keys[Operator::OPERATOR_NOT_EQUAL] ||
                value==Operator::keys[Operator::OPERATOR_EQUALS]))
            { operation_index = i; break; }
        }
    }
    return operation_index;
}

inline int Shell::FirstLogicIndex(std::vector<Token> tokens)
{
    int operation_index=-1;
    // Loop Through Tokens
    for (int i=0; i<int(tokens.size()); i++)
    {
        std::string value = tokens[i].GetValue();
        int L=false, R=false, _R=false;
        if (i>0) { L=CanComputeToken(tokens[i-1]); }
        if (i<int(tokens.size()-1))
        {
            Token right_token = tokens.at(i+1);
            R=CanComputeToken(right_token);
            // Check For (<=, >=, !=, ||, &&)
            _R=(right_token.GetValue()==Operator::keys[Operator::OPERATOR_SET] ||
                right_token.GetValue()==Operator::keys[Operator::OPERATOR_LOGIC_AND] ||
                right_token.GetValue()==Operator::keys[Operator::OPERATOR_LOGIC_OR]);
        }
        // Check For Operator Keys
        for (int o=Operator::OPERATOR_SET; o<=Operator::OPERATOR_EQUALS; o++)
        {
            if (value==Operator::keys[o] && ((L && R) || _R)) { operation_index = i; break; }
        }
    }
    return operation_index;
}

inline bool Shell::CanComputeToken(Token token)
{
    //const int T = token.GetType();
    //if (T==SyntaxType::TYPE_BUILT_IN && FunctionExists(token.GetValue()) && FunctionReturns(token.GetValue())) { return true; }
    //return (T>=SyntaxType::TYPE_UNKNOWN && T<SyntaxType::TYPE_SYNTAX);
    return (token.GetType()<SyntaxType::TYPE_SYNTAX);
}



Token Shell::SolveUnary(Token a, Token o)
{
    Token token = Token(a.GetIndex(), a.GetType(), SyntaxGlobal::unsolved_problem);
    std::string op = o.GetValue();
    if      (a.GetType()==SyntaxType::TYPE_BOOLEAN)
    {
        int va = std::stoi(a.GetValue());
        token.SetType(SyntaxType::TYPE_BOOLEAN);
        if      (op==Operator::keys[Operator::OPERATOR_NOT]) { token.SetValue(std::to_string(va)); }
    }
    else if (a.GetType()==SyntaxType::TYPE_INTEGER)
    {
        int va = std::stoi(a.GetValue());
        token.SetType(SyntaxType::TYPE_INTEGER);
        if      (op==Operator::keys[Operator::OPERATOR_NOT]) { token.SetValue(std::to_string(va)); }
        else if (op==Operator::keys[Operator::OPERATOR_SUB]) { token.SetValue(std::to_string(va*-1)); }
    }
    else if (a.GetType()==SyntaxType::TYPE_DECIMAL)
    {
        float va = std::stof(a.GetValue());
        token.SetType(SyntaxType::TYPE_DECIMAL);
        if      (op==Operator::keys[Operator::OPERATOR_NOT]) { token.SetValue(std::to_string(va)); }
        else if (op==Operator::keys[Operator::OPERATOR_SUB]) { token.SetValue(std::to_string(va*-1.0)); }
    }
    else if (a.GetType()==SyntaxType::TYPE_STRING)
    {
        std::string va = a.GetValue();
        token.SetType(SyntaxType::TYPE_STRING);
        if      (op==Operator::keys[Operator::OPERATOR_NOT]) { token.SetValue(std::to_string(va.size()>0)); }
        else if (op==Operator::keys[Operator::OPERATOR_SUB]) { std::string rev = va; std::reverse(rev.begin(), rev.end()); token.SetValue(rev); }
    }
    return token;
}

Token Shell::SolveMath(Token a, Token b, Token o)
{
    std::string name = a.GetName(); if (name=="" || name==SyntaxGlobal::unsolved_problem) { name=b.GetName(); }
    Token token = Token(a.GetIndex(), a.GetType(), SyntaxGlobal::unsolved_problem, name);
    std::string op = o.GetValue();
    int a_type=a.GetType(), b_type=b.GetType();
    // Solve Boolean Math
    if (a_type==SyntaxType::TYPE_BOOLEAN)
    {
        // Boolean And Boolean
        int va = std::stoi(a.GetValue());
        if      (b_type==SyntaxType::TYPE_BOOLEAN)
        {
            int vb = std::stoi(b.GetValue());
            token.SetType(SyntaxType::TYPE_BOOLEAN);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(int(pow(va, vb)))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(va%vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { bool check=va==vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { bool check=va!=vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { bool check=va<vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { bool check=va>vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { bool check=va<=vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { bool check=va>=vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { int check=va&vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { int check=va|vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { bool check=va&&vb; token.SetValue(std::to_string(check)); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { bool check=va||vb; token.SetValue(std::to_string(check)); }
        }
        // Boolean And Integer
        else if (b_type==SyntaxType::TYPE_INTEGER)
        {
            int vb = std::stoi(b.GetValue());
            token.SetType(SyntaxType::TYPE_INTEGER);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(int(pow(va, vb)))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(va%vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { bool check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { bool check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { bool check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { bool check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { bool check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { bool check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { int check=va&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { int check=va|vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { bool check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { bool check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Boolean And Float
        else if (b_type==SyntaxType::TYPE_DECIMAL)
        {
            float vb = std::stof(b.GetValue());
            token.SetType(SyntaxType::TYPE_DECIMAL);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(pow(va, vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(va%int(vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { bool check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { bool check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { bool check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { bool check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { bool check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { bool check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { bool check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { bool check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Boolean And String
        else if (b_type==SyntaxType::TYPE_STRING)
        {
            std::string vb = b.GetValue();
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va)+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { bool check=va==(vb.size()>0); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { bool check=va!=(vb.size()>0); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { bool check=va&&(vb.size()>0); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { bool check=va||(vb.size()>0); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
    }
    // Solve Integer Math
    else if (a_type==SyntaxType::TYPE_INTEGER)
    {
        // Integer And Boolean
        int va = std::stoi(a.GetValue());
        if      (b_type==SyntaxType::TYPE_BOOLEAN)
        {
            int vb = std::stoi(b.GetValue());
            token.SetType(SyntaxType::TYPE_BOOLEAN);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(int(pow(va, vb)))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(va%vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { int check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { int check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { int check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { int check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { int check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { int check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { int check=va&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { int check=va|vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { int check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { int check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Integer And Integer
        else if (b_type==SyntaxType::TYPE_INTEGER)
        {
            int vb = std::stoi(b.GetValue());
            token.SetType(SyntaxType::TYPE_INTEGER);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(int(pow(va, vb)))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(va%vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { int check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { int check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { int check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { int check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { int check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { int check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { int check=va&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_INTEGER); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { int check=va|vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_INTEGER); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { int check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { int check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Integer And Float
        else if (b_type==SyntaxType::TYPE_DECIMAL)
        {
            float vb = std::stof(b.GetValue());
            token.SetType(SyntaxType::TYPE_DECIMAL);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(pow(va, vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(std::fmod(va,vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { int check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { int check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { int check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { int check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { int check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { int check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { int check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { int check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Integer And String
        else if (b_type==SyntaxType::TYPE_STRING)
        {
            std::string vb = b.GetValue();
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va)+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
        }
    }
    // Solve Float Math
    else if (a_type==SyntaxType::TYPE_DECIMAL)
    {
        // Float And Boolean
        float va = std::stof(a.GetValue());
        if      (b_type==SyntaxType::TYPE_BOOLEAN)
        {
            int vb = std::stoi(b.GetValue());
            token.SetType(SyntaxType::TYPE_BOOLEAN);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(int(pow(va, vb)))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(std::fmod(va,vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { int check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { int check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { int check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { int check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { int check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { int check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { int check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { int check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Float And Integer
        else if (b_type==SyntaxType::TYPE_INTEGER)
        {
            int vb = std::stoi(b.GetValue());
            token.SetType(SyntaxType::TYPE_DECIMAL);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(pow(va, vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(std::fmod(va,vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { int check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { int check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { int check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { int check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { int check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { int check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { int check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { int check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Float And Float
        else if (b_type==SyntaxType::TYPE_DECIMAL)
        {
            float vb = std::stof(b.GetValue());
            token.SetType(SyntaxType::TYPE_DECIMAL);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { token.SetValue(std::to_string(pow(va, vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { token.SetValue(std::to_string(va*vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { token.SetValue(std::to_string(va/vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { token.SetValue(std::to_string(std::fmod(va,vb))); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va+vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { token.SetValue(std::to_string(va-vb)); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { int check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { int check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { int check=va<vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { int check=va>vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { int check=va<=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { int check=va>=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { int check=va&&vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { int check=va||vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
        }
        // Float And String
        else if (b_type==SyntaxType::TYPE_STRING)
        {
            std::string vb = b.GetValue();
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(std::to_string(va)+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
        }
    }
    // Solve String Math
    else if (a_type==SyntaxType::TYPE_STRING)
    {
        std::string va = a.GetValue();
        // String And Boolean
        if      (b_type==SyntaxType::TYPE_BOOLEAN)
        {
            std::string vb = b.GetValue();
            token.SetType(SyntaxType::TYPE_STRING);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(va+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
        }
        // String And Integer
        else if (b_type==SyntaxType::TYPE_INTEGER)
        {
            std::string vb = b.GetValue();
            token.SetType(SyntaxType::TYPE_STRING);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(va+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
        }
        // String And Decimal
        else if (b_type==SyntaxType::TYPE_DECIMAL)
        {
            std::string vb = b.GetValue();
            token.SetType(SyntaxType::TYPE_STRING);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(va+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
        }
        // String And String
        else if (b_type==SyntaxType::TYPE_STRING)
        {
            std::string vb = b.GetValue();
            token.SetType(SyntaxType::TYPE_STRING);
            if      (op==Operator::keys[Operator::OPERATOR_POW])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MUL])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_DIV])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_MOD])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_ADD])           { token.SetValue(va+vb); }
            else if (op==Operator::keys[Operator::OPERATOR_SUB])           { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_EQUALS])        { bool check=va==vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_NOT_EQUAL])     { bool check=va!=vb; token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER])        { bool check=va.size()<vb.size(); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER])       { bool check=va.size()>vb.size(); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LESSER_EQUAL])  { bool check=va.size()<=vb.size(); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_GREATER_EQUAL]) { bool check=va.size()>=vb.size(); token.SetValue(std::to_string(check)); token.SetType(SyntaxType::TYPE_BOOLEAN); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_AND])     { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_LOGIC_OR])      { PrintShellError("Cannot Perform Operation "+op+" On "+SyntaxType::keys[b.GetType()]); }
            else if (op==Operator::keys[Operator::OPERATOR_AND])           { bool check=((int(va.size())>0)&&(vb.size()>0)); token.SetValue(Boolean::keys[check]); }
            else if (op==Operator::keys[Operator::OPERATOR_OR])            { bool check=((int(va.size())>0)||(vb.size()>0)); token.SetValue(Boolean::keys[check]); }
        }
    }
    return token;
}

std::vector<Token> Shell::ParseMathChunk(std::vector<Token> tokens)
{
    std::vector<Token> condensed = tokens;
    bool computing = true;
    int math_state = MathState::MATH_PAR, operation_index;
    while (computing)
    {
        // Early Return
        if (!HasOperation(condensed)) { break; }
        operation_index = FirstOperationIndex(condensed, math_state);
        const int left_index = operation_index-1, right_index = operation_index+1;
        if (operation_index>-1)
        {
            Token solved;
            bool L=false, R=false;
            if (operation_index>0) { L=CanComputeToken(condensed[left_index]); } if (operation_index<int(condensed.size()-1)) { R=CanComputeToken(condensed.at(right_index)); }
            // Normal Problem
            if (L==true && R==true)
            {
                solved = SolveMath(condensed.at(left_index), condensed.at(right_index), condensed.at(operation_index));
                if (solved.GetValue()!=SyntaxGlobal::unsolved_problem)
                {
                    // Cull Problem
                    for (int c=0; c<3; c++)
                    {
                        condensed.erase(condensed.begin()+left_index);
                    }
                    // Insert Solution
                    condensed.insert(condensed.begin()+(left_index), solved);
                }
                else { math_state++; if (math_state>MathState::MATH_LOG) { computing = false; break; } }
            }
            // Unary Problem
            else if (R==true)
            {
                solved = SolveUnary(condensed.at(right_index), condensed.at(operation_index));
                if (solved.GetValue()!=SyntaxGlobal::unsolved_problem)
                {
                    // Cull Problem
                    for (int c=0; c<2; c++)
                    {
                        condensed.erase(condensed.begin()+(operation_index));
                    }
                    // Insert Solution
                    condensed.insert(condensed.begin()+(operation_index), solved);
                }
                else { math_state++; if (math_state>MathState::MATH_LOG) { computing = false; break; } }
            }
            // TODO: Clean This Up By Parsing Logic Operators Separately (!=, <=, >=, &&, ||)
            // Logic Problem
            else
            {
                if (operation_index<int(condensed.size()-1))
                {
                    std::string _R = condensed[right_index].GetValue();
                    if (_R==Operator::keys[Operator::OPERATOR_SET])
                    {
                        std::string check = condensed[operation_index].GetValue()+_R;
                        int operation = -1;
                        for (int i=Operator::OPERATOR_AND; i<=Operator::OPERATOR_EQUALS; i++)
                        {
                            if (check==Operator::keys[i]) { operation=i; break; }
                        }
                        if (operation>-1)
                        {
                            solved = Token(condensed[operation_index].GetIndex(), condensed[operation_index].GetType(), check);
                            // Cull Problem
                            for (int c=0; c<2; c++)
                            {
                                condensed.erase(condensed.begin()+(operation_index));
                            }
                            // Insert Solution
                            condensed.insert(condensed.begin()+(operation_index), solved);
                        }
                    }
                    else if (_R==Operator::keys[Operator::OPERATOR_LOGIC_AND] && condensed[operation_index].GetValue()==Operator::keys[Operator::OPERATOR_LOGIC_AND])
                    {
                        solved = Token(condensed[operation_index].GetIndex(), condensed[operation_index].GetType(), Operator::keys[Operator::OPERATOR_AND]);
                        // Cull Problem
                        for (int c=0; c<2; c++)
                        {
                            condensed.erase(condensed.begin()+(operation_index));
                        }
                        // Insert Solution
                        condensed.insert(condensed.begin()+(operation_index), solved);
                    }
                    else if (_R==Operator::keys[Operator::OPERATOR_LOGIC_OR] && condensed[operation_index].GetValue()==Operator::keys[Operator::OPERATOR_LOGIC_OR])
                    {
                        solved = Token(condensed[operation_index].GetIndex(), condensed[operation_index].GetType(), Operator::keys[Operator::OPERATOR_OR]);
                        // Cull Problem
                        for (int c=0; c<2; c++)
                        {
                            condensed.erase(condensed.begin()+(operation_index));
                        }
                        // Insert Solution
                        condensed.insert(condensed.begin()+(operation_index), solved);
                    }
                }
            }
        }
        else
        {
            // Continue Through Order Of Operations
            math_state++; if (math_state>MathState::MATH_LOG) { computing = false; break; }
        }
    }
    return condensed;
}

std::vector<Token> Shell::ParseMath(std::vector<Token> tokens)
{
    std::vector<Token> condensed = tokens;
    bool computing = true;
    int left_index=-1, right_index=-1;
    while (computing)
    {
        bool left_is_func = false;
        left_index=-1; right_index=-1;
        // Check For Parentheses
        for (int i=0; i<int(condensed.size()); i++)
        {
            // Make Sure We're Not Parsing Function Arguments
            if (left_index>0) { left_is_func = HasFunction({condensed.at(left_index-1)}); }
            std::string value = condensed.at(i).GetValue();
            // Set The Left Index
            if (value==Operator::keys[Operator::OPERATOR_PAR_L] && left_is_func==false)
            {
                left_index = i;
            }
            // Set The Right Index
            else if (value==Operator::keys[Operator::OPERATOR_PAR_R] && left_is_func==false)
            {
                if (left_index>-1)
                {
                    right_index = i;
                    break;
                }
            }
        }
        // Parse The Current Chunk
        std::vector<Token> chunk;
        if (left_index>-1 && right_index>left_index)
        {
            // Cull Problem
            for (int i=left_index; i<=right_index; i++)
            {
                Token t = condensed.at(left_index);
                std::string value = t.GetValue();
                if (value!=Operator::keys[Operator::OPERATOR_PAR_L] && value!=Operator::keys[Operator::OPERATOR_PAR_R]) { chunk.push_back(t); }
                condensed.erase(condensed.begin()+left_index);
            }
            // Insert Solution
            std::vector<Token> solved = ParseMathChunk(chunk);
            for (int i=0; i<int(solved.size()); i++)
            {
                condensed.insert(condensed.begin()+left_index+i, solved.at(i));
            }
        }
        else { condensed = ParseMathChunk(condensed); computing = false; break; }
    }
    return condensed;
}