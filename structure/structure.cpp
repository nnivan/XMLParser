#include <iostream>
#include <vector>
using namespace std;


class Attribute{

public:
	string getName(){

	}
	string getValue(){

	}
	void setValue(string newValue){

	}
	string toString(unsigned int tabs = 0){

	}

private:
	string name;
	string value;
};

class Element{

public:
	void remove(){

	}
	virtual string toString(unsigned int tabs = 0) = 0;

private:
	Element* parent;
};

class TagElement: public Element{

public:
	void addChildElement(Element &element){

	}
	void removeChildElement(string id){

	}
	void addAttribute(Attribute &attribute){

	}
	void removeAttribute(string name){

	}
	void setText(string text){

	}
	void setComment(string text){

	}
	string toString(unsigned int tabs = 0){

	}

private:
	unsigned int id;
    string key;
    vector< Attribute* > attributes;
    vector< Element* > childElements;
};

class TextElement: public Element{

public:
	void set(string text){

	}
	string toString(unsigned int tabs = 0){

	}

private:
    string text;
};

class CommentElement: public Element{

public:
	void set(string text){

	}
	string toString(unsigned int tabs = 0){

	}

private:
    string text;
};

class XMLStructure{

public:
	void addPrologAttribute(Attribute &attribute){

	}
	void setPrologAttributeValue(string name, string newValue){

	}
	bool hasPrologAttributeValue(string name){

	}
	string getPrologAttributeValue(string name){

	}
	void removePrologAttributeValue(string name){

	}

	void addAttribute(string id, Attribute &attribute){

	}
	void setAttributeValue(string id, string name, string newValue){

	}
	bool hasAttributeValue(string id, string name){

	}
	string getAttributeValue(string id, string name){

	}
	void removeAttributeValue(string id, string name){

	}

	const TagElement getElement(string id){

	}
	const TagElement gerRootElement(){

	}

	void addElement(string id, Element &element){

	}
	void removeElement(string id){

	}

	void setText(string id, string text){

	}
	void removeText(string id){

	}

	void setComment(string id, string text){

	}
	void removeComment(string id){

	}

	string toString(unsigned int tabs = 0){

	}

private:
	vector< string > usedId;
    vector< Attribute* > prolog;
    TagElement root;
};

int main () {

    return 0;
}




