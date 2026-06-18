#include "Staff.h"

#include <iostream>
#include <utility>

Staff::Staff(std::string firstName, std::string middleName, std::string lastName,
             std::string gender, int age,
             std::string position, std::string department, std::string hireDate)
    : Person(std::move(firstName), std::move(middleName), std::move(lastName),
             std::move(gender), age),
      position_(std::move(position)),
      department_(std::move(department)),
      hireDate_(std::move(hireDate)) {
}

Staff::~Staff() {
    std::cout << "[~Staff] " << getLastName() << " (staff) deleted." << std::endl;
}

std::string Staff::getPosition()   const { return position_; }
std::string Staff::getDepartment() const { return department_; }
std::string Staff::getHireDate()   const { return hireDate_; }

bool Staff::setPosition(const std::string& position) {
    const std::size_t maxPositionLen = 50;
    if (!position.empty() && position.length() <= maxPositionLen) {
        position_ = position;
        return true;
    }
    return false;
}

bool Staff::setDepartment(const std::string& department) {
    if (!department.empty()) {
        department_ = department;
        return true;
    }
    return false;
}

bool Staff::setHireDate(const std::string& hireDate) {
    if (!hireDate.empty()) {
        hireDate_ = hireDate;
        return true;
    }
    return false;
}

std::string Staff::getRole() const { return "Staff"; }

std::string Staff::getInfo() const {
    // Формат навмисно відрізняється від Student/Teacher (Тема 2 ДЗ).
    return lastName_ + " " + getFirstName() + " " + getMiddleName()
        + " | position: " + position_
        + " | department: " + department_
        + " | hired: " + hireDate_
        + " | age: " + std::to_string(age_);
}

std::string Staff::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Female") ? "Dear Ms. " : "Dear Mr. ";
    return prefix + getFirstName() + " " + getMiddleName() + "!";
}

std::string Staff::getBadgeText() const {
    return position_ + " (" + department_ + "): " + lastName_ + ", " + std::to_string(age_) + " y.o.";
}

std::string Staff::getAdultStatus() const {
    if (isAdult()) {
        return "Staff member " + lastName_ + " can work a full shift.";
    }
    return "Staff member " + lastName_ + " needs a limited schedule by age.";
}
