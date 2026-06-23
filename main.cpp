#include <iostream>
#include "User.h"
#include "Student.h"
#include "Instructor.h"
#include "Admin.h"
#include "Course.h"
#include "sqlite3.h"
#include <vector>

using namespace std;
using std::cout;

string s_first = "";
string s_last = "";
int s_grad = 0;
string s_major = "";

string i_first = "";
string i_last = "";
string i_title = "";
int i_hyear = 0;
string i_dept = "";


string a_first = "";
string a_last = "";
string a_title = "";
string a_office = "";

void executeSQL(sqlite3* DB, string sql);

static int studentCallback(void* data, int argc, char** argv, char** colName)
{
	if (argc < 4)
	{
		return 0;
	}

	if (argv[0] != NULL)
	{
		s_first = argv[0];
	}
	else
	{
		s_first = "";
	}

	if (argv[1] != NULL)
	{
		s_last = argv[1];
	}
	else
	{
		s_last = "";
	}

	if (argv[2] != NULL)
	{
		s_grad = stoi(argv[2]);
	}
	else
	{
		s_grad = 0;
	}

	if (argv[3] != NULL)
	{
		s_major = argv[3];
	}
	else
	{
		s_major = "";
	}

	return 0;
}

static int instCallback(void* data, int argc, char** argv, char** colName)
{
	if (argc >= 5)
	{
		i_first = argv[0];
		i_last = argv[1];
		i_title = argv[2];
		i_hyear = stoi(argv[3]);
		i_dept = argv[4];
	}

	return 0;
}

static int adminCallback(void* data, int argc, char** argv, char** colName)
{
	if (argc < 4)
	{
		return 0;
	}

	if (argv[0] != NULL)
	{
		a_first = argv[0];
	}
	else
	{
		a_first = "";
	}

	if (argv[1] != NULL)
	{
		a_last = argv[1];
	}
	else
	{
		a_last = "";
	}

	if (argv[2] != NULL)
	{
		a_title = argv[2];
	}
	else
	{
		a_title = "";
	}

	if (argv[3] != NULL)
	{
		a_office = argv[3];
	}
	else
	{
		a_office = "";
	}

	return 0;
}

void insertStudent(sqlite3* DB, Student s)
{
	string sql =
		"INSERT INTO STUDENT VALUES ('" +
		s.getId() + "', '" +
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
		"INSERT INTO INSTRUCTOR VALUES ('" +
		i.getId() + "', '" +
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
		"INSERT INTO ADMIN VALUES ('" +
		a.getId() + "', '" +
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

bool idExists(sqlite3* DB, string table, string id)
{
	string sql =
		"SELECT ID FROM " + table +
		" WHERE ID='" + id + "';";

	if (table == "COURSE")
	{
		sql = 
			"SELECT CRN FROM " + table +
			" WHERE CRN='" + id + "';";

	}

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
	{
		return false;
	}

	bool found = (sqlite3_step(stmt) == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return found;
}

bool password(sqlite3* DB, string id, string pass)
{
	string sql =
		"SELECT ID "
		"FROM LOGIN "
		"WHERE ID='" + id +
		"' AND PASSWORD='" + pass + "';";

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
	{
		return false;
	}

	bool valid = (sqlite3_step(stmt) == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return valid;
}

void studentP(Student s, sqlite3* DB)
{
	int coursenum;
	int selection = 0;
	while (selection != 6)
	{
		cout << "Welcome to the student portal!" << endl;
		cout << "1. Search courses" << endl;
		cout << "2. Search courses based on parameters" << endl;
		cout << "3. Add/remove course from your semester." << endl;
		cout << "4. Check for conflicts in your Schedule." << endl;
		cout << "5. Print your schedule." << endl;
		cout << "6. Exit the student portal" << endl;
		cin >> selection;
		switch (selection) {
		case 1:
			s.search();
			break;
		case 2:
			cout << "Enter course number:\n";
			while (true)
			{
				if (!(cin >> coursenum))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					cout << "Invalid course number: ";
					continue;
				}

				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (idExists(DB, "COURSE", to_string(coursenum)))
					break;

				cout << "Course not found:\n";
			}
			s.search(coursenum);
			break;
		case 3:
			int course_sel;
			cout << "Do you wish to add or drop a course?" << endl;
			cout << "1. Add course" << endl;
			cout << "2. Drop course" << endl;
			cin >> course_sel;
			switch (course_sel) {
			case 1:
				cout << "What class do you wish to add? (Please use the course number)" << endl;
				while (true)
				{
					if (!(cin >> coursenum))
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');

						cout << "Invalid course number: ";
						continue;
					}

					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					if (idExists(DB, "COURSE", to_string(coursenum)))
						break;

					cout << "Course not found:\n";
				}
				s.add_drop(1, coursenum);
				break;
			case 2:
				cout << "What class do you wish to drop? (Please use the course number)" << endl;
				while (true)
				{
					if (!(cin >> coursenum))
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');

						cout << "Invalid course number: ";
						continue;
					}

					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					if (idExists(DB, "COURSE", to_string(coursenum)))
						break;

					cout << "Course not found:\n";
				}
				s.add_drop(0, coursenum);
				break;
			}
			break;
		case 4:
			s.conflict();
			break;
		case 5:
			s.print_sch();
			break;
		case 6:
			cout << "Thank you for using our portal" << endl;
		}
	}
}

