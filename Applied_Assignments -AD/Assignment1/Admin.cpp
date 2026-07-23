#include "Admin.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;
static void clearInvalidInput()
{
    cin.clear();

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}
static void printCourseRow(sqlite3_stmt* statement)
{
    cout << "\n------------------------------" << endl;

    cout << "CRN: "
        << sqlite3_column_int(statement, 0)
        << endl;

    cout << "Title: "
        << sqlite3_column_text(statement, 1)
        << endl;

    cout << "Department: "
        << sqlite3_column_text(statement, 2)
        << endl;

    cout << "Time: "
        << sqlite3_column_text(statement, 3)
        << endl;

    cout << "Days: "
        << sqlite3_column_text(statement, 4)
        << endl;

    cout << "Semester: "
        << sqlite3_column_text(statement, 5)
        << endl;

    cout << "Year: "
        << sqlite3_column_int(statement, 6)
        << endl;

    cout << "Credits: "
        << sqlite3_column_int(statement, 7)
        << endl;
}

static bool recordExists(sqlite3* DB,string sql,string textValue)
{
    sqlite3_stmt* statement = NULL;

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(
        statement,
        1,
        textValue.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool found =
        sqlite3_step(statement) == SQLITE_ROW;

    sqlite3_finalize(statement);

    return found;
}
static bool courseExists(sqlite3* DB, int CRN)
{
    sqlite3_stmt* statement = NULL;

    string sql =
        "SELECT CRN FROM COURSE WHERE CRN = ?;";

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(statement, 1, CRN);

    bool found =
        sqlite3_step(statement) == SQLITE_ROW;

    sqlite3_finalize(statement);

    return found;
}
Admin::Admin(string in_first,string in_last,string in_id,string in_title,string in_office)
    : User(in_first, in_last, in_id)
{
    title = in_title;
    office = in_office;

    email = getLast();

    if (getFirst().empty() == false)
    {
        email += static_cast<char>(
            tolower(getFirst().at(0))
            );
    }

    email += "@wit.edu";
}
void Admin::search_courses(sqlite3* DB){
    string sql =
        "SELECT "
        "CRN, TITLE, DEPT, TIME, DAYS, "
        "SEMESTER, YEAR, CREDITS "
        "FROM COURSE "
        "ORDER BY CRN;";
    sqlite3_stmt* statement = NULL;
    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );
    if (result != SQLITE_OK){
        cout << "Unable to search courses: "<< sqlite3_errmsg(DB)<< endl;
        return;
    }
    bool courseFound = false;
    cout << "\n===== ALL COURSES =====" << endl;
    while (sqlite3_step(statement) == SQLITE_ROW){
        courseFound = true;
        printCourseRow(statement);
    }
    if (courseFound == false){
        cout << "No courses were found." << endl;
    }

    sqlite3_finalize(statement);
}
void Admin::search_courses_by_parameter(sqlite3* DB){
    int selection;
    cout << "\n===== COURSE PARAMETER SEARCH ====="<< endl;
    cout << "1. Search by CRN" << endl;
    cout << "2. Search by course title" << endl;
    cout << "3. Search by department" << endl;
    cout << "4. Search by semester" << endl;
    cout << "5. Search by year" << endl;
    cout << "Enter selection: ";
    if (!(cin >> selection)){
        clearInvalidInput();
        cout << "Invalid selection." << endl;
        return;
    }
    string sql;
    sqlite3_stmt* statement = NULL;
    int result;
    if (selection == 1){
        int CRN;
        cout << "Enter the course CRN: ";
        if (!(cin >> CRN)){
            clearInvalidInput();
            cout << "Invalid CRN." << endl;
            return;
        }
        sql =
            "SELECT "
            "CRN, TITLE, DEPT, TIME, DAYS, "
            "SEMESTER, YEAR, CREDITS "
            "FROM COURSE "
            "WHERE CRN = ? "
            "ORDER BY CRN;";

        result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );

        if (result != SQLITE_OK){
            cout << "Search failed: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }
        sqlite3_bind_int(statement, 1, CRN);
    }
    else if (selection == 2 ||selection == 3 ||selection == 4){
        string searchValue;
        clearInvalidInput();
        if (selection == 2){
            cout << "Enter all or part of the course title: ";
            getline(cin, searchValue);
            sql =
                "SELECT "
                "CRN, TITLE, DEPT, TIME, DAYS, "
                "SEMESTER, YEAR, CREDITS "
                "FROM COURSE "
                "WHERE TITLE LIKE ? "
                "ORDER BY CRN;";
            searchValue =
                "%" + searchValue + "%";
        }
        else if (selection == 3){
            cout << "Enter the department: ";
            getline(cin, searchValue);
            sql =
                "SELECT "
                "CRN, TITLE, DEPT, TIME, DAYS, "
                "SEMESTER, YEAR, CREDITS "
                "FROM COURSE "
                "WHERE DEPT = ? "
                "ORDER BY CRN;";
        }
        else{
            cout << "Enter the semester: ";
            getline(cin, searchValue);
            sql =
                "SELECT "
                "CRN, TITLE, DEPT, TIME, DAYS, "
                "SEMESTER, YEAR, CREDITS "
                "FROM COURSE "
                "WHERE SEMESTER = ? "
                "ORDER BY CRN;";
        }
        result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );
        if (result != SQLITE_OK){
            cout << "Search failed: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }
        sqlite3_bind_text(
            statement,
            1,
            searchValue.c_str(),
            -1,
            SQLITE_TRANSIENT
        );
    }
    else if (selection == 5){
        int year;
        cout << "Enter the course year: ";
        if (!(cin >> year)){
            clearInvalidInput();
            cout << "Invalid year." << endl;
            return;
        }
        sql =
            "SELECT "
            "CRN, TITLE, DEPT, TIME, DAYS, "
            "SEMESTER, YEAR, CREDITS "
            "FROM COURSE "
            "WHERE YEAR = ? "
            "ORDER BY CRN;";

        result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );

        if (result != SQLITE_OK){
            cout << "Search failed: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }
        sqlite3_bind_int(statement, 1, year);
    }
    else{
        cout << "Invalid search option." << endl;
        return;
    }
    bool courseFound = false;
    while (sqlite3_step(statement) == SQLITE_ROW){
        courseFound = true;
        printCourseRow(statement);
    }
    if (courseFound == false){
        cout << "No matching courses were found."<< endl;
    }
    sqlite3_finalize(statement);
}
void Admin::add_course(sqlite3* DB){
    int CRN;
    string courseTitle;
    string department;
    string courseTime;
    string courseDays;
    string semester;
    int year;
    int credits;
    cout << "\n===== ADD COURSE =====" << endl;
    cout << "Enter CRN: ";
    if (!(cin >> CRN)){
        clearInvalidInput();
        cout << "Invalid CRN." << endl;
        return;
    }
    if (courseExists(DB, CRN)){
        cout << "A course with CRN "<< CRN<< " already exists."<< endl;
        return;
    }
    clearInvalidInput();
    cout << "Enter course title: ";
    getline(cin, courseTitle);
    cout << "Enter department: ";
    getline(cin, department);
    if (department.length() != 4){
        cout << "Department must contain exactly "<< "4 characters."<< endl;
        return;
    }
    cout << "Enter course time: ";
    getline(cin, courseTime);
    cout << "Enter course days: ";
    getline(cin, courseDays);
    cout << "Enter semester: ";
    getline(cin, semester);
    cout << "Enter year: ";
    if (!(cin >> year)){
        clearInvalidInput();
        cout << "Invalid year." << endl;
        return;
    }
    cout << "Enter credits: ";
    if (!(cin >> credits)){
        clearInvalidInput();
        cout << "Invalid credit value." << endl;
        return;
    }
    string sql =
        "INSERT INTO COURSE "
        "(CRN, TITLE, DEPT, TIME, DAYS, "
        "SEMESTER, YEAR, CREDITS) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* statement = NULL;

    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK){
        cout << "Unable to prepare course insertion: "<< sqlite3_errmsg(DB)<< endl;
        return;
    }
    sqlite3_bind_int(statement, 1, CRN);
    sqlite3_bind_text(
        statement,
        2,
        courseTitle.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_text(
        statement,
        3,
        department.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_text(
        statement,
        4,
        courseTime.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_text(
        statement,
        5,
        courseDays.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_text(
        statement,
        6,
        semester.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_int(statement, 7, year);
    sqlite3_bind_int(statement, 8, credits);
    result = sqlite3_step(statement);
    if (result == SQLITE_DONE){
        cout << "Course "<< CRN<< " was successfully added."<< endl;
    }
    else{
        cout << "Unable to add course: "<< sqlite3_errmsg(DB)<< endl;
    }
    sqlite3_finalize(statement);
}
void Admin::add_student_or_instructor(sqlite3* DB){
    int selection;
    cout << "\n===== ADD USER =====" << endl;
    cout << "1. Add student" << endl;
    cout << "2. Add instructor" << endl;
    cout << "Enter selection: ";
    if (!(cin >> selection)){
        clearInvalidInput();
        cout << "Invalid selection." << endl;
        return;
    }
    if (selection == 1){
        string studentID;
        string firstName;
        string lastName;
        int gradYear;
        string major;
        cout << "Enter student ID: ";
        cin >> studentID;
        if (studentID.length() != 5 ||studentID.at(0) != '1'){
            cout << "Student IDs must contain 5 digits "<< "and begin with 1."<< endl;
            return;
        }
        if (recordExists(DB,"SELECT ID FROM STUDENT WHERE ID = ?;",studentID)){
            cout << "Student "<< studentID<< " already exists."<< endl;
            return;
        }
        cout << "Enter first name: ";
        cin >> firstName;
        cout << "Enter last name: ";
        cin >> lastName;
        cout << "Enter graduation year: ";
        if (!(cin >> gradYear)){
            clearInvalidInput();
            cout << "Invalid graduation year." << endl;
            return;
        }
        cout << "Enter major: ";
        cin >> major;
        string studentEmail = lastName;
        if (firstName.empty() == false){
            studentEmail += static_cast<char>(tolower(firstName.at(0)));
        }
        studentEmail += "@wit.edu";
        string sql =
            "INSERT INTO STUDENT "
            "(ID, NAME, SURNAME, GRADYEAR, MAJOR, EMAIL) "
            "VALUES (?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* statement = NULL;

        int result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );
        if (result != SQLITE_OK){
            cout << "Unable to add student: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }

        sqlite3_bind_text(
            statement,
            1,
            studentID.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            2,
            firstName.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            3,
            lastName.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_int(
            statement,
            4,
            gradYear
        );

        sqlite3_bind_text(
            statement,
            5,
            major.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            6,
            studentEmail.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        result = sqlite3_step(statement);
        sqlite3_finalize(statement);

        if (result != SQLITE_DONE){
            cout << "Unable to add student: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }

        string studentPassword =
            studentID.substr(0, 1) +
            studentID.substr(
                studentID.length() - 3
            );

        sql =
            "INSERT INTO LOGIN "
            "(ID, PASSWORD, ROLE) "
            "VALUES (?, ?, 'Student');";

        statement = NULL;

        result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );

        if (result == SQLITE_OK){
            sqlite3_bind_text(
                statement,
                1,
                studentID.c_str(),
                -1,
                SQLITE_TRANSIENT
            );

            sqlite3_bind_text(
                statement,
                2,
                studentPassword.c_str(),
                -1,
                SQLITE_TRANSIENT
            );

            sqlite3_step(statement);
        }

        sqlite3_finalize(statement);

        cout << "Student successfully added." << endl;
        cout << "Student ID: "<< studentID<< endl;
        cout << "Password: "<< studentPassword<< endl;
    }
    else if (selection == 2){
        string instructorID;
        string firstName;
        string lastName;
        string instructorTitle;
        int hireYear;
        string department;
        cout << "Enter instructor ID: ";
        cin >> instructorID;
        if (instructorID.length() != 5 ||instructorID.at(0) != '2'){
            cout << "Instructor IDs must contain 5 digits "<< "and begin with 2."<< endl;
            return;
        }
        if (recordExists(
            DB,
            "SELECT ID FROM INSTRUCTOR WHERE ID = ?;",
            instructorID))
        {
            cout << "Instructor "
                << instructorID
                << " already exists."
                << endl;

            return;
        }
        cout << "Enter first name: ";
        cin >> firstName;
        cout << "Enter last name: ";
        cin >> lastName;
        clearInvalidInput();
        cout << "Enter instructor title: ";
        getline(cin, instructorTitle);
        cout << "Enter hire year: ";
        if (!(cin >> hireYear)){
            clearInvalidInput();
            cout << "Invalid hire year." << endl;
            return;
        }
        cout << "Enter department: ";
        cin >> department;
        string instructorEmail = lastName;
        if (firstName.empty() == false){
            instructorEmail += static_cast<char>(tolower(firstName.at(0)));
        }
        instructorEmail += "@wit.edu";
        string sql =
            "INSERT INTO INSTRUCTOR "
            "(ID, NAME, SURNAME, TITLE, "
            "HIREYEAR, DEPT, EMAIL) "
            "VALUES (?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* statement = NULL;

        int result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );

        if (result != SQLITE_OK){
            cout << "Unable to add instructor: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }
        sqlite3_bind_text(
            statement,
            1,
            instructorID.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            2,
            firstName.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            3,
            lastName.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            4,
            instructorTitle.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_int(
            statement,
            5,
            hireYear
        );

        sqlite3_bind_text(
            statement,
            6,
            department.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            statement,
            7,
            instructorEmail.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        result = sqlite3_step(statement);
        sqlite3_finalize(statement);

        if (result != SQLITE_DONE){
            cout << "Unable to add instructor: "<< sqlite3_errmsg(DB)<< endl;
            return;
        }
        string instructorPassword =
            instructorID.substr(0, 1) +
            instructorID.substr(
                instructorID.length() - 3
            );

        sql =
            "INSERT INTO LOGIN "
            "(ID, PASSWORD, ROLE) "
            "VALUES (?, ?, 'Instructor');";

        statement = NULL;

        result = sqlite3_prepare_v2(
            DB,
            sql.c_str(),
            -1,
            &statement,
            NULL
        );

        if (result == SQLITE_OK){
            sqlite3_bind_text(
                statement,
                1,
                instructorID.c_str(),
                -1,
                SQLITE_TRANSIENT
            );

            sqlite3_bind_text(
                statement,
                2,
                instructorPassword.c_str(),
                -1,
                SQLITE_TRANSIENT
            );

            sqlite3_step(statement);
        }

        sqlite3_finalize(statement);

        cout << "Instructor successfully added."<< endl;
        cout << "Instructor ID: "<< instructorID<< endl;
        cout << "Password: "<< instructorPassword<< endl;
    }
    else{
        cout << "Invalid selection." << endl;
    }
}
void Admin::manage_instructor_course(sqlite3* DB){
    int selection;
    string instructorID;
    int CRN;
    cout << "\n===== INSTRUCTOR-COURSE MANAGEMENT ====="<< endl;
    cout << "1. Link instructor to course" << endl;
    cout << "2. Unlink instructor from course" << endl;
    cout << "Enter selection: ";
    if (!(cin >> selection)){
        clearInvalidInput();
        cout << "Invalid selection." << endl;
        return;
    }
    cout << "Enter instructor ID: ";
    cin >> instructorID;
    if (recordExists(
        DB,
        "SELECT ID FROM INSTRUCTOR WHERE ID = ?;",
        instructorID) == false)
    {
        cout << "Instructor "
            << instructorID
            << " was not found."
            << endl;

        return;
    }

    cout << "Enter course CRN: ";
    if (!(cin >> CRN)){
        clearInvalidInput();
        cout << "Invalid CRN." << endl;
        return;
    }
    if (courseExists(DB, CRN) == false){
        cout << "Course "<< CRN<< " was not found."<< endl;
        return;
    }
    string sql;
    if (selection == 1){
        sql =
            "INSERT OR IGNORE INTO INSTRUCTOR_COURSE "
            "(INSTRUCTOR_ID, CRN) "
            "VALUES (?, ?);";
    }
    else if (selection == 2){
        sql =
            "DELETE FROM INSTRUCTOR_COURSE "
            "WHERE INSTRUCTOR_ID = ? "
            "AND CRN = ?;";
    }
    else{
        cout << "Invalid selection." << endl;
        return;
    }
    sqlite3_stmt* statement = NULL;
    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );

    if (result != SQLITE_OK){
        cout << "Operation failed: "<< sqlite3_errmsg(DB)<< endl;
        return;
    }
    sqlite3_bind_text(
        statement,
        1,
        instructorID.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_int(statement, 2, CRN);
    result = sqlite3_step(statement);
    if (result == SQLITE_DONE){
        if (selection == 1){
            if (sqlite3_changes(DB) > 0){
                cout << "Instructor "<< instructorID<< " was linked to course "<< CRN<< "."<< endl;
            }
            else{
                cout << "That instructor is already linked "<< "to that course."<< endl;
            }
        }
        else{
            if (sqlite3_changes(DB) > 0){
                cout << "Instructor "<< instructorID<< " was unlinked from course "<< CRN<< "."<< endl;
            }
            else{
                cout << "That instructor-course link "<< "was not found."<< endl;
            }
        }
    }
    else{
        cout << "Operation failed: "<< sqlite3_errmsg(DB)<< endl;
    }
    sqlite3_finalize(statement);
}
void Admin::manage_student_course(sqlite3* DB){
    int selection;
    string studentID;
    int CRN;
    cout << "\n===== STUDENT-COURSE MANAGEMENT ====="<< endl;
    cout << "1. Add student to course" << endl;
    cout << "2. Remove student from course" << endl;
    cout << "Enter selection: ";
    if (!(cin >> selection)){
        clearInvalidInput();
        cout << "Invalid selection." << endl;
        return;
    }
    cout << "Enter student ID: ";
    cin >> studentID;
    if (recordExists(
        DB,
        "SELECT ID FROM STUDENT WHERE ID = ?;",
        studentID) == false)
    {
        cout << "Student "<< studentID<< " was not found."<< endl;
        return;
    }
    cout << "Enter course CRN: ";
    if (!(cin >> CRN)){
        clearInvalidInput();
        cout << "Invalid CRN." << endl;
        return;
    }
    if (courseExists(DB, CRN) == false){
        cout << "Course "<< CRN<< " was not found."<< endl;
        return;
    }
    string sql;
    if (selection == 1){
        sql =
            "INSERT OR IGNORE INTO STUDENT_COURSE "
            "(STUDENT_ID, CRN) "
            "VALUES (?, ?);";
    }
    else if (selection == 2){
        sql =
            "DELETE FROM STUDENT_COURSE "
            "WHERE STUDENT_ID = ? "
            "AND CRN = ?;";
    }
    else{
        cout << "Invalid selection." << endl;
        return;
    }
    sqlite3_stmt* statement = NULL;
    int result = sqlite3_prepare_v2(
        DB,
        sql.c_str(),
        -1,
        &statement,
        NULL
    );
    if (result != SQLITE_OK){
        cout << "Operation failed: "<< sqlite3_errmsg(DB)<< endl;
        return;
    }
    sqlite3_bind_text(
        statement,
        1,
        studentID.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(statement, 2, CRN);

    result = sqlite3_step(statement);

    if (result == SQLITE_DONE){
        if (selection == 1){
            if (sqlite3_changes(DB) > 0){
                cout << "Student "<< studentID<< " was added to course "<< CRN<< "."<< endl;
            }
            else{
                cout << "The student is already enrolled "<< "in that course."<< endl;
            }
        }
        else{
            if (sqlite3_changes(DB) > 0){
                cout << "Student "<< studentID<< " was removed from course "<< CRN<< "."<< endl;
            }
            else{
                cout << "The student was not enrolled "<< "in that course."<< endl;
            }
        }
    }
    else{
        cout << "Operation failed: "<< sqlite3_errmsg(DB)<< endl;
    }
    sqlite3_finalize(statement);
}

string Admin::getTitle()
{
    return title;
}

string Admin::getOffice()
{
    return office;
}

string Admin::getEmail()
{
    return email;
}