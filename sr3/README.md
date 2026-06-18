# StudentPortfolio — Самостійна робота 3 (Тема 3)

Гілка `sr3` — успадковує `tema2`. Console C++. `StudentPortfolio.sln` (Debug | x64).

## Зміни Теми 3 (СР)

Модель імені РОЗБИТО:
- `firstName_`, `midName_` -> **private** (лише через геттери);
- `lastName_`, `gender_`, `age_` -> **protected** (доступні нащадкам напряму).

Додано до `Student`:
- `getLiveGreeting()` — читає `firstName_` через геттер;
- `getListFormat()` — використовує `lastName_` напряму (protected) + `getFirstName()`.

У `main()` пояснено, чому `protected` приховує поле від `main()`, але лишає
доступним нащадкам (спроба `s.lastName_` ззовні -> помилка компіляції).

Збірка `/W4` чиста.
