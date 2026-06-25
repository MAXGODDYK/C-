# StudentPortfolio — Тема 8 (об'єднано СР8 + ДЗ8)

Гілка `tema8` = `sr8` + `hw8` (merge). Підсумковий проєкт теми 8: файлове
введення/виведення, серіалізація та `Settings` поверх `DataGridView`. C++/CLI,
.NET 4.8 (+`System.Configuration`). Відкрити `StudentPortfolio.sln`
(Debug | x64), `F5`.

## Що всередині
- **СР8** (основа Save/Load у `SaveToFile`/`LoadFromFile`) — маркери `// СР8 #k`:
  маркер типу `S/T/F` перед полями (#1), потоки закрито у `finally` (#2),
  збереження `Settings` LastPath (#3), очищення `vector` із `delete` перед
  завантаженням (#4).
- **ДЗ8** (надбудова) — маркери `// ДЗ8 #k`: повний Save/Load з парсингом TXT
  `TYP|...|...` (#1), автозбереження на `FormClosing` + відновлення при старті
  (#2), збереження/відновлення розміру й позиції вікна через `Settings` з
  перевіркою «в межах екрана» (#3), `Export CSV` з екрануванням ком/лапок (#4),
  Варіант Б — `error.log` з датою/часом (#5).

Налаштування — ручний `Settings.h` (`ApplicationSettingsBase`,
`LastPath`/`WindowLocation`/`WindowSize`). Модель перенесена з `tema7`.
Коментарі українською, код англійською. Збірка Debug|x64 без помилок.
