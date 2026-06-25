#include "Teacher.h"

#include <iostream>

Teacher::Teacher(const std::string& firstName, const std::string& midName, const std::string& lastName,
                 const std::string& gender, int age, const std::string& subject, int experience)
    : Person(firstName, midName, lastName, gender, age), subject_(subject), experience_(experience) {}

std::string Teacher::getRole() const { return "Teacher"; }
std::string Teacher::getInfo() const {
    return "Teacher: " + getFullName() + ", subject: " + subject_
        + ", experience: " + std::to_string(experience_) + " y.";
}
std::string Teacher::getFormalGreeting() const { return "Prof. " + getFullName() + ", welcome!"; }
std::string Teacher::getAdultStatus() const {
    std::string who = (gender_ == "Female") ? "Ms." : "Mr.";
    if (isAdult()) return who + " " + getFullName() + " has full job duties.";
    return who + " " + getFullName() + " cannot yet hold adult duties.";
}

std::string Teacher::getSubject() const { return subject_; }
int Teacher::getExperience() const { return experience_; }
bool Teacher::setSubject(const std::string& subject) { if (!subject.empty()) { subject_ = subject; return true; } return false; }
bool Teacher::setExperience(int years) { if (years >= 0 && years <= 80) { experience_ = years; return true; } return false; }

Teacher::~Teacher() { std::cout << "[~Teacher] " << getLastName() << " (teacher) deleted." << std::endl; }
