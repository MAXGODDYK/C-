#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

using namespace std;

// підрахунок обєктів за типом через dynamic_cast
void countStaff(const vector<unique_ptr<Person>>& people) {
    int students = 0, teachers = 0, staff = 0;
    for (const auto& p : people) {
        if (dynamic_cast<Student*>(p.get()))      ++students;
        else if (dynamic_cast<Teacher*>(p.get())) ++teachers;
        else if (dynamic_cast<Staff*>(p.get()))   ++staff;
    }
    cout << "  Студентів:  " << students << endl;
    cout << "  Викладачів: " << teachers << endl;
    cout << "  Персоналу:  " << staff    << endl;
}

int main() {
    SetConsoleOutputCP(65001);  // UTF-8 для консолі Windows
    cout << "========================================" << endl;
    cout << " StudentPortfolio — Теми 1-4: Підсумок " << endl;
    cout << "========================================" << endl << endl;

    vector<unique_ptr<Person>> people;

    people.push_back(make_unique<Teacher>(
        "Олена", "Ігорівна", "Коваленко", "Жіноча", 34,
        "ООП C++", "старший викладач"));

    people.push_back(make_unique<Staff>(
        "Андрій", "Миколайович", "Шевченко", "Чоловіча", 45,
        "лаборант", "денна"));

    // замініть на свої реальні дані
    people.push_back(make_unique<Student>(
        "Максим", "Олексійович", "Годик", "Чоловіча", 19,
        "ПІ-21", "кіт Барсик", "футбол"));

    people.push_back(make_unique<Student>(
        "Іван", "Петрович", "Іваненко", "Чоловіча", 17,
        "ПІ-22", "немає", "шахи"));

    cout << "--- Списочний склад ---" << endl;
    for (const auto& p : people) {
        cout << "  " << p->getInfo() << endl;
    }

    cout << endl << "--- Офіційні привітання ---" << endl;
    for (const auto& p : people) {
        cout << "  " << p->getFormalGreeting() << endl;
    }

    cout << endl << "--- Студенти (dynamic_cast) ---" << endl;
    for (const auto& p : people) {
        if (Student* s = dynamic_cast<Student*>(p.get())) {
            cout << "  " << s->getLiveGreeting() << endl;
            cout << "  Список: " << s->getListFormat() << endl;
            cout << "  " << s->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Викладачі (dynamic_cast) ---" << endl;
    for (const auto& p : people) {
        if (Teacher* t = dynamic_cast<Teacher*>(p.get())) {
            cout << "  " << t->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Персонал (dynamic_cast) ---" << endl;
    for (const auto& p : people) {
        if (Staff* st = dynamic_cast<Staff*>(p.get())) {
            cout << "  " << st->getBadgeText() << endl;
            cout << "  " << st->getAdultStatus() << endl;
        }
    }

    cout << endl << "--- Статистика ---" << endl;
    countStaff(people);

    cout << endl << "--- Сеттери з валідацією ---" << endl;
    for (const auto& p : people) {
        if (Student* s = dynamic_cast<Student*>(p.get())) {
            cout << "  Спорт до: " << s->getSport() << endl;
            s->setSport("шахи");
            cout << "  Спорт після: " << s->getSport() << endl;
            bool bad = s->setGroup("ДУЖЕ-ДОВГА-НАЗВА");
            cout << "  setGroup (занадто довга): " << (bad ? "OK" : "відхилено") << endl;
            break;
        }
    }

    cout << endl << "--- Завершення (деструктори) ---" << endl;
    return 0;
}
