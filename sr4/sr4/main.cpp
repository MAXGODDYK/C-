// Тема 4 (СР). Абстрактний клас, dynamic_cast, ручне звільнення пам'яті.

#include <iostream>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

int main() {
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 4 (Self-study): abstract class, dynamic_cast, memory ===" << endl << endl;

    const int N = 3;
    Person* people[N];
    people[0] = new Student("Ivan", "Petrovych", "Ivanenko", "Male", 19, "PI-21", "College of IT", "none", "chess");
    people[1] = new Teacher("Olena", "Ihorivna", "Kovalenko", "Female", 44, "OOP C++", 20);
    people[2] = new Staff("Andriy", "Mykolayovych", "Shevchenko", "Male", 45, "Lab assistant", "IT department", "2018-09-01");

    cout << "--- Polymorphic info (abstract Person) ---" << endl;
    for (int i = 0; i < N; ++i) cout << "  " << people[i]->getInfo() << endl;

    cout << endl << "--- dynamic_cast -> getCasualGreeting (students only) ---" << endl;
    for (int i = 0; i < N; ++i) {
        if (Student* s = dynamic_cast<Student*>(people[i])) {
            cout << "  " << s->getCasualGreeting() << endl;
        }
    }

    cout << endl << "--- Formal greetings ---" << endl;
    for (int i = 0; i < N; ++i) cout << "  " << people[i]->getFormalGreeting() << endl;

    cout << endl << "--- Free memory (virtual destructors) ---" << endl;
    for (int i = 0; i < N; ++i) delete people[i];   // нащадок, потім Person

    return 0;
}
