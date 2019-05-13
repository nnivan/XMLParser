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
            ret += " />";
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
    XMLStructure(string key, string id){
        this->root = new TagElement(key, id);
    }

	const TagElement getRootElement() const {
		return *(this->root);
	}

	void addElement(string id, string newKey, string newId){
        newId = this->getFreeId(newId);
        this->usedId.push_back(newId);
        TagElement newElement(newKey, newId);
	    TagElement* temp = this->findElement(id);
        if(temp){
            temp->childElements.push_back( new TagElement(newKey, newId) );
        }
	}

	void removeElement(string id){

	}

	void addComment(string id, string text){

	}

	void removeComment(string id){

	}

	void addText(string id, string text){

	}

	void removeText(string id){

	}

	void addAttribute(string id, string attributeName, string attributeValue){

	}

	void removeAttribute(string id, string attributeName){

	}

	string toString() const{
	    string ret = "";
        if(prolog.size() != 0){
            ret += "<?xml";
            for(int i = 0; i < prolog.size(); i++){
                ret += prolog[i]->toString();
            }
            ret += "xml?>\n";
        }
	    ret += this->root->toString();
		return ret;
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
		if(this->root->id == id){
			return this->root;
		}
		return findElementRecursive(root, id);
	}

	string getFreeId(string id){
	    if(isIdFree(id)) return id;
	    int i = 1;
	    while(1){
            if(isIdFree(id + "_" + to_string(i)))
                return id + "_" + to_string(i);
            i++;
	    }
	}

	bool isIdFree(string id){
	    for(int i = 0; i < usedId.size(); i++){
            if(usedId[i] == id){
                return false;
            }
	    }
	    return true;
	}

	vector < string > usedId;
	TagElement* root;
};

int main () {

    TextElement text("just a test");

    XMLStructure structure("letter", "0");
    structure.addElement("0", "note", "1");
    structure.addElement("1", "from", "1");
    structure.addElement("1", "to", "1");
    structure.addElement("0", "note", "2");

    TagElement pesho = structure.getRootElement();
    pesho.childElements.push_back( &text);

    cout<< pesho.toString() << endl;

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
