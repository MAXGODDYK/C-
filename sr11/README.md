# StudentPortfolio — Самостійна робота 11 (Тема 11)

Гілка `sr11` (від `tema10`). Тема — **інтеграція з MS Excel через COM Interop**.
Кнопка **Export to Excel** створює книгу, заповнює аркуш зі `std::vector<Person*>`,
форматує та зберігає `.xlsx`. C++/CLI, .NET 4.8. Відкрити `StudentPortfolio.sln`
(Debug | x64), `F5`.

> ⚠ Потребує встановленого **MS Excel** та PIA `Microsoft.Office.Interop.Excel`
> (підключено у `.vcxproj`, версія 15.0.0.0 з GAC). Без Excel проєкт не збереться.

## Виконано (СР11) — маркери `// СР11 #k` у `MainForm.h`
- **#1** Додано COM-посилання `Microsoft.Office.Interop.Excel` у `.vcxproj`.
- **#2** `rowIndex` 1-based: дані пишуться з рядка **2** (рядок 1 — заголовок),
  а не з 0/1 (інакше заголовок затирається).
- **#3** Звільнення COM-об'єктів у `finally`
  (`Marshal::ReleaseComObject` + `GC::Collect`/`WaitForPendingFinalizers`,
  `workbook->Close`, `excelApp->Quit`) — інакше `EXCEL.EXE` лишається у фоні.
- **#4** Автоформат: **Bold**-заголовок (`A1:C1`), межі (`Borders` =
  `xlContinuous`), автоширина колонок (`Columns->AutoFit`).
- **#5** Перевірено експорт (у Диспетчері завдань немає завислого `EXCEL.EXE`).

Excel-типи під псевдонімом `namespace Excel = Microsoft::Office::Interop::Excel`,
щоб уникнути конфлікту `Excel::Application` з `Forms::Application`. Коментарі
українською, код англійською. Збірка Debug|x64 без помилок (за наявності Excel).
