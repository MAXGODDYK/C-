#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

// «Фабрика об'єктів» (Тема 4, завдання 1): повертає Person* потрібного типу.
// Тип обирається рядком type через if/else. Спеціфічні поля заповнюємо
// типовими значеннями — головне показати поліморфне створення.
Person* createPerson(const string& type,
                     const string& firstName, const string& middleName,
                     const string& lastName, const string& gender, int age) {
    if (type == "student") {
        return new Student(firstName, middleName, lastName, gender, age,
                           "PI-00", "none", "none");
    }
    if (type == "teacher") {
        return new Teacher(firstName, middleName, lastName, gender, age,
                           "General", "Lecturer", 5);
    }
    if (type == "staff") {
        return new Staff(firstName, middleName, lastName, gender, age,
                         "Assistant", "General dept.", "2020-01-01");
    }
    return nullptr;   // невідомий тип
}

// Виводить коротку інформацію (ПІБ + роль) про всіх осіб (Тема 2 ДЗ).
void printAllShortInfo(const vector<unique_ptr<Person>>& people) {
    for (const auto& person : people) {
        cout << "  " << person->getShortInfo() << endl;
    }
}

// Підрахунок об'єктів за типом через dynamic_cast (Тема 4, завдання 3).
void countStaff(const vector<unique_ptr<Person>>& people) {
    int students = 0, teachers = 0, staff = 0;
    for (const auto& person : people) {
        if (dynamic_cast<Student*>(person.get()))      ++students;
        else if (dynamic_cast<Teacher*>(person.get())) ++teachers;
        else if (dynamic_cast<Staff*>(person.get()))   ++staff;
    }
    cout << "  Students: " << students << endl;
    cout << "  Teachers: " << teachers << endl;
    cout << "  Staff:    " << staff    << endl;
}

// --- Тема 4, завдання 2: ключове слово final ---
// Teacher::getInfo() оголошено як 'override final', тому НАЩАДОК Teacher
// не може його перевизначити. Якщо розкоментувати клас нижче — буде
// помилка компіляції C3248 ('function declared as final cannot be overridden'):
//
// class SeniorTeacher : public Teacher {
// public:
//     using Teacher::Teacher;
//     string getInfo() const override { return "Senior: " + getLastName(); }
// };
//
// 'final' корисний, коли поведінку методу не можна змінювати в підкласах:
// гарантія інваріанта класу, безпека та можливість девіртуалізації (оптимізація).

int main() {
    SetConsoleOutputCP(65001);  // UTF-8 для консолі Windows
    cout << "========================================" << endl;
    cout << " StudentPortfolio - Themes 1-4: Summary " << endl;
    cout << "========================================" << endl << endl;

    // Чому protected, а не public/private для age_, gender_, lastName_:
    //  - private приховало б ці поля навіть від нащадків (довелося б писати геттери);
    //  - public відкрив би їх усьому коду в main() і зламав би інкапсуляцію;
    //  - protected — золота середина: нащадки (Student/Teacher/Staff) читають поля
    //    напряму (повторне використання), але ззовні вони недоступні.
    // Так само isAdult() зроблено protected: це деталь реалізації для нащадків,
    // а не частина публічного інтерфейсу.

    // Колекція володіючих покажчиків: vector<unique_ptr<Person>> уникає зрізу
    // об'єктів і сама звільняє пам'ять (RAII).
    vector<unique_ptr<Person>> people;

    people.push_back(make_unique<Teacher>(
        "Olena", "Ihorivna", "Kovalenko", "Female", 34,
        "OOP C++", "Senior Lecturer", 12));

    people.push_back(make_unique<Staff>(
        "Andriy", "Mykolayovych", "Shevchenko", "Male", 45,
        "Lab assistant", "IT department", "2018-09-01"));

    // Двоє студентів із РІЗНИМИ хобі (Тема 2 ДЗ).
    auto maksym = make_unique<Student>(
        "Maksym", "Oleksiyovych", "Hodyk", "Male", 19,
        "PI-21", "cat Barsyk", "football");
    maksym->addHobby("guitar");
    maksym->addHobby("3D modeling");
    people.push_back(std::move(maksym));

    auto ivan = make_unique<Student>(
        "Ivan", "Petrovych", "Ivanenko", "Male", 17,   // вік < 18 -> неповнолітній
        "PI-22", "none", "chess");
    ivan->addHobby("chess club");
    people.push_back(std::move(ivan));

    // П'ятий об'єкт створено ФАБРИКОЮ (демонстрація createPerson).
    Person* madeByFactory = createPerson("teacher", "Petro", "Mykolayovych",
                                         "Shevchuk", "Male", 33);
    if (madeByFactory != nullptr) {
        people.push_back(unique_ptr<Person>(madeByFactory));
    }

    cout << "--- Roster (full info) ---" << endl;
    for (const auto& person : people) {
        cout << "  " << person->getInfo() << endl;
    }

    cout << endl << "--- Short info (name + role) ---" << endl;
    printAllShortInfo(people);

    cout << endl << "--- Formal greetings ---" << endl;
    for (const auto& person : people) {
        cout << "  " << person->getFormalGreeting() << endl;
    }

    cout << endl << "--- Students (dynamic_cast) ---" << endl;
    for (const auto& person : people) {
        if (Student* student = dynamic_cast<Student*>(person.get())) {
            cout << "  " << student->getLiveGreeting() << endl;
            cout << "  List: " << student->getListFormat() << endl;
            cout << "  " << student->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Teachers (dynamic_cast) ---" << endl;
    for (const auto& person : people) {
        if (Teacher* teacher = dynamic_cast<Teacher*>(person.get())) {
            cout << "  " << teacher->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Staff (dynamic_cast) ---" << endl;
    for (const auto& person : people) {
        if (Staff* staffMember = dynamic_cast<Staff*>(person.get())) {
            cout << "  " << staffMember->getBadgeText() << endl;
            cout << "  " << staffMember->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Statistics ---" << endl;
    countStaff(people);

    cout << endl << "--- Setters with validation ---" << endl;
    for (const auto& person : people) {
        if (Student* student = dynamic_cast<Student*>(person.get())) {
            cout << "  Sport before: " << student->getSport() << endl;
            student->setSport("chess");
            cout << "  Sport after:  " << student->getSport() << endl;
            bool accepted = student->setGroup("VERY-LONG-NAME");   // > 10 символів
            cout << "  setGroup (too long): " << (accepted ? "accepted" : "rejected") << endl;
            break;
        }
    }

    cout << endl << "--- Finalization (destructors) ---" << endl;
    // Вихід зі scope -> unique_ptr звільняє об'єкти -> викликаються деструктори
    // нащадка, а потім Person (видно у виводі завдяки cout у кожному деструкторі).
    return 0;
}
