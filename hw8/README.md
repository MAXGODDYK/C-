# StudentPortfolio — Домашнє завдання 8 (Тема 8)

Гілка `hw8` (від `tema7`). Повний файловий цикл поверх `DataGridView`. Вікно
змінює розмір (Sizable); сітка й кнопки на якорях. C++/CLI, .NET 4.8
(+`System.Configuration`). Відкрити `StudentPortfolio.sln` (Debug | x64), `F5`.

## Виконано (ДЗ8) — маркери `// ДЗ8 #k` у `MainForm.h`
- **#1** Повний **Save/Load** з парсингом TXT, формат
  `TYP|first|mid|last|gender|age|extra1|extra2` (`SerializePerson` /
  `DeserializeLine`); потоки закриваються у `finally`.
- **#2** **Автозбереження** при закритті форми (`FormClosing` → `autosave.txt` у
  каталозі програми); при старті, якщо є autosave, дані відновлюються замість
  демо-набору.
- **#3** Збереження/відновлення **розміру та позиції вікна** через
  `Properties::Settings` (`WindowLocation`/`WindowSize`). `RestoreWindowState()`
  застосовує збережені межі лише якщо вони перетинаються з робочою областю
  якогось екрана (`Screen::AllScreens` … `WorkingArea.IntersectsWith`), інакше —
  `CenterScreen`.
- **#4** Кнопка **Export CSV** (кома-роздільник, лапки для полів із комами,
  подвоєння внутрішніх лапок, UTF-8) — `CsvEscape`.
- **#5 (Варіант Б)** **Лог помилок**: усі помилки I/O дописуються у `error.log`
  із датою/часом (`LogError`); сам лог не валить застосунок.

Налаштування — ручний `Settings.h` на базі `ApplicationSettingsBase`. Модель
лишилась незмінною з `tema7`. Коментарі українською, код англійською.
Збірка Debug|x64 без помилок.
