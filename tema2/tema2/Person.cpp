#include "Person.h"

Person::Person(const std::string& fullName, int age)
    : fullName_(fullName), age_(age) {}

std::string Person::getFullName() const { return fullName_; }
int Person::getAge() const { return age_; }

bool Person::setAge(int newAge) {
    if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; }
    return false;
}

std::string Person::getRole() const { return "Person"; }
std::string Person::getInfo() const { return "Person: " + fullName_ + ", age: " + std::to_string(age_); }

// ДЗ2: коротка інформація — лише ПІБ та роль.
std::string Person::getShortInfo() const { return fullName_ + " (" + getRole() + ")"; }

Person::~Person() {}
