#include "Admin.h"
Admin::Admin(string in_first, string in_last, string in_id, string in_title, string in_office) :User(in_first, in_last, in_id) 
{
	title = in_title;
	office = in_office;
	email = this->getLast();
	email += (char)tolower(this->getFirst().at(0));
	email += "@wit.edu";
}
void Admin::add_course() {
	cout << "Add_course function has been executed" << endl;
}
void Admin::remove_course() {
	cout << "Remove_course function has been executed" << endl;
}
void Admin::add_remove_user() {
	cout << "Add_remove_user function has been executed" << endl;
}
void Admin::add_remove_student() {
	cout << "Add_remove_student function has been executed" << endl;
}
void Admin::search_roster() {
	cout << "Search_roster function has been executed" << endl;
}
void Admin::print_roster() {
	cout << "print_roster function has been executed" << endl;
}
void Admin::search_courses() {
	cout << "Search_courses function has been executed" << endl;
}
void Admin::print_courses() {
	cout << "print_courses function has been executed" << endl;
}
string Admin::getTitle() {
	return title;
}

string Admin::getOffice() {
	return office;
}

string Admin::getEmail() {
	return email;
}