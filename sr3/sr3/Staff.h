#pragma once

#include "Person.h"

// Тема 3 (СР). Переведено на нову модель імені.
class Staff : public Person {
private:
    std::string position_;
    std::string department_;
    std::string hireDate_;

public:
    Staff(const std::string& firstName, const std::string& midName, const std::string& lastName,
          const std::string& gender, int age,
          const std::string& position, const std::string& department, const std::string& hireDate)
        : Person(firstName, midName, lastName, gender, age),
          position_(position), department_(department), hireDate_(hireDate) {}

    std::string getRole() const override { return "Staff"; }
    std::string getInfo() const override {
        return "Staff [" + position_ + "] " + getFullName()
            + " | dept: " + department_ + " | hired: " + hireDate_;
    }

    std::string getPosition()   const { return position_; }
    std::string getDepartment() const { return department_; }
    std::string getHireDate()   const { return hireDate_; }
    bool setPosition(const std::string& position) { if (!position.empty()) { position_ = position; return true; } return false; }
    bool setDepartment(const std::string& department) { if (!department.empty()) { department_ = department; return true; } return false; }
    bool setHireDate(const std::string& hireDate) { if (!hireDate.empty()) { hireDate_ = hireDate; return true; } return false; }
};
