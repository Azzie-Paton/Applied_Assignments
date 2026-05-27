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
void Student::add_drop() {
	cout << "add_drop function has been executed" << endl;
}
void Student::print_sch() {
	cout << "print_sch function has been executed" << endl;
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