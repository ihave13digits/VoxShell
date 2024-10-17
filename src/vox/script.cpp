#include <iostream>
#include "script.h"

Script::Script()
{
    //
}

std::vector<std::string> Script::GetLines()
{
    return lines;
}

void Script::SetLines(std::vector<std::string> _lines)
{
    lines = _lines;
}

std::string Script::GetName()
{
    return name;
}

void Script::SetName(std::string _name)
{
    name = _name;
}

void Script::LoadFile(std::string _file)
{
    std::string line;
    std::fstream data_file(_file);
    if (data_file.is_open())
    {
        while (getline(data_file, line)) { lines.push_back(line); }
        data_file.close();
    }
    else {  }
}