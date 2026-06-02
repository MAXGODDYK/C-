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

std::string Student::getGroup() const {
    return group_;
}

void Student::setGroup(const std::string& group) {
    group_ = group;
}

std::string Student::getHomePets() const {
    return homePets_;
}

void Student::setHomePets(const std::string& homePets) {
    homePets_ = homePets;
}

std::string Student::getSport() const {
    return sport_;
}

void Student::setSport(const std::string& sport) {
    sport_ = sport;
}

std::string Student::getInfo() const {
    return Person::getInfo()
        + ", група: " + group_
        + ", домашні улюбленці: " + homePets_
        + ", спорт: " + sport_;
}

std::string Student::getLiveGreeting() const {
    
    return "Привіт, " + getFirstName() + "!";
}

std::string Student::getListFormat() const {
    
    return lastName_ + " " + getFirstName();
}

std::string Student::getAdultStatus() const {
    
    if (isAdult()) {
        return getFirstName() + " є повнолітнім студентом (" + std::to_string(age_) + " років).";
    }

    return getFirstName() + " ще неповнолітній студент (" + std::to_string(age_) + " років).";
}