void instructP(Instructor i, sqlite3* DB)
{
	int coursenum;
	int studentid;
	int selection = 0;
	while (selection != 6)
	{
		cout << "Welcome to the instructor portal!" << endl;
		cout << "1. Search courses" << endl;
		cout << "2. Search courses based on parameters" << endl;
		cout << "3. Print course teaching schedule" << endl;
		cout << "4. Search course roster for a student" << endl;
		cout << "5. Print course roster" << endl;
		cout << "6. Exit instructor portal" << endl;

		cin >> selection;

		switch (selection)
		{
		case 1:
			i.search_course();
			break;
		case 2:
			cout << "Enter course number:\n";

			while (true)
			{
				if (!(cin >> coursenum))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					cout << "Invalid course number: ";
					continue;
				}

				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (idExists(DB, "COURSE", to_string(coursenum)))
				{
					break;
				}
				cout << "Course not found:\n";
			}
			i.search_course(coursenum);
			break;
		case 3:
			i.print_schedule();
			break;
		case 4:
			cout << "Enter student ID:\n";

			while (true)
			{
				if (!(cin >> studentid))
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					cout << "Invalid student ID: ";
					continue;
				}

				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (idExists(DB, "STUDENT", to_string(studentid)))
				{
					break;
				}
				cout << "Student not found:\n";
			}
			i.search_roster(studentid);
			break;
		case 5:
			i.print_class_list();
			break;
		case 6:
			cout << "Thank you for using our portal" << endl;
			break;
		default:
			cout << "Invalid option." << endl;
		}
	}
}

