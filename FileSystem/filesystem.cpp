#include <iostream>
#include <fstream>

using namespace std;

class FileSystem{
public:
    string readFromFile(string path){
        input.open(path.c_str());

        if( !input ){
            return "";
        }

        string line;
        getline (input, line, '\0');

        input.close();
        return line;
    }

    void writeToFile(string path, string data){
        output.open(path.c_str());

        if( !output ){
            return;
        }

        output << data;

        output.close();
        return;
    }

private:
    ifstream input;
    ofstream output;
};

int main(){
    FileSystem f;
    string s = f.readFromFile("xml.xml");
    f.writeToFile("xml2.xml", s);
    cout << s;
    return 0;
}
