#pragma once

#include "Person.h"

// Тема 3 (об'єднання). Split-name + getAdultStatus. Реалізація — Teacher.cpp.
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& firstName, const std::string& midName, const std::string& lastName,
            const std::string& gender, int age, const std::string& subject, int experience);

    std::string getRole() const override;
    std::string getInfo() const override;
    std::string getAdultStatus() const;

    std::string getSubject() const;
    int getExperience() const;
    bool setSubject(const std::string& subject);
    bool setExperience(int years);
};
