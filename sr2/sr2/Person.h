#pragma once

#include <string>

// Тема 2 (СР). Базовий клас.
// Демонструє інкапсуляцію (private-поля + геттери), віртуальний метод
// для поліморфізму та віртуальний деструктор.
class Person {
private:
    std::string fullName_;   // ПІБ одним рядком
    int age_;

public:
    Person(const std::string& fullName, int age)
        : fullName_(fullName), age_(age) {}

    std::string getFullName() const { return fullName_; }
    int getAge() const { return age_; }

    // Сеттер з валідацією діапазону: вік від 16 до 100.
    bool setAge(int newAge) {
        if (newAge >= 16 && newAge <= 100) {
            age_ = newAge;
            return true;
        }
        return false;
    }

    // ВІРТУАЛЬНИЙ метод -> працює поліморфізм через Person*.
    virtual std::string getInfo() const {
        return "Person: " + fullName_ + ", age: " + std::to_string(age_);
    }

    // Віртуальний деструктор: коректне видалення через базовий покажчик.
    virtual ~Person() = default;
};
