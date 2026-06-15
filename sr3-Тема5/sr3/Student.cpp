#include "Student.h"

#include <utility>

Student::Student(
    std::string fn,
    std::string mn,
    std::string ln,
    std::string g,
    int a,
    std::string grp,
    std::string homePets,
    std::string sport)
    : Person(std::move(fn), std::move(mn), std::move(ln), std::move(g), a),
      group_(std::move(grp)),
      homePets_(std::move(homePets)),
      sport_(std::move(sport)) {
}

Student::~Student() {
}

std::string Student::getGroup()    const { return group_; }
std::string Student::getHomePets() const { return homePets_; }
std::string Student::getSport()    const { return sport_; }

bool Student::setGroup(const std::string& grp) {
    if (!grp.empty() && grp.length() <= 10) {
        group_ = grp;
        return true;
    }
    return false;
}

bool Student::setHomePets(const std::string& homePets) {
    if (!homePets.empty()) {
        homePets_ = homePets;
        return true;
    }
    return false;
}

bool Student::setSport(const std::string& sport) {
    if (!sport.empty()) {
        sport_ = sport;
        return true;
    }
    return false;
}

std::string Student::getRole() const { return "Student"; }

std::string Student::getInfo() const {
    return lastName_ + " " + getFirstName() + " " + getMidName()
        + " | group: " + group_
        + " | pets: " + homePets_
        + " | sport: " + sport_
        + " | age: " + std::to_string(age_);
}

std::string Student::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Female") ? "Dear Ms. " : "Dear Mr. ";
    return prefix + getFirstName() + " " + getMidName() + "!";
}

std::string Student::getListFormat() const {
    return lastName_ + " " + getFirstName();
}

std::string Student::getAdultStatus() const {
    if (isAdult()) {
        return getFirstName() + " is an adult student (" + std::to_string(age_) + " y.o.)";
    }
    return getFirstName() + " is a minor student (" + std::to_string(age_) + " y.o.)";
}
