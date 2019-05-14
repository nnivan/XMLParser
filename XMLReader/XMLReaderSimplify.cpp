#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

class XMLValidator{
public:
    static string validate(string content){
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
                v.skipWhiteSpace(n, content);

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
                j = n;
                v.skipWhiteSpace(j, content);
                if(content[j] != '<'){
                    j = n;
                    if(!v.getText(j, content, elementText)) return "ERROR15";
                    cout << "element text: -" << elementText << "-" << endl;
                }else{
                    cout << "no element text" << endl;
                }
                n = j;

                tags.push(elementKey);
                id.push(elementId);
            }
            n = n + 1;
        }
        return "EOF\n";
    }

private:
    bool getText(int &i, string &content, string &text){
        int j = i;
        if(!getStartOfTag(i, content)) return false;
        text = content.substr(j, i - j);
        return true;
    }
    bool getStartOfTag(int &i, string &content){
        while(i < content.size()){
            if(content[i] == '<') return true;
            i++;
        }
        return false;
    }
    bool getEndOfTag(int &i, string &content){
        while(i < content.size()){
            if(content[i] == '<') return false;
            if(content[i] == '>') return true;
            i++;
        }
        return false;
    }
    bool getTagName(int &i, string &content, string &key){
        int j = i;
        if(isWhiteSpace(content[i])) return false;
        if(!getEndOfTagName(i, content)) return false;
        key = content.substr(j, i - j);
        return true;
    }
    bool getEndOfTagName(int &i, string &content){
        while(i < content.size()){
            if(content[i] == '<') return false;
            if(isWhiteSpace(content[i])) return true;
            if(content[i] == '>') return true;
            i++;
        }
        return false;
    }
    bool getAttribute(int &i, string &content, string &name, string &value){
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
    bool getEndOfQuote(int &i, string &content){
        if(content[i++] != '"') return false;
        while(i < content.size()){
            if(content[i] == '<') return false;
            if(content[i] == '"') return true;
            i++;
        }
        return false;
    }
    bool getEndOfAttributeName(int &i, string &content){
        while(i < content.size()){
            if(content[i] == '<') return false;
            if(isWhiteSpace(content[i])) return true;
            if(content[i] == '=') return true;
            i++;
        }
        return false;
    }
    bool getEndOfProlog(int &i, string &content){
        while(i < content.size()){
            if(content[i] == '<') return false;
            if(content[i] == '?' && content[i+1] == '>'){
                return true;
            }
            i++;
        }
        return false;
    }
    void skipWhiteSpace(int &i, string &content){
        while(i < content.size() && isWhiteSpace(content[i])) i++;
    }
    bool isWhiteSpace(char x){
        return x==' ' or x=='\n' or x=='\t';
    }
};

/**
prolog
inTag
inQuotes
inText
*/

class XMLReader{
public:
    string getXMLfromFile(string path){
        string content = readFile(path);
        content = XMLValidator::validate(content);
        return content;
    }
private:

    string readFile(string path){
        input.open(path.c_str());

        if(input.is_open()){
            string line;
            getline (input, line, '\0');
            input.close();
            return line;
        }

        input.close();
        return "";
    }

    void writeFile(string path, string data){
        output.open(path.c_str());

        if(output.is_open()){
            output << data;
            output.close();
        }

        input.close();
    }

    ifstream input;
    ofstream output;
};

int main(){
    XMLReader reader;
    string file = reader.getXMLfromFile("worstCase.xml");
    cout << file;

    return 0;
}

// reader.writeFile("newFile.xml", file);
/**
<?xml version="1.0" encoding="UTF-8"?>
<bookstore>
    <book id="1" category="children">
        <title>Harry Potter</title>
        <author>J K. Rowling</author>
        <year>2005</year>
        <price>29.99</price>
    </book>
    <book id="1" category="web">
        <title>Learning XML</title>
        <author>Erik T. Ray</author>
        <year>2003</year>
        <price>39.95</price>
    </book>
</bookstore>
*/
