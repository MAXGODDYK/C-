#pragma once

#include "Person.h"

// Тема 2 (СР). Нащадок Person через PUBLIC-спадкування.
class Student : public Person {
private:
    std::string group_;
    std::string college_;
    double averageGrade_;

public:
    Student(const std::string& fullName, int age,
            const std::string& group, const std::string& college)
        // Обов'язково ініціалізуємо базовий клас у списку ініціалізації.
        : Person(fullName, age), group_(group), college_(college), averageGrade_(0.0) {}

    // Перевизначення getInfo() з ключовим словом override.
    std::string getInfo() const override {
        return "Student: " + getFullName()        // getFullName() з базового класу
            + ", group: " + group_
            + ", college: " + college_
            + ", GPA: " + std::to_string(averageGrade_);
    }

    std::string getGroup() const { return group_; }
    void setGroup(const std::string& group) { group_ = group; }

    double getAverageGrade() const { return averageGrade_; }

    // Валідація: середній бал від 2.0 до 5.0.
    bool setAverageGrade(double grade) {
        if (grade >= 2.0 && grade <= 5.0) {
            averageGrade_ = grade;
            return true;
        }
        return false;
    }
};
