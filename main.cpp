#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

#include "libs\XMLAttribute.h"
#include "libs\XMLElement.h"
#include "libs\XMLStructure.h"
#include "libs\XMLValidator.h"
#include "libs\XMLReader.h"

int main(){
    // getValidXMLStructureFromString
    XMLStructure xmls;
    string path, fileContent, errormsg;
    XMLReader reader;
    fileContent = reader.readFile("worstCase.xml");
    xmls = XMLValidator::getValidXMLStructureFromString(fileContent, errormsg);
    // cout << xmls.toString() << endl;
    // cout << errormsg << endl;

    vector<XMLElement> v;
    xmls.fillVectorWithElements(xmls.getRootId(), v);

    for(int i = 0; i < v.size(); i++){
        cout << "Element[" << i << "]:" << endl;
        cout << v[i].toString() << endl;
    }
    /*XMLStructure xmls("bookstore");
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

    cout << xmls.toString() << endl;*/

    return 0;
}