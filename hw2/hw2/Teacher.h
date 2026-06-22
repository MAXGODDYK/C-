#pragma once

#include "Person.h"

// Тема 2 (ДЗ). Реалізація — у Teacher.cpp.
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& fullName, int age,
            const std::string& subject, int experience);

    std::string getRole() const override;
    std::string getInfo() const override;

    std::string getSubject() const;
    int getExperience() const;
    bool setSubject(const std::string& subject);
    bool setExperience(int years);
};
