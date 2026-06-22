#include "Person.h"

Person::Person(const std::string& firstName, const std::string& midName,
               const std::string& lastName, const std::string& gender, int age)
    : firstName_(firstName), midName_(midName), lastName_(lastName), gender_(gender), age_(age) {}

bool Person::isAdult() const { return age_ >= 18; }

std::string Person::getFirstName() const { return firstName_; }
std::string Person::getMidName()   const { return midName_; }
std::string Person::getLastName()  const { return lastName_; }
std::string Person::getGender()    const { return gender_; }
int         Person::getAge()       const { return age_; }

bool Person::setAge(int newAge) { if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; } return false; }
std::string Person::getFullName() const { return lastName_ + " " + firstName_ + " " + midName_; }

std::string Person::getRole() const { return "Person"; }
std::string Person::getInfo() const { return getFullName() + ", age: " + std::to_string(age_); }
std::string Person::getShortInfo() const { return getFullName() + " (" + getRole() + ")"; }

Person::~Person() {}
