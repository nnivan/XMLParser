#ifndef __XMLREADER__
#define __XMLREADER__

class XMLReader{
public:
    string readFile(string path);
    string writeFile(string path, string data);
private:
    ifstream input;
    ofstream output;
};


#endif // __XMLREADER__