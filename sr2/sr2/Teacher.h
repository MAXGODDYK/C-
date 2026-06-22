#pragma once

#include "Person.h"

// Тема 2 (СР). Ще один нащадок Person. Реалізація — у Teacher.cpp.
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;   // стаж, років

public:
    Teacher(const std::string& fullName, int age,
            const std::string& subject, int experience);

    std::string getInfo() const override;

    std::string getSubject() const;
    int getExperience() const;

    // Валідація у сеттерах.
    bool setSubject(const std::string& subject);
    bool setExperience(int years);
};
