#include <iostream>
#include <vector>
using namespace std;


class Attribute{

public:
    Attribute(string name, string value){
        this->name = name;
        this->value = value;
    }

public:
	string getName() const {
        return this->name;
	}
	string getValue() const {
        return this->value;
	}
	void setValue(string newValue){
        this->value = newValue;
	}
	string toString(unsigned int tabs = 0) const {
        return " " + this->name + "=\"" + this->value + "\"";
	}

private:
	string name;
	string value;
};

class Element{

public:
    Element(Element* parent){
        this->parent = parent;
    }
public:
    const Element* getParent() const {
        return this->parent;
	}
	virtual string toString(unsigned int tabs = 0) = 0;
public:
    string getKey(){
        return this->key;
    }
private:
	Element* parent;

protected:
    string key;
};

class TextElement: public Element{

public:
    TextElement(string text):Element(this){
        this->key = "&TextElement";
        this->text = text;
    }

	void set(string text){
        this->text = text;
	}
	string toString(unsigned int tabs = 0){

	}

private:
    string text;
};

class CommentElement: public Element{

public:
    CommentElement(string text):Element(this){
        this->key = "&CommentElement";
        this->text = text;
    }
	void set(string text){
        this->text = text;
	}
	string toString(unsigned int tabs = 0){

	}

private:
    string text;
};

class TagElement: public Element{

public:
    TagElement(string key):Element(this){
        this->key = key;
    }
	void addChildElement(Element &element){
        this->childElements.push_back( &element);
	}
	void removeChildElement(unsigned int i){
        this->childElements.erase(this->childElements.begin() + i);
	}
	void removeChildElement(string key){
	    for(int i = 0; i < this->childElements.size(); i++){
            if(this->childElements[i]->getKey() == key){
                this->childElements.erase(this->childElements.begin() + i);
                return;
            }
	    }
	}
    unsigned int numberOfChildElements(){
        return this->childElements.size();
    }
    const Element* childElement(unsigned int i){
        return this->childElements[i];
    }
	void addAttribute(Attribute &attribute){
        this->attributes.push_back( &attribute);
	}
	void removeAttribute(string name){
	    for(int i = 0; i < this->attributes.size(); i++){
            if(this->attributes[i]->getName() == name){
                this->attributes.erase(this->attributes.begin() + i);
                return;
            }
	    }
	}
    unsigned int numberOfAttribute(){
        return this->attributes.size();
    }
    const Attribute* attribute(unsigned int i){
        return this->attributes[i];
    }
	void setText(string text){
	    for(int i = 0; i < this->childElements.size(); i++){
            if(this->childElements[i]->getKey() == "&TextElement"){
                this->childElements.erase(this->childElements.begin() + i);
            }
	    }
	    TextElement t(text);
	    this->addChildElement( t );
	}
	void setComment(string text){
	    for(int i = 0; i < this->childElements.size(); i++){
            if(this->childElements[i]->getKey() == "&CommentElement"){
                this->childElements.erase(this->childElements.begin() + i);
            }
	    }
	    CommentElement c(text);
	    this->addChildElement( c );
	}
    string getId(){
        return this->id;
    }
    void setId(string id){
        this->id = id;
    }
	string toString(unsigned int tabs = 0){

	}
private:
	string id;
    vector< Attribute* > attributes;
    vector< Element* > childElements;
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

    Attribute test1("key", "value");
    cout<< test1.toString() << endl;

    return 0;
}




