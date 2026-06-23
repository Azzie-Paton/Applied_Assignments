#pragma once
#include "User.h"

class Admin :
    public User
{
public:
    string title;
    string office;
    string email;

    Admin(string in_first, string in_last, string in_id, string in_title, string in_office);

    void add_course();
    void remove_course();
    void add_remove_user();
    void add_remove_student();
    void search_roster();
    void print_roster();
    void search_courses();
    void print_courses();
    string getTitle();
    string getOffice();
    string getEmail();
};

