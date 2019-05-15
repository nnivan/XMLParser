#include <iostream>
#include <stack>
using namespace std;

#include "XMLValidator.h"

string XMLValidator::validate(string content){
    content.append("        ");
    XMLValidator v;
    int n = 0, j = 0;
    v.skipWhiteSpace(n, content);
    if(content[n] != '<') return "ERROR1";
    if(content[n+1] == '?'){
        cout<<"Prolog:"<<endl;
        if(content[n+2] != 'x') return "ERROR2";
        if(content[n+3] != 'm') return "ERROR3";
        if(content[n+4] != 'l') return "ERROR4";
        n = n + 5;
        j = n;
        if(!v.getEndOfProlog(j, content)) return "ERROR5";
        if(content[j] != '?') return "ERROR6";
        while(n < j){
            string attributeName, attributeValue;
            if(!v.getAttribute(n, content, attributeName, attributeValue)) return "ERROR7";
            cout << "\t(name:" << attributeName << ", ";
            cout << "value:" << attributeValue << ")" << endl;
            v.skipWhiteSpace(n, content);
        }
        n = n + 2;
    }
    v.skipWhiteSpace(n, content);
    if(content[n++] != '<') return "ERROR8";

    if(content[n] == '/') return "ERROR9";

    stack<string> tags;
    stack<string> id;

    while( n < content.size() ){
        string elementKey;
        string elementId;
        string elementText;
        if(content[n] == '/'){
            n = n + 1;
            if(!v.getTagName(n, content, elementKey)) return "ERROR10";
            cout<<"-close element:"<<elementKey<<"-"<<endl;
            v.skipWhiteSpace(n, content);
            if(content[n] != '>') return "ERROR11";
            n = n + 1;

            if(tags.empty()) return "ERROR100";
            if(tags.top() != elementKey) return "ERROR101";
            tags.pop();
            id.pop();
        }else{
            if(!v.getTagName(n, content, elementKey)) return "ERROR12";
            cout<<"-open element:"<<elementKey<<"-"<<endl;
            v.skipWhiteSpace(n, content);
            j = n;
            if(!v.getEndOfTag(j, content)) return "ERROR13";
            while(n < j){
                string attributeName, attributeValue;
                if(!v.getAttribute(n, content, attributeName, attributeValue)) return "ERROR14";
                cout << "\tAttribute(name:" << attributeName << ", ";
                cout << "value:" << attributeValue << ")"<<endl;
                if(attributeName == "id"){
                    elementId = attributeValue;
                }
                v.skipWhiteSpace(n, content);
            }
            cout << "element id:" << elementId <<endl;
            n = n + 1;

            tags.push(elementKey);
            id.push(elementId);
        }

        j = n;
        v.skipWhiteSpace(j, content);

        if(tags.empty()){
            if(j < content.size()){
                return "ERROR16";
            }
        }else{
            if(content[j] != '<'){
                j = n;
                if(!v.getText(j, content, elementText)) return "ERROR15";
                cout << "element text: -" << elementText << "-" << endl;
            }else{
                cout << "no element text" << endl;
            }
        }
        n = j + 1;
    }
    return "VALID";
}

bool XMLValidator::getText(int &i, string &content, string &text){
    int j = i;
    if(!getStartOfTag(i, content)) return false;
    text = content.substr(j, i - j);
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