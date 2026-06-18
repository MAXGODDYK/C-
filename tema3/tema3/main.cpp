// Тема 3 (об'єднання СР3 + ДЗ3).

#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    cout << "=== Theme 3 (combined: SR3 + HW3) ===" << endl << endl;

    vector<unique_ptr<Person>> people;
    auto s = make_unique<Student>("Maksym", "Oleksiyovych", "Hodyk", "Male", 19,
        "PI-21", "College of IT", "cat Barsyk", "football");
    s->setAverageGrade(4.5); s->addHobby("guitar");
    cout << "  " << s->getLiveGreeting() << "  List: " << s->getListFormat() << endl;
    people.push_back(std::move(s));
    people.push_back(make_unique<Student>("Ivan", "Petrovych", "Ivanenko", "Male", 17,
        "PI-22", "College of Economics", "none", "chess"));
    people.push_back(make_unique<Teacher>("Olena", "Ihorivna", "Kovalenko", "Female", 44, "OOP C++", 20));
    people.push_back(make_unique<Staff>("Andriy", "Mykolayovych", "Shevchenko", "Male", 45,
        "Lab assistant", "IT department", "2018-09-01"));

    cout << endl << "--- Info ---" << endl;
    for (const auto& p : people) cout << "  " << p->getInfo() << endl;

    cout << endl << "--- Adult status (protected isAdult) ---" << endl;
    for (const auto& p : people) {
        if (auto* st = dynamic_cast<Student*>(p.get()))      cout << "  " << st->getAdultStatus() << endl;
        else if (auto* t = dynamic_cast<Teacher*>(p.get()))  cout << "  " << t->getAdultStatus() << endl;
        else if (auto* sf = dynamic_cast<Staff*>(p.get()))   cout << "  " << sf->getAdultStatus() << endl;
    }

    return 0;
}
