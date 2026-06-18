#pragma once

#include "Person.h"

// Тема 3 (ДЗ). getAdultStatus() напряму використовує protected isAdult() та gender_.
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& fullName, const std::string& gender, int age,
            const std::string& subject, int experience)
        : Person(fullName, gender, age), subject_(subject), experience_(experience) {}

    std::string getRole() const override { return "Teacher"; }
    std::string getInfo() const override {
        return "Teacher: " + getFullName() + ", subject: " + subject_
            + ", experience: " + std::to_string(experience_) + " y.";
    }

    // Прямий доступ до protected gender_ та isAdult().
    std::string getAdultStatus() const {
        std::string who = (gender_ == "Female") ? "Ms." : "Mr.";
        if (isAdult()) return who + " " + getFullName() + " has full job duties.";
        return who + " " + getFullName() + " cannot yet hold adult duties.";
    }

    std::string getSubject() const { return subject_; }
    int getExperience() const { return experience_; }
    bool setSubject(const std::string& s) { if (!s.empty()) { subject_ = s; return true; } return false; }
    bool setExperience(int y) { if (y >= 0 && y <= 80) { experience_ = y; return true; } return false; }
};
