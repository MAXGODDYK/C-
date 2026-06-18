#pragma once

#include "Person.h"

// Тема 3 (ДЗ). getAdultStatus() напряму використовує protected isAdult().
class Staff : public Person {
private:
    std::string position_;
    std::string department_;
    std::string hireDate_;

public:
    Staff(const std::string& fullName, const std::string& gender, int age,
          const std::string& position, const std::string& department, const std::string& hireDate)
        : Person(fullName, gender, age), position_(position),
          department_(department), hireDate_(hireDate) {}

    std::string getRole() const override { return "Staff"; }
    std::string getInfo() const override {
        return "Staff [" + position_ + "] " + getFullName()
            + " | dept: " + department_ + " | hired: " + hireDate_;
    }

    std::string getAdultStatus() const {
        if (isAdult()) return "Staff " + getFullName() + " can work a full shift.";
        return "Staff " + getFullName() + " needs a limited schedule by age.";
    }

    std::string getPosition()   const { return position_; }
    std::string getDepartment() const { return department_; }
    std::string getHireDate()   const { return hireDate_; }
    bool setPosition(const std::string& p) { if (!p.empty()) { position_ = p; return true; } return false; }
    bool setDepartment(const std::string& d) { if (!d.empty()) { department_ = d; return true; } return false; }
    bool setHireDate(const std::string& h) { if (!h.empty()) { hireDate_ = h; return true; } return false; }
};
