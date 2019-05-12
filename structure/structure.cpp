#include <iostream>
#include <vector>
using namespace std;

struct Attribute{

    Attribute(string name, string value){
        this->name = name;
        this->value = value;
    }

	string name;
	string value;

	string toString() const {
		return " " + name + "=\"" + value + "\"";
	}
};

class Element{
public:

	Element(Element* parent, string _key): key( _key ){
		this->parent = parent;
	}

	Element* getParent() const {
		return this->parent;
	}

	virtual string toString(unsigned int) const = 0;

	const string key;

private:

	Element* parent;
};

class TagElement: public Element{
public:
    TagElement(string key, string id): Element(this, key){
    	this->id = id;
    }

	string toString(unsigned int tabs = 0) const {

		string ret = "";

		ret += tabsString(tabs);

		ret += "<";
        ret += this->key;


        ret += " id=\"";
        ret += this->id;
        ret += "\"";


        for(int i = 0; i < this->attributes.size(); i++){
            ret += this->attributes[i]->toString();
        }


        if(this->childElements.size() == 0){
            ret += " />\n";
            return ret;
        }

        ret += ">\n";

        for(int i = 0; i < this->childElements.size(); i++){
        	ret += this->childElements[i]->toString(tabs+1);
            ret += "\n";
        }


		ret += tabsString(tabs);
        ret += "</";
        ret += this->key;
        ret += ">";

        return ret;
	}

	string id;
	vector < Attribute* > attributes;
	vector < Element* > childElements;

private:
	string tabsString(unsigned int tabs) const {
		string ret = "";
		for(int i = 0; i < tabs; i++){
			ret += "\t";
		}
		return ret;
	}
};

class TextElement: public Element{
public:
    TextElement(string text = ""): Element(this, "&TextElement"){
        this->text = text;
    }

    string toString(unsigned int tabs = 0) const {
	    string ret = "";
	    for(int i = 0; i < tabs; i++){
            ret += "\t";
	    }
	    ret += text;
		return ret;
	}

	string text;
};

class CommentElement: public Element{
public:
    CommentElement(string text = ""): Element(this, "&CommentElement"){
        this->text = text;
    }

	string toString(unsigned int tabs = 0) const {
	    string ret = "";
	    for(int i = 0; i < tabs; i++){
            ret += "\t";
	    }
	    ret += "<--" + text + "-->";
		return ret;
	}

	string text;
};

class XMLStructure{
public:

	const TagElement getRootElement() const{
		return this->root;
	}

	void addComment(string id){

	}

	void addText(string id){

	}

	void removeComment(string id, string text){

	}

	void removeText(string id, string text){

	}

	void addAttribute(string id, string attributeName, string attributeValue){

	}

	void removeAttribute(string id, string attributeName){

	}

	string toString() const{
		return "not implemented yet";
	}


	vector < Attribute* > prolog;

private:

	TagElement* findElementRecursive(TagElement* current, string id){

		if(current->id == id) return current;

		for(int i = 0; i < current->childElements.size(); i++){
			if(current->childElements[i]->key == "&TextElement") continue;
			if(current->childElements[i]->key == "&CommentElement") continue;

			TagElement* temp = findElementRecursive( (TagElement*) current->childElements[i], id);
			if(temp){
				return temp;
			}
		}
		return nullptr;
	}

	TagElement* findElement(string id){
		if(this->root.id == id){
			return &(this->root);
		}
		return findElementRecursive(&root, id);
	}

	vector < string > usedId;
	TagElement root;
};

int main () {
    TagElement root("root", "0");

    TagElement note1("note", "1");
    TagElement note2("note", "2");

    TextElement text("First note");
    CommentElement comment("This is just a comment");

    note1.childElements.push_back( &text );
    note2.childElements.push_back( &comment );

    note1.attributes.push_back( new Attribute("date", "12.05.2019") );
    note2.attributes.push_back( new Attribute("date", "18.05.2019") );

    root.childElements.push_back( &note1 );
    root.childElements.push_back( &note2 );


    cout<< root.toString() << endl;

    // cout << root.toString() << endl;
    return 0;
}
/**
<root id="0">
	<note id="1" date="12.05.2019">
		First note
    </note>
	<note id="2" date="18.05.2019">
		<--This is just a comment-->
	</note>
</root>
*/
