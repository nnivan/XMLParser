#ifndef __XMLSTRUCTURE__
#define __XMLSTRUCTURE__


class XMLStructure{
public:
    XMLStructure(string key = "root", string id = "0");
    void setRootValues(string key, string id);
	string addElement(string id, string newKey, string newText = "", string newId = "");
	bool removeElement(string id);
	void setText(string id, string text);
	void appendText(string id, string text);
	string getText(string id);
	void addAttribute(string id, string name, string value);
	void setAttributeValue(string id, string name, string value);
	bool removeAttribute(string id, string attributeName);
	void addPrologAttribute(string name, string value);
    int sizePrologAttribute() const;
    Attribute* getPrologAttribute(int i);
	void removePrologAttribute(string name);
	string getRootId() const;
	const XMLElement getRootElement() const;
	void fillVectorWithElements(string id, vector<XMLElement> &elements);
	string toString() const;
private:
	void fillVectorWithElementsRecursive(XMLElement* current, vector<XMLElement> &elements);
    XMLElement* findElementRecursive(XMLElement* current, string id);
	XMLElement* findElement(string id);
	XMLElement* findElementParentRecursive(XMLElement* current, string id);
	XMLElement* findElementParent(string id);
	string getFreeId(string id);
	bool isIdFree(string id);

	vector<Attribute> prolog;
	vector<string> usedId;
    XMLElement root;

};

#endif // __XMLSTRUCTURE__