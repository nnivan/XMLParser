#ifndef __XMLELEMENT__
#define __XMLELEMENT__

class XMLElement{
public:
    XMLElement(string key = "", string text = "", string id = "");
    void setRootValues(string key, string id);
    void set(string key, string text);
    void set(string key, string text, string id);
    void setKey(string key);
    void setText(string text);
    void setId(string id);
    string getKey() const;
    string getText() const;
    string getId() const;
    void addChildElement(string key, string text = "", string id = "");
    int sizeChildElement() const;
    XMLElement* getChildElement(int i);
    void removeChildElement(int i);
    void addAttribute(string name, string value = "");
    int sizeAttribute() const;
    Attribute* getAttribute(int i);
    void removeAttribute(int i);
	string toString(int tabs = 0) const;
private:
    void doSetKey(string key);
    void doSetText(string text);
    void doSetId(string id);
	string tabsString(int tabs) const;
	string id;
	string key;
	string text;
	vector<Attribute> attributes;
	vector<XMLElement> childElements;
};

#endif // __XMLELEMENT__