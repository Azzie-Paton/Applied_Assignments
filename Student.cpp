#include "Student.h"
Student::Student(string in_first, string in_last, string in_id, int grad, string m):User(in_first, in_last, in_id){
	gradY = grad;
	major = m;
	email = this->getLast();
	email += (char)tolower(this->getFirst().at(0));
	email += "@wit.edu";
}
void Student::search() {
	cout << "Search function has been executed" << endl;
}

void Student::search(int crn)
{
	cout << "Searched for course " << crn << ":" << endl;
}

void Student::add_drop(int a_d, int crn) 
{
	if (a_d == 0)
	{
		//drop
		cout << "add function has been executed" << endl;
		cout << crn << " was added to your course schedule!" << endl;
	}
	else if (a_d == 1)
	{
		//add
		cout << "drop function has been executed" << endl;
		cout << crn << " was dropped from your course schedule!" << endl;
	}
}
void Student::print_sch() {
	cout << "print_sch function has been executed" << endl;
}
void Student::conflict()
{
	cout << "conflict function has been executed." << endl;
}

string Student::getMajor() {
	return major;
}

int Student::getGradYear() {
	return gradY;
}

string Student::getEmail() {
	return email;
}