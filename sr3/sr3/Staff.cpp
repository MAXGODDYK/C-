#include "Staff.h"

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

std::string Staff::getInfo() const {
    return Person::getInfo()
        + ", посада: " + position_
        + ", зміна: " + shift_;
}

std::string Staff::getBadgeText() const {
    // lastName_ і age_ protected, тому Staff може сформувати службовий бейдж.
    return position_ + ": " + lastName_ + ", " + std::to_string(age_) + " років";
}

std::string Staff::getAdultStatus() const {
    // Прямий виклик protected-методу Person::isAdult() у нащадку.
    if (isAdult()) {
        return "Співробітник " + lastName_ + " може виконувати повну зміну.";
    }

    return "Співробітник " + lastName_ + " потребує обмеженого графіка за віком.";
}
