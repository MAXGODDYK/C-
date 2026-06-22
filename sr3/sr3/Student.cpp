#include "Student.h"

Student::Student(const std::string& firstName, const std::string& midName, const std::string& lastName,
                 const std::string& gender, int age, const std::string& group, const std::string& college)
    : Person(firstName, midName, lastName, gender, age), group_(group), college_(college), averageGrade_(0.0) {}

std::string Student::getRole() const { return "Student"; }

void Student::addHobby(const std::string& hobby) { if (!hobby.empty()) hobbies_.push_back(hobby); }
std::vector<std::string> Student::getHobbies() const { return hobbies_; }

std::string Student::getInfo() const {
    std::string info = "Student: " + getFullName()
        + ", group: " + group_ + ", college: " + college_
        + ", GPA: " + std::to_string(averageGrade_);
    if (!hobbies_.empty()) {
        info += ", hobbies: ";
        for (std::size_t i = 0; i < hobbies_.size(); ++i) { info += hobbies_[i]; if (i + 1 < hobbies_.size()) info += ", "; }
    }
    return info;
}

// firstName_ — private у Person, тож читаємо через геттер.
std::string Student::getLiveGreeting() const { return "Hi, " + getFirstName() + "!"; }
// lastName_ — protected, тож доступний напряму.
std::string Student::getListFormat() const { return lastName_ + " " + getFirstName(); }

std::string Student::getGroup() const { return group_; }
void Student::setGroup(const std::string& group) { group_ = group; }
double Student::getAverageGrade() const { return averageGrade_; }
bool Student::setAverageGrade(double grade) { if (grade >= 2.0 && grade <= 5.0) { averageGrade_ = grade; return true; } return false; }
