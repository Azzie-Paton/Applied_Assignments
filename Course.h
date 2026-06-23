#pragma once
#include <string>

using namespace std;

class Course
{
public:
	int CRN;
	string title;
	string department;
	string time;
	string days;
	string semester;
	int year;
	int credits;

	Course(int in_CRN, string in_title, string in_depart, string in_time, string in_days, string in_sem, int in_year, int in_credits);

	int getCRN();
	string getTitle();
	string getDepartment();
	string getTime();
	string getDays();
	string getSemester();
	int getYear();
	int getCredits();
};

