#include <iostream>
#include <vector>
using namespace std;

struct Attribute{
    string name;
    string value;

    Attribute(string name = "", string value = ""){
        this->name = name;
        this->value = value;
    }

    string toString() const {
        return " " + name + "=\"" + value + "\"";
    }
};

struct Element{
    string key;
    string text;
    vector<Attribute*> attributes;
    vector<Element*> elements;

    Element(string key = "", string text = ""){
        this->key = key;
        this->text = text;
    }

    void add_attribute(Attribute &attribute){
        this->attributes.push_back(&attribute);
    }

    void add_element(Element &element){
        this->elements.push_back(&element);
    }

    string toString(int tabs = 0) const {
        string ret;

        for(int i = 0; i < tabs; i++) ret += "\t";

        ret += "<" + key;

        for(int i = 0; i < attributes.size(); i++)
            ret += attributes[i]->toString();

        ret += ">\n";

        if(text != ""){
            for(int i = 0; i <= tabs; i++) ret += "\t";
            ret += text + "\n";
        }

        for(int i = 0; i < elements.size(); i++)
            ret += elements[i]->toString(tabs + 1);

        for(int i = 0; i < tabs; i++) ret += "\t";

        ret += "</" + key + ">\n";

        return ret;
    }
};

class XMLStructure{
public:
    string toString(Element &element, int tabs = 0) const {

        string ret;

        if(this->meta.size() != 0){
            for(int i = 0; i < tabs; i++) ret += "\t";

            ret += "<?xml";

            for(int i = 0; i < this->meta.size(); i++)
                ret += this->meta[i].toString();

            ret += "?>\n";
        }

        for(int i = 0; i < tabs; i++) ret += "\t";

        ret += "<" + element.key;

        for(int i = 0; i < element.attributes.size(); i++)
            ret += element.attributes[i]->toString();

        ret += ">\n";

        if(element.text != ""){
            for(int i = 0; i <= tabs; i++) ret += "\t";
            ret += element.text + "\n";
        }

        for(int i = 0; i < element.elements.size(); i++)
            ret += element.elements[i]->toString(tabs + 1);

        for(int i = 0; i < tabs; i++) ret += "\t";

        ret += "</" + element.key + ">\n";

        return ret;
    }

    string toString(){
        return toString( this->root );
    }


    const Element rootElement(){
        return this->root;
    }

public:

    Element root;
    vector<Attribute> meta;
};

int main (){

    Element root("people");
    Element person1("person");
    Element person2("person");
    Element name1("name", "Alice");
    Element name2("name", "Bob");
    Attribute gender1("gender", "female");
    Attribute gender2("gender", "male");

    person1.add_attribute( gender1 );
    person2.add_attribute( gender2 );

    root.add_element(person1);
    root.add_element(person2);

    person1.add_element(name1);
    person2.add_element(name2);

    XMLStructure xmls;

    xmls.root = root;
    xmls.meta.push_back(Attribute("version", "1.0"));
    xmls.meta.push_back(Attribute("encoding", "UTF-8"));


    cout << xmls.toString() << endl;
    cout << "sizeof(xmls): " << sizeof(xmls) << endl;
    return 0;
}
/**
<people>
    <person gender="female">
        <name>Alice</name>
    </person>
    <person gender="male">
        <-- Just for testing -->
        <name>Bob</name>
    </person>
<people>
*/
