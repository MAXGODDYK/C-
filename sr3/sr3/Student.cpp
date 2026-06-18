#include "Student.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>

Student::Student(std::string firstName, std::string middleName, std::string lastName,
                 std::string gender, int age,
                 std::string group, std::string homePets, std::string sport,
                 std::string college)
    : Person(std::move(firstName), std::move(middleName), std::move(lastName),
             std::move(gender), age),
      group_(std::move(group)),
      college_(std::move(college)),
      averageGrade_(0.0),          // Тема 2: бал спершу 0, далі через setAverageGrade()
      homePets_(std::move(homePets)),
      sport_(std::move(sport)) {
}

Student::~Student() {
    std::cout << "[~Student] " << getLastName() << " (student) deleted." << std::endl;
}

std::string Student::getGroup()        const { return group_; }
std::string Student::getHomePets()     const { return homePets_; }
std::string Student::getSport()        const { return sport_; }
std::string Student::getCollege()      const { return college_; }
double      Student::getAverageGrade() const { return averageGrade_; }

bool Student::setGroup(const std::string& group) {
    const std::size_t maxGroupLen = 10;
    if (!group.empty() && group.length() <= maxGroupLen) {
        group_ = group;
        return true;
    }
    return false;
}

bool Student::setHomePets(const std::string& homePets) {
    if (!homePets.empty()) {
        homePets_ = homePets;
        return true;
    }
    return false;
}

bool Student::setSport(const std::string& sport) {
    if (!sport.empty()) {
        sport_ = sport;
        return true;
    }
    return false;
}

bool Student::setCollege(const std::string& college) {
    const std::size_t maxCollegeLen = 50;
    if (!college.empty() && college.length() <= maxCollegeLen) {
        college_ = college;
        return true;
    }
    return false;
}

// Тема 2: валідація середнього балу — від 2.0 до 5.0.
bool Student::setAverageGrade(double grade) {
    if (grade >= 2.0 && grade <= 5.0) {
        averageGrade_ = grade;
        return true;
    }
    return false;
}

void Student::addHobby(const std::string& hobby) {
    // Порожні хобі не додаємо (проста валідація).
    if (!hobby.empty()) {
        hobbies_.push_back(hobby);
    }
}

std::vector<std::string> Student::getHobbies() const {
    return hobbies_;
}

std::string Student::getRole() const { return "Student"; }

std::string Student::getInfo() const {
    // Середній бал форматуємо до 2 знаків після коми (Тема 2).
    std::ostringstream gradeStr;
    gradeStr << std::fixed << std::setprecision(2) << averageGrade_;

    std::string info = lastName_ + " " + getFirstName() + " " + getMiddleName()
        + " | group: " + group_
        + " | college: " + college_
        + " | pets: " + homePets_
        + " | sport: " + sport_
        + " | GPA: " + gradeStr.str()
        + " | age: " + std::to_string(age_);

    // Хобі виводимо лише якщо вони є.
    if (!hobbies_.empty()) {
        info += " | hobbies: ";
        for (std::size_t i = 0; i < hobbies_.size(); ++i) {
            info += hobbies_[i];
            if (i + 1 < hobbies_.size()) {
                info += ", ";
            }
        }
    }
    return info;
}

std::string Student::getFormalGreeting() const {
    const std::string prefix = (gender_ == "Female") ? "Dear Ms. " : "Dear Mr. ";
    return prefix + getFirstName() + " " + getMiddleName() + "!";
}

std::string Student::getLiveGreeting() const {
    // Неформальне привітання до одногрупника.
    return "Hi, " + getFirstName() + "!";
}

std::string Student::getListFormat() const {
    // Прямий доступ до protected lastName_ + public-геттер getFirstName().
    return lastName_ + " " + getFirstName();
}

std::string Student::getAdultStatus() const {
    // Викликаємо protected-метод isAdult() базового класу.
    if (isAdult()) {
        return getFirstName() + " is an adult student (" + std::to_string(age_) + " y.o.)";
    }
    return getFirstName() + " is a minor student (" + std::to_string(age_) + " y.o.)";
}
