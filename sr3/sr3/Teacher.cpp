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
    std::cout << "[~Teacher] " << getLastName() << " (викладач) видалено." << std::endl;
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
        + " | предмет: " + subject_
        + " | звання: " + academicTitle_
        + " | вік: " + std::to_string(age_);
}

std::string Teacher::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Жіноча") ? "Шановна пані " : "Шановний пане ";
    return prefix + getFirstName() + " " + getMidName() + "!";
}

std::string Teacher::getAdultStatus() const {
    if (isAdult()) {
        return "Викладач " + lastName_ + " має повний доступ до службових обов'язків.";
    }
    return "Викладач " + lastName_ + " ще не може мати дорослі службові обов'язки.";
}
