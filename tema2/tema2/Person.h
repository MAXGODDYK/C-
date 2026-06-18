#pragma once

#include <string>

// Тема 2 (ДЗ). Базовий клас. Додано getRole() та getShortInfo().
class Person {
private:
    std::string fullName_;
    int age_;

public:
    Person(const std::string& fullName, int age)
        : fullName_(fullName), age_(age) {}

    std::string getFullName() const { return fullName_; }
    int getAge() const { return age_; }

    bool setAge(int newAge) {
        if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; }
        return false;
    }

    // Роль для коротких звітів (перевизначається у нащадках).
    virtual std::string getRole() const { return "Person"; }

    virtual std::string getInfo() const {
        return "Person: " + fullName_ + ", age: " + std::to_string(age_);
    }

    // ДЗ2: коротка інформація — лише ПІБ та роль.
    std::string getShortInfo() const {
        return fullName_ + " (" + getRole() + ")";
    }

    virtual ~Person() = default;
};
