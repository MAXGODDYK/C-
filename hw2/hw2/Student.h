#pragma once

#include "Person.h"

#include <vector>

// Тема 2 (ДЗ). Реалізація — у Student.cpp.
class Student : public Person {
private:
    std::string group_;
    std::string college_;
    double averageGrade_;
    std::vector<std::string> hobbies_;

public:
    Student(const std::string& fullName, int age,
            const std::string& group, const std::string& college);

    std::string getRole() const override;

    void addHobby(const std::string& hobby);
    std::vector<std::string> getHobbies() const;

    std::string getInfo() const override;

    std::string getGroup() const;
    void setGroup(const std::string& group);
    double getAverageGrade() const;
    bool setAverageGrade(double grade);
};
