#pragma once

#include "Person.h"

class Teacher : public Person {
private:
    std::string subject_;
    std::string academicTitle_;

public:
    Teacher(
        std::string fn,
        std::string mn,
        std::string ln,
        std::string g,
        int a,
        std::string subject,
        std::string academicTitle);

    std::string getInfo() const override;
    std::string getFormalGreeting() const;
    std::string getAdultStatus() const;
};
