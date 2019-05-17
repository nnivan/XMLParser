#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLStructure.h"
#include "XMLValidator.h"
#include "XMLXPath.h"


string XMLXPath::XPath(string command, XMLStructure &xmls){
    vector<XMLElement> elements; 
    xmls.fillVectorWithElements(xmls.getRootId(), elements);
    XMLXPath x;
    command += '/';
    
    bool at;
    string elementName = "", attributeName = "", attributeValue = "";

    for(int i = 0; i < command.size(); i++){
    	at = false;
    	x.skipToXPathSing(i, command, elementName);
    	if(	command[i] == '/'){
    		if(command[i+1] == '/' or command[i+1] == '(' or command[i+1] == '[')
    			return "ERROR: ILLIGAL SIGN AFTER '/'";
    		// cout << "get elements with key -" << elementName << "-" << endl;
    		x.removeElementsByTag(elements, elementName);
    	}else if( command[i] == '[' ){
    		i++;
    		int index;
    		x.skipToChar(i, ']', command, attributeValue);
    		if(!x.stringToInt(attributeValue, index)) return "ERROR: BAD INDEX VALUE";
    		i++;
    		if(command[i] != '/') return "ERROR: EXPECTED '/' AFTER ']'";
    		// cout << "get element -" << elementName << "- ";
    		// cout << "with index -" << index << "-" << endl; 
    		x.removeElementsByTag(elements, elementName);
    		x.removeElementsByIndex(elements, index);
		}else if( command[i] == '('){
			i++;
			if( command[i] == '@'){
				at = true;
				i++;
				x.skipToChar(i, ')', command, attributeName);
				i++;
				if(command[i] != '/') return "ERROR: EXPECTED '/' AFTER ']'";
				// cout << "get elements with key -" << elementName << "- ";
				// cout << "that have attribute -" << attributeName << "-" << endl;
    			x.removeElementsByTag(elements, elementName);
				x.removeElementsByAttribute(elements, attributeName);
			}else{
				x.skipToChar(i, '=', command, attributeName);
				i++;
				if(command[i] != '"') return "ERROR: ATTRIBUTE VALUE MUST BE QUOTED";
				else{
					i++;
					x.skipToChar(i, '"', command, attributeValue);
				}
				i++;
				if(command[i] != ')') return "ERROR: EXPECTED ')' AFTER '\"'";
	    		i++;
	    		if(command[i] != '/') return "ERROR: EXPECTED '/' AFTER ']'";
				// cout << "get elements with key -" << elementName << "- ";
				// cout << "that has -" << attributeName << "- ";
				// cout << "equals to -" << attributeValue << "-" << endl;
    			x.removeElementsByTag(elements, elementName);
				x.removeElementsByData(elements, attributeName, attributeValue);
			}
		}
    }
    string result = "";
    if(at){
	    for(int i = 0; i < elements.size(); i++){
	    	string temp = elements[i].getDataValueByName(attributeName);
	    	if(temp != "") result += temp + "\n";
	    }
    }else{
	    for(int i = 0; i < elements.size(); i++){
	    	string temp = elements[i].getText();
	    	if(temp != "") result += temp + "\n";
	    }
	}
    return result;
}


void XMLXPath::removeElementsByTag(vector<XMLElement> &elements, string tagName){
	vector<XMLElement> result;
	for(int i = 0; i < elements.size(); i++){
		if(elements[i].hasChildElement(tagName)){
			for(int x = 0; x < elements[i].sizeChildElement(); x++){
				XMLElement* temp = elements[i].getChildElement(x);
				if(temp->getKey() == tagName){
					result.push_back(*temp);
				}
			}
		}
	}
	elements.clear();
	for(int i = 0; i < result.size(); i++){
		elements.push_back(result[i]);
	}
}
void XMLXPath::removeElementsByIndex(vector<XMLElement> &elements, int index){
	XMLElement x = elements[index];
	elements.clear();
	elements.push_back(x);
}
void XMLXPath::removeElementsByAttribute(vector<XMLElement> &elements, string dataName){
	vector<XMLElement> result;
	for(int i = 0; i < elements.size(); i++){
		if(elements[i].hasDataName(dataName) or elements[i].hasChildElement(dataName)){
			XMLElement temp = elements[i];
			result.push_back(temp);
		}
	}
	elements.clear();
	for(int i = 0; i < result.size(); i++){
		elements.push_back(result[i]);
	}
}
void XMLXPath::removeElementsByData(vector<XMLElement> &elements, string dataName, string dataValue){
	vector<XMLElement> result;
	for(int i = 0; i < elements.size(); i++){
		if(elements[i].hasData(dataName, dataValue)){
			XMLElement temp = elements[i];
			result.push_back(temp);
		}
	}
	elements.clear();
	for(int i = 0; i < result.size(); i++){
		elements.push_back(result[i]);
	}
}

bool XMLXPath::skipToXPathSing(int &i, string content, string &data){
	data = "";
	while(i < content.size()){
		data += content[i++];
		if(content[i] == '/' or content[i] == '(' or content[i] == '[') return true;
	}
	return false;
}


bool XMLXPath::skipToChar(int &i, char x, string content, string &data){
	data = "";
	while(i < content.size()){
		data += content[i++];
		if(content[i] == x) return true;
	}
	return false;
}


bool XMLXPath::stringToInt(string data, int &ind){
	ind = 0;
	for(int i = 0; i < data.size(); i++){
		if(data[i] < '0' || data[i] > '9') return false;
		ind *= 10;
		ind += data[i] - '0';
	}
	return true;
}