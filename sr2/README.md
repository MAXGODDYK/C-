# StudentPortfolio — Самостійна робота 2 (Тема 2)

**Інкапсуляція, спадкування, поліморфізм** (C++ console, .NET не потрібен).
Окрема гілка `sr2` — перший етап ланцюга `sr2 → hw2 → tema2 → …`.

Відкрити `StudentPortfolio.sln` у Visual Studio (Debug | x64), `F5`.

## Що зроблено (виправлений скелет СР2)

- `Person`: `virtual getInfo()`, `virtual ~Person()`, `setAge()` з валідацією 16–100.
- `Student : public Person` (правильний специфікатор), виклик `Person(...)` у списку
  ініціалізації, `getInfo() override`, `setAverageGrade()` з валідацією 2.0–5.0.
- `Teacher : public Person` (subject + experience, валідація у сеттерах).
- `main()`: індивідуальні дані, `printInfo(const Person&)`, поліморфізм через
  `vector<Person*>`.
- Збірка `/W4` без попереджень.

Коментарі — українською, код — англійською.
