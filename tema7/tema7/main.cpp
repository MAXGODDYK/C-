// Тема 7 (об'єднано СР7 + ДЗ7). Точка входу керованого WinForms-застосунку (C++/CLI).

#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]                                  // обов'язково для WinForms
int Main(array<String^>^ args) {             // керована точка входу (див. EntryPointSymbol)
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Portfolio::MainForm form;                // керований об'єкт (семантика стека)
    Application::Run(% form);                 // цикл подій
    return 0;
}
