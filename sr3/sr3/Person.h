#pragma once

#include <string>

// Тема 3 (СР). Модель імені розбито (оголошення; реалізація — у Person.cpp).
//  - firstName_, midName_ -> private; lastName_, gender_, age_ -> protected.
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
           const std::string& lastName, const std::string& gender, int age);

    std::string getFirstName() const;
    std::string getMidName()   const;
    std::string getLastName()  const;
    std::string getGender()    const;
    int         getAge()       const;

    bool setAge(int newAge);
    std::string getFullName() const;

    virtual std::string getRole() const;
    virtual std::string getInfo() const;
    std::string getShortInfo() const;

    virtual ~Person();
};
