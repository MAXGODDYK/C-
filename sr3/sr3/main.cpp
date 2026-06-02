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

    // Доказ доступу:
    // cout << student.lastName_ << endl;
    // Помилка компіляції: lastName_ є protected.
    // Protected краще за public для успадкування, бо нащадки можуть повторно
    // використовувати спільні поля Person, але main() не може напряму ламати
    // інкапсуляцію або залежати від внутрішньої будови класу.
    //
    // cout << student.isAdult() << endl;
    // Помилка компіляції: isAdult() є protected.
    // Цей метод краще зробити protected, а не public, бо це внутрішній
    // допоміжний метод для Student, Teacher і Staff. Зовнішній код повинен
    // отримувати готові повідомлення через public-методи getAdultStatus().
    //
    // cout << student.firstName_ << endl;
    // Помилка компіляції: firstName_ є private.
    // Private відрізняється від protected тим, що private видно тільки
    // всередині Person. Навіть Student, Teacher і Staff не можуть читати
    // firstName_ напряму, тому використовують public-геттер getFirstName().

    return 0;
}
