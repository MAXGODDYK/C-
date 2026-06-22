#pragma once

#include <string>

// Тема 4 (СР). АБСТРАКТНИЙ клас (getInfo/getRole = 0). Реалізація — у Person.cpp.
class Person {
private:
    std::string firstName_;
    std::string midName_;

protected:
    std::string lastName_;
    std::string gender_;
    int age_;

    bool isAdult() const;

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

    // Чисто віртуальні -> Person абстрактний.
    virtual std::string getRole() const = 0;
    virtual std::string getInfo() const = 0;

    std::string getFormalGreeting() const;   // залежно від статі
    std::string getShortInfo() const;

    virtual ~Person();   // друкує повідомлення (Person.cpp)
};
