#include "Student.h"

Student::Student(const std::string& fullName, int age,
                 const std::string& group, const std::string& college)
    // Обов'язково ініціалізуємо базовий клас у списку ініціалізації.
    : Person(fullName, age), group_(group), college_(college), averageGrade_(0.0) {}

std::string Student::getInfo() const {
    return "Student: " + getFullName()        // getFullName() з базового класу
        + ", group: " + group_
        + ", college: " + college_
        + ", GPA: " + std::to_string(averageGrade_);
}

std::string Student::getGroup() const { return group_; }
void Student::setGroup(const std::string& group) { group_ = group; }

double Student::getAverageGrade() const { return averageGrade_; }

bool Student::setAverageGrade(double grade) {
    if (grade >= 2.0 && grade <= 5.0) { averageGrade_ = grade; return true; }
    return false;
}
