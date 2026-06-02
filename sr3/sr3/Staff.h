#pragma once

#include "Person.h"

class Staff : public Person {
private:
    std::string position_;
    std::string shift_;

public:
    Staff(
        std::string fn,
        std::string mn,
        std::string ln,
        std::string g,
        int a,
        std::string position,
        std::string shift);

    std::string getInfo() const override;
    std::string getBadgeText() const;
    std::string getAdultStatus() const;
};
