#include "Teacher.h"

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

std::string Teacher::getInfo() const {
    return Person::getInfo()
        + ", предмет: " + subject_
        + ", звання: " + academicTitle_;
}

std::string Teacher::getFormalGreeting() const {
    
    const std::string appeal = (gender_ == "Жіноча") ? "Шановна пані " : "Шановний пане ";
    return appeal + lastName_ + "!";
}

std::string Teacher::getAdultStatus() const {
    
    if (isAdult()) {
        return "Викладач " + lastName_ + " має повний доступ до дорослих обов'язків.";
    }

    return "Викладач " + lastName_ + " ще не може мати дорослі службові обов'язки.";
}
