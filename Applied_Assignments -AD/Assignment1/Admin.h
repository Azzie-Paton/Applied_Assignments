#pragma once

#include "User.h"
#include "sqlite3.h"

class Admin : public User
{
public:
    string title;
    string office;
    string email;
    Admin(
        string in_first,
        string in_last,
        string in_id,
        string in_title,
        string in_office
    );
    void search_courses(sqlite3* DB);
    void search_courses_by_parameter(sqlite3* DB);
    void add_course(sqlite3* DB);
    void add_student_or_instructor(sqlite3* DB);
    void manage_instructor_course(sqlite3* DB);
    void manage_student_course(sqlite3* DB);
    string getTitle();
    string getOffice();
    string getEmail();
};