#include "Student.h"

#include <iostream>
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
    std::cout << "[~Student] " << getLastName() << " (студент) видалено." << std::endl;
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

std::string Student::getInfo() const {
    return lastName_ + " " + getFirstName() + " " + getMidName()
        + " | група: " + group_
        + " | улюбленці: " + homePets_
        + " | спорт: " + sport_
        + " | вік: " + std::to_string(age_);
}

std::string Student::getFormalGreeting() const {
    // gender_ protected — доступний напряму
    const std::string prefix = (gender_ == "Жіноча") ? "Шановна пані " : "Шановний пане ";
    return prefix + getFirstName() + " " + getMidName() + "!";
}

std::string Student::getLiveGreeting() const {
    return "Привіт, " + getFirstName() + "!";
}

std::string Student::getListFormat() const {
    return lastName_ + " " + getFirstName();
}

std::string Student::getAdultStatus() const {
    // isAdult() — protected метод з Person
    if (isAdult()) {
        return getFirstName() + " — повнолітній студент (" + std::to_string(age_) + " р.)";
    }
    return getFirstName() + " — неповнолітній студент (" + std::to_string(age_) + " р.)";
}
