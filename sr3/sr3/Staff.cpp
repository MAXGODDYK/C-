#include "Staff.h"

#include <iostream>
#include <utility>

Staff::Staff(
    std::string fn,
    std::string mn,
    std::string ln,
    std::string g,
    int a,
    std::string position,
    std::string shift)
    : Person(std::move(fn), std::move(mn), std::move(ln), std::move(g), a),
      position_(std::move(position)),
      shift_(std::move(shift)) {
}

Staff::~Staff() {
    std::cout << "[~Staff] " << getLastName() << " (персонал) видалено." << std::endl;
}

std::string Staff::getPosition() const { return position_; }
std::string Staff::getShift()    const { return shift_; }

bool Staff::setPosition(const std::string& pos) {
    if (!pos.empty() && pos.length() <= 50) {
        position_ = pos;
        return true;
    }
    return false;
}

bool Staff::setShift(const std::string& shift) {
    if (!shift.empty()) {
        shift_ = shift;
        return true;
    }
    return false;
}

std::string Staff::getInfo() const {
    return lastName_ + " " + getFirstName() + " " + getMidName()
        + " | посада: " + position_
        + " | зміна: " + shift_
        + " | вік: " + std::to_string(age_);
}

std::string Staff::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Жіноча") ? "Шановна пані " : "Шановний пане ";
    return prefix + getFirstName() + " " + getMidName() + "!";
}

std::string Staff::getBadgeText() const {
    return position_ + ": " + lastName_ + ", " + std::to_string(age_) + " р.";
}

std::string Staff::getAdultStatus() const {
    if (isAdult()) {
        return "Співробітник " + lastName_ + " може виконувати повну зміну.";
    }
    return "Співробітник " + lastName_ + " потребує обмеженого графіка за віком.";
}
