#include "Teacher.h"

Teacher::Teacher(const std::string& fullName, int age,
                 const std::string& subject, int experience)
    : Person(fullName, age), subject_(subject), experience_(experience) {}

std::string Teacher::getRole() const { return "Teacher"; }
std::string Teacher::getInfo() const {
    return "Teacher: " + getFullName()
        + ", subject: " + subject_
        + ", experience: " + std::to_string(experience_) + " y.";
}

std::string Teacher::getSubject() const { return subject_; }
int Teacher::getExperience() const { return experience_; }
bool Teacher::setSubject(const std::string& subject) { if (!subject.empty()) { subject_ = subject; return true; } return false; }
bool Teacher::setExperience(int years) { if (years >= 0 && years <= 80) { experience_ = years; return true; } return false; }
