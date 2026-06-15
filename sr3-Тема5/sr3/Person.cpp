#include "Person.h"

#include <utility>

Person::Person(std::string fn, std::string mn, std::string ln, std::string g, int a)
    : firstName_(std::move(fn)),
      midName_(std::move(mn)),
      lastName_(std::move(ln)),
      gender_(std::move(g)),
      age_(a) {
}

Person::~Person() {
}

bool Person::isAdult() const {
    return age_ >= 18;
}

std::string Person::getFirstName() const { return firstName_; }
std::string Person::getMidName()   const { return midName_; }
std::string Person::getLastName()  const { return lastName_; }
std::string Person::getGender()    const { return gender_; }
int         Person::getAge()       const { return age_; }

bool Person::setAge(int a) {
    if (a >= 16 && a <= 100) {
        age_ = a;
        return true;
    }
    return false;
}
