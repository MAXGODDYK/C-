# StudentPortfolio — Домашнє завдання 3 (Тема 3)

Гілка `hw3` — успадковує `tema2` (паралельно до `sr3`). Console C++.

## Зміни Теми 3 (ДЗ), Варіант А

- `Person`: `gender_`, `age_` -> **protected**; додано **protected** `isAdult()`.
- `Student`/`Teacher`/`Staff`: кожен має метод `getAdultStatus()`, що НАПРЯМУ
  використовує protected `isAdult()` / `age_` / `gender_`.
- `Student`: індивідуальні поля `homePets_`, `sport_` з геттерами/сеттерами.
- У `main()` пояснено, чому `protected` краще за `public`/`private`.

Збірка `/W4` чиста. Наступний крок: `tema3` об'єднує `sr3` + `hw3`.
