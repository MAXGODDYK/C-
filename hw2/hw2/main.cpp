// Тема 2 (ДЗ). 5+ об'єктів у vector<unique_ptr<Person>>, getShortInfo, хобі.

#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

// ДЗ2: вивід короткої інформації про всіх осіб.
void printAllShortInfo(const vector<unique_ptr<Person>>& people) {
    for (const auto& person : people) {
        cout << "  " << person->getShortInfo() << endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 2 (Homework): Staff, getShortInfo, hobbies, unique_ptr ===" << endl << endl;

    vector<unique_ptr<Person>> people;

    // Двоє студентів із РІЗНИМИ хобі.
    auto s1 = make_unique<Student>("Hodyk Maksym Oleksiyovych", 19, "PI-21", "College of IT");
    s1->setAverageGrade(4.5);
    s1->addHobby("guitar");
    s1->addHobby("3D modeling");
    people.push_back(std::move(s1));

    auto s2 = make_unique<Student>("Ivanenko Ivan Petrovych", 18, "PI-22", "College of Economics");
    s2->setAverageGrade(3.8);
    s2->addHobby("chess");
    people.push_back(std::move(s2));

    people.push_back(make_unique<Teacher>("Kovalenko Olena Ihorivna", 44, "OOP C++", 20));
    people.push_back(make_unique<Teacher>("Shevchuk Petro Mykolayovych", 33, "Algorithms", 8));
    people.push_back(make_unique<Staff>("Shevchenko Andriy Mykolayovych", 45,
        "Lab assistant", "IT department", "2018-09-01"));

    cout << "--- Full info ---" << endl;
    for (const auto& person : people) {
        cout << "  " << person->getInfo() << endl;
    }

    cout << endl << "--- Short info (name + role) ---" << endl;
    printAllShortInfo(people);

    return 0;
}
