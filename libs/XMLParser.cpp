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
	string id, key, value;
	char c;

	file = "";

	while(1){
		if(file == ""){
			cout << '>';
		}else{
			cout << '(' << file << ')' << '>';
		}
		cin >> command;
		if(command == "forseexit"){
			return;
		}else if(command == "exit"){
			if(file == ""){
				return;
			}else{
				cout << "Do you want to save your changes [y/n]: ";
				cin >> c;
				if(c == 'y'){
					errormsg = reader.writeFile(file, structure.toString());
					if(errormsg != "VALID"){
						cout << "Error saving file.";
						continue;
					}
					cout << "File saved.";
				}
				cout << "File \'" << file << "\' closed." << endl;
				file = "";
				return;
			}
		}else if(command == "open"){
			if(file == ""){
				cout << "Open file [path/name]: ";
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
		}else if(command == "new"){
			if(file == ""){
				cout << "New file [path/name]> ";
				cin >> path;
				cout << "input: Root tag key> ";
				cin >> content;
				structure = XMLStructure(content);
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
		}else if(command == "close"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "Do you want to save your changes [y/n]: ";
				cin >> c;
				if(c == 'y'){
					errormsg = reader.writeFile(file, structure.toString());
					if(errormsg != "VALID"){
						cout << "Error saving file.";
						continue;
					}
					cout << "File saved.";
				}
				cout << "File \'" << file << "\' closed." << endl;
				file = "";
			}
		}else if(command == "save"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				errormsg = reader.writeFile(file, structure.toString());
				if(errormsg != "VALID"){
					cout << "Error saving file.";
					continue;
				}
				cout << "File saved." << endl;
			}
		}else if(command == "saveAs"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "Save to [path/name]> ";
				cin >> path;
				errormsg = reader.writeFile(path, structure.toString());
				if(errormsg != "VALID"){
					cout << "Error saving file.";
					continue;
				}
				cout << "File saved." << endl;
				file = path;
			}
		}else if(command == "get"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "Get [prolog/attribute/text]> ";
				cin >> key;
				if(key == "prolog"){
					cout << "Prolog attribute [name]> ";
					cin >> key;
					Attribute* atr= structure.getPrologAttributeByName(key);
					if(atr){
						cout << "Prolog attribute \'" << key << "\' has value: ";
						cout << "\'" << atr->getValue() << "\'." << endl;
					}else{
						cout << "No prolog attribute with name \'" << key << "\'." << endl;
						continue;
					}
				}else if(key == "attribute"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Element data [name]> ";
						cin >> key;
						cout << "Element data \'" << key << "\' has value: ";
						cout << "\'" << elem->getDataValueByName(key) << "\'." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else if(key == "text"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Element with id \'" << id << "\' has text: ";
						cout << "\'" << elem->getText() << "\'." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else{
					cout << "please choose one of the following: [prolog/attribute/text]." << endl;
				}
			}
		}else if(command == "set"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "Set [prolog/attribute/text]> ";
				cin >> key;
				if(key == "prolog"){
					cout << "Prolog attribute [name]> ";
					cin >> key;
					Attribute* atr= structure.getPrologAttributeByName(key);
					if(atr){
						cout << "Set prolog attribute \'" << key << "\' to [value]> ";
						cin >> content;
						atr->setValue(content);
						cout << "Prolog attribute \'" << key << "\' has value: ";
						cout << "\'" << atr->getValue() << "\'." << endl;
					}else{
						cout << "No prolog attribute with name \'" << key << "\'." << endl;
						continue;
					}
				}else if(key == "attribute"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Element data [name]> ";
						cin >> key;
						cout << "Set element data to [value]> ";
						cin >> content;
						elem->setDataValueByName(key, content);
						cout << "Element data \'" << key << "\' has value: ";
						cout << "\'" << elem->getDataValueByName(key) << "\'." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else if(key == "text"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Set element text to [value] (type \'&none;\' for empty string)> ";
						cin >> content;
						if(content == "&none;") content = "";
						elem->setText(content);
						cout << "Element with id \'" << id << "\' has text: ";
						cout << "\'" << elem->getText() << "\'." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else{
					cout << "please choose one of the following: [prolog/attribute/text]." << endl;
				}
			}
		}else if(command == "add"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "Add [prolog/attribute/element]> ";
				cin >> key;
				if(key == "prolog"){
					cout << "Prolog attribute [name]> ";
					cin >> key;
					cout << "Prolog attribute [value]> ";
					cin >> content;
					structure.addPrologAttribute(key, content);
					cout << "Done." << endl;
				}else if(key == "attribute"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Aattribute [name]> ";
						cin >> key;
						cout << "Attribute [value]> ";
						cin >> content;
						elem->addAttribute(key, content);
						cout << "Done." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else if(key == "element"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Element [key]> ";
						cin >> key;
						cout << "Element [id]> ";
						cin >> content;
						cout << "Element [text] (type \'&none;\' for empty string)> ";
						cin >> value;
						if(value == "&none;") value = "";
						structure.addElement(id, key, value, content);
						cout << "Done." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else{
					cout << "please choose one of the following: [prolog/attribute/element]." << endl;
				}
			}
		}else if(command == "remove"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "Remove [prolog/attribute/element]> ";
				cin >> key;
				if(key == "prolog"){
					cout << "Prolog attribute [name]> ";
					cin >> key;
					Attribute* atr= structure.getPrologAttributeByName(key);
					if(atr){
						structure.removePrologAttribute(key);
						cout << "Done." << endl;
					}else{
						cout << "No prolog attribute with name \'" << key << "\'." << endl;
						continue;
					}
				}else if(key == "attribute"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						cout << "Attribute [name]> ";
						cin >> key;
						structure.removeAttribute(id, key);
						cout << "Done." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else if(key == "element"){
					cout << "Element [id]> ";
					cin >> id;
					XMLElement* elem = structure.getElementById(id);
					if(elem){
						structure.removeElement(id);
						cout << "Done." << endl;
					}else{
						cout << "No element with id \'" << id << "\'." << endl;
						continue;
					}
				}else{
					cout << "please choose one of the following: [prolog/attribute/element]." << endl;
				}
			}
		}else if(command == "XPath"){
			if(file == ""){
				cout << "You are not working on any file." << endl;
				cout << "Try \'open\' or \'new\' to open/create file." << endl;
			}else{
				cout << "XPath from element [id] (type \'&root;\' for root element)> ";
				cin >> id;
				if(id == "&root;") id = structure.getRootId();
				cout << "[XPath]> ";
				cin >> content;
				cout << "XPath result:" << endl;
				cout << XMLXPath::XPath(content, id, structure) << endl;
			}
		}else if(command == "help"){
			cout << "Available commands:" << endl;
			cout << "\tforseexit    - description ;)" << endl;
			cout << "\texit         - description ;)" << endl;
			cout << "\topen         - description ;)" << endl;
			cout << "\tnew          - description ;)" << endl;
			cout << "\tclose        - description ;)" << endl;
			cout << "\tsave         - description ;)" << endl;
			cout << "\tsaveAs       - description ;)" << endl;
			cout << "\tprint        - description ;)" << endl;
			cout << "\tget          - description ;)" << endl;
			cout << "\tset          - description ;)" << endl;
			cout << "\tadd          - description ;)" << endl;
			cout << "\tremove       - description ;)" << endl;
			cout << "\tXPath        - description ;)" << endl;
		}else{
			cout << "Command not found: \'" << command << "\'. ";
			cout << "Try \'help\' for more information." << endl;
		}
	}
}