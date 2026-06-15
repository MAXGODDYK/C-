#pragma once

#include "Person.h"

class Teacher : public Person {
private:
    std::string subject_;
    std::string academicTitle_;
    int experienceYears_;        // years of teaching experience

public:
    Teacher(
        std::string fn,
        std::string mn,
        std::string ln,
        std::string g,
        int a,
        std::string subject,
        std::string academicTitle,
        int experienceYears);

    std::string getSubject()       const;
    std::string getAcademicTitle() const;

    bool setSubject(const std::string& subj);

    std::string getRole()            const override;   // "Teacher"
    int         getExperienceYears() const override;
    std::string getInfo()            const override;
    std::string getFormalGreeting()  const override;

    std::string getAdultStatus() const;

    ~Teacher() override;
};
