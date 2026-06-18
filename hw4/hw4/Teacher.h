#pragma once

#include "Person.h"

// Тема 4 (ДЗ). getInfo() позначено 'final'; getFormalGreeting() перевизначено.
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& firstName, const std::string& midName, const std::string& lastName,
            const std::string& gender, int age, const std::string& subject, int experience)
        : Person(firstName, midName, lastName, gender, age), subject_(subject), experience_(experience) {}

    std::string getRole() const override { return "Teacher"; }

    // ДЗ4 (завдання 2): 'final' -> нащадок Teacher не може перевизначити getInfo().
    std::string getInfo() const override final {
        return "Teacher: " + getFullName() + ", subject: " + subject_
            + ", experience: " + std::to_string(experience_) + " y.";
    }

    // ДЗ4: перевизначення getFormalGreeting().
    std::string getFormalGreeting() const override {
        return "Prof. " + getFullName() + ", welcome!";
    }

    std::string getAdultStatus() const {
        std::string who = (gender_ == "Female") ? "Ms." : "Mr.";
        if (isAdult()) return who + " " + getFullName() + " has full job duties.";
        return who + " " + getFullName() + " cannot yet hold adult duties.";
    }

    std::string getSubject() const { return subject_; }
    int getExperience() const { return experience_; }
    bool setSubject(const std::string& s) { if (!s.empty()) { subject_ = s; return true; } return false; }
    bool setExperience(int y) { if (y >= 0 && y <= 80) { experience_ = y; return true; } return false; }

    ~Teacher() override { std::cout << "[~Teacher] " << getLastName() << " (teacher) deleted." << std::endl; }
};
