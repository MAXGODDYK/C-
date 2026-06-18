#pragma once

#include <string>

// Тема 3 (ДЗ). Спільні для всіх поля винесено в protected: gender_, age_.
// isAdult() — protected-метод (деталь реалізації для нащадків, не public API).
class Person {
private:
    std::string fullName_;

protected:
    std::string gender_;
    int age_;

    // Чому protected, а не public: це внутрішня логіка для нащадків.
    bool isAdult() const { return age_ >= 18; }

public:
    Person(const std::string& fullName, const std::string& gender, int age)
        : fullName_(fullName), gender_(gender), age_(age) {}

    std::string getFullName() const { return fullName_; }
    std::string getGender() const { return gender_; }
    int getAge() const { return age_; }

    bool setAge(int newAge) {
        if (newAge >= 16 && newAge <= 100) { age_ = newAge; return true; }
        return false;
    }

    virtual std::string getRole() const { return "Person"; }
    virtual std::string getInfo() const { return "Person: " + fullName_ + ", age: " + std::to_string(age_); }
    std::string getShortInfo() const { return fullName_ + " (" + getRole() + ")"; }

    virtual ~Person() = default;
};
