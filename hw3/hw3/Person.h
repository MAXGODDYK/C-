#pragma once

#include <string>

// Тема 3 (ДЗ). protected gender_/age_ + protected isAdult().
// Оголошення; реалізація — у Person.cpp.
class Person {
private:
    std::string fullName_;

protected:
    std::string gender_;
    int age_;

    bool isAdult() const;   // protected: лише для нащадків

public:
    Person(const std::string& fullName, const std::string& gender, int age);

    std::string getFullName() const;
    std::string getGender() const;
    int getAge() const;
    bool setAge(int newAge);

    virtual std::string getRole() const;
    virtual std::string getInfo() const;
    std::string getShortInfo() const;

    virtual ~Person();
};
