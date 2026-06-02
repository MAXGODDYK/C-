#pragma once

#include "Person.h"

class Student : public Person {
private:
    std::string group_;
    std::string homePets_;
    std::string sport_;

public:
    Student(
        std::string fn,
        std::string mn,
        std::string ln,
        std::string g,
        int a,
        std::string grp,
        std::string homePets,
        std::string sport);

    std::string getGroup() const;
    void setGroup(const std::string& group);

    std::string getHomePets() const;
    void setHomePets(const std::string& homePets);

    std::string getSport() const;
    void setSport(const std::string& sport);

    std::string getInfo() const override;
    std::string getLiveGreeting() const;
    std::string getListFormat() const;
    std::string getAdultStatus() const;
};
