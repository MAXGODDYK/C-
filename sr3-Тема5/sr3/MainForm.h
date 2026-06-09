#pragma once

// ТЕМА 5. C++/CLI + Windows Forms — UI-шар (керований).
// Бізнес-логіка (Student) лишається нативною (Тема 1-4).
// Правило: UI = керований (gcnew, ^), дані = нативні (new/delete).

#include <string>
#include <msclr/marshal_cppstd.h>   // marshal_as<std::string>

#include "Student.h"                // нативний клас з Теми 1-4

namespace Portfolio {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class MainForm : public Form {
    public:
        MainForm() {
            InitializeComponent();
            currentStudent_ = nullptr;   // нативний об'єкт ще не створено
            LoadDefaults();              // мої реальні дані за замовчуванням
        }

    protected:
        // деструктор (Dispose) — звільняє керовані контроли + нативний об'єкт
        ~MainForm() {
            if (components) { delete components; }
            CleanupNative();
        }
        // фіналізатор — страхує від витоку, якщо Dispose не викликали
        !MainForm() { CleanupNative(); }

    private:
        // --- нативна частина (керується вручну через new/delete) ---
        Student* currentStudent_;

        void CleanupNative() {
            if (currentStudent_) { delete currentStudent_; currentStudent_ = nullptr; }
        }

        // --- керовані контроли (.NET, прибирає GC) ---
        System::ComponentModel::Container^ components;

        Label^   lblFirst;  TextBox^  txtFirst;
        Label^   lblMid;    TextBox^  txtMid;
        Label^   lblLast;   TextBox^  txtLast;
        Label^   lblGender; ComboBox^ cbGender;
        Label^   lblAge;    TextBox^  txtAge;
        Label^   lblGroup;  TextBox^  txtGroup;
        Label^   lblPets;   TextBox^  txtPets;
        Label^   lblSport;  TextBox^  txtSport;

        Button^  btnCreate;
        Button^  btnClear;
        Label^   lblResult;
        Label^   lblHistoryCaption;
        ListBox^ lstHistory;

