#pragma once

#include <string>

// Тема 2 (СР). Базовий клас (оголошення; реалізація — у Person.cpp).
class Person {
private:
    std::string fullName_;   // ПІБ одним рядком
    int age_;

public:
    Person(const std::string& fullName, int age);

    std::string getFullName() const;
    int getAge() const;

    // Сеттер з валідацією: вік від 16 до 100.
    bool setAge(int newAge);

    // ВІРТУАЛЬНИЙ метод -> працює поліморфізм через Person*.
    virtual std::string getInfo() const;

    // Віртуальний деструктор.
    virtual ~Person();
};
