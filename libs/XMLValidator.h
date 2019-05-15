#ifndef __XMLVALIDATOR__
#define __XMLVALIDATOR__

class XMLValidator{
public:
    static string validate(string content);
private:
    bool getText(int &i, string &content, string &text);
    bool getStartOfTag(int &i, string &content);
    bool getEndOfTag(int &i, string &content);
    bool getTagName(int &i, string &content, string &key);
    bool getEndOfTagName(int &i, string &content);
    bool getAttribute(int &i, string &content, string &name, string &value);
    bool getEndOfQuote(int &i, string &content);
    bool getEndOfAttributeName(int &i, string &content);
    bool getEndOfProlog(int &i, string &content);
    void skipWhiteSpace(int &i, string &content);
    bool isWhiteSpace(char x);
};

#endif // __XMLVALIDATOR__