# StudentPortfolio — Самостійна робота 7 (Тема 7)

Гілка `sr7` (від `tema6`). Тема — **DataGridView + синхронізація з vector**.
`MainForm` тримає поліморфний нативний список `std::vector<Person*>`
(Student/Teacher/Staff) і відображає його в `DataGridView` (Full name / Role /
Details). Під таблицею — панель деталей обраного рядка. C++/CLI, .NET 4.8.
Відкрити `StudentPortfolio.sln` (Debug | x64), `F5`.

## Виконано (СР7) — маркери `// СР7 #k` у `MainForm.h`
Виправлено «закладені» у завданні помилки:
- **#1** `AllowUserToAddRows = false` — прибрано порожній UI-рядок, що ламав
  відповідність «рядок ↔ Person*».
- **#2** Коректний виклик `getInfo()` для колонки «Details».
- **#3** Перевірка `SelectedRows->Count == 0` перед доступом до `SelectedRows[0]`.
- **#4** Умовне фарбування: рядок `Student` отримує фон `LightBlue`
  (через `dynamic_cast<Student*>`).
- **#5** Перевірено вибір рядка та відображення деталей (`getFormalGreeting()`
  + `getInfo()`) у панелі під таблицею.

Демо-дані засіваються в `SeedSampleData()` (по одному Student/Teacher/Staff).
Інтерфейс керований (`gcnew`, `^`), бізнес-логіка нативна (`Person*`,
`std::vector`). Коментарі українською, код англійською. Збірка Debug|x64 без помилок.
