#pragma once

#include "Person.h"

class Teacher : public Person {
private:
    std::string subject_;
    std::string academicTitle_;
    int experienceYears_;        // педагогічний стаж, років

public:
    Teacher(std::string firstName, std::string middleName, std::string lastName,
            std::string gender, int age,
            std::string subject, std::string academicTitle, int experienceYears);

    std::string getSubject()         const;
    std::string getAcademicTitle()   const;
    int         getExperienceYears() const;

    // Сеттери з валідацією (порожні значення / некоректний діапазон відхиляються).
    bool setSubject(const std::string& subject);
    bool setAcademicTitle(const std::string& academicTitle);
    bool setExperienceYears(int experienceYears);

    std::string getRole() const override;   // "Teacher"
    // 'final': getInfo() не можна перевизначити в нащадках Teacher (Тема 4, завд. 2).
    std::string getInfo() const override final;
    std::string getFormalGreeting() const override;

    std::string getAdultStatus() const;

    ~Teacher() override;
};
