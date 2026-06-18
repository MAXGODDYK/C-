#pragma once

#include "Person.h"

// Тема 2 (ДЗ). Завершено: валідація у сеттерах + getRole().
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& fullName, int age,
            const std::string& subject, int experience)
        : Person(fullName, age), subject_(subject), experience_(experience) {}

    std::string getRole() const override { return "Teacher"; }

    std::string getInfo() const override {
        return "Teacher: " + getFullName()
            + ", subject: " + subject_
            + ", experience: " + std::to_string(experience_) + " y.";
    }

    std::string getSubject() const { return subject_; }
    int getExperience() const { return experience_; }

    bool setSubject(const std::string& subject) {
        if (!subject.empty()) { subject_ = subject; return true; }
        return false;
    }
    bool setExperience(int years) {
        if (years >= 0 && years <= 80) { experience_ = years; return true; }
        return false;
    }
};
