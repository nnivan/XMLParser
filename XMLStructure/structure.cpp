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

struct Element{

    Element(string key = "", string text = "", string id = ""){
        this->key = key;
        this->text = text;
        this->id = id;
    }

    void addChildElement(Element newElement){
        this->childElements.push_back(newElement);
    }
    void childElementRemove(int i) {
        this->childElements.erase(this->childElements.begin() + i);
    }

    void addAttribute(Attribute newAttribute){
        this->attributes.push_back(newAttribute);
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

	string id;
	string key;
	string text;
	vector<Attribute> attributes;
	vector<Element> childElements;

private:
	string tabsString(int tabs) const {
		string ret = "";
		for(int i = 0; i < tabs; i++){
			ret += "\t";
		}
		return ret;
	}

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
            Element newElement(newKey, newId);
            temp->childElements.push_back( Element(newKey, newText, newId) );
            return newId;
        }
        return "";
	}
	void removeElement(string id){
        Element* temp = findElementParent(id);
        if(temp){
            for(int i = 0; i < temp->childElements.size(); i++){
                if(temp->childElements[i].id == id){
                    temp->childElementRemove(i);
                    return;
                }
            }
        }
	}

	void setText(string id, string text){
        Element* temp = findElement(id);
        if(temp){
            temp->text = text;
        }
	}

	void addAttribute(string id, string name, string value){
	    Element* temp = this->findElement(id);
        if(temp){
            temp->addAttribute( Attribute(name, value) );
        }
	}
	void removeAttribute(string id, string attributeName){
	    Element* temp = this->findElement(id);
        if(temp){
            for(int i = 0; i < temp->attributes.size(); i++){
                if(temp->attributes[i].name == attributeName){
                    temp->removeAttribute(i);
                    i--;
                }
            }
        }
	}

	void addPrologAttribute(string name, string value){
        this->prolog.push_back( Attribute(name, value) );
	}

	void removePrologAttribute(string name){
        for(int i = 0; i < this->prolog.size(); i++){
            if(this->prolog[i].name == name){
                this->prolog.erase(this->prolog.begin() + i);
                return;
            }
        }
	}

	string getRootId() const {
        return this->root.id;
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

	vector<Attribute> prolog;

private:

    Element* findElementRecursive(Element* current, string id){

		if(current->id == id) return current;

		for(int i = 0; i < current->childElements.size(); i++){

			Element* temp = findElementRecursive( &current->childElements[i], id);
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

		for(int i = 0; i < current->childElements.size(); i++){
            if(current->childElements[i].id == id){
                return current;
            }
			Element* temp = findElementRecursive( &current->childElements[i], id);
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

	vector<string> usedId;
    Element root;

};

int main () {

    XMLStructure xmls("bookstore");
    xmls.addPrologAttribute("version", "1.0");
    xmls.addPrologAttribute("encoding", "UTF-8");

    string id;
    id = xmls.addElement(xmls.getRootId(), "book", "", "1");
    xmls.addAttribute(id, "category", "children");
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
