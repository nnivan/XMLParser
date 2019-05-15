#ifndef __XMLREADER__
#define __XMLREADER__

class XMLReader{
public:
    string getXMLfromFile(string path);
private:
    string readFile(string path);
    void writeFile(string path, string data);
    ifstream input;
    ofstream output;
};


#endif // __XMLREADER__