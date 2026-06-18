#pragma once

#include "Person.h"

#include <vector>

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
            const std::string& homePets, const std::string& sport)
        : Person(firstName, midName, lastName, gender, age),
          group_(group), college_(college), homePets_(homePets), sport_(sport), averageGrade_(0.0) {}

    std::string getRole() const override { return "Student"; }

    void addHobby(const std::string& hobby) { if (!hobby.empty()) hobbies_.push_back(hobby); }
    std::vector<std::string> getHobbies() const { return hobbies_; }

    std::string getInfo() const override {
        std::string info = "Student: " + getFullName()
            + ", group: " + group_ + ", college: " + college_
            + ", pets: " + homePets_ + ", sport: " + sport_
            + ", GPA: " + std::to_string(averageGrade_);
        if (!hobbies_.empty()) {
            info += ", hobbies: ";
            for (std::size_t i = 0; i < hobbies_.size(); ++i) { info += hobbies_[i]; if (i + 1 < hobbies_.size()) info += ", "; }
        }
        return info;
    }

    std::string getCasualGreeting() const { return "Hi, " + getFirstName() + "!"; }
    std::string getListFormat() const { return lastName_ + " " + getFirstName(); }
    std::string getAdultStatus() const {
        if (isAdult()) return getFullName() + " is an adult student (" + std::to_string(age_) + " y.o.)";
        return getFullName() + " is a minor student (" + std::to_string(age_) + " y.o.)";
    }

    std::string getHomePets() const { return homePets_; }
    bool setHomePets(const std::string& p) { if (!p.empty()) { homePets_ = p; return true; } return false; }
    std::string getSport() const { return sport_; }
    bool setSport(const std::string& s) { if (!s.empty()) { sport_ = s; return true; } return false; }
    std::string getGroup() const { return group_; }
    void setGroup(const std::string& g) { group_ = g; }
    double getAverageGrade() const { return averageGrade_; }
    bool setAverageGrade(double g) { if (g >= 2.0 && g <= 5.0) { averageGrade_ = g; return true; } return false; }

    ~Student() override { std::cout << "[~Student] " << getLastName() << " (student) deleted." << std::endl; }
};
