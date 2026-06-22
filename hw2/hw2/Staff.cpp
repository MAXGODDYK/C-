#include "Staff.h"

Staff::Staff(const std::string& fullName, int age,
             const std::string& position, const std::string& department, const std::string& hireDate)
    : Person(fullName, age), position_(position), department_(department), hireDate_(hireDate) {}

std::string Staff::getRole() const { return "Staff"; }
std::string Staff::getInfo() const {
    return "Staff [" + position_ + "] " + getFullName()
        + " | dept: " + department_ + " | hired: " + hireDate_;
}

std::string Staff::getPosition()   const { return position_; }
std::string Staff::getDepartment() const { return department_; }
std::string Staff::getHireDate()   const { return hireDate_; }
bool Staff::setPosition(const std::string& position) { if (!position.empty()) { position_ = position; return true; } return false; }
bool Staff::setDepartment(const std::string& department) { if (!department.empty()) { department_ = department; return true; } return false; }
bool Staff::setHireDate(const std::string& hireDate) { if (!hireDate.empty()) { hireDate_ = hireDate; return true; } return false; }
