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

    std::string getGroup()    const;
    std::string getHomePets() const;
    std::string getSport()    const;

    // Return false if the data fails validation.
    bool setGroup(const std::string& grp);
    bool setHomePets(const std::string& homePets);
    bool setSport(const std::string& sport);

    std::string getRole()           const override;   // "Student"
    std::string getInfo()           const override;
    std::string getFormalGreeting() const override;

    std::string getListFormat()  const;
    std::string getAdultStatus() const;

    ~Student() override;
};
