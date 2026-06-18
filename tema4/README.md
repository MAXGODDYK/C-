# StudentPortfolio — Тема 4 (об'єднання) — фінал ланцюга

Гілка `tema4` = `sr4` + `hw4` (git-merge) + цілісний проєкт `tema4/`.
Console C++. Це остання ланка ланцюга `sr2 → hw2 → tema2 → sr3 → hw3 → tema3 → sr4 → hw4 → tema4`.

## Підсумок Теми 4

- `Person` **абстрактний** (`getInfo()`/`getRole()` чисто віртуальні);
  `getFormalGreeting()` віртуальний; віртуальні деструктори з `cout`.
- `Teacher`: `getInfo()` **final**, `getFormalGreeting()` перевизначено.
- `Staff`: `getFormalGreeting()` перевизначено.
- `Student`: `getCasualGreeting()`, `getAdultStatus()`, хобі тощо.
- `createPerson()` — фабрика; `countStaff()` — підрахунок через `dynamic_cast`;
  ручне `delete` показує порядок виклику деструкторів.

У гілці присутні всі етапи: `sr2/ hw2/ tema2/ sr3/ hw3/ tema3/ sr4/ hw4/ tema4/`.
Збірка `/W4` чиста.