        // ---- хелпери, щоб не дублювати створення контролів ----
        Label^ MakeLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y + 3);
            l->AutoSize = true;
            return l;
        }
        TextBox^ MakeBox(int x, int y, int w) {
            TextBox^ t = gcnew TextBox();
            t->Location = Point(x, y);
            t->Size = System::Drawing::Size(w, 23);
            return t;
        }

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            const int LX = 15;    // x міток
            const int FX = 150;   // x полів вводу
            const int FW = 380;   // ширина полів
            const int RH = 33;    // крок між рядками
            int y = 15;

            lblFirst  = MakeLabel(L"Ім'я:",        LX, y); txtFirst  = MakeBox(FX, y, FW); y += RH;
            lblMid    = MakeLabel(L"По батькові:",  LX, y); txtMid    = MakeBox(FX, y, FW); y += RH;
            lblLast   = MakeLabel(L"Прізвище:",     LX, y); txtLast   = MakeBox(FX, y, FW); y += RH;

            lblGender = MakeLabel(L"Стать:", LX, y);
            cbGender  = gcnew ComboBox();
            cbGender->Location = Point(FX, y);
            cbGender->Size = System::Drawing::Size(FW, 23);
            cbGender->DropDownStyle = ComboBoxStyle::DropDownList;  // тільки вибір зі списку
            cbGender->Items->Add(L"Чоловіча");
            cbGender->Items->Add(L"Жіноча");
            y += RH;

            lblAge   = MakeLabel(L"Вік:",         LX, y); txtAge   = MakeBox(FX, y, FW); y += RH;
            lblGroup = MakeLabel(L"Група:",       LX, y); txtGroup = MakeBox(FX, y, FW); y += RH;
            lblPets  = MakeLabel(L"Улюбленці:",   LX, y); txtPets  = MakeBox(FX, y, FW); y += RH;
            lblSport = MakeLabel(L"Спорт:",       LX, y); txtSport = MakeBox(FX, y, FW); y += RH;

            // --- кнопки ---
            btnCreate = gcnew Button();
            btnCreate->Text = L"Створити студента";
            btnCreate->Location = Point(FX, y);
            btnCreate->Size = System::Drawing::Size(185, 30);
            // ЗАВДАННЯ 1 (самостійна): підписка на подію Click
            btnCreate->Click += gcnew EventHandler(this, &MainForm::btnCreate_Click);

            btnClear = gcnew Button();           // варіант Б — скидання форми
            btnClear->Text = L"Очистити";
            btnClear->Location = Point(FX + 195, y);
            btnClear->Size = System::Drawing::Size(185, 30);
            btnClear->Click += gcnew EventHandler(this, &MainForm::btnClear_Click);
            y += 45;

            // --- результат (getInfo нативного об'єкта) ---
            lblResult = gcnew Label();
            lblResult->Location = Point(LX, y);
            lblResult->Size = System::Drawing::Size(FX + FW - LX, 44);
            lblResult->BorderStyle = BorderStyle::FixedSingle;
            lblResult->TextAlign = ContentAlignment::MiddleLeft;
            y += 54;

            // --- варіант А: історія створення ---
            lblHistoryCaption = MakeLabel(L"Історія створення:", LX, y);
            y += 26;
            lstHistory = gcnew ListBox();
            lstHistory->Location = Point(LX, y);
            lstHistory->Size = System::Drawing::Size(FX + FW - LX, 120);
            y += 130;

            // --- сама форма ---
            this->Text = L"StudentPortfolio — Тема 5 (C++/CLI + WinForms)";
            this->ClientSize = System::Drawing::Size(FX + FW + 15, y);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                lblFirst, txtFirst, lblMid, txtMid, lblLast, txtLast,
                lblGender, cbGender, lblAge, txtAge, lblGroup, txtGroup,
                lblPets, txtPets, lblSport, txtSport,
                btnCreate, btnClear, lblResult, lblHistoryCaption, lstHistory
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        // мої реальні дані як значення за замовчуванням
        void LoadDefaults() {
            txtFirst->Text = L"Максим";
            txtMid->Text   = L"Олексійович";
            txtLast->Text  = L"Годик";
            cbGender->SelectedIndex = 0;     // Чоловіча
            txtAge->Text   = L"19";
            txtGroup->Text = L"ПІ-21";
            txtPets->Text  = L"кіт Барсик";
            txtSport->Text = L"футбол";
            lblResult->Text = L"Готово. Заповніть поля та натисніть «Створити студента».";
        }

        // ЗАВДАННЯ 2-4 (самостійна) + валідація (домашнє): обробник Click
        System::Void btnCreate_Click(Object^ sender, EventArgs^ e) {
            // --- ВАЛІДАЦІЯ на межі (ДО створення нативного об'єкта) ---
            // 1) ім'я та прізвище не порожні
            if (String::IsNullOrWhiteSpace(txtFirst->Text) ||
                String::IsNullOrWhiteSpace(txtLast->Text)) {
                MessageBox::Show(L"Введіть ім'я та прізвище.", L"Помилка вводу",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            // 2) стать обрана
            if (cbGender->SelectedIndex < 0) {
                MessageBox::Show(L"Оберіть стать зі списку.", L"Помилка вводу",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            // 3) вік — ціле число у діапазоні 16..100
            int age = 0;
            if (!Int32::TryParse(txtAge->Text, age) || age < 16 || age > 100) {
                MessageBox::Show(L"Вік має бути цілим числом від 16 до 100.", L"Помилка вводу",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // --- String^ -> std::string через marshal_as (ЗАВДАННЯ 2) ---
            std::string f  = marshal_as<std::string>(txtFirst->Text->Trim());
            std::string m  = marshal_as<std::string>(txtMid->Text->Trim());
            std::string l  = marshal_as<std::string>(txtLast->Text->Trim());
            std::string g  = marshal_as<std::string>(cbGender->Text);
            std::string gr = marshal_as<std::string>(txtGroup->Text->Trim());
            std::string pet= marshal_as<std::string>(txtPets->Text->Trim());
            std::string sp = marshal_as<std::string>(txtSport->Text->Trim());

            // --- звільняємо попередній нативний об'єкт (delete старий) ---
            CleanupNative();

            // --- створюємо нативний Student (бізнес-логіка Теми 1-4) ---
            currentStudent_ = new Student(f, m, l, g, age, gr, pet, sp);

            // --- вивід у Label: std::string -> String^ через gcnew String (ЗАВДАННЯ 3) ---
            String^ info = gcnew String(currentStudent_->getInfo().c_str());
            lblResult->Text = info;

            // --- варіант А: додаємо рядок в історію ---
            lstHistory->Items->Add(info);
        }

        // варіант Б — скидання форми та label
        System::Void btnClear_Click(Object^ sender, EventArgs^ e) {
            txtFirst->Clear(); txtMid->Clear(); txtLast->Clear();
            txtAge->Clear();   txtGroup->Clear(); txtPets->Clear(); txtSport->Clear();
            cbGender->SelectedIndex = -1;
            lblResult->Text = L"Очищено. Введіть нові дані.";
            CleanupNative();
        }
    };
}
