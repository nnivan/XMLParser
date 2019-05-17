#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "XMLReader.h"


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
