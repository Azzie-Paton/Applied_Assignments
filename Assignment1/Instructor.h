#pragma once
#include "User.h"
class Instructor :
    public User
{
public:
    Instructor();
    Instructor(string in_first, string in_last, string in_id);
    void print_schedule();
    void print_class_list();
    void search_course();
};

