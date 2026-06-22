#pragma once

#include "Person.h"

#include <vector>

// Тема 3 (ДЗ). homePets_/sport_ + getAdultStatus(). Реалізація — у Student.cpp.
class Student : public Person {
private:
    std::string group_;
    std::string college_;
    std::string homePets_;
    std::string sport_;
    double averageGrade_;
    std::vector<std::string> hobbies_;

public:
    Student(const std::string& fullName, const std::string& gender, int age,
            const std::string& group, const std::string& college,
            const std::string& homePets, const std::string& sport);

    std::string getRole() const override;
    void addHobby(const std::string& hobby);
    std::vector<std::string> getHobbies() const;
    std::string getInfo() const override;

    std::string getAdultStatus() const;   // використовує protected isAdult()/age_

    std::string getHomePets() const;
    bool setHomePets(const std::string& p);
    std::string getSport() const;
    bool setSport(const std::string& s);
    std::string getGroup() const;
    void setGroup(const std::string& g);
    double getAverageGrade() const;
    bool setAverageGrade(double g);
};
