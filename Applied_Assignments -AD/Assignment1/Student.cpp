#include "Student.h"
Student::Student(string in_first, string in_last, string in_id, int grad, string m):User(in_first, in_last, in_id){
	gradY = grad;
	major = m;
	email = this->getLast();
	email += (char)tolower(this->getFirst().at(0));
	email += "@wit.edu";
}
void Student::search(sqlite3* DB, int CRN){
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
        cout << "Search failed: "
            << sqlite3_errmsg(DB) << endl;
        return;
    }
    sqlite3_bind_int(statement, 1, CRN);
    result = sqlite3_step(statement);
    if (result == SQLITE_ROW){
        cout << "\nCourse found:\n";
        cout << "CRN: "
            << sqlite3_column_int(statement, 0) << endl;

        cout << "Title: "
            << sqlite3_column_text(statement, 1) << endl;

        cout << "Department: "
            << sqlite3_column_text(statement, 2) << endl;

        cout << "Time: "
            << sqlite3_column_text(statement, 3) << endl;

        cout << "Days: "
            << sqlite3_column_text(statement, 4) << endl;

        cout << "Semester: "
            << sqlite3_column_text(statement, 5) << endl;

        cout << "Year: "
            << sqlite3_column_int(statement, 6) << endl;

        cout << "Credits: "
            << sqlite3_column_int(statement, 7) << endl;
    }
    else{
        cout << "No course found with CRN "
            << CRN << "." << endl;
    }
    sqlite3_finalize(statement);
}

void Student::add_drop(sqlite3* DB, int CRN, int choice){
    sqlite3_stmt* statement = NULL;
    string sql;
    if (choice == 1){
        sql =
            "INSERT INTO STUDENT_COURSE "
            "(STUDENT_ID, CRN) "
            "VALUES (?, ?);";
    }
    else if (choice == 2){
        sql =
            "DELETE FROM STUDENT_COURSE "
            "WHERE STUDENT_ID = ? AND CRN = ?;";
    }
    else{
        cout << "Invalid choice." << endl;
        return;
    }
    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );
    if (result != SQLITE_OK){
        cout << "Database error: "
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
    sqlite3_bind_int(statement, 2, CRN);
    result = sqlite3_step(statement);
    if (result == SQLITE_DONE){
        if (choice == 1){
            cout << "Course " << CRN
                << " was added to your schedule."
                << endl;
        }
        else{
            if (sqlite3_changes(DB) > 0){
                cout << "Course " << CRN
                    << " was dropped from your schedule."
                    << endl;
            }
            else{
                cout << "You are not enrolled in course "
                    << CRN << "." << endl;
            }
        }
    }
    else{
        if (choice == 1){
            cout << "You may already be enrolled in course "
                << CRN << "." << endl;
        }
        else{
            cout << "Add/drop failed: "
                << sqlite3_errmsg(DB) << endl;
        }
    }
    sqlite3_finalize(statement);
}

void Student::conflict(sqlite3* DB){
    string sql =
        "SELECT "
        "C1.CRN, C1.TITLE, C1.DAYS, C1.TIME, "
        "C2.CRN, C2.TITLE, C2.DAYS, C2.TIME "
        "FROM STUDENT_COURSE AS SC1 "
        "JOIN COURSE AS C1 ON SC1.CRN = C1.CRN "
        "JOIN STUDENT_COURSE AS SC2 "
        "ON SC1.STUDENT_ID = SC2.STUDENT_ID "
        "AND SC1.CRN < SC2.CRN "
        "JOIN COURSE AS C2 ON SC2.CRN = C2.CRN "
        "WHERE SC1.STUDENT_ID = ? "
        "AND C1.DAYS = C2.DAYS "
        "AND C1.TIME = C2.TIME;";
    sqlite3_stmt* statement = NULL;
    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );
    if (result != SQLITE_OK){
        cout << "Conflict check failed: "
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
    bool conflictFound = false;
    while (sqlite3_step(statement) == SQLITE_ROW){
        conflictFound = true;
        int firstCRN =
            sqlite3_column_int(statement, 0);
        string firstTitle =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
                );

        string firstDays =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 2)
                );

        string firstTime =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 3)
                );

        int secondCRN =
            sqlite3_column_int(statement, 4);

        string secondTitle =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 5)
                );

        cout << "\nSchedule conflict found:\n";

        cout << firstCRN << " - "
            << firstTitle << endl;

        cout << secondCRN << " - "
            << secondTitle << endl;

        cout << "Both courses meet on "
            << firstDays << " at "
            << firstTime << "." << endl;
    }

    if (!conflictFound){
        cout << "No schedule conflicts were found."
            << endl;
    }
    sqlite3_finalize(statement);
}

void Student::print_sch(sqlite3* DB){
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
        "FROM STUDENT_COURSE "
        "JOIN COURSE "
        "ON STUDENT_COURSE.CRN = COURSE.CRN "
        "WHERE STUDENT_COURSE.STUDENT_ID = ? "
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
        cout << "Unable to print schedule: "
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
    cout << "\n===== STUDENT SCHEDULE =====" << endl;
    cout << "Student ID: " << getId() << endl;
    cout << "Student: "
        << getFirst() << " "
        << getLast() << endl;
    cout << "----------------------------" << endl;

    while (sqlite3_step(statement) == SQLITE_ROW){
        courseFound = true;
        int CRN =
            sqlite3_column_int(statement, 0);

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

        int year =
            sqlite3_column_int(statement, 6);

        int credits =
            sqlite3_column_int(statement, 7);

        cout << "CRN: " << CRN << endl;
        cout << "Course: " << title << endl;
        cout << "Department: " << department << endl;
        cout << "Days: " << days << endl;
        cout << "Time: " << time << endl;
        cout << "Semester: "
            << semester << " " << year << endl;
        cout << "Credits: " << credits << endl;
        cout << "----------------------------" << endl;
    }
    if (courseFound == false){
        cout << "You are not currently enrolled in any courses."
            << endl;
    }
    sqlite3_finalize(statement);
}

string Student::getMajor() {
	return major;
}

int Student::getGradYear() {
	return gradY;
}

string Student::getEmail() {
	return email;
}