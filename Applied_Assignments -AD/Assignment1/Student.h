#pragma once
#include "User.h"
#include "sqlite3.h"

class Student : public User
{
public:
    int gradY;
    string major;
    string email;

    Student(string in_first, string in_last, string in_id,
        int grad, string m);

    void search(sqlite3* DB, int CRN);

    void add_drop(sqlite3* DB, int CRN, int choice);
    void print_sch(sqlite3* DB);
    void conflict(sqlite3* DB);

    string getMajor();
    int getGradYear();
    string getEmail();
};