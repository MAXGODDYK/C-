#pragma once

#include "Person.h"

// Тема 3 (ДЗ). getAdultStatus() через protected isAdult()/gender_. Реалізація — Teacher.cpp.
class Teacher : public Person {
private:
    std::string subject_;
    int experience_;

public:
    Teacher(const std::string& fullName, const std::string& gender, int age,
            const std::string& subject, int experience);

    std::string getRole() const override;
    std::string getInfo() const override;
    std::string getAdultStatus() const;

    std::string getSubject() const;
    int getExperience() const;
    bool setSubject(const std::string& subject);
    bool setExperience(int years);
};
