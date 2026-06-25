# StudentPortfolio — Самостійна робота 8 (Тема 8)

Гілка `sr8` (від `tema7`). Тема — **файлове введення/виведення + Settings**.
`MainForm` показує `std::vector<Person*>` у `DataGridView`; кнопки **Save** і
**Load** серіалізують список у текстовий файл і відновлюють його назад.
C++/CLI, .NET 4.8 (+`System.Configuration`). Відкрити `StudentPortfolio.sln`
(Debug | x64), `F5`.

## Виконано (СР8) — маркери `// СР8 #k` у `MainForm.h`
Виправлено «закладені» помилки із завдання:
- **#1** Маркер типу `S`/`T`/`F` перед полями — формат рядка
  `TYP|first|mid|last|gender|age|extra1|extra2`, тож тип об'єкта відновлюється
  при завантаженні (`DeserializeLine`).
- **#2** Потоки `StreamWriter`/`StreamReader` явно закриваються у блоці `finally`
  (`Close()`), тож файл не лишається заблокованим.
- **#3** Останній каталог зберігається в `Properties::Settings::Default->LastPath`
  і фіксується викликом `Settings::Save()` (ручний `Settings.h` на базі
  `ApplicationSettingsBase`).
- **#4** Перед завантаженням `vector` очищується з `delete` кожного об'єкта
  (`ClearPeople`), щоб уникнути витоку та дублювання.
- **#5** Перевірено повний цикл **Save → Load** (рядок статусу показує к-сть записів).

Серіалізація/розбір — у формі (`SerializePerson`/`DeserializeLine`), модель
лишилась незмінною з `tema7`. Коментарі українською, код англійською.
Збірка Debug|x64 без помилок.
