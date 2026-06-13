// ТЕМА 6. Точка входу керованого додатка Windows Forms (C++/CLI).
// MainForm тримає список студентів; редагування — через діалог StudentEditForm.

#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]                                  // обов'язково для WinForms
int Main(array<String^>^ args) {             // керована точка входу (див. EntryPointSymbol)
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Portfolio::MainForm form;                // керований об'єкт (stack semantics)
    Application::Run(% form);                 // подійний цикл: чекаємо на дії користувача
    return 0;
}
