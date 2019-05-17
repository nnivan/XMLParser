#ifndef __XMLXPath__
#define __XMLXPath__

class XMLXPath{
public:
	static string XPath(string command, string id, XMLStructure &xml);
private:
	void removeElementsByTag(vector<XMLElement> &elements, string tagName);
	void removeElementsByIndex(vector<XMLElement> &elements, int index);
	void removeElementsByAttribute(vector<XMLElement> &elements, string dataName);
	void removeElementsByData(vector<XMLElement> &elements, string dataName, string dataValue);

	bool skipToChar(int &i, char x, string content, string &data);
	bool skipToXPathSing(int &i, string content, string &data);
	bool stringToInt(string data, int &index);
};

#endif // __XMLXPath__
