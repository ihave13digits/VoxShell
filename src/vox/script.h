#pragma once
#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include <vector>
#include <fstream>

class Script
{

private:

    std::vector<std::string> lines;
    std::string name;

public:

    Script();

    std::vector<std::string> GetLines();
    void SetLines(std::vector<std::string> _lines);

    std::string GetName();
    void SetName(std::string _name);

    void LoadFile(std::string _file);

};

#endif // SCRIPT_H