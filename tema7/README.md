# StudentPortfolio — Тема 7 (об'єднано СР7 + ДЗ7)

Гілка `tema7` = `sr7` + `hw7` (merge). Підсумковий проєкт теми 7: один спільний
нативний список `std::vector<Person*>` (Student/Teacher/Staff) показано двома
способами на вкладках `TabControl`. C++/CLI, .NET 4.8. Відкрити
`StudentPortfolio.sln` (Debug | x64), `F5`.

## Що всередині
- **Вкладка «Grid (SR7)»** — `DataGridView` із синхронізацією: колонки
  Full name / Role / Details, панель деталей під таблицею. Маркери `// СР7 #1..#4`:
  `AllowUserToAddRows=false`, `getInfo()` у деталях, перевірка `SelectedRows->Count`,
  фарбування рядка `Student` у `LightBlue`.
- **Вкладка «List (HW7)»** — `ListView` з режимами Details/LargeIcon
  (`RadioButton`), сортуванням за прізвищем (клік на заголовок), контекстним меню
  Edit/Delete/Details, умовним форматуванням (вік<18 червоний, стаж>15 жирний) і
  підсумковим рядком (Варіант Б: кількість + середній вік). Маркери `// ДЗ7 #1..#5`.

Зміна даних (Edit/Delete/сортування) оновлює обидва подання через `RefreshAll()`.
Модель перенесена з `tema6`. Інтерфейс керований (`gcnew`, `^`), бізнес-логіка
нативна (`Person*`, `std::vector`). Коментарі українською, код англійською.
Збірка Debug|x64 без помилок.
