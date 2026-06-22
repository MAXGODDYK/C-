#pragma once

#include "Person.h"

// Тема 3 (об'єднання). Split-name + getAdultStatus. Реалізація — Staff.cpp.
class Staff : public Person {
private:
    std::string position_;
    std::string department_;
    std::string hireDate_;

public:
    Staff(const std::string& firstName, const std::string& midName, const std::string& lastName,
          const std::string& gender, int age, const std::string& position,
          const std::string& department, const std::string& hireDate);

    std::string getRole() const override;
    std::string getInfo() const override;
    std::string getAdultStatus() const;

    std::string getPosition()   const;
    std::string getDepartment() const;
    std::string getHireDate()   const;
    bool setPosition(const std::string& position);
    bool setDepartment(const std::string& department);
    bool setHireDate(const std::string& hireDate);
};
