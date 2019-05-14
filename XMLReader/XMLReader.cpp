#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

struct XMLValidator{

public:

    static string getValidatedString(string content){
        XMLValidator valid;
        content = valid.removeComments(content);
        if(content == "error") return "error";
        if(!valid.validateBrackets(content)) return "error";
        content = valid.removeSpaces(content);
        if(content == "error") return "error";
        if(valid.endOfProlog(content) == -1) return "error";
        content = valid.fixSingleLineTags(content);
        if(!valid.validateTags(content)) return "error";
        if(!valid.validateAttributes(content)) return "error";
        return content;
    }

private:

    bool validateAttributes(string content){
        int j = endOfProlog(content);
        if(j!=0){
            int lastAttributeEnd = 5;
            string currentTag = content.substr(0, j);
            while(currentTag[lastAttributeEnd] != '?'){
                string currentAttribute = getNextAttribute(lastAttributeEnd + 1, currentTag, true);
                if(currentAttribute == "error") return false;
                lastAttributeEnd += currentAttribute.size() + 1;
            }
        }
        for(int i = j; i < content.size(); i++){
            if(content[i] == '<'){
                string currentTag = getTagContent(i, content);
                string currentTagName = getTagName(i, content);
                int lastAttributeEnd = currentTagName.size() + 1;
                while(currentTag[lastAttributeEnd] != '>'){
                    string currentAttribute = getNextAttribute(lastAttributeEnd + 1, currentTag);
                    if(currentAttribute == "error") return false;
                    lastAttributeEnd += currentAttribute.size() + 1;
                }
            }
        }
        return true;
    }

    string getNextAttribute(int j, string content, bool isProlog = false){
        string ret = "";
        for(int i = j; i < content.size(); i++){
            ret+=content[i];
            if(content[i] == ' '){
                return "error";
            }
            if(content[i] == '='){
                j = i + 1;
                break;
            }
        }
        if(content[j]!='"') return "error";
        ret += content[j];
        for(int i = j + 1; i < content.size(); i++){
            ret += content[i];
            if(content[i] == '"'){
                if(isProlog){
                    if(content[i+1] != ' ' && content[i+1] != '?') return "error";
                }else{
                    if(content[i+1] != ' ' && content[i+1] != '>') return "error";
                }
                return ret;
            }
        }
        return "error";
    }

    string fixSingleLineTags(string content){
        int j = endOfProlog(content);
        string ret = content.substr(0, j);
        for(int i = j; i < content.size(); i++){
            if(content[i] == '<'){
                string current = getTagContent(i, content);
                if(current[current.size()-2] == '/'){
                    current.erase(current.size()-2, 1);
                    ret+=current;
                    ret+="</" + getTagName(0, current) + ">";
                }else{
                    ret+=current;
                }
            }
        }
        return ret;
    }

    bool validateTags(string content){
        int j = endOfProlog(content);
        stack<string> tags;
        for(int i = j; i < content.size(); i++){
            if(content[i] == '<'){
                string currentTagName = getTagName(i, content);
                if(currentTagName[0] == '/'){
                    if(tags.empty()) return false;
                    if(tags.top() != currentTagName.substr(1, currentTagName.size()- 1)){
                        return false;
                    }
                    tags.pop();
                }else{
                    tags.push(currentTagName);
                }
            }
        }
        return tags.empty();
    }

    string getTagName(int j, string content){
        for(int i = j + 1; i < content.size(); i++){
            if(isWhiteSpace(content[i]))
                return content.substr(j + 1, i - j - 1);
            if(content[i] == '>')
                return content.substr(j + 1, i - j - 1);
        }
        return content.substr(j + 1, content.size() - j - 1);
    }

    string getTagContent(int j, string content){
        for(int i = j + 1; i < content.size(); i++){
            if(content[i] == '<')
                return content.substr(j, i - j);
        }
        return content.substr(j, content.size() - j);
    }

    int endOfProlog(string content){
        if(content[1] != '?'){
            return 0;
        }else{
            if( content[0] != '<' ||
                content[1] != '?' ||
                content[2] != 'x' ||
                content[3] != 'm' ||
                content[4] != 'l' ||
                content[5] != ' '){
                return -1;
            }
            bool inString = false;
            for(int i = 0; i < content.size(); i++){
                if(content[i] == '\"') inString = !inString;
                if(content[i] == '>') return -1;
                if(content[i] == '?' && content[i + 1] == '>' && !inString) return i + 2;
            }
        }
    }

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
    int nextWhiteSpace(int i, string content){
        for(i = i + 1;i < content.size(); i++){
            if(isWhiteSpace(content[i])){
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
                    if(content[j] == '/'){
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
        int startIndex, endIndex;
        while(1){
            startIndex = content.find("<!--");
            endIndex = content.find("-->", startIndex + 4);
            if(startIndex == string::npos){
                if(endIndex == string::npos){
                    return content;
                }else{
                    // XMLValidator::ERROR += "has -->, but not <!--\n";
                    return "error";
                }
            }
            if(endIndex == string::npos){
                // XMLValidator::ERROR += "has <!--, but not -->\n";
                return "error";
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
