// Тема 4 (об'єднання СР4 + ДЗ4). Підсумковий демонстраційний приклад.

#include <iostream>
#include <vector>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

// ДЗ4, завдання 1: фабрика об'єктів.
Person* createPerson(const string& type,
                     const string& firstName, const string& midName,
                     const string& lastName, const string& gender, int age) {
    if (type == "student") return new Student(firstName, midName, lastName, gender, age, "PI-00", "College of IT", "none", "none");
    if (type == "teacher") return new Teacher(firstName, midName, lastName, gender, age, "General", 5);
    if (type == "staff")   return new Staff(firstName, midName, lastName, gender, age, "Assistant", "General dept.", "2020-01-01");
    return nullptr;
}

// ДЗ4, завдання 3: підрахунок за типом через dynamic_cast.
void countStaff(const vector<Person*>& arr) {
    int students = 0, teachers = 0, staff = 0;
    for (Person* p : arr) {
        if (dynamic_cast<Student*>(p))      ++students;
        else if (dynamic_cast<Teacher*>(p)) ++teachers;
        else if (dynamic_cast<Staff*>(p))   ++staff;
    }
    cout << "  Students: " << students << ", Teachers: " << teachers << ", Staff: " << staff << endl;
}

// ДЗ4, завдання 2: 'final' на Teacher::getInfo() — нащадок не перевизначить його
// (розкоментуйте SeniorTeacher, щоб побачити помилку C3248).

int main() {
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 4 (combined: SR4 + HW4) ===" << endl << endl;

    vector<Person*> people;
    people.push_back(createPerson("teacher", "Olena", "Ihorivna", "Kovalenko", "Female", 44));   // фабрика
    people.push_back(createPerson("staff", "Andriy", "Mykolayovych", "Shevchenko", "Male", 45));
    auto* maksym = new Student("Maksym", "Oleksiyovych", "Hodyk", "Male", 19, "PI-21", "College of IT", "cat Barsyk", "football");
    maksym->setAverageGrade(4.5); maksym->addHobby("guitar");
    people.push_back(maksym);
    people.push_back(new Student("Ivan", "Petrovych", "Ivanenko", "Male", 17, "PI-22", "College of Economics", "none", "chess"));

    cout << "--- Info + formal greeting (polymorphic) ---" << endl;
    for (Person* p : people) {
        cout << "  " << p->getInfo() << endl;
        cout << "    -> " << p->getFormalGreeting() << endl;
    }

    cout << endl << "--- dynamic_cast: students greet casually, others show adult status ---" << endl;
    for (Person* p : people) {
        if (Student* s = dynamic_cast<Student*>(p))      cout << "  " << s->getCasualGreeting() << endl;
        else if (Teacher* t = dynamic_cast<Teacher*>(p)) cout << "  " << t->getAdultStatus() << endl;
        else if (Staff* sf = dynamic_cast<Staff*>(p))    cout << "  " << sf->getAdultStatus() << endl;
    }

    cout << endl << "--- Statistics (dynamic_cast) ---" << endl;
    countStaff(people);

    cout << endl << "--- Free memory (virtual destructors) ---" << endl;
    for (Person* p : people) delete p;

    return 0;
}
