#pragma once

#include "Person.h"

// Тема 3 (СР). Переведено на нову модель імені (firstName/midName/lastName).
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& firstName, const std::string& midName, const std::string& lastName,
            const std::string& gender, int age, const std::string& subject, int experience)
        : Person(firstName, midName, lastName, gender, age),
          subject_(subject), experience_(experience) {}

    std::string getRole() const override { return "Teacher"; }
    std::string getInfo() const override {
        return "Teacher: " + getFullName()
            + ", subject: " + subject_
            + ", experience: " + std::to_string(experience_) + " y.";
    }

    std::string getSubject() const { return subject_; }
    int getExperience() const { return experience_; }
    bool setSubject(const std::string& subject) { if (!subject.empty()) { subject_ = subject; return true; } return false; }
    bool setExperience(int years) { if (years >= 0 && years <= 80) { experience_ = years; return true; } return false; }
};
