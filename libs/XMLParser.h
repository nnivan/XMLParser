#ifndef __XMLPARSER__
#define __XMLPARSER__

class XMLParser{
public:
	// void commandLineInteface(istream in, ostream out);
	void commandLineInteface();
private:
	string file;
	XMLStructure structure;
	XMLReader reader;
};

#endif // __XMLPARSER__
