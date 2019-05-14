#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

struct XMLValidator{

public:

    static string getValidatedString(string content){
        XMLValidator valid;
        cout << valid.removeSpaces(content);
        content = valid.removeComments(content);
        if(!valid.validateBrackets(content)) return "error";
        content = valid.removeSpaces(content);
        return content;
    }

private:

    bool isWhiteSpace(char x){
        return x==' ' or x=='\t' or x=='\n';
    }

    int nextCharecter(int i, string content){
        for(i = i + 1;i < content.size(); i++){
            if(!isWhiteSpace(content[i])){
                return i;
            }
        }
    }

    string removeSpaces(string content){
        content += "&";
        string ret;
        bool inBrackets = false;
        bool inString = false;
        for(int i = 0; i < content.size() - 1; i++){
            if(content[i] == '<' or content[i] == '>') inBrackets = !inBrackets;
            if(content[i] == '\"') inString = !inString;
            if(inString){
                ret += content[i];
            }else{
                if(inBrackets){
                    int j = nextCharecter(i, content);
                    if(isWhiteSpace(content[i])){
                        content[i] = ' ';
                    }
                    if(content[i] == '<'){
                        ret += content[i];
                        i = j - 1;
                        continue;
                    }
                    if(content[i] == '?'){
                        ret += content[i];
                        i = j - 1;
                        continue;
                    }
                    if(content[j] == '>'){
                        ret += content[i];
                        i = j - 1;
                        continue;
                    }
                    if(content[j] == '?'){
                        ret += content[i];
                        i = j - 1;
                        continue;
                    }
                    if(content[i] == ' ' and isWhiteSpace(content[i+1])) continue;
                    if(content[i] == ' ' and content[i+1] == '=') continue;
                    if(content[i] == '='){
                        ret += content[i];
                        i = j - 1;
                        continue;
                    }
                    ret += content[i];
                }else{
                    if(!isWhiteSpace(content[i])){
                        ret += content[i];
                        if(content[i] == '>'){
                            i = nextCharecter(i, content) - 1;
                            continue;
                        }
                    }else{
                        int j = nextCharecter(i, content);
                        if(content[j] == '<'){
                            i = j - 1;
                        }else{
                            ret += content[i];
                        }
                    }
                }
            }
        }
        return ret;
    }

    bool validateBrackets(string content){
        bool inBrackets = false;
        bool inString = false;
        for(int i = 0; i < content.size(); i++){
                 if(content[i] == '<' and !inBrackets and !inString) inBrackets = true;
            else if(content[i] == '>' and  inBrackets and !inString) inBrackets = false;
            else if(content[i] == '>' and  inBrackets and  inString) return false;
            else if(content[i] == '<' and !inBrackets and  inString) return false;
            else if(content[i] == '<' and  inBrackets) return false;
            else if(content[i] == '>' and !inBrackets) return false;
            else if(content[i] == '"' and  inBrackets) inString = !inString;
            else if(content[i] == '"' and !inBrackets) return false;
        }
        return !inBrackets;
    }

    string removeComments(string content){
        if(validateBrackets(content)) return "";
        int startIndex, endIndex;
        while(1){
            startIndex = content.find("<!--");
            endIndex = content.find("-->", startIndex + 4);
            if(startIndex == string::npos){
                if(endIndex == string::npos){
                    return content;
                }else{
                    // XMLValidator::ERROR += "has -->, but not <!--\n";
                    return ">>>";
                }
            }
            if(endIndex == string::npos){
                // XMLValidator::ERROR += "has <!--, but not -->\n";
                return ">>>";
            }
            content.erase(startIndex, endIndex - startIndex + 3);
        }
    }

};

class XMLReader{
public:
    string getXMLfromFile(string path){
        string content = readFile(path);
        content = XMLValidator::getValidatedString(content);
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
