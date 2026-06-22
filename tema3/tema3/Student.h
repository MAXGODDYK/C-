#pragma once

#include "Person.h"

#include <vector>

// Тема 3 (об'єднання). СР3: getLiveGreeting/getListFormat. ДЗ3: homePets/sport, getAdultStatus.
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
    void addHobby(const std::string& hobby);
    std::vector<std::string> getHobbies() const;
    std::string getInfo() const override;

    std::string getLiveGreeting() const;
    std::string getListFormat()   const;
    std::string getAdultStatus()  const;

    std::string getHomePets() const;
    bool setHomePets(const std::string& p);
    std::string getSport() const;
    bool setSport(const std::string& s);
    std::string getGroup() const;
    void setGroup(const std::string& g);
    double getAverageGrade() const;
    bool setAverageGrade(double g);
};
