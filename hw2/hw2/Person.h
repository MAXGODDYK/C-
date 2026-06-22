#pragma once

#include <string>

// Тема 2 (ДЗ). Базовий клас (оголошення; реалізація — у Person.cpp).
class Person {
private:
    std::string fullName_;
    int age_;

public:
    Person(const std::string& fullName, int age);

    std::string getFullName() const;
    int getAge() const;
    bool setAge(int newAge);

    virtual std::string getRole() const;   // роль (перевизначається у нащадках)
    virtual std::string getInfo() const;
    std::string getShortInfo() const;       // ПІБ + роль

    virtual ~Person();
};
