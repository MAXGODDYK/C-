#pragma once

#include <string>

// Abstract base class — cannot be instantiated directly.
class Person {
private:
    // Accessible only inside Person; subclasses use the getters.
    std::string firstName_;
    std::string midName_;

protected:
    // Subclasses may access these directly.
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

    // Role label for the UI: "Student" / "Teacher" / "Staff".
    virtual std::string getRole() const = 0;
    // Years of experience; 0 for people without seniority (students).
    virtual int getExperienceYears() const { return 0; }

    // Subclasses must implement these.
    virtual std::string getInfo()           const = 0;
    virtual std::string getFormalGreeting() const = 0;

    // Virtual destructor so deleting via a base pointer calls the subclass dtor.
    virtual ~Person();
};
