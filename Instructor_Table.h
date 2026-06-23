#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Instructor
{
public:
    string id;
    string firstName;
    string lastName;
    string dateOfBirth;
    string dateOfHire;
    double salary;
    string programName;

    Instructor(string i, string fn, string ln, string dob, string doh, double sal, string prog)
    {
        id = i;
        firstName = fn;
        lastName = ln;
        dateOfBirth = dob;
        dateOfHire = doh;
        salary = sal;
        programName = prog;
    }
};