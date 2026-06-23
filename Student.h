#pragma once
#include "User.h"
class Student :
    public User
{
public:
    int gradY;
    string major;
    string email;
    //Student();
    Student(string in_first, string in_last, string in_id, int grad, string m);
    void search();
    void search(int crn);
    void add_drop(int a_d, int crn);
    void print_sch();
    void conflict();
    string getMajor();
    int getGradYear();
    string getEmail();
};

