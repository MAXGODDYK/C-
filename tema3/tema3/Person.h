#pragma once

#include <string>

// Тема 3 (об'єднання СР3 + ДЗ3).
// Модель імені розбито (firstName/midName private; lastName/gender/age protected),
// + protected isAdult() для нащадків.
class Person {
private:
    std::string firstName_;
    std::string midName_;

protected:
    std::string lastName_;
    std::string gender_;
    int age_;

    bool isAdult() const { return age_ >= 18; }

public:
    Person(const std::string& firstName, const std::string& midName, const std::string& lastName,
           const std::string& gender, int age)
        : firstName_(firstName), midName_(midName), lastName_(lastName),
          gender_(gender), age_(age) {}

    std::string getFirstName() const { return firstName_; }
    std::string getMidName()   const { return midName_; }
    std::string getLastName()  const { return lastName_; }
    std::string getGender()    const { return gender_; }
    int         getAge()       const { return age_; }

    bool setAge(int newAge) { if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; } return false; }

    std::string getFullName() const { return lastName_ + " " + firstName_ + " " + midName_; }

    virtual std::string getRole() const { return "Person"; }
    virtual std::string getInfo() const { return getFullName() + ", age: " + std::to_string(age_); }
    std::string getShortInfo() const { return getFullName() + " (" + getRole() + ")"; }

    virtual ~Person() = default;
};
