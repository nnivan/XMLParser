#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "StackString.h"
#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLStructure.h"
#include "XMLValidator.h"

XMLStructure XMLValidator::getValidXMLStructureFromString(string content, string &error){

    XMLValidator v;
    XMLStructure xmls;
    int n = 0;//, j = 0;
    content.append("        "); 

    if(!v.getValidPrologAttributes(n, xmls, content, error)) return xmls;
    if(!v.getValidElements(n, xmls, content, error)) return xmls;

    return xmls;
}

bool XMLValidator::getValidPrologAttributes(int &n, XMLStructure& xmls, string content, string &error){
    int j;
    skipWhiteSpace(n, content);
    if(content[n] != '<') {
        error = "ERROR1";
        return false;
    }
    if(content[n+1] == '?'){
        /// cout<<"Prolog:"<<endl;
        if(content[n+2] != 'x') {
            error = "ERROR2";
            return false;
        }
        if(content[n+3] != 'm') {
            error = "ERROR3";
            return false;
        }
        if(content[n+4] != 'l') {
            error = "ERROR4";
            return false;
        }
        n = n + 5;
        j = n;
        if(!getEndOfProlog(j, content)) {
            error = "ERROR5";
            return false;
        }
        if(content[j] != '?') {
            error = "ERROR6";
            return false;
        }
        while(n < j){
            string attributeName, attributeValue;
            if(!getAttribute(n, content, attributeName, attributeValue)) {
                error = "ERROR7";
                return false;
            }
            /// cout << "\t(name:" << attributeName << ", ";
            /// cout << "value:" << attributeValue << ")" << endl;
            xmls.addPrologAttribute(attributeName, attributeValue);
            skipWhiteSpace(n, content);
        }
        n = n + 2;
    }
    skipWhiteSpace(n, content);
    if(content[n++] != '<') {
        error = "ERROR8";
        return false;
    }

    if(content[n] == '/') {
        error = "ERROR9";
        return false;
    }
    return true;
}
bool XMLValidator::getValidElements(int &n, XMLStructure& xmls, string content, string &error){
    int j;

    StackString tags;
    StackString id;

    while( n < content.size() ){
        string elementKey = "";
        string elementId = "";
        string elementText = "";
        if(content[n] == '/'){
            n = n + 1;
            if(!getTagName(n, content, elementKey)) {
                error = "ERROR10";
                return false;
            }
            /// cout<<"-close element:"<<elementKey<<"-"<<endl;
            skipWhiteSpace(n, content);
            if(content[n] != '>') {
                error = "ERROR11";
                return false;
            }
            n = n + 1;

            if(tags.empty()) {
                error = "ERROR101";
                return false;
            }
            if(tags.top() != elementKey) {
                error = "ERROR102";
                return false;
            }
            tags.pop();
            id.pop();
        }else{
            if(!getTagName(n, content, elementKey)) {
                error = "ERROR12";
                return false;
            }
            /// cout<<"-open element:"<<elementKey<<"-"<<endl;
            skipWhiteSpace(n, content);
            j = n;
            if(!getEndOfTag(j, content)) {
                error = "ERROR13";
                return false;
            }
            vector< Attribute > attributes;
            while(n < j){
                string attributeName, attributeValue;
                if(!getAttribute(n, content, attributeName, attributeValue)) {
                    error = "ERROR14";
                    return false;
                }
                /// cout << "\tAttribute(name:" << attributeName << ", ";
                /// cout << "value:" << attributeValue << ")"<<endl;
                if(attributeName == "id"){
                    elementId = attributeValue;
                }else{
                    attributes.push_back( Attribute(attributeName, attributeValue) );
                }
                skipWhiteSpace(n, content);
            }
            /// cout << "element id:" << elementId <<endl;
            n = n + 1;

            if(tags.empty()){
                xmls.setRootValues(elementKey, elementId);
                elementId = xmls.getRootId();
                /// cout << "this is root element -" << elementId << "-" << endl;
            }else{
                elementId = xmls.addElement(id.top(), elementKey, elementText, elementId);
            }

            for(int i = 0; i < attributes.size(); i++){
                xmls.addAttribute(elementId, attributes[i].getName(), attributes[i].getValue());
            }

            tags.push(elementKey);
            id.push(elementId);
        }

        j = n;
        skipWhiteSpace(j, content);

        if(tags.empty()){
            if(j < content.size()){
                error = "ERROR16";
                return false;
            }
        }else{
            if(content[j] != '<'){
                j = n;
                if(!getText(j, content, elementText)) {
                    error = "ERROR15";
                    return false;
                }
                /// cout << "element text: -" << elementText << "-" << endl;
                xmls.appendText(id.top(), elementText);
            }else{
                /// cout << "no element text" << endl;
            }
        }
        n = j + 1;
    }
    if(!tags.empty()){
        error = "ERROR100";
    }
    error = "VALID";
    return true;
}

bool XMLValidator::getText(int &i, string &content, string &text){
    int j = i;
    if(!getStartOfTag(i, content)) return false;
    text = content.substr(j, i - j);
    int p = 0, q = text.size() - 1;
    skipWhiteSpace(p, text);
    while(q >= 0 && isWhiteSpace(text[q])) q--;
    text = text.substr(p, q - p + 1);
    return true;
}
bool XMLValidator::getStartOfTag(int &i, string &content){
    while(i < content.size()){
        if(content[i] == '<') return true;
        i++;
    }
    return false;
}
bool XMLValidator::getEndOfTag(int &i, string &content){
    while(i < content.size()){
        if(content[i] == '<') return false;
        if(content[i] == '>') return true;
        i++;
    }
    return false;
}
bool XMLValidator::getTagName(int &i, string &content, string &key){
    int j = i;
    if(isWhiteSpace(content[i])) return false;
    if(!getEndOfTagName(i, content)) return false;
    key = content.substr(j, i - j);
    return true;
}
bool XMLValidator::getEndOfTagName(int &i, string &content){
    while(i < content.size()){
        if(content[i] == '<') return false;
        if(isWhiteSpace(content[i])) return true;
        if(content[i] == '>') return true;
        i++;
    }
    return false;
}
bool XMLValidator::getAttribute(int &i, string &content, string &name, string &value){
    int j;
    skipWhiteSpace(i, content);
    j = i;
    if(!getEndOfAttributeName(i, content)) return false;
    name = content.substr(j, i - j);
    skipWhiteSpace(i, content);
    if(content[i++] != '=') return false;
    skipWhiteSpace(i, content);
    j = i + 1;
    if(!getEndOfQuote(i, content)) return false;
    value = content.substr(j, i - j);
    i = i + 1;
    return true;
}
bool XMLValidator::getEndOfQuote(int &i, string &content){
    if(content[i++] != '"') return false;
    while(i < content.size()){
        if(content[i] == '<') return false;
        if(content[i] == '"') return true;
        i++;
    }
    return false;
}
bool XMLValidator::getEndOfAttributeName(int &i, string &content){
    while(i < content.size()){
        if(content[i] == '<') return false;
        if(isWhiteSpace(content[i])) return true;
        if(content[i] == '=') return true;
        i++;
    }
    return false;
}
bool XMLValidator::getEndOfProlog(int &i, string &content){
    while(i < content.size()){
        if(content[i] == '<') return false;
        if(content[i] == '?' && content[i+1] == '>'){
            return true;
        }
        i++;
    }
    return false;
}
void XMLValidator::skipWhiteSpace(int &i, string &content){
    while(i < content.size() && isWhiteSpace(content[i])) i++;
}
bool XMLValidator::isWhiteSpace(char x){
    return x==' ' or x=='\n' or x=='\t';
}