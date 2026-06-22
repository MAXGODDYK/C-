#pragma once

#include <string>

// Тема 4 (ДЗ). Абстрактний Person; getFormalGreeting() — ВІРТУАЛЬНИЙ.
// Оголошення; реалізація — у Person.cpp.
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

    virtual std::string getRole() const = 0;
    virtual std::string getInfo() const = 0;

    // ДЗ4: віртуальний, щоб Teacher/Staff могли перевизначити.
    virtual std::string getFormalGreeting() const;
    std::string getShortInfo() const;

    virtual ~Person();
};
