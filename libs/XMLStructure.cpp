#include <iostream>
#include <vector>
using namespace std;

#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLStructure.h"

XMLStructure::XMLStructure(string key, string id){
    this->root = XMLElement(key, "", id);
    this->usedId.push_back("");
    this->usedId.push_back("0");
}

string XMLStructure::addElement(string id, string newKey, string newText, string newId){
    XMLElement* temp = this->findElement(id);
    if(temp){
        if(newId == "") newId = id;
        newId = this->getFreeId(newId);
        this->usedId.push_back(newId);
        temp->addChildElement( newKey, newText, newId );
        return newId;
    }
    return "";
}
bool XMLStructure::removeElement(string id){
    XMLElement* temp = findElementParent(id);
    if(temp){
        for(int i = 0; i < temp->sizeChildElement(); i++){
            if(temp->getChildElement(i)->getId() == id){
                temp->removeChildElement(i);
                return true;
            }
        }
    }
    return false;
}

void XMLStructure::setText(string id, string text){
    XMLElement* temp = findElement(id);
    if(temp){
        temp->setText(text);
    }
}

void XMLStructure::appendText(string id, string text){
    XMLElement* temp = findElement(id);
    if(temp){
        temp->setText(temp->getText() + text);
    }
}

string XMLStructure::getText(string id){
    XMLElement* temp = findElement(id);
    return temp->getText();
}

void XMLStructure::addAttribute(string id, string name, string value){
    XMLElement* temp = this->findElement(id);
    if(temp){
        temp->addAttribute( name, value );
    }
}
void XMLStructure::setAttributeValue(string id, string name, string value){
    XMLElement* temp = this->findElement(id);
    if(temp){
        for(int i = 0; i < temp->sizeAttribute(); i++){
            if(temp->getAttribute(i)->getName() == name){
                temp->getAttribute(i)->setValue(value);
            }
        }
    }
}
bool XMLStructure::removeAttribute(string id, string attributeName){
    XMLElement* temp = this->findElement(id);
    if(temp){
        for(int i = 0; i < temp->sizeAttribute(); i++){
            if(temp->getAttribute(i)->getName() == attributeName){
                temp->removeAttribute(i);
                return true;
            }
        }
    }
    return false;
}

void XMLStructure::addPrologAttribute(string name, string value){
    this->prolog.push_back( Attribute(name, value) );
}

int XMLStructure::sizePrologAttribute() const {
    this->prolog.size();
}
Attribute* XMLStructure::getPrologAttribute(int i) {
    return &(this->prolog[i]);
}

void XMLStructure::removePrologAttribute(string name){
    for(int i = 0; i < this->prolog.size(); i++){
        if(this->prolog[i].getName() == name){
            this->prolog.erase(this->prolog.begin() + i);
            return;
        }
    }
}

string XMLStructure::getRootId() const {
    return this->root.getId();
}

const XMLElement XMLStructure::getRootElement() const {
    return this->root;
}
string XMLStructure::toString() const {
    string ret = "";
    if(this->prolog.size()){
        ret += "<?xml";
        for(int i = 0; i < this->prolog.size(); i++){
            ret += this->prolog[i].toString();
        }
        ret += "?>\n";
    }
    ret += this->root.toString();
    return ret;
}

XMLElement* XMLStructure::findElementRecursive(XMLElement* current, string id){

	if(current->getId() == id) return current;

	for(int i = 0; i < current->sizeChildElement(); i++){

		XMLElement* temp = findElementRecursive( current->getChildElement(i), id);
		if(temp){
			return temp;
		}
	}
	return nullptr;
}

XMLElement* XMLStructure::findElement(string id){
	return findElementRecursive( &this->root, id);
}

XMLElement* XMLStructure::findElementParentRecursive(XMLElement* current, string id){

	for(int i = 0; i < current->sizeChildElement(); i++){
        if(current->getChildElement(i)->getId() == id){
            return current;
        }
		XMLElement* temp = findElementRecursive( current->getChildElement(i), id);
		if(temp){
			return temp;
		}
	}
	return nullptr;
}

XMLElement* XMLStructure::findElementParent(string id){
	return findElementParentRecursive( &this->root, id);
}

string XMLStructure::getFreeId(string id){
    if(isIdFree(id)) return id;
    int i = 1;
    while(1){
        if(isIdFree(id + "_" + to_string(i)))
            return id + "_" + to_string(i);
        i++;
    }
}

bool XMLStructure::isIdFree(string id){
    for(int i = 0; i < usedId.size(); i++){
        if(usedId[i] == id){
            return false;
        }
    }
    return true;
}

