#include <iostream>
#include <vector>
using namespace std;

struct Attribute{
public:
    Attribute(string name, string value){
        this->doSetName(name);
        this->doSetValue(value);
    }

    set(string name, string value){
        this->doSetName(name);
        this->doSetValue(value);
    }

    void setName(string name){
        this->doSetName(name);
    }
    void setValue(string value){
        this->doSetValue(value);
    }

    string getName() const {
        return this->name;
    }
    string getValue() const {
        return this->value;
    }

	string toString() const {
        return " " + name + "=\"" + value + "\"";
	}

private:

    void doSetName(string name){
        this->name = name;
    }

    void doSetValue(string value){
        this->value = value;
    }

	string name;
	string value;
};

struct Element{

    Element(string key = "", string text = "", string id = ""){
        doSetKey(key);
        doSetText(text);
        doSetId(id);
    }


    void set(string key, string text){
        doSetKey(key);
        doSetText(text);
    }

    void set(string key, string text, string id){
        doSetKey(key);
        doSetText(text);
        doSetId(id);
    }

    void setKey(string key){
        doSetKey(key);
    }
    void setText(string text){
        doSetText(text);
    }
    void setId(string id){
        doSetId(id);
    }

    string getKey() const {
        return this->key;
    }
    string getText() const {
        return this->text;
    }
    string getId() const {
        return this->id;
    }

    void addChildElement(string key, string text = "", string id = ""){
        this->childElements.push_back( Element(key, text, id) );
    }
    int sizeChildElement() const {
        return this->childElements.size();
    }
    Element* getChildElement(int i) {
        return &(this->childElements[i]);
    }
    void removeChildElement(int i) {
        this->childElements.erase(this->childElements.begin() + i);
    }

    void addAttribute(string name, string value = ""){
        if(name == "id") return;
        this->attributes.push_back( Attribute(name, value) );
    }
    int sizeAttribute() const {
        return this->attributes.size();
    }
    Attribute* getAttribute(int i) {
        return &(this->attributes[i]);
    }
    void removeAttribute(int i) {
        this->attributes.erase(this->attributes.begin() + i);
    }

	string toString(int tabs = 0) const {
        string ret = "";

        /// (\t<KEY id="ID")
		ret += this->tabsString(tabs);
		ret += "<";
        ret += this->key;
        ret += " id=\"";
        ret += this->id;
        ret += "\"";

        /// n:( NAME="VALUE")
        for(int i = 0; i < this->attributes.size(); i++){
            ret += this->attributes[i].toString();
        }

        if(this->childElements.empty()){
            /// (>TEXT</KEY>)
            ret += ">";
            ret += this->text;
            ret += "</";
            ret += this->key;
            ret += ">";
            return ret;
        }

        /// (>TEXT\n)
        ret += ">";
        ret += this->text;
        ret += "\n";

        /// n:(CHILDELEMENT\n)
        for(int i = 0; i < this->childElements.size(); i++){
        	ret += this->childElements[i].toString(tabs+1);
            ret += "\n";
        }

        /// (\t</KEY>)
		ret += this->tabsString(tabs);
        ret += "</";
        ret += this->key;
        ret += ">";

        return ret;
	}


private:

    void doSetKey(string key){
        if(key == "") key = "unknown";
        this->key = key;
    }
    void doSetText(string text){
        this->text = text;
    }
    void doSetId(string id){
        this->id = id;
    }

	string tabsString(int tabs) const {
		string ret = "";
		for(int i = 0; i < tabs; i++){
			ret += "\t";
		}
		return ret;
	}

	string id;
	string key;
	string text;
	vector<Attribute> attributes;
	vector<Element> childElements;
};

class XMLStructure{
public:
    XMLStructure(string key = "root", string id = "0"){
        this->root = Element(key, "", id);
        this->usedId.push_back("");
        this->usedId.push_back("0");
    }

