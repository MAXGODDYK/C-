#pragma once

#include "Person.h"

class Staff : public Person {
private:
    std::string position_;     // посада
    std::string department_;   // відділ
    std::string hireDate_;     // дата прийому (рядок, напр. "2019-09-01")

public:
    Staff(std::string firstName, std::string middleName, std::string lastName,
          std::string gender, int age,
          std::string position, std::string department, std::string hireDate);

    std::string getPosition()   const;
    std::string getDepartment() const;
    std::string getHireDate()   const;

    // Сеттери з валідацією (порожні значення відхиляються).
    bool setPosition(const std::string& position);
    bool setDepartment(const std::string& department);
    bool setHireDate(const std::string& hireDate);

    std::string getRole() const override;   // "Staff"
    std::string getInfo() const override;    // формат, відмінний від Student/Teacher
    std::string getFormalGreeting() const override;

    std::string getBadgeText()   const;
    std::string getAdultStatus() const;

    ~Staff() override;
};
