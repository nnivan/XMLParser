#ifndef __MYSTACK__
#define __MYSTACK__

struct StackStringNode{
	string data;
	StackStringNode* last;	
};

class StackString{
public:
	StackString();
	StackString(const StackString& other);
	StackString& operator=(const StackString& other);
	~StackString();

	bool empty();
	void push(string data);
	bool pop();
	string top();

private:
	void doCopy(const StackString& other);
	void doCopyRecursive(StackStringNode* node);
	void doDelete();
	void doAddNode(string data);
	void doDeleteTopNode();

	StackStringNode* current;
};

#endif // __MYSTACK__
