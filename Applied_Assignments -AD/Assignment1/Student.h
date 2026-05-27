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
    void add_drop();
    void print_sch();
    string getMajor();
    int getGradYear();
    string getEmail();
};

