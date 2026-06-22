#pragma once

#include "Person.h"

// Тема 2 (СР). Нащадок Person (public). Реалізація — у Student.cpp.
class Student : public Person {
private:
    std::string group_;
    std::string college_;
    double averageGrade_;

public:
    Student(const std::string& fullName, int age,
            const std::string& group, const std::string& college);

    std::string getInfo() const override;

    std::string getGroup() const;
    void setGroup(const std::string& group);

    double getAverageGrade() const;
    // Валідація: середній бал від 2.0 до 5.0.
    bool setAverageGrade(double grade);
};