void adminP(Admin a, sqlite3* DB)
{
	int selection = 0;
	int course_num;
	string name;
	string department;
	string time;
	string days;
	string semester;
	string year;
	int credits;
	string student_name;
	int student_id;
	string instructor_name;
	int instructor_id;
	int inst_id_course_add;
	int id_course_add;
	int inst_id_course_remove;
	int id_course_remove;
	while (selection != 10)
	{
		cout << "Welcome to the Admin portal!" << endl;
		cout << "1. Search courses" << endl;
		cout << "2. Search courses based on parameters" << endl;
		cout << "3. Add a course to the system." << endl;
		cout << "4. Add a student to the system." << endl;
		cout << "5. Add an instrutor to the system." << endl;
		cout << "6. Link an instructor to a course." << endl;
		cout << "7. Unlink an instructor to a course" << endl;
		cout << "8. Add a student to a course." << endl;
		cout << "9. Remove a student to a course." << endl;
		cout << "10. Exit the Admin portal" << endl;
		cin >> selection;
	
		switch (selection) {
		case 1:
			a.search_courses();
			break;
		case 2:
			int class_search;
			cout << "What class do you want to search for? (Please use the course number)" << endl;
			while (!(cin >> class_search)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a number." << endl;
			}
			cout << "You search for, " << class_search << "." << endl << "Here are all of the classes associated:" << endl << endl;
			break;
		case 3:
			cout << "What is the course that you want to add to the system." << endl;
			cout << "Course num: " << endl;
			while (!(cin >> course_num)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a number." << endl;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << course_num << " is the course num." << endl;
			cout << "What is the course name that you want to add to the system." << endl;
			getline(cin, name);
			cout << name << " is the course name." << endl;
			cout << "What is the course department that you want to add to the system." << endl;
			cin >> department;
			while (department.length() < 4 || department.length() > 4) {
				cin >> department;
				if (department.length() < 4 || department.length() > 4) {
					cout << department << " is not an acceptable entry. It must be 4 letters" << endl;
				}
			}
			cout << department << " is the courses department." << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What is the time block for the course." << endl;
			getline(cin, time);
			cout << time << " is the time time block for the class." << endl;
			cout << "What days are the class going to be?" << endl;
			getline(cin, days);
			cout << days << " are the days for the class." << endl;
			cout << "What is the semester(s) that it runs?" << endl;
			getline(cin, semester);
			cout << semester << " is the semester that this class runs" << endl;
			cout << "What is the year that this class runs?" << endl;
			getline(cin, year);
			cout << year << " is the year of this class." << endl;
			cout << "How many credits are the class?" << endl;
			while (!(cin >> credits)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a number." << endl;
			}
			cout << credits << " is the amount of credits for this class." << endl;
			break;
		case 4:
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What is the students name: " << endl;
			getline(cin, student_name);
			cout << "What is the students id number: " << endl;
			while (!(cin >> student_id)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a number." << endl;
			}
			cout << student_name << " " << student_id << ", is added to the student roster." << endl;
			break;
		case 5:
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What is the instructors name: " << endl;
			getline(cin, instructor_name);
			cout << "What is the Instructors ID: " << endl;
			while (!(cin >> instructor_id)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a number." << endl;
			}
			cout << instructor_name << " " << instructor_id << ", is added to the student roster." << endl;
			break;
		case 6:
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What is the instructor you want to add to a course? (Please use the instructors ID)" << endl;
			cin >> inst_id_course_add;
			cout << "What is the class you want to add them to? (Please use the classes CRN)" << endl;
			cin >> id_course_add;
			cout << "Instructor " << inst_id_course_add << " added to: " << id_course_add << endl;
			break;
		case 7:
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What is the instructor you want to remove from a course? (Please use the instructors ID)" << endl;
			cin >> inst_id_course_remove;
			cout << "What is the class you want them removed from? (Please use the classes CRN)" << endl;
			cin >> id_course_remove;
			cout << "Instructor " << inst_id_course_remove << " removed from: " << id_course_remove << endl;
			break;
		case 8:
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What student do you want to add to the class? (Please use their ID number)" << endl;
			cin >> student_id;
			cout << "What is the course number you want to add them to? (Please use the CRN number)" << endl;
			cin >> course_num;
			cout << student_id << " added to class: " << course_num << endl;
			break;
		case 9:
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "What student do you want to remove to the class? (Please use their ID number)" << endl;
			cin >> student_id;
			cout << "What is the course number you want to remove them from? (Please use the CRN number)" << endl;
			cin >> course_num;
			cout << student_id << " removed from class: " << course_num << endl;
			break;
		case 10:
			cout << "Thank you for using our portal!" << endl;
			break;
		default:
			cout << "Invalid option.\n";
		}
	}
}

void login(sqlite3* DB)
{
	string id;
	string pass;

	cout << "\n===== UNIVERSITY LOGIN =====\n";
	cout << "Enter ID: ";
	cin >> id;
	cout << "Enter password: ";
	cin >> pass;

	if (!password(DB, id, pass))
	{
		cout << "Invalid ID or password.\n";
		return;
	}

	char type = id[0];
	string sql;

	switch (type){
	case '1': // Student
		if (idExists(DB, "STUDENT", id))
		{
			sql =
				"SELECT NAME, SURNAME, GRADYEAR, MAJOR "
				"FROM STUDENT "
				"WHERE ID='" + id + "';";

			char* err;

			int exit = sqlite3_exec(DB, sql.c_str(), studentCallback,NULL,&err);

			if (exit == SQLITE_OK)
			{
				if (s_first != "")
				{
					Student currentStudent(s_first, s_last, id, s_grad, s_major);
					cout << "Opening Student Menu...\n";

					studentP(currentStudent, DB);
				}
				else
				{
					cout << "Student load failed.\n";
				}
			}
			else
			{
				cout << "Error: " << err << endl;
				sqlite3_free(err);
			}
		}
		else
		{
			cout << "Student ID not found.\n";
		}
		break;
	case '2': // Instructor
		if (idExists(DB, "INSTRUCTOR", id))
		{
			sql =
				"SELECT NAME, SURNAME, TITLE, HIREYEAR, DEPT "
				"FROM INSTRUCTOR WHERE ID ='" + id + "';";
			char* err;

			int exit = sqlite3_exec(DB, sql.c_str(), instCallback, NULL, &err);

			if (exit == SQLITE_OK)
			{
				if (i_first != "")
				{
					Instructor currentInst(i_first, i_last, id, i_title, i_hyear, i_dept);
					cout << "Opening Instructor Menu...\n";
					
					instructP(currentInst, DB);
				}
				else
				{
					cout << "Instructor load failed.\n";
				}
			}
			else
			{
				cout << "Error: " << err << endl;
				sqlite3_free(err);
			}
		}
		else
		{
			cout << "Instructor ID not found.\n";
		}
		break;
	case '3': // Admin
		if (idExists(DB, "ADMIN", id))
		{
			sql =
				"SELECT NAME, SURNAME, TITLE, OFFICE "
				"FROM ADMIN WHERE ID ='" + id + "';";

			char* err;

			int exit = sqlite3_exec(DB, sql.c_str(), adminCallback, NULL, &err);

			if (exit == SQLITE_OK)
			{
				if (a_first != "")
				{
					Admin currentAdmin(a_first, a_last, id, a_title, a_office);
					cout << "Opening Admin Menu...\n";

					adminP(currentAdmin, DB);
				}
				else
				{
					cout << "Admin load failed.\n";
				}
			}
			else
			{
				cout << "Error: " << err << endl;
				sqlite3_free(err);
			}
		}
		else
		{
			cout << "Admin ID not found.\n";
		}
		break;
	default:
		cout << "Unknown ID format.\n";
	}
}

int main() {

	sqlite3* DB;
	int exit = sqlite3_open("assignment3.db", &DB);

	if (exit) {
		cout << "Error opening database!" << endl;
	}
	else {
		cout << "Database opened successfully!" << endl;
	}

	string createStudentTable =
		"CREATE TABLE IF NOT EXISTS STUDENT ("
		"ID TEXT PRIMARY KEY NOT NULL,"
		"NAME TEXT NOT NULL,"
		"SURNAME TEXT NOT NULL,"
		"GRADYEAR INT NOT NULL,"
		"MAJOR TEXT NOT NULL,"
		"EMAIL TEXT NOT NULL"
		");";

	executeSQL(DB, createStudentTable);

	string createInstructorTable =
		"CREATE TABLE IF NOT EXISTS INSTRUCTOR ("
		"ID TEXT PRIMARY KEY NOT NULL,"
		"NAME TEXT NOT NULL,"
		"SURNAME TEXT NOT NULL,"
		"TITLE TEXT NOT NULL,"
		"HIRE_YEAR INT NOT NULL,"
		"DEPT TEXT NOT NULL,"
		"EMAIL TEXT NOT NULL"
		");";

	executeSQL(DB, createInstructorTable);

	string createAdminTable =
		"CREATE TABLE IF NOT EXISTS ADMIN ("
		"ID TEXT PRIMARY KEY NOT NULL,"
		"NAME TEXT NOT NULL,"
		"SURNAME TEXT NOT NULL,"
		"TITLE TEXT NOT NULL,"
		"OFFICE TEXT NOT NULL,"
		"EMAIL TEXT NOT NULL"
		");";

	executeSQL(DB, createAdminTable);

	string logTable =
		"CREATE TABLE IF NOT EXISTS LOGIN("
		"ID TEXT PRIMARY KEY,"
		"PASSWORD TEXT,"
		"ROLE TEXT);";

	executeSQL(DB, logTable);

	string updateL =
		"INSERT INTO LOGIN "
		"SELECT "
		"ID, "
		"substr(ID,1,1)||substr(ID,-3), "
		"'Student' "
		"FROM STUDENT;"

		"INSERT INTO LOGIN "
		"SELECT "
		"ID, "
		"substr(ID,1,1)||substr(ID,-3), "
		"'Instructor' "
		"FROM INSTRUCTOR;"

		"INSERT INTO LOGIN "
		"SELECT "
		"ID, "
		"substr(ID,1,1)||substr(ID,-3), "
		"'Admin' "
		"FROM ADMIN;";

	executeSQL(DB, updateL);
		

	Student s1("Chud", "Johnson", "10011", 2027, "BSAS");
	Student s2("Tim", "BernersLee", "10012", 2028, "BSCO");
	Student s3("Emma", "Watson", "10013", 2027, "BSCO");
	Student s4("Michael", "Brown", "10014", 2028, "BSEE");
	Student s5("Sophia", "Davis", "10015", 2026, "BSME");
	Student s6("James", "Wilson", "10016", 2027, "BSAS");
	Student s7("Olivia", "Taylor", "10017", 2029, "BSCO");
	Student s8("Daniel", "Martinez", "10018", 2028, "BSEE");
	Student s9("Isabella", "Anderson", "10019", 2027, "BSME");
	Student s10("Matthew", "Thomas", "10020", 2026, "BSCO");
	Student s11("Mia", "Jackson", "10021", 2028, "BSAS");
	Student s12("Ethan", "White", "10022", 2027, "BSEE");
	Student s13("Charlotte", "Harris", "10023", 2029, "BSME");
	Student s14("Noah", "Martin", "10024", 2026, "BSCO");
	Student s15("Amelia", "Garcia", "10025", 2028, "BSAS");
	Student s16("Liam", "Clark", "10026", 2027, "BSEE");
	Student s17("Harper", "Lewis", "10027", 2029, "BSME");
	Student s18("Benjamin", "Lee", "10028", 2026, "BSCO");
	Student s19("Evelyn", "Walker", "10029", 2027, "BSAS");
	Student s20("Lucas", "Hall", "10030", 2028, "BSEE");

	vector<Student> students = {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20};
	for (Student& s : students)
	{
		insertStudent(DB, s);
	}

	Instructor i1("Alan", "Turing", "20001",
		"Professor", 2010, "BSCO");

	Instructor i2("Grace", "Hopper", "20002",
		"Professor", 2012, "BSCO");

	Instructor i3("Nikola", "Tesla", "20003",
		"Associate Professor", 2015, "BSEE");

	Instructor i4("Marie", "Curie", "20004",
		"Professor", 2011, "BSAS");

	Instructor i5("Isaac", "Newton", "20005",
		"Professor", 2008, "BSAS");

	Instructor i6("Ada", "Lovelace", "20006",
		"Assistant Professor", 2020, "BSCO");

	Instructor i7("Carl", "Gauss", "20007",
		"Professor", 2014, "BSAS");

	Instructor i8("Katherine", "Johnson", "20008",
		"Associate Professor", 2016, "BSCO");

	Instructor i9("James", "Maxwell", "20009",
		"Professor", 2009, "BSEE");

	Instructor i10("Richard", "Feynman", "20010",
		"Professor", 2013, "BSAS");

	Instructor i11("Robert", "Boyle", "20011",
		"Professor", 2018, "BSAS");

	Instructor i12("George", "Boole", "20012",
		"Associate Professor", 2017, "BSCO");

	Instructor i13("Michael", "Faraday", "20013",
		"Professor", 2010, "BSEE");

	Instructor i14("Stephen", "Hawking", "20014",
		"Professor", 2011, "BSAS");

	Instructor i15("Barbara", "Liskov", "20015",
		"Professor", 2019, "BSCO");

	vector<Instructor> instructors = { i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15 };
	for (Instructor& i : instructors)
	{
		insertInstructor(DB, i);
	}

	Admin a1("Sarah", "Mitchell",
		"30001",
		"Registrar",
		"Administration Building 101");

	insertAdmin(DB, a1);

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

	Course c6(1006, "Operating Systems", "BSCO",
		"8:00AM", "TR", "Fall", 2026, 3);

	Course c7(1007, "Database Systems", "BSCO",
		"3:00PM", "MWF", "Fall", 2026, 3);

	Course c8(1008, "Computer Networks", "BSCO",
		"10:00AM", "TR", "Fall", 2026, 3);

	Course c9(1009, "Digital Logic", "BSEE",
		"11:00AM", "MWF", "Fall", 2026, 3);

	Course c10(1010, "Electromagnetics", "BSEE",
		"1:00PM", "TR", "Fall", 2026, 3);

	Course c11(1011, "Machine Design", "BSME",
		"9:00AM", "TR", "Fall", 2026, 3);

	Course c12(1012, "Fluid Mechanics", "BSME",
		"12:00PM", "MWF", "Fall", 2026, 3);

	Course c13(1013, "Calculus I", "BSAS",
		"8:00AM", "MWF", "Fall", 2026, 4);

	Course c14(1014, "Calculus II", "BSAS",
		"9:00AM", "MWF", "Fall", 2026, 4);

	Course c15(1015, "Linear Algebra", "BSAS",
		"2:00PM", "TR", "Fall", 2026, 3);

	Course c16(1016, "Discrete Mathematics", "BSCO",
		"4:00PM", "TR", "Fall", 2026, 3);

	Course c17(1017, "Engineering Economics", "BSME",
		"10:00AM", "TR", "Fall", 2026, 3);

	Course c18(1018, "Signals and Systems", "BSEE",
		"3:00PM", "MWF", "Fall", 2026, 3);

	Course c19(1019, "Artificial Intelligence", "BSCO",
		"1:00PM", "MWF", "Fall", 2026, 3);

	Course c20(1020, "Software Engineering", "BSCO",
		"11:00AM", "TR", "Fall", 2026, 3);

	vector<Course> courses = { c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20 };
	for (Course& c : courses)
	{
		insertCourse(DB, c);
	}


	login(DB);

	/*cout << "\n-- STUDENTS --\n";
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
	executeSQL(DB, courseInstructorMatch);*/

	sqlite3_close(DB);
	cout << "\nDatabase closed.\n";


	return 0;
}