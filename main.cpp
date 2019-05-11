#include <iostream>
using namespace std;

class XMLValidator{
public:
	XMLValidator(){
		
	}
	bool read(string path){
		
	}
private:
};

class XMLReader{
public:
	XMLReader(){
		
	}
	bool read(string path){
		
	}
private:
	XMLValidator validator;
};

class XMLParser{
public:
	XMLParser(){
 		
	}
	bool open(string path){
		reader.read(path);
	}
private:
	XMLReader reader;
};

int main(){
	XMLParser parser;

	parser.open("first.xml");
}