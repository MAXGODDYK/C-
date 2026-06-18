# StudentPortfolio — Домашнє завдання 2 (Тема 2)

Гілка `hw2` — другий етап ланцюга (`sr2 → hw2 → tema2 → …`), успадковує `sr2`.
Console C++. Відкрити `StudentPortfolio.sln` (Debug | x64).

## Додано до СР2

- `Staff` (`position_`, `department_`, `hireDate_`) з `getInfo()` іншого формату.
- `getRole()` + `getShortInfo()` (ПІБ + роль) у кожному класі.
- `Student.hobbies_` (`vector<string>`) + `addHobby()` / `getHobbies()`; хобі у `getInfo()`.
- `main()`: 5 об'єктів у `vector<unique_ptr<Person>>`, `printAllShortInfo()`,
  двоє студентів із різними хобі.
- Завершено валідацію у сеттерах `Teacher`/`Staff`.

Коментарі — українською, код — англійською. Збірка `/W4` чиста.
