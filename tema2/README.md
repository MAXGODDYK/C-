# StudentPortfolio — Тема 2 (об'єднання)

Гілка `tema2` — об'єднання `sr2` + `hw2` у цілісний проєкт Теми 2.
Console C++. Відкрити `StudentPortfolio.sln` (Debug | x64).

## Підсумок Теми 2

- `Person` (`fullName_`, `age_`): `virtual getInfo()`, `getRole()`, `getShortInfo()`,
  `setAge()` (16–100), віртуальний деструктор.
- `Student`: group/college/averageGrade (валідація 2.0–5.0), `hobbies_`.
- `Teacher`: subject/experience (валідація).
- `Staff`: position/department/hireDate (інший формат `getInfo`).
- `main()`: `vector<unique_ptr<Person>>`, `printAllShortInfo()`, поліморфізм.

Наступний крок: `sr3` та `hw3` успадковують цю гілку. Збірка `/W4` чиста.
