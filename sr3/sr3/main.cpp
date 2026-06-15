#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

// Count objects by type via dynamic_cast.
void countStaff(const vector<unique_ptr<Person>>& people) {
    int students = 0, teachers = 0, staff = 0;
    for (const auto& p : people) {
        if (dynamic_cast<Student*>(p.get()))      ++students;
        else if (dynamic_cast<Teacher*>(p.get())) ++teachers;
        else if (dynamic_cast<Staff*>(p.get()))   ++staff;
    }
    cout << "  Students: " << students << endl;
    cout << "  Teachers: " << teachers << endl;
    cout << "  Staff:    " << staff    << endl;
}

int main() {
    SetConsoleOutputCP(65001);  // UTF-8 for the Windows console
    cout << "========================================" << endl;
    cout << " StudentPortfolio - Themes 1-4: Summary " << endl;
    cout << "========================================" << endl << endl;

    vector<unique_ptr<Person>> people;

    people.push_back(make_unique<Teacher>(
        "Olena", "Ihorivna", "Kovalenko", "Female", 34,
        "OOP C++", "Senior Lecturer"));

    people.push_back(make_unique<Staff>(
        "Andriy", "Mykolayovych", "Shevchenko", "Male", 45,
        "Lab assistant", "day"));

    // replace with your own real data
    people.push_back(make_unique<Student>(
        "Maksym", "Oleksiyovych", "Hodyk", "Male", 19,
        "PI-21", "cat Barsyk", "football"));

    people.push_back(make_unique<Student>(
        "Ivan", "Petrovych", "Ivanenko", "Male", 17,
        "PI-22", "none", "chess"));

    cout << "--- Roster ---" << endl;
    for (const auto& p : people) {
        cout << "  " << p->getInfo() << endl;
    }

    cout << endl << "--- Formal greetings ---" << endl;
    for (const auto& p : people) {
        cout << "  " << p->getFormalGreeting() << endl;
    }

    cout << endl << "--- Students (dynamic_cast) ---" << endl;
    for (const auto& p : people) {
        if (Student* s = dynamic_cast<Student*>(p.get())) {
            cout << "  " << s->getLiveGreeting() << endl;
            cout << "  List: " << s->getListFormat() << endl;
            cout << "  " << s->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Teachers (dynamic_cast) ---" << endl;
    for (const auto& p : people) {
        if (Teacher* t = dynamic_cast<Teacher*>(p.get())) {
            cout << "  " << t->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Staff (dynamic_cast) ---" << endl;
    for (const auto& p : people) {
        if (Staff* st = dynamic_cast<Staff*>(p.get())) {
            cout << "  " << st->getBadgeText() << endl;
            cout << "  " << st->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Statistics ---" << endl;
    countStaff(people);

    cout << endl << "--- Setters with validation ---" << endl;
    for (const auto& p : people) {
        if (Student* s = dynamic_cast<Student*>(p.get())) {
            cout << "  Sport before: " << s->getSport() << endl;
            s->setSport("chess");
            cout << "  Sport after: " << s->getSport() << endl;
            bool bad = s->setGroup("VERY-LONG-NAME");
            cout << "  setGroup (too long): " << (bad ? "OK" : "rejected") << endl;
            break;
        }
    }

    cout << endl << "--- Finalization (destructors) ---" << endl;
    return 0;
}
