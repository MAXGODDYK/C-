# StudentPortfolio — Домашнє завдання 4 (Тема 4)

Гілка `hw4` — успадковує `tema3` (паралельно до `sr4`). Console C++.

## Зміни Теми 4 (ДЗ)

- `Person` абстрактний; `getFormalGreeting()` зроблено віртуальним.
- `Teacher`/`Staff` перевизначають `getInfo()` та `getFormalGreeting()`.
- **Завдання 1:** `createPerson(type, ...)` — фабрика об'єктів.
- **Завдання 2:** `Teacher::getInfo()` позначено `final` + закоментований
  `SeniorTeacher` (демонструє помилку компіляції C3248).
- **Завдання 3:** `countStaff(const vector<Person*>&)` через `dynamic_cast`.
- Деструктори друкують повідомлення (`cout`).

Збірка `/W4` чиста. Наступний крок: `tema4` об'єднує `sr4` + `hw4`.
