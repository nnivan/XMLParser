#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "libs\StackString.h"
#include "libs\XMLAttribute.h"
#include "libs\XMLElement.h"
#include "libs\XMLStructure.h"
#include "libs\XMLValidator.h"
#include "libs\XMLReader.h"
#include "libs\XMLXPath.h"
#include "libs\XMLParser.h"

int main(){

    XMLParser parser;
    parser.commandLineInteface();
    
    return 0;
}