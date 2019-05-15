#include <iostream>
#include <fstream>
using namespace std;

#include "XMLValidator.h"
#include "XMLReader.h"

string XMLReader::getXMLfromFile(string path){
    string content = readFile(path);
    content = XMLValidator::validate(content);
    return content;
}

string XMLReader::readFile(string path){
    input.open(path.c_str());

    if(input.is_open()){
        string line;
        getline (input, line, '\0');
        input.close();
        return line;
    }

    input.close();
    return "ERROR OPENING FILE\n";
}

void XMLReader::writeFile(string path, string data){
    output.open(path.c_str());

    if(output.is_open()){
        output << data;
        output.close();
    }

    input.close();
    return;
}
