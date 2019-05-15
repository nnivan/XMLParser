#ifndef __XMLATTRIBUTE__
#define __XMLATTRIBUTE__

struct Attribute{
public:
    Attribute(string name, string value);

    void set(string name, string value);
    void setName(string name);
    void setValue(string value);

    string getName() const;
    string getValue() const;
    string toString() const;
private:
    void doSetName(string name);
    void doSetValue(string value);
    string name;
    string value;
};

#endif // __XMLATTRIBUTE__
