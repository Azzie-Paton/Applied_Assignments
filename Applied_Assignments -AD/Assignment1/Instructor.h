#pragma once
#include "User.h"
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
    void print_schedule();
    void print_class_list();
    void search_course();
    string getTitle();
    int getHireYear();
    string getDepartment();
    string getEmail();
};

