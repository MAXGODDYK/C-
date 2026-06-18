#include "Teacher.h"

#include <iostream>
#include <utility>

Teacher::Teacher(std::string firstName, std::string middleName, std::string lastName,
                 std::string gender, int age,
                 std::string subject, std::string academicTitle)
    : Person(std::move(firstName), std::move(middleName), std::move(lastName),
             std::move(gender), age),
      subject_(std::move(subject)),
      academicTitle_(std::move(academicTitle)) {
}

Teacher::~Teacher() {
    std::cout << "[~Teacher] " << getLastName() << " (teacher) deleted." << std::endl;
}

std::string Teacher::getSubject()       const { return subject_; }
std::string Teacher::getAcademicTitle() const { return academicTitle_; }

bool Teacher::setSubject(const std::string& subject) {
    if (!subject.empty()) {
        subject_ = subject;
        return true;
    }
    return false;
}

bool Teacher::setAcademicTitle(const std::string& academicTitle) {
    if (!academicTitle.empty()) {
        academicTitle_ = academicTitle;
        return true;
    }
    return false;
}

std::string Teacher::getRole() const { return "Teacher"; }

std::string Teacher::getInfo() const {
    return lastName_ + " " + getFirstName() + " " + getMiddleName()
        + " | subject: " + subject_
        + " | title: " + academicTitle_
        + " | age: " + std::to_string(age_);
}

std::string Teacher::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Female") ? "Dear Ms. " : "Dear Mr. ";
    return prefix + getFirstName() + " " + getMiddleName() + "!";
}

std::string Teacher::getAdultStatus() const {
    // Прямий доступ до protected lastName_ + protected-метод isAdult().
    if (isAdult()) {
        return "Teacher " + lastName_ + " has full access to job duties.";
    }
    return "Teacher " + lastName_ + " cannot yet hold adult job duties.";
}
