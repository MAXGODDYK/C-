#include "Person.h"

Person::Person(const std::string& fullName, const std::string& gender, int age)
    : fullName_(fullName), gender_(gender), age_(age) {}

bool Person::isAdult() const { return age_ >= 18; }

std::string Person::getFullName() const { return fullName_; }
std::string Person::getGender() const { return gender_; }
int Person::getAge() const { return age_; }
bool Person::setAge(int newAge) { if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; } return false; }

std::string Person::getRole() const { return "Person"; }
std::string Person::getInfo() const { return "Person: " + fullName_ + ", age: " + std::to_string(age_); }
std::string Person::getShortInfo() const { return fullName_ + " (" + getRole() + ")"; }

Person::~Person() {}
