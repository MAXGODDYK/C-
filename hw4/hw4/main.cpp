// Тема 4 (ДЗ). Фабрика об'єктів, final, countStaff через dynamic_cast.

#include <iostream>
#include <vector>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

// ДЗ4, завдання 1: «Фабрика об'єктів» — повертає Person* потрібного типу.
Person* createPerson(const string& type,
                     const string& firstName, const string& midName,
                     const string& lastName, const string& gender, int age) {
    if (type == "student") return new Student(firstName, midName, lastName, gender, age, "PI-00", "College of IT", "none", "none");
    if (type == "teacher") return new Teacher(firstName, midName, lastName, gender, age, "General", 5);
    if (type == "staff")   return new Staff(firstName, midName, lastName, gender, age, "Assistant", "General dept.", "2020-01-01");
    return nullptr;
}

// ДЗ4, завдання 3: підрахунок об'єктів за типом через dynamic_cast.
void countStaff(const vector<Person*>& arr) {
    int students = 0, teachers = 0, staff = 0;
    for (Person* p : arr) {
        if (dynamic_cast<Student*>(p))      ++students;
        else if (dynamic_cast<Teacher*>(p)) ++teachers;
        else if (dynamic_cast<Staff*>(p))   ++staff;
    }
    cout << "  Students: " << students << ", Teachers: " << teachers << ", Staff: " << staff << endl;
}

// ДЗ4, завдання 2: 'final'. Нащадок Teacher не може перевизначити getInfo() —
// розкоментуйте, щоб побачити помилку компіляції C3248:
//
// class SeniorTeacher : public Teacher {
// public:
//     using Teacher::Teacher;
//     string getInfo() const override { return "Senior: " + getFullName(); }
// };
//
// 'final' гарантує, що поведінку методу не змінять у підкласах.

int main() {
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 4 (Homework): factory, final, countStaff ===" << endl << endl;

    vector<Person*> people;
    people.push_back(createPerson("student", "Maksym", "Oleksiyovych", "Hodyk", "Male", 19));
    people.push_back(createPerson("teacher", "Olena", "Ihorivna", "Kovalenko", "Female", 44));
    people.push_back(createPerson("staff", "Andriy", "Mykolayovych", "Shevchenko", "Male", 45));
    people.push_back(new Student("Ivan", "Petrovych", "Ivanenko", "Male", 17, "PI-22", "College of Economics", "none", "chess"));

    cout << "--- Info + formal greeting (polymorphic) ---" << endl;
    for (Person* p : people) {
        cout << "  " << p->getInfo() << endl;
        cout << "    -> " << p->getFormalGreeting() << endl;
    }

    cout << endl << "--- Statistics (dynamic_cast) ---" << endl;
    countStaff(people);

    cout << endl << "--- Free memory (virtual destructors) ---" << endl;
    for (Person* p : people) delete p;

    return 0;
}