	string addElement(string id, string newKey, string newText = "", string newId = "0"){
	    Element* temp = this->findElement(id);
        if(temp){
            newId = this->getFreeId(newId);
            this->usedId.push_back(newId);
            temp->addChildElement( newKey, newText, newId );
            return newId;
        }
        return "";
	}
	bool removeElement(string id){
        Element* temp = findElementParent(id);
        if(temp){
            for(int i = 0; i < temp->sizeChildElement(); i++){
                if(temp->getChildElement(i)->getId() == id){
                    temp->removeChildElement(i);
                    return true;
                }
            }
        }
        return false;
	}

	void setText(string id, string text){
        Element* temp = findElement(id);
        if(temp){
            temp->setText(text);
        }
	}
	string getText(string id){
        Element* temp = findElement(id);
        return temp->getText();
	}

	void addAttribute(string id, string name, string value){
	    Element* temp = this->findElement(id);
        if(temp){
            temp->addAttribute( name, value );
        }
	}
	void setAttributeValue(string id, string name, string value){
	    Element* temp = this->findElement(id);
        if(temp){
            for(int i = 0; i < temp->sizeAttribute(); i++){
                if(temp->getAttribute(i)->getName() == name){
                    temp->getAttribute(i)->setValue(value);
                }
            }
        }
	}
	bool removeAttribute(string id, string attributeName){
	    Element* temp = this->findElement(id);
        if(temp){
            for(int i = 0; i < temp->sizeAttribute(); i++){
                if(temp->getAttribute(i)->getName() == attributeName){
                    temp->removeAttribute(i);
                    return true;
                }
            }
        }
        return false;
	}

	void addPrologAttribute(string name, string value){
        this->prolog.push_back( Attribute(name, value) );
	}

    int sizePrologAttribute() const {
        this->prolog.size();
    }
    Attribute* getPrologAttribute(int i) {
        return &(this->prolog[i]);
    }

	void removePrologAttribute(string name){
        for(int i = 0; i < this->prolog.size(); i++){
            if(this->prolog[i].getName() == name){
                this->prolog.erase(this->prolog.begin() + i);
                return;
            }
        }
	}

	string getRootId() const {
        return this->root.getId();
	}

	const Element getRootElement() const {
        return this->root;
	}
	string toString() const {
        string ret = "";
        if(this->prolog.size()){
            ret += "<?xml";
            for(int i = 0; i < this->prolog.size(); i++){
                ret += this->prolog[i].toString();
            }
            ret += "?>\n";
        }
        ret += this->root.toString();
        return ret;
	}

private:

    Element* findElementRecursive(Element* current, string id){

		if(current->getId() == id) return current;

		for(int i = 0; i < current->sizeChildElement(); i++){

			Element* temp = findElementRecursive( current->getChildElement(i), id);
			if(temp){
				return temp;
			}
		}
		return nullptr;
	}

	Element* findElement(string id){
		return findElementRecursive( &this->root, id);
	}

	Element* findElementParentRecursive(Element* current, string id){

		for(int i = 0; i < current->sizeChildElement(); i++){
            if(current->getChildElement(i)->getId() == id){
                return current;
            }
			Element* temp = findElementRecursive( current->getChildElement(i), id);
			if(temp){
				return temp;
			}
		}
		return nullptr;
	}

	Element* findElementParent(string id){
		return findElementParentRecursive( &this->root, id);
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

	vector<Attribute> prolog;
	vector<string> usedId;
    Element root;

};

int main () {

    XMLStructure xmls("bookstore");
    xmls.addPrologAttribute("version", "1.0");
    xmls.addPrologAttribute("encoding", "UTF-8");

    string id;
    id = xmls.addElement(xmls.getRootId(), "book", "", "1");
    xmls.addAttribute(id, "category", "kids");
    xmls.addElement(id, "title", "");
    xmls.addElement(id, "author", "J K. Rowling");
    xmls.addElement(id, "year", "2005");
    xmls.addElement(id, "price", "29.99");
    id = xmls.addElement(xmls.getRootId(), "book", "", "2");
    xmls.addAttribute(id, "category", "web");
    xmls.addElement(id, "title", "Learning XML");
    xmls.addElement(id, "author", "Erik T. Ray");
    xmls.addElement(id, "year", "2003");
    xmls.addElement(id, "price", "39.95");

    xmls.setAttributeValue("1", "category", "children");

    cout << xmls.toString() << endl;

    return 0;
}
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
