// Тема 3 (ДЗ). Варіант А «Захищений помічник»: protected isAdult().

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
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 3 (Homework): protected fields + isAdult() (Variant A) ===" << endl << endl;

    // Чому protected для gender_/age_, а не public чи private:
    //  - private приховало б ці поля навіть від нащадків (потрібні були б геттери);
    //  - public відкрив би їх усьому коду в main() і зламав інкапсуляцію;
    //  - protected — нащадки читають поля напряму (повторне використання),
    //    але ззовні (з main()) вони недоступні.
    // isAdult() теж protected: це внутрішня деталь для нащадків, не публічний API.

    vector<unique_ptr<Person>> people;
    people.push_back(make_unique<Student>("Hodyk Maksym", "Male", 19, "PI-21", "College of IT", "cat Barsyk", "football"));
    people.push_back(make_unique<Student>("Ivanenko Ivan", "Male", 17, "PI-22", "College of Economics", "none", "chess"));
    people.push_back(make_unique<Teacher>("Kovalenko Olena", "Female", 44, "OOP C++", 20));
    people.push_back(make_unique<Staff>("Shevchenko Andriy", "Male", 45, "Lab assistant", "IT department", "2018-09-01"));

    cout << "--- getAdultStatus (uses protected isAdult / age_ / gender_) ---" << endl;
    for (const auto& p : people) {
        if (auto* s = dynamic_cast<Student*>(p.get()))      cout << "  " << s->getAdultStatus() << endl;
        else if (auto* t = dynamic_cast<Teacher*>(p.get())) cout << "  " << t->getAdultStatus() << endl;
        else if (auto* st = dynamic_cast<Staff*>(p.get()))  cout << "  " << st->getAdultStatus() << endl;
    }

    return 0;
}
