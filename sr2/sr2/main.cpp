// Тема 2 (СР). Тестування інкапсуляції, спадкування та поліморфізму.

#include <iostream>
#include <vector>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"

using namespace std;

// Поліморфний вивід через ПОСИЛАННЯ на базовий клас.
void printInfo(const Person& p) {
    cout << "  " << p.getInfo() << endl;
}

int main() {
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 2 (Self-study): encapsulation / inheritance / polymorphism ===" << endl << endl;

    // Об'єкти з індивідуальними даними.
    Student myStudent("Hodyk Maksym Oleksiyovych", 19, "PI-21", "College of IT");
    myStudent.setAverageGrade(4.5);

    Teacher myTeacher("Kovalenko Olena Ihorivna", 44, "OOP C++", 20);

    cout << "--- printInfo (reference to base) ---" << endl;
    printInfo(myStudent);
    printInfo(myTeacher);

    // Поліморфізм через ВЕКТОР ПОКАЖЧИКІВ на базовий клас.
    vector<Person*> people;
    people.push_back(&myStudent);
    people.push_back(&myTeacher);

    cout << endl << "--- Polymorphic loop (vector<Person*>) ---" << endl;
    for (const Person* p : people) {
        cout << "  " << p->getInfo() << endl;
    }

    // setAge з валідацією.
    cout << endl << "--- setAge validation ---" << endl;
    cout << "  setAge(200): " << (myStudent.setAge(200) ? "accepted" : "rejected") << endl;
    cout << "  setAge(20):  " << (myStudent.setAge(20) ? "accepted" : "rejected") << endl;

    return 0;
}
