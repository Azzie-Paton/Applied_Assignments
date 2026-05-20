#include <iostream>
#include "User.h"
#include "Student.h"
#include "Instructor.h"
#include "Admin.h"
using namespace std;

int main() {
	Student chud("Chud", "Maxx", "420");
	chud.search();
	chud.add_drop();
	chud.print_sch();
	Instructor babu("Babu", "S.", "1221");
	babu.print_class_list();
	babu.print_schedule();
	babu.search_course();
	Admin prin("Principal", "A.", "987654321");
	prin.add_course();
	prin.remove_course();
	prin.add_remove_student();
	prin.add_remove_user();
	prin.print_courses();
	prin.print_roster();
	prin.search_courses();
	prin.search_roster();
	return 0;
}