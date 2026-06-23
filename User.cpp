#include "User.h"
User::User(string in_first, string in_last, string in_id) {
	fname = in_first;
	lname = in_last;
	id = in_id;
}
string User::getFirst() {
	return fname;
}
string User::getLast() {
	return lname;
}
string User::getId() {
	return id;
}
void User::printInfo() {
	cout << "First name is: " << fname << endl;
	cout << "Last name is: " << lname << endl;
	cout << "ID is: " << id << endl;
}
