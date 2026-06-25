# StudentPortfolio — Самостійна робота 9 (Тема 9)

Гілка `sr9` (від `tema8`). Тема — **навігація вкладками + ErrorProvider**.
`TabControl` (All/Students/Teachers/Staff) діє як перемикач фільтра; знизу —
`DataGridView`, що показує відфільтрований `std::vector<Person*>`. Поле віку
перевіряється через `ErrorProvider`. C++/CLI, .NET 4.8. Відкрити
`StudentPortfolio.sln` (Debug | x64), `F5`.

## Виконано (СР9) — маркери `// СР9 #k` у `MainForm.h`
Виправлено «закладені» помилки із завдання:
- **#1** Підписка на `tabControl1->SelectedIndexChanged` у `SetupNavigation()`.
- **#2** Оператор порівняння `==` (а не присвоєння `=`) у виборі активної вкладки.
- **#3** `errorProvider1->SetError(txtAge, "")` — очищення помилки при успішній
  валідації (у `txtAge_Validating`).
- **#4** `FilterByType()` — перебір `vector` із перевіркою типу через
  `dynamic_cast<Student*/Teacher*/Staff*>`.
- **#5** Перевірено перемикання вкладок (фільтр) та валідацію поля віку
  (`Validating` + `e->Cancel`).

Інтерфейс керований (`gcnew`, `^`), бізнес-логіка нативна (`Person*`,
`std::vector`). Коментарі українською, код англійською. Збірка Debug|x64 без помилок.
