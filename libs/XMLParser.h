#ifndef __XMLPARSER__
#define __XMLPARSER__

class XMLParser{
public:
	// void commandLineInteface(istream in, ostream out);
	void commandLineInteface();
private:

	bool exitCommand();
	bool openCommand();
	bool newCommand();
	bool closeCommand();
	bool saveCommand();
	bool saveAsCommand();
	bool printCommand();
	bool getCommand();
	bool setCommand();
	bool addCommand();
	bool removeCommand();
	bool XPathCommand();
	bool helpCommand();
	bool notFoundCommand(string);

	string file;
	XMLStructure structure;
	XMLReader reader;
};

#endif // __XMLPARSER__
