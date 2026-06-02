#pragma once

#include <string>

class Person {
private:
    std::string firstName_;
    std::string midName_;

protected:
    
    std::string lastName_;
    std::string gender_;
    int age_;

    
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
