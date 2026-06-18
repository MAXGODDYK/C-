// Тема 3 (СР). Тестування доступу до полів за новою моделлю імені.

#include <iostream>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

int main() {
    SetConsoleOutputCP(65001);   // UTF-8 для консолі Windows
    cout << "=== Theme 3 (Self-study): split-name model, protected access ===" << endl << endl;

    Student s("Ivan", "Petrovych", "Ivanenko", "Male", 19, "PI-21", "College of IT");
    s.setAverageGrade(4.2);

    cout << s.getInfo() << endl;            // public-геттери
    cout << s.getLiveGreeting() << endl;    // доступ всередині класу
    cout << "List: " << s.getListFormat() << endl;

    // ВАЖЛИВО: доступ до protected-поля ЗЗОВНІ заборонено — рядок нижче НЕ компілюється:
    //   cout << s.lastName_;   // error C2248: 'Person::lastName_' is protected
    // Саме тому protected приховує поле від main(), але лишає доступним нащадкам.

    return 0;
}
