#pragma once

#include "Person.h"

#include <vector>

// Тема 4 (СР). Реалізація — у Student.cpp.
class Student : public Person {
private:
    std::string group_;
    std::string college_;
    std::string homePets_;
    std::string sport_;
    double averageGrade_;
    std::vector<std::string> hobbies_;

public:
    Student(const std::string& firstName, const std::string& midName, const std::string& lastName,
            const std::string& gender, int age, const std::string& group, const std::string& college,
            const std::string& homePets, const std::string& sport);

    std::string getRole() const override;
    std::string getInfo() const override;

    void addHobby(const std::string& hobby);
    std::vector<std::string> getHobbies() const;

    std::string getCasualGreeting() const;   // через dynamic_cast
    std::string getListFormat() const;
    std::string getAdultStatus() const;

    std::string getHomePets() const;
    bool setHomePets(const std::string& p);
    std::string getSport() const;
    bool setSport(const std::string& s);
    std::string getGroup() const;
    void setGroup(const std::string& g);
    double getAverageGrade() const;
    bool setAverageGrade(double g);

    ~Student() override;
};
