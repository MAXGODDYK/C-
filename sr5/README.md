# StudentPortfolio — Самостійна робота 5 (Тема 5)

Гілка `sr5` — **початок WinForms-гілки** ланцюга (від `tema4`, console). Міграція
console → WinForms: модель `Person/Student/Teacher/Staff` перенесено з `tema4`,
зверху — базова форма WinForms. C++/CLI, .NET 4.8. Відкрити `StudentPortfolio.sln`
(Debug | x64), `F5`.

## Виконано (СР5) — маркери `// СР5 #k` у `MainForm.h`
- **#1** Підписка на подію `btnCreate->Click`.
- **#2** Перетворення `String^ → std::string` через `marshal_as`.
- **#3** Вивід у `Label` через `gcnew String(...)` (`std::string → String^`).
- **#4** Валідація віку (16–100) перед `new`.
- **#5** Створення `Student*` (старий об'єкт видаляється) і показ `getInfo()`.

Модель — split-name (`firstName/midName/lastName` + group/college/pets/sport).
Коментарі українською, код англійською. Збірка Debug|x64 без помилок.
