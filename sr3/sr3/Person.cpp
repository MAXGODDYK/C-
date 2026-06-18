#include "Person.h"

#include <iostream>
#include <utility>

Person::Person(std::string firstName, std::string middleName, std::string lastName,
               std::string gender, int age)
    : firstName_(std::move(firstName)),
      middleName_(std::move(middleName)),
      lastName_(std::move(lastName)),
      gender_(std::move(gender)),
      age_(age) {
}

Person::~Person() {
    // Доводить, що деструктор справді викликається (Тема 4).
    std::cout << "[~Person] " << lastName_ << " deleted." << std::endl;
}

bool Person::isAdult() const {
    return age_ >= 18;
}

std::string Person::getFirstName()  const { return firstName_; }
std::string Person::getMiddleName() const { return middleName_; }
std::string Person::getLastName()   const { return lastName_; }
std::string Person::getGender()     const { return gender_; }
int         Person::getAge()        const { return age_; }

bool Person::setAge(int age) {
    // Іменовані межі замість "магічних чисел".
    const int minAge = 16;
    const int maxAge = 100;
    if (age >= minAge && age <= maxAge) {
        age_ = age;
        return true;
    }
    return false;
}

std::string Person::getFullName() const {
    // ПІБ одним рядком: Прізвище Ім'я По-батькові.
    return lastName_ + " " + firstName_ + " " + middleName_;
}

std::string Person::getShortInfo() const {
    // ПІБ + роль; ПІБ беремо з getFullName(), роль — поліморфно через getRole().
    return getFullName() + " (" + getRole() + ")";
}
