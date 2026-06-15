#include "Staff.h"

#include <utility>

Staff::Staff(
    std::string fn,
    std::string mn,
    std::string ln,
    std::string g,
    int a,
    std::string position,
    std::string shift,
    int experienceYears)
    : Person(std::move(fn), std::move(mn), std::move(ln), std::move(g), a),
      position_(std::move(position)),
      shift_(std::move(shift)),
      experienceYears_(experienceYears) {
}

Staff::~Staff() {
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

std::string Staff::getRole()            const { return "Staff"; }
int         Staff::getExperienceYears() const { return experienceYears_; }

std::string Staff::getInfo() const {
    return lastName_ + " " + getFirstName() + " " + getMidName()
        + " | position: " + position_
        + " | shift: " + shift_
        + " | experience: " + std::to_string(experienceYears_) + " y."
        + " | age: " + std::to_string(age_);
}

std::string Staff::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Female") ? "Dear Ms. " : "Dear Mr. ";
    return prefix + getFirstName() + " " + getMidName() + "!";
}

std::string Staff::getBadgeText() const {
    return position_ + ": " + lastName_ + ", " + std::to_string(age_) + " y.o.";
}

std::string Staff::getAdultStatus() const {
    if (isAdult()) {
        return "Staff member " + lastName_ + " can work a full shift.";
    }
    return "Staff member " + lastName_ + " needs a limited schedule by age.";
}
