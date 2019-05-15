#include <iostream>
using namespace std;

#include "XMLAttribute.h"

Attribute::Attribute(string name, string value){
    this->doSetName(name);
    this->doSetValue(value);
}

void Attribute::set(string name, string value){
    this->doSetName(name);
    this->doSetValue(value);
}

void Attribute::setName(string name){
    this->doSetName(name);
}
void Attribute::setValue(string value){
    this->doSetValue(value);
}

string Attribute::getName() const {
    return this->name;
}
string Attribute::getValue() const {
    return this->value;
}

string Attribute::toString() const {
    return " " + name + "=\"" + value + "\"";
}

void Attribute::doSetName(string name){
    this->name = name;
}

void Attribute::doSetValue(string value){
    this->value = value;
}
