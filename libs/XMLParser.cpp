#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLStructure.h"
#include "XMLValidator.h"
#include "XMLReader.h"
#include "XMLXPath.h"
#include "XMLParser.h"


// void XMLParser::commandLineInteface(istream &in, ostream &out){
void XMLParser::commandLineInteface(){
	cout << "XMLParser" << endl;
	string command;
	string path, content, errormsg;
	char c;

	file = "";

	while(1){
		if(file == ""){
			cout << '>';
		}else{
			cout << '(' << file << ')' << '>';
		}
		cin >> command;
		if(command == "forseExit"){
			return;
		}else if(command == "testPrint"){
			cout << "Hello, world!" << endl;
		}else if(command == "open"){
			if(file == ""){
				cout << "\tFile name: ";
				cin >> path;
				content = reader.readFile(path);
				if(content == "ERROR"){
					cout << "Error opening file \'" << path << "\'." << endl;
					continue;
				}
				structure = XMLValidator::getValidXMLStructureFromString(content, errormsg);
				if(errormsg != "VALID"){
					cout << "Error: file \'" << path << "\' is not valid. (" << errormsg << ")" << endl;
					continue;
				}
				file = path;
			}else{
				cout << "You are working on file \'" << file << "\'." << endl;
				cout << "Try \'close\' to close your current file." << endl;
			}
		}else if(command == "print"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << structure.toString() << endl;
			}
		}else if(command == ""){
		}else{
			cout << "Command not found: \'" << command << "\'. ";
			cout << "Try \'help\' for more information." << endl;
		}
	}
}