# StudentPortfolio — Тема 11 (об'єднано СР11 + ДЗ11)

Гілка `tema11` = `sr11` + `hw11` (merge). Підсумковий проєкт теми 11: інтеграція
з MS Excel через COM Interop. C++/CLI, .NET 4.8. Відкрити `StudentPortfolio.sln`
(Debug | x64), `F5`.

> ⚠ Потребує встановленого **MS Excel** та PIA `Microsoft.Office.Interop.Excel`
> (підключено у `.vcxproj`, версія 15.0.0.0 з GAC). Без Excel проєкт не збереться.

## Що всередині
- **СР11** (маркери `// СР11 #k`) — основа експорту: COM-посилання (#1), `rowIndex`
  1-based із пропуском заголовка (#2), звільнення COM у `finally`
  (`Marshal::ReleaseComObject` + `GC`) (#3), автоформат Bold/Borders/AutoFit (#4).
- **ДЗ11** (маркери `// ДЗ11 #k`) — повний цикл створення/заповнення/форматування/
  збереження (#1), `ToolStripProgressBar` + `Application::DoEvents()` (#2), окремі
  аркуші **Students/Teachers/Staff** (#3), перевірка наявності Excel (#4),
  **Варіант А** — стовпчикова діаграма за віком студентів (#5).

Excel-типи під псевдонімом `namespace Excel = Microsoft::Office::Interop::Excel`.
Модель перенесена з `tema10`. Коментарі українською, код англійською.
Збірка Debug|x64 без помилок (за наявності Excel).

---
Це **остання тема** ланцюга `srN → hwN → temaN` (теми 5–11, WinForms).
