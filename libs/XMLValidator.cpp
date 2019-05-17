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

    XMLStructure xmls;

    content.append("        ");
    XMLValidator v;
    int n = 0, j = 0;
    v.skipWhiteSpace(n, content);
    if(content[n] != '<') {
        error = "ERROR1";
        return xmls;
    }
    if(content[n+1] == '?'){
        /// cout<<"Prolog:"<<endl;
        if(content[n+2] != 'x') {
            error = "ERROR2";
            return xmls;
        }
        if(content[n+3] != 'm') {
            error = "ERROR3";
            return xmls;
        }
        if(content[n+4] != 'l') {
            error = "ERROR4";
            return xmls;
        }
        n = n + 5;
        j = n;
        if(!v.getEndOfProlog(j, content)) {
            error = "ERROR5";
            return xmls;
        }
        if(content[j] != '?') {
            error = "ERROR6";
            return xmls;
        }
        while(n < j){
            string attributeName, attributeValue;
            if(!v.getAttribute(n, content, attributeName, attributeValue)) {
                error = "ERROR7";
                return xmls;
            }
            /// cout << "\t(name:" << attributeName << ", ";
            /// cout << "value:" << attributeValue << ")" << endl;
            xmls.addPrologAttribute(attributeName, attributeValue);
            v.skipWhiteSpace(n, content);
        }
        n = n + 2;
    }
    v.skipWhiteSpace(n, content);
    if(content[n++] != '<') {
        error = "ERROR8";
        return xmls;
    }

    if(content[n] == '/') {
        error = "ERROR9";
        return xmls;
    }

    StackString tags;
    StackString id;

    while( n < content.size() ){
        string elementKey = "";
        string elementId = "";
        string elementText = "";
        if(content[n] == '/'){
            n = n + 1;
            if(!v.getTagName(n, content, elementKey)) {
                error = "ERROR10";
                return xmls;
            }
            /// cout<<"-close element:"<<elementKey<<"-"<<endl;
            v.skipWhiteSpace(n, content);
            if(content[n] != '>') {
                error = "ERROR11";
                return xmls;
            }
            n = n + 1;

            if(tags.empty()) {
                error = "ERROR101";
                return xmls;
            }
            if(tags.top() != elementKey) {
                error = "ERROR102";
                return xmls;
            }
            tags.pop();
            id.pop();
        }else{
            if(!v.getTagName(n, content, elementKey)) {
                error = "ERROR12";
                return xmls;
            }
            /// cout<<"-open element:"<<elementKey<<"-"<<endl;
            v.skipWhiteSpace(n, content);
            j = n;
            if(!v.getEndOfTag(j, content)) {
                error = "ERROR13";
                return xmls;
            }
            vector< Attribute > attributes;
            while(n < j){
                string attributeName, attributeValue;
                if(!v.getAttribute(n, content, attributeName, attributeValue)) {
                    error = "ERROR14";
                    return xmls;
                }
                /// cout << "\tAttribute(name:" << attributeName << ", ";
                /// cout << "value:" << attributeValue << ")"<<endl;
                if(attributeName == "id"){
                    elementId = attributeValue;
                }else{
                    attributes.push_back( Attribute(attributeName, attributeValue) );
                }
                v.skipWhiteSpace(n, content);
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
        v.skipWhiteSpace(j, content);

        if(tags.empty()){
            if(j < content.size()){
                error = "ERROR16";
                return xmls;
            }
        }else{
            if(content[j] != '<'){
                j = n;
                if(!v.getText(j, content, elementText)) {
                    error = "ERROR15";
                    return xmls;
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
    return xmls;
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