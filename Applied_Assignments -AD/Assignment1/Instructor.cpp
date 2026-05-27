#include "Instructor.h"
Instructor::Instructor(string in_first, string in_last, string in_id, string in_title, int in_yofH, string in_depart) :User(in_first, in_last, in_id) 
{
	title = in_title;
	yofH = in_yofH;
	department = in_depart;
	email = this->getLast();
	email += (char)tolower(this->getFirst().at(0));
	email += "@wit.edu";
}
void Instructor::print_schedule() {
	cout << "Print_schedule function has been executed" << endl;
}
void Instructor::print_class_list() {
	cout << "Print_class_list function has been executed" << endl;
}
void Instructor::search_course() {
	cout << "Search_course function has been executed" << endl;
}
string Instructor::getTitle() {
	return title;
}

int Instructor::getHireYear() {
	return yofH;
}

string Instructor::getDepartment() {
	return department;
}

string Instructor::getEmail() {
	return email;
}