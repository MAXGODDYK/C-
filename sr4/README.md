# StudentPortfolio — Самостійна робота 4 (Тема 4)

Гілка `sr4` — успадковує `tema3`. Console C++.

## Зміни Теми 4 (СР)

- `Person` став **абстрактним**: `getInfo()` та `getRole()` чисто віртуальні (`= 0`).
- Віртуальний деструктор друкує повідомлення (`cout`) — видно порядок виклику.
- `Student::getInfo()` з `override`; додано `getCasualGreeting()`.
- `getFormalGreeting()` у `Person` (залежно від статі).
- `main()`: масив `Person*`, поліморфний вивід, `dynamic_cast` для
  `getCasualGreeting()`, ручне `delete` у циклі (видно деструктори).

Збірка `/W4` чиста. Наступний крок: `hw4` (фабрика, final, countStaff), потім `tema4`.
