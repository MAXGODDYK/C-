#pragma once

#include "Person.h"

// Тема 2 (ДЗ). Третій нащадок — співробітник коледжу.
// getInfo() має формат, відмінний від Student/Teacher.
class Staff : public Person {
private:
    std::string position_;     // посада
    std::string department_;   // відділ
    std::string hireDate_;     // дата прийому

public:
    Staff(const std::string& fullName, int age,
          const std::string& position, const std::string& department,
          const std::string& hireDate)
        : Person(fullName, age), position_(position),
          department_(department), hireDate_(hireDate) {}

    std::string getRole() const override { return "Staff"; }

    // Формат навмисно інший, ніж у Student/Teacher.
    std::string getInfo() const override {
        return "Staff [" + position_ + "] " + getFullName()
            + " | dept: " + department_
            + " | hired: " + hireDate_;
    }

    std::string getPosition()   const { return position_; }
    std::string getDepartment() const { return department_; }
    std::string getHireDate()   const { return hireDate_; }

    bool setPosition(const std::string& position) {
        if (!position.empty()) { position_ = position; return true; }
        return false;
    }
    bool setDepartment(const std::string& department) {
        if (!department.empty()) { department_ = department; return true; }
        return false;
    }
    bool setHireDate(const std::string& hireDate) {
        if (!hireDate.empty()) { hireDate_ = hireDate; return true; }
        return false;
    }
};
