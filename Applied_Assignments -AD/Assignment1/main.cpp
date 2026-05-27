#include <iostream>
#include "User.h"
#include "Student.h"
#include "Instructor.h"
#include "Admin.h"
#include "Course.h"
#include "sqlite3.h"

using namespace std;

void executeSQL(sqlite3* DB, string sql);

void insertStudent(sqlite3* DB, Student s)
{
	string sql =
		"INSERT INTO STUDENT VALUES (" +
		s.getId() + ", '" +
		s.getFirst() + "', '" +
		s.getLast() + "', " +
		to_string(s.getGradYear()) + ", '" +
		s.getMajor() + "', '" +
		s.getEmail() + "');";

	executeSQL(DB, sql);
}

void insertInstructor(sqlite3* DB, Instructor i)
{
	string sql =
		"INSERT INTO INSTRUCTOR VALUES (" +
		i.getId() + ", '" +
		i.getFirst() + "', '" +
		i.getLast() + "', '" +
		i.getTitle() + "', " +
		to_string(i.getHireYear()) + ", '" +
		i.getDepartment() + "', '" +
		i.getEmail() + "');";

	executeSQL(DB, sql);
}

void insertAdmin(sqlite3* DB, Admin a)
{
	string sql =
		"INSERT INTO ADMIN VALUES (" +
		a.getId() + ", '" +
		a.getFirst() + "', '" +
		a.getLast() + "', '" +
		a.getTitle() + "', '" +
		a.getOffice() + "', '" +
		a.getEmail() + "');";

	executeSQL(DB, sql);
}

void insertCourse(sqlite3* DB, Course c)
{
	string sql =
		"INSERT INTO COURSE VALUES (" +
		to_string(c.getCRN()) + ", '" +
		c.getTitle() + "', '" +
		c.getDepartment() + "', '" +
		c.getTime() + "', '" +
		c.getDays() + "', '" +
		c.getSemester() + "', " +
		to_string(c.getYear()) + ", " +
		to_string(c.getCredits()) + ");";

	executeSQL(DB, sql);
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << azColName[i] << ": "
			<< (argv[i] ? argv[i] : "NULL") << endl;
	}

	cout << "-----------------------------" << endl;
	return 0;
}

void executeSQL(sqlite3* DB, string sql)
{
	char* messageError;

	int exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

	if (exit != SQLITE_OK)
	{
		cerr << "Error: " << messageError << endl;
		sqlite3_free(messageError);
	}
	else
	{
		cout << "SQL operation successful.\n";
	}
}


int main() {
	/*Student chud("Chud", "Maxx", "420", 2027, "BSAS");
	chud.search();
	chud.add_drop();
	chud.print_sch();
	Instructor babu("Babu", "S.", "1221", "Head of School of Engineering", 2020, "Computer Enginerring");
	babu.print_class_list();
	babu.print_schedule();
	babu.search_course();
	Admin prin("Principal", "A.", "987654321", "Dean of Students", "CEIS 302");
	prin.add_course();
	prin.remove_course();
	prin.add_remove_student();
	prin.add_remove_user();
	prin.print_courses();
	prin.print_roster();
	prin.search_courses();
	prin.search_roster();*/

	sqlite3* DB;
	int exit = sqlite3_open("assignment3.db", &DB);

	if (exit) {
		cout << "Error opening database!" << endl;
	}
	else {
		cout << "Database opened successfully!" << endl;
	}

	Student s1("Chud", "Johnson", "10011", 2027, "BSAS");
	Student s2("Tim", "BernersLee", "10012", 2028, "BSCO");

	insertStudent(DB, s1);
	insertStudent(DB, s2);

	string deleteInstructor =
		"DELETE FROM INSTRUCTOR "
		"WHERE NAME='Alan' AND SURNAME='Turing';";

	executeSQL(DB, deleteInstructor);
	
	string updateAdmin =
		"UPDATE ADMIN "
		"SET TITLE='Vice-President' "
		"WHERE NAME='Vera' AND SURNAME='Rubin';";

	executeSQL(DB, updateAdmin);

	string createCourseTable =
		"CREATE TABLE IF NOT EXISTS COURSE ("
		"CRN INT PRIMARY KEY NOT NULL,"
		"TITLE TEXT NOT NULL,"
		"DEPT CHAR(4) NOT NULL,"
		"TIME TEXT NOT NULL,"
		"DAYS TEXT NOT NULL,"
		"SEMESTER TEXT NOT NULL,"
		"YEAR INT NOT NULL,"
		"CREDITS INT NOT NULL);";
	executeSQL(DB, createCourseTable);


	Course c1(1001, "Data Structures", "BSCO",
		"10:00AM", "MWF", "Fall", 2026, 3);

	Course c2(1002, "Physics I", "BSAS",
		"1:00PM", "TR", "Fall", 2026, 4);

	Course c3(1003, "Circuit Analysis", "BSEE",
		"9:00AM", "MWF", "Fall", 2026, 3);

	Course c4(1004, "Thermodynamics", "BSME",
		"11:00AM", "TR", "Fall", 2026, 3);

	Course c5(1005, "Algorithms", "BSCO",
		"2:00PM", "MWT", "Fall", 2026, 3);

	insertCourse(DB, c1);
	insertCourse(DB, c2);
	insertCourse(DB, c3);
	insertCourse(DB, c4);
	insertCourse(DB, c5);

	cout << "\n-- STUDENTS --\n";
	string queryStudents = "SELECT * FROM STUDENT;";
	executeSQL(DB, queryStudents);
	
	cout << "\n-- INSTRUCTORS --\n";
	string queryInstructors = "SELECT * FROM INSTRUCTOR;";
	executeSQL(DB, queryInstructors);
	
	cout << "\n-- ADMINS --\n";
	string queryAdmins = "SELECT * FROM ADMIN;";
	executeSQL(DB, queryAdmins);
	
	cout << "\n-- COURSES --\n";
	string queryCourses = "SELECT * FROM COURSE;";
	executeSQL(DB, queryCourses);
	
	cout << "\n-- COURSE / INSTRUCTOR MATCHES --\n";
	string courseInstructorMatch =
		"SELECT COURSE.TITLE AS COURSE, "
		"COURSE.DEPT, "
		"INSTRUCTOR.NAME || ' ' || INSTRUCTOR.SURNAME AS INSTRUCTOR "
		"FROM COURSE "
		"LEFT JOIN INSTRUCTOR "
		"ON COURSE.DEPT = INSTRUCTOR.DEPT;";
	executeSQL(DB, courseInstructorMatch);

	sqlite3_close(DB);
	cout << "\nDatabase closed.\n";


	return 0;
}