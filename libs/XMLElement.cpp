#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "XMLAttribute.h"
#include "XMLElement.h"

XMLElement::XMLElement(string key, string text, string id){
    doSetKey(key);
    doSetText(text);
    doSetId(id);
}

void XMLElement::set(string key, string text){
    doSetKey(key);
    doSetText(text);
}

void XMLElement::set(string key, string text, string id){
    doSetKey(key);
    doSetText(text);
    doSetId(id);
}

void XMLElement::setKey(string key){
    doSetKey(key);
}

void XMLElement::setText(string text){
    doSetText(text);
}

void XMLElement::setId(string id){
    doSetId(id);
}

string XMLElement::getKey() const {
    return this->key;
}

string XMLElement::getText() const {
    return this->text;
}

string XMLElement::getId() const {
    return this->id;
}

void XMLElement::addChildElement(string key, string text, string id){
    this->childElements.push_back( XMLElement(key, text, id) );
}

int XMLElement::sizeChildElement() const {
    return this->childElements.size();
}

XMLElement* XMLElement::getChildElement(int i) {
    return &(this->childElements[i]);
}

void XMLElement::removeChildElement(int i) {
    this->childElements.erase(this->childElements.begin() + i);
}

void XMLElement::addAttribute(string name, string value){
    if(name == "id") return;
    this->attributes.push_back( Attribute(name, value) );
}

int XMLElement::sizeAttribute() const {
    return this->attributes.size();
}

Attribute* XMLElement::getAttribute(int i) {
    return &(this->attributes[i]);
}

void XMLElement::removeAttribute(int i) {
    this->attributes.erase(this->attributes.begin() + i);
}


bool XMLElement::hasChildElement(string key){
    for(int i = 0; i < this->childElements.size(); i++){
        if(this->childElements[i].getKey() == key){
            return true;
        }
    }
    return false;
}


bool XMLElement::hasDataName(string name){
    if(name == "id" and this->id != "") return true;
    for(int i = 0; i < this->attributes.size(); i++){
        if(this->attributes[i].getName() == name){
            return true;
        }
    }
    for(int i = 0; i < this->childElements.size(); i++){
        if(this->childElements[i].getKey() == name){
            return true;
        }
    }
    return false;
}

string XMLElement::getDataValueByName(string name){
    if(name == "id") return this->id;
    for(int i = 0; i < this->attributes.size(); i++){
        if(this->attributes[i].getName() == name){
            return this->attributes[i].getValue();
        }
    }
    for(int i = 0; i < this->childElements.size(); i++){
        if(this->childElements[i].getKey() == name){
            return this->childElements[i].getText();
        }
    }
    return "";
}
void XMLElement::setDataValueByName(string name, string value){
    for(int i = 0; i < this->attributes.size(); i++){
        if(this->attributes[i].getName() == name){
            return this->attributes[i].setValue(value);
        }
    }
    for(int i = 0; i < this->childElements.size(); i++){
        if(this->childElements[i].getKey() == name){
            return this->childElements[i].setText(value);
        }
    }
}

bool XMLElement::hasData(string dataName, string dataValue){
    if(dataName == "id" and dataValue == this->id) return true;
    for(int i = 0; i < this->childElements.size(); i++){
        if(this->childElements[i].getKey() == dataName){
            if(this->childElements[i].getText() == dataValue){
                return true;
            }
        }
    }
    for(int i = 0; i < this->attributes.size(); i++){
        if(this->attributes[i].getName() == dataName){
            if(this->attributes[i].getValue() == dataValue){
                return true;
            }
        }
    }
    return false;
}

string XMLElement::toString(int tabs) const {
    string ret = "";

    /// (\t<KEY id="ID")
	ret += this->tabsString(tabs);
	ret += "<";
    ret += this->key;
    ret += " id=\"";
    ret += this->id;
    ret += "\"";

    /// n:( NAME="VALUE")
    for(int i = 0; i < this->attributes.size(); i++){
        ret += this->attributes[i].toString();
    }

    if(this->childElements.empty()){
        /// (>TEXT</KEY>)
        ret += ">";
        ret += this->text;
        ret += "</";
        ret += this->key;
        ret += ">";
        return ret;
    }

    /// (>TEXT\n)
    ret += ">";
    ret += this->text;

    /// n:(CHILDELEMENT\n)
    for(int i = 0; i < this->childElements.size(); i++){
        ret += "\n";
    	ret += this->childElements[i].toString(tabs+1);
    }
    ret += "\n";

    /// (\t</KEY>)
	ret += this->tabsString(tabs);
    ret += "</";
    ret += this->key;
    ret += ">";

    return ret;
}

void XMLElement::doSetKey(string key){
    if(key == "") key = "unknown";
    this->key = key;
}

void XMLElement::doSetText(string text){
    this->text = text;
}

void XMLElement::doSetId(string id){
    this->id = id;
}

string XMLElement::tabsString(int tabs) const {
	string ret = "";
	for(int i = 0; i < tabs; i++){
		ret += "\t";
	}
	return ret;
}
