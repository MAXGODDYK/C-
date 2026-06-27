# StudentPortfolio — Домашнє завдання 11 (Тема 11)

Гілка `hw11` (від `tema10`). Повний цикл експорту в Excel через COM Interop.
C++/CLI, .NET 4.8. Відкрити `StudentPortfolio.sln` (Debug | x64), `F5`.

> ⚠ Потребує встановленого **MS Excel** та PIA `Microsoft.Office.Interop.Excel`
> (підключено у `.vcxproj`, версія 15.0.0.0 з GAC). Без Excel проєкт не збереться.

## Виконано (ДЗ11) — маркери `// ДЗ11 #k` у `MainForm.h`
- **#1** Повний цикл: створення книги → заповнення → форматування → збереження
  (`SaveCopyAs`).
- **#2** `ToolStripProgressBar`, що оновлюється під час експорту з
  `Application::DoEvents()`.
- **#3** Окремі аркуші за типом: **Students / Teachers / Staff**
  (`Worksheets->Add`, перейменування `Name`).
- **#4** Перевірка наявності Excel перед запуском
  (`Type::GetTypeFromProgID("Excel.Application")`).
- **#5 (Варіант А)** Стовпчикова **діаграма** за віком студентів
  (`ChartObjects->Add`, `SetSourceData`, `xlColumnClustered`).

COM-об'єкти звільняються у `finally` (`Marshal::ReleaseComObject` + `GC`,
`Close`/`Quit`) — без завислого `EXCEL.EXE`. Excel-типи під псевдонімом
`namespace Excel = …`. Коментарі українською, код англійською. Збірка Debug|x64
без помилок (за наявності Excel).
