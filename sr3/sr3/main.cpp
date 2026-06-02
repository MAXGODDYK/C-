#include <iostream>

#include "Staff.h"
#include "Student.h"
#include "Teacher.h"

using namespace std;

int main() {
    Student student(
        "Іван",
        "Петрович",
        "Іваненко",
        "Чоловіча",
        19,
        "ПІ-21",
        "кіт Барсик",
        "футбол");

    Teacher teacher(
        "Олена",
        "Ігорівна",
        "Коваленко",
        "Жіноча",
        34,
        "ООП C++",
        "старший викладач");

    Staff staff(
        "Андрій",
        "Миколайович",
        "Шевченко",
        "Чоловіча",
        17,
        "лаборант",
        "денна");

    cout << "=== Student ===" << endl;
    cout << student.getInfo() << endl;
    cout << student.getLiveGreeting() << endl;
    cout << student.getListFormat() << endl;
    cout << student.getAdultStatus() << endl;
    cout << "Спорт до зміни: " << student.getSport() << endl;
    student.setSport("шахи");
    cout << "Спорт після зміни: " << student.getSport() << endl;

    cout << endl << "=== Teacher ===" << endl;
    cout << teacher.getInfo() << endl;
    cout << teacher.getFormalGreeting() << endl;
    cout << teacher.getAdultStatus() << endl;

    cout << endl << "=== Staff ===" << endl;
    cout << staff.getInfo() << endl;
    cout << staff.getBadgeText() << endl;
    cout << staff.getAdultStatus() << endl;

    

    return 0;
}
