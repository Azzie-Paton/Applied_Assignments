#pragma once
#include <string>
#include <iostream>
using namespace std;

class User
{
private:
	string fname;
	string lname;
	string id;
public:
	//User();
	User(string in_first, string in_last, string in_id);
	virtual string getFirst();
	virtual string getLast();
	virtual string getId();
	virtual void printInfo();
	
};

