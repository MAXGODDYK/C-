#pragma once

#include "Person.h"

#include <vector>

// Тема 3 (СР). getLiveGreeting() та getListFormat() демонструють
// доступ до полів базового класу (private — через геттер, protected — напряму).
class Student : public Person {
private:
    std::string group_;
    std::string college_;
    double averageGrade_;
    std::vector<std::string> hobbies_;

public:
    Student(const std::string& firstName, const std::string& midName, const std::string& lastName,
            const std::string& gender, int age,
            const std::string& group, const std::string& college)
        : Person(firstName, midName, lastName, gender, age),
          group_(group), college_(college), averageGrade_(0.0) {}

    std::string getRole() const override { return "Student"; }

    void addHobby(const std::string& hobby) { if (!hobby.empty()) hobbies_.push_back(hobby); }
    std::vector<std::string> getHobbies() const { return hobbies_; }

    std::string getInfo() const override {
        std::string info = "Student: " + getFullName()
            + ", group: " + group_
            + ", college: " + college_
            + ", GPA: " + std::to_string(averageGrade_);
        if (!hobbies_.empty()) {
            info += ", hobbies: ";
            for (std::size_t i = 0; i < hobbies_.size(); ++i) {
                info += hobbies_[i];
                if (i + 1 < hobbies_.size()) info += ", ";
            }
        }
        return info;
    }

    // firstName_ — private у Person, тож читаємо через геттер getFirstName().
    std::string getLiveGreeting() const { return "Hi, " + getFirstName() + "!"; }
    // lastName_ — protected, тож доступний НАПРЯМУ; ім'я — через public-геттер.
    std::string getListFormat() const { return lastName_ + " " + getFirstName(); }

    std::string getGroup() const { return group_; }
    void setGroup(const std::string& group) { group_ = group; }
    double getAverageGrade() const { return averageGrade_; }
    bool setAverageGrade(double grade) {
        if (grade >= 2.0 && grade <= 5.0) { averageGrade_ = grade; return true; }
        return false;
    }
};
