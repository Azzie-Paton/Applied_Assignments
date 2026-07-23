#pragma once
#include "User.h"
#include "sqlite3.h"
class Instructor :
    public User
{
public:
    string title;
    int yofH;
    string department;
    string email;
    //Instructor();
    Instructor(string in_first, string in_last, string in_id, string in_title, int in_yofH, string in_depart);
    void print_schedule(sqlite3* DB);
    void print_class_list(sqlite3* DB);
    void search_course(sqlite3* DB, int CRN);
    void search_roster(sqlite3* DB, int studentID);
    string getTitle();
    int getHireYear();
    string getDepartment();
    string getEmail();
};

