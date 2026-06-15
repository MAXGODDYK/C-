#include "Teacher.h"

#include <iostream>
#include <utility>

Teacher::Teacher(
    std::string fn,
    std::string mn,
    std::string ln,
    std::string g,
    int a,
    std::string subject,
    std::string academicTitle)
    : Person(std::move(fn), std::move(mn), std::move(ln), std::move(g), a),
      subject_(std::move(subject)),
      academicTitle_(std::move(academicTitle)) {
}

Teacher::~Teacher() {
    std::cout << "[~Teacher] " << getLastName() << " (teacher) deleted." << std::endl;
}

std::string Teacher::getSubject()       const { return subject_; }
std::string Teacher::getAcademicTitle() const { return academicTitle_; }

bool Teacher::setSubject(const std::string& subj) {
    if (!subj.empty()) {
        subject_ = subj;
        return true;
    }
    return false;
}

std::string Teacher::getInfo() const {
    return lastName_ + " " + getFirstName() + " " + getMidName()
        + " | subject: " + subject_
        + " | title: " + academicTitle_
        + " | age: " + std::to_string(age_);
}

std::string Teacher::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Female") ? "Dear Ms. " : "Dear Mr. ";
    return prefix + getFirstName() + " " + getMidName() + "!";
}

std::string Teacher::getAdultStatus() const {
    if (isAdult()) {
        return "Teacher " + lastName_ + " has full access to job duties.";
    }
    return "Teacher " + lastName_ + " cannot yet hold adult job duties.";
}
