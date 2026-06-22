#pragma once

#include "Person.h"

// Тема 2 (ДЗ). Третій нащадок — співробітник. Реалізація — у Staff.cpp.
class Staff : public Person {
private:
    std::string position_;
    std::string department_;
    std::string hireDate_;

public:
    Staff(const std::string& fullName, int age,
          const std::string& position, const std::string& department, const std::string& hireDate);

    std::string getRole() const override;
    std::string getInfo() const override;   // формат, відмінний від Student/Teacher

    std::string getPosition()   const;
    std::string getDepartment() const;
    std::string getHireDate()   const;
    bool setPosition(const std::string& position);
    bool setDepartment(const std::string& department);
    bool setHireDate(const std::string& hireDate);
};
