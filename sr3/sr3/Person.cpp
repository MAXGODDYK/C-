#include "Person.h"

#include <utility>

Person::Person(std::string fn, std::string mn, std::string ln, std::string g, int a)
    : firstName_(std::move(fn)),
      midName_(std::move(mn)),
      lastName_(std::move(ln)),
      gender_(std::move(g)),
      age_(a) {
}

bool Person::isAdult() const {
    return age_ >= 18;
}

std::string Person::getFirstName() const {
    return firstName_;
}

std::string Person::getMidName() const {
    return midName_;
}

std::string Person::getLastName() const {
    return lastName_;
}

std::string Person::getGender() const {
    return gender_;
}

int Person::getAge() const {
    return age_;
}

std::string Person::getInfo() const {
    return "ПІБ: " + lastName_ + " " + firstName_ + " " + midName_
        + ", стать: " + gender_
        + ", вік: " + std::to_string(age_);
}
