#include "Instructor.h"
Instructor::Instructor(string in_first, string in_last, string in_id) :User(in_first, in_last, in_id) {
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