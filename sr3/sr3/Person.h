#pragma once

#include <string>

class Person {
private:
    std::string firstName_;
    std::string midName_;

protected:
    // Спільні поля для студентів і співробітників коледжу.
    // protected дозволяє нащадкам повторно використовувати ці дані,
    // але не відкриває їх напряму для main() та іншого зовнішнього коду.
    std::string lastName_;
    std::string gender_;
    int age_;

    // Варіант А: захищений помічник для нащадків.
    bool isAdult() const;

public:
    Person(std::string fn, std::string mn, std::string ln, std::string g, int a);

    std::string getFirstName() const;
    std::string getMidName() const;
    std::string getLastName() const;
    std::string getGender() const;
    int getAge() const;

    virtual std::string getInfo() const;
    virtual ~Person() = default;
};
