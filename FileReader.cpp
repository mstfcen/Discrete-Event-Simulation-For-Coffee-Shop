#include <fstream>
#include <vector>
#include <string>
#include <iostream>
class FileReader
{

public:
   static std::vector<std::string> readFile(std::string fileName);
};


std::vector<std::string> FileReader::readFile(std::string fileName)
{
    std::vector<std::string> returnVector;
    std::string input;
    std::fstream file;
    file.open(fileName.c_str());
    while (file >>input)
    {
        returnVector.push_back(input);
    }
    file.close();
    return returnVector;
}