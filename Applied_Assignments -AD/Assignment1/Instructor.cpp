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
void Instructor::print_schedule(sqlite3* DB){
    string sql =
        "SELECT "
        "COURSE.CRN, "
        "COURSE.TITLE, "
        "COURSE.DEPT, "
        "COURSE.TIME, "
        "COURSE.DAYS, "
        "COURSE.SEMESTER, "
        "COURSE.YEAR, "
        "COURSE.CREDITS "
        "FROM INSTRUCTOR_COURSE "
        "JOIN COURSE "
        "ON INSTRUCTOR_COURSE.CRN = COURSE.CRN "
        "WHERE INSTRUCTOR_COURSE.INSTRUCTOR_ID = ? "
        "ORDER BY COURSE.CRN;";

    sqlite3_stmt* statement = NULL;

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK){
        cout << "Unable to print teaching schedule: "
            << sqlite3_errmsg(DB) << endl;
        return;
    }

    sqlite3_bind_text(
        statement,
        1,
        getId().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool courseFound = false;
    cout << "\n===== INSTRUCTOR TEACHING SCHEDULE =====" << endl;
    cout << "Instructor: "<< getFirst() << " "<< getLast() << endl;
    cout << "Instructor ID: " << getId() << endl;
    cout << "Department: " << getDepartment() << endl;
    cout << "----------------------------------------" << endl;

    while (sqlite3_step(statement) == SQLITE_ROW){
        courseFound = true;

        int CRN = sqlite3_column_int(statement, 0);

        string title =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
                );

        string department =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
                );

        string time =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 3)
                );

        string days =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
                );

        string semester =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 5)
                );

        int year = sqlite3_column_int(statement, 6);
        int credits = sqlite3_column_int(statement, 7);

        cout << "CRN: " << CRN << endl;
        cout << "Course: " << title << endl;
        cout << "Department: " << department << endl;
        cout << "Days: " << days << endl;
        cout << "Time: " << time << endl;
        cout << "Semester: "
            << semester << " " << year << endl;
        cout << "Credits: " << credits << endl;
        cout << "----------------------------------------" << endl;
    }
    if (courseFound == false){
        cout << "No courses are currently assigned to you."
            << endl;
    }
    sqlite3_finalize(statement);
}
void Instructor::print_class_list(sqlite3* DB){
    string sql =
        "SELECT "
        "COURSE.CRN, "
        "COURSE.TITLE, "
        "STUDENT.ID, "
        "STUDENT.NAME, "
        "STUDENT.SURNAME, "
        "STUDENT.GRADYEAR, "
        "STUDENT.MAJOR "
        "FROM INSTRUCTOR_COURSE "
        "JOIN COURSE "
        "ON INSTRUCTOR_COURSE.CRN = COURSE.CRN "
        "LEFT JOIN STUDENT_COURSE "
        "ON COURSE.CRN = STUDENT_COURSE.CRN "
        "LEFT JOIN STUDENT "
        "ON STUDENT_COURSE.STUDENT_ID = STUDENT.ID "
        "WHERE INSTRUCTOR_COURSE.INSTRUCTOR_ID = ? "
        "ORDER BY COURSE.CRN, STUDENT.SURNAME, STUDENT.NAME;";

    sqlite3_stmt* statement = NULL;

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK){
        cout << "Unable to print class list: "
            << sqlite3_errmsg(DB) << endl;
        return;
    }
    sqlite3_bind_text(
        statement,
        1,
        getId().c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    bool courseFound = false;
    bool studentFound = false;
    int previousCRN = -1;

    cout << "\n===== INSTRUCTOR CLASS LIST =====" << endl;
    cout << "Instructor: "<< getFirst() << " "<< getLast() << endl;
    cout << "Instructor ID: " << getId() << endl;
    while (sqlite3_step(statement) == SQLITE_ROW){
        courseFound = true;
        int CRN = sqlite3_column_int(statement, 0);
        string courseTitle =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
                );
        if (CRN != previousCRN){
            cout << "\n----------------------------------------" << endl;
            cout << "CRN: " << CRN << endl;
            cout << "Course: " << courseTitle << endl;
            cout << "----------------------------------------" << endl;
            previousCRN = CRN;
        }
        if (sqlite3_column_type(statement, 2) == SQLITE_NULL){
            cout << "No students are enrolled in this course."<< endl;
        }
        else{
            studentFound = true;

            string studentID =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(statement, 2)
                    );

            string firstName =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(statement, 3)
                    );

            string lastName =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(statement, 4)
                    );

            int gradYear =
                sqlite3_column_int(statement, 5);

            string major =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(statement, 6)
                    );

            cout << "Student ID: " << studentID << endl;
            cout << "Name: "<< firstName << " "<< lastName << endl;
            cout << "Graduation year: "<< gradYear << endl;
            cout << "Major: " << major << endl;
            cout << endl;
        }
    }
    if (courseFound == false){
        cout << "No courses are currently assigned to this instructor."
            << endl;
    }
    else if (studentFound == false){
        cout << "\nNo students are currently enrolled in any of your courses."
            << endl;
    }
    sqlite3_finalize(statement);
}
void Instructor::search_course(sqlite3* DB, int CRN){
    string sql =
        "SELECT CRN, TITLE, DEPT, TIME, DAYS, SEMESTER, YEAR, CREDITS "
        "FROM COURSE "
        "WHERE CRN = ?;";

    sqlite3_stmt* statement = NULL;

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK){
        cout << "Course search failed: "
            << sqlite3_errmsg(DB) << endl;
        return;
    }
    sqlite3_bind_int(statement, 1, CRN);
    result = sqlite3_step(statement);
    if (result == SQLITE_ROW){
        cout << "\n===== COURSE FOUND =====" << endl;
        cout << "CRN: "<< sqlite3_column_int(statement, 0)<< endl;
        cout << "Title: "<< sqlite3_column_text(statement, 1)<< endl;
        cout << "Department: "<< sqlite3_column_text(statement, 2)<< endl;
        cout << "Time: "<< sqlite3_column_text(statement, 3)<< endl;
        cout << "Days: "<< sqlite3_column_text(statement, 4)<< endl;
        cout << "Semester: "<< sqlite3_column_text(statement, 5)<< endl;
        cout << "Year: "<< sqlite3_column_int(statement, 6)<< endl;
        cout << "Credits: "<< sqlite3_column_int(statement, 7)<< endl;
    }
    else if (result == SQLITE_DONE){
        cout << "No course was found with CRN "
            << CRN << "." << endl;
    }
    else{
        cout << "Course search failed: "
            << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_finalize(statement);
}
void Instructor::search_roster(sqlite3* DB, int studentID){
    string sql =
        "SELECT "
        "STUDENT.ID, "
        "STUDENT.NAME, "
        "STUDENT.SURNAME, "
        "STUDENT.GRADYEAR, "
        "STUDENT.MAJOR, "
        "COURSE.CRN, "
        "COURSE.TITLE "
        "FROM INSTRUCTOR_COURSE "
        "JOIN COURSE "
        "ON INSTRUCTOR_COURSE.CRN = COURSE.CRN "
        "JOIN STUDENT_COURSE "
        "ON COURSE.CRN = STUDENT_COURSE.CRN "
        "JOIN STUDENT "
        "ON STUDENT_COURSE.STUDENT_ID = STUDENT.ID "
        "WHERE INSTRUCTOR_COURSE.INSTRUCTOR_ID = ? "
        "AND STUDENT.ID = ?;";

    sqlite3_stmt* statement = NULL;

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK){
        cout << "Roster search failed: "
            << sqlite3_errmsg(DB) << endl;
        return;
    }
    sqlite3_bind_text(
        statement,
        1,
        getId().c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    string studentIDString = to_string(studentID);
    sqlite3_bind_text(
        statement,
        2,
        studentIDString.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool studentFound = false;

    while (sqlite3_step(statement) == SQLITE_ROW){
        studentFound = true;

        string id =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 0)
                );

        string firstName =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
                );

        string lastName =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
                );

        int gradYear =
            sqlite3_column_int(statement, 3);

        string major =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
                );

        int CRN =
            sqlite3_column_int(statement, 5);

        string courseTitle =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 6)
                );

        cout << "\n===== STUDENT FOUND =====" << endl;
        cout << "Student ID: " << id << endl;
        cout << "Name: "<< firstName << " "<< lastName << endl;
        cout << "Graduation year: "<< gradYear << endl;
        cout << "Major: " << major << endl;
        cout << "Enrolled in your course:" << endl;
        cout << CRN << " - "<< courseTitle << endl;
    }
    if (studentFound == false){
        cout << "Student " << studentID
            << " is not enrolled in any course you teach."
            << endl;
    }
    sqlite3_finalize(statement);
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