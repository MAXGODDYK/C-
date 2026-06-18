#pragma once

#include "Person.h"

#include <vector>

class Student : public Person {
private:
    std::string group_;
    // Індивідуальні поля студента (власний контрибут):
    std::string homePets_;
    std::string sport_;
    std::vector<std::string> hobbies_;   // Тема 2 (ДЗ): список хобі

public:
    Student(std::string firstName, std::string middleName, std::string lastName,
            std::string gender, int age,
            std::string group, std::string homePets, std::string sport);

    std::string getGroup()    const;
    std::string getHomePets() const;
    std::string getSport()    const;

    // Сеттери повертають false, якщо дані не пройшли валідацію.
    bool setGroup(const std::string& group);
    bool setHomePets(const std::string& homePets);
    bool setSport(const std::string& sport);

    // Хобі: додавання одного та отримання повного списку.
    void addHobby(const std::string& hobby);
    std::vector<std::string> getHobbies() const;

    std::string getRole()           const override;   // "Student"
    std::string getInfo()           const override;
    std::string getFormalGreeting() const override;

    // Власні методи, що використовують поля/protected базового класу.
    std::string getLiveGreeting() const;
    std::string getListFormat()   const;
    std::string getAdultStatus()  const;

    ~Student() override;
};
