// THEME 7. Entry point of the managed Windows Forms application (C++/CLI).
// MainForm holds a mixed list of people in a ListView; editing via StudentEditForm.

#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]                                  // required for WinForms
int Main(array<String^>^ args) {             // managed entry point (see EntryPointSymbol)
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Portfolio::MainForm form;                // managed object (stack semantics)
    Application::Run(% form);                 // event loop: wait for user actions
    return 0;
}
