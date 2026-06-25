# StudentPortfolio — Домашнє завдання 9 (Тема 9)

Гілка `hw9` (від `tema8`). Розширені елементи інтерфейсу поверх `DataGridView`:
меню, панель інструментів, контекстне меню та рядок стану. C++/CLI, .NET 4.8.
Відкрити `StudentPortfolio.sln` (Debug | x64), `F5`.

## Виконано (ДЗ9) — маркери `// ДЗ9 #k` у `MainForm.h`
- **#1** `MenuStrip`: **File**(Save, Exit) / **Edit**(Add, Delete).
- **#2** `ToolStrip` з іконками для тих самих дій — **спільні обробники**
  (`OnSave`/`OnAdd`/`OnDelete`), що й у меню.
- **#3** `ContextMenuStrip` на `DataGridView` (ПКМ → **Edit / Delete**); ПКМ
  спершу виділяє рядок під курсором.
- **#4** Валідація **ПІБ** через `ErrorProvider`: не порожнє, лише
  літери/пробіли/дефіс (`IsValidName` + `Validating` на `txtFirst`/`txtLast`).
- **#5 (Варіант Б)** `StatusStrip`: час останнього збереження (оновлюється у
  `OnSave`) та кількість виділених рядків (оновлюється у `SelectionChanged`).

Інтерфейс керований (`gcnew`, `^`), бізнес-логіка нативна (`Person*`,
`std::vector`). Коментарі українською, код англійською. Збірка Debug|x64 без помилок.
