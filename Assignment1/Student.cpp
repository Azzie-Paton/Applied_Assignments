#include "Student.h"
Student::Student(string in_first, string in_last, string in_id):User(in_first, in_last, in_id){
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