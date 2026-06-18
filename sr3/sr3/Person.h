#pragma once

#include <string>

// Абстрактний базовий клас: створити Person напряму не можна,
// бо є чисто віртуальні методи (getInfo, getFormalGreeting, getRole).
// Демонструє інкапсуляцію: private-поля доступні лише через геттери,
// protected-поля — напряму в нащадках, але приховані від main().
class Person {
private:
    // Доступні ТІЛЬКИ через геттери — навіть нащадки читають їх через getFirstName().
    std::string firstName_;
    std::string middleName_;

protected:
    // Доступні напряму в нащадках (Student/Teacher/Staff), але не з main().
    std::string lastName_;
    std::string gender_;
    int age_;

    // protected-метод: спільна логіка для нащадків, прихована від зовнішнього коду.
    bool isAdult() const;

public:
    Person(std::string firstName, std::string middleName, std::string lastName,
           std::string gender, int age);

    std::string getFirstName()  const;
    std::string getMiddleName() const;
    std::string getLastName()   const;
    std::string getGender()     const;
    int         getAge()        const;

    // Сеттер з валідацією діапазону; повертає false, якщо значення некоректне.
    bool setAge(int age);

    // Роль для коротких звітів ("Student" / "Teacher" / "Staff").
    virtual std::string getRole() const = 0;

    // Чисто віртуальні: кожен нащадок зобов'язаний реалізувати.
    virtual std::string getInfo()           const = 0;
    virtual std::string getFormalGreeting() const = 0;

    // Повне ім'я одним рядком у форматі ПІБ (Тема 2: аналог getFullName()).
    std::string getFullName() const;

    // Спільний для всіх: коротка інформація (ПІБ + роль). Роль — поліморфно через getRole().
    std::string getShortInfo() const;

    // Віртуальний деструктор: видалення через Person* викликає деструктор нащадка.
    virtual ~Person();
};
