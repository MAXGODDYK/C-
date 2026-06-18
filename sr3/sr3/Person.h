#pragma once

#include <string>

// Тема 3 (СР). Модель імені РОЗБИТО на частини.
//  - firstName_, midName_ -> private (лише через геттери);
//  - lastName_, gender_, age_ -> protected (доступні нащадкам напряму,
//    але приховані від main() та зовнішнього коду).
class Person {
private:
    std::string firstName_;
    std::string midName_;

protected:
    std::string lastName_;
    std::string gender_;
    int age_;

public:
    Person(const std::string& firstName, const std::string& midName,
           const std::string& lastName, const std::string& gender, int age)
        : firstName_(firstName), midName_(midName), lastName_(lastName),
          gender_(gender), age_(age) {}

    std::string getFirstName() const { return firstName_; }
    std::string getMidName()   const { return midName_; }
    std::string getLastName()  const { return lastName_; }
    std::string getGender()    const { return gender_; }
    int         getAge()       const { return age_; }

    bool setAge(int newAge) {
        if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; }
        return false;
    }

    // ПІБ одним рядком (Прізвище Ім'я По-батькові).
    std::string getFullName() const { return lastName_ + " " + firstName_ + " " + midName_; }

    virtual std::string getRole() const { return "Person"; }
    virtual std::string getInfo() const { return getFullName() + ", age: " + std::to_string(age_); }
    std::string getShortInfo() const { return getFullName() + " (" + getRole() + ")"; }

    virtual ~Person() = default;
};
