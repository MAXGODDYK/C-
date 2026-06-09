#pragma once

#include <string>

// абстрактний базовий клас — не можна створити напряму
class Person {
private:
    // доступні тільки всередині Person, нащадки юзають геттери
    std::string firstName_;
    std::string midName_;

protected:
    // нащадки можуть звертатись напряму
    std::string lastName_;
    std::string gender_;
    int age_;

    bool isAdult() const;

public:
    Person(std::string fn, std::string mn, std::string ln, std::string g, int a);

    std::string getFirstName() const;
    std::string getMidName()   const;
    std::string getLastName()  const;
    std::string getGender()    const;
    int         getAge()       const;

    bool setAge(int a);

    // нащадки обовязково реалізують ці методи
    virtual std::string getInfo()           const = 0;
    virtual std::string getFormalGreeting() const = 0;

    // без virtual деструктора при delete через базовий вказівник деструктор нащадка не викличеться
    virtual ~Person();
};
