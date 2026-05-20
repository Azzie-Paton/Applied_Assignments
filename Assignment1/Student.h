#pragma once
#include "User.h"
class Student :
    public User
{
public:
    Student();
    Student(string in_first, string in_last, string in_id);
    void search();
    void add_drop();
    void print_sch();
};

