#include "Course.h"

Course::Course(int in_CRN, string in_title, string in_depart,string in_time, string in_days, string in_sem, int in_year, int in_credits)
{
	CRN = in_CRN;
	title = in_title;
	department = in_depart;
	time = in_time;
	days = in_days;
	semester = in_sem;
	year = in_year;
	credits = in_credits;
}

int Course::getCRN()
{
	return CRN;
}

string Course::getTitle() {
	return title;
}

string Course::getDepartment() {
	return department;
}

string Course::getTime() {
	return time;
}

string Course::getDays() {
	return days;
}

string Course::getSemester() {
	return semester;
}

int Course::getYear() {
	return year;
}

int Course::getCredits() {
	return credits;
}