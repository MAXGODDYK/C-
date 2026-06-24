#pragma once

// ДЗ5 (Тема 5). Розширена форма: додано індивідуальне поле (pets),
// повну валідацію (ім'я/стать/вік), MessageBox при помилці та кнопку "Clear"
// (Варіант Б). Базу СР5 (marshal_as, gcnew String, валідація) збережено.
// Інтерфейс = керований (gcnew, ^); бізнес-логіка (Student*) = нативна (new/delete).

#include <string>
#include <msclr/marshal_cppstd.h>

#include "Student.h"

namespace Portfolio {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class MainForm : public Form {
    public:
        MainForm() {
            currentStudent_ = nullptr;
            InitializeComponent();
        }

    protected:
        ~MainForm() { if (components) { delete components; } CleanupNative(); }
        !MainForm() { CleanupNative(); }

    private:
        // Нативний об'єкт — керуємо пам'яттю вручну (new/delete).
        Student* currentStudent_;
        void CleanupNative() { if (currentStudent_) { delete currentStudent_; currentStudent_ = nullptr; } }

        System::ComponentModel::Container^ components;
        Label^ lblFirst;  TextBox^ txtFirst;
        Label^ lblMid;    TextBox^ txtMid;
        Label^ lblLast;   TextBox^ txtLast;
        Label^ lblGender; ComboBox^ cbGender;
        Label^ lblAge;    TextBox^ txtAge;
        Label^ lblGroup;  TextBox^ txtGroup;
        Label^ lblPets;   TextBox^ txtPets;   // ДЗ5 #1: індивідуальне поле (homePets)
        Button^ btnCreate; Button^ btnClear;
        Label^ lblResult;

        Label^ MakeLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label(); l->Text = text; l->Location = Point(x, y + 3); l->AutoSize = true; return l;
        }
        TextBox^ MakeBox(int x, int y, int w) {
            TextBox^ b = gcnew TextBox(); b->Location = Point(x, y); b->Size = System::Drawing::Size(w, 23); return b;
        }

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            const int LX = 15, FX = 140, FW = 300, RH = 33;
            int y = 15;

            lblFirst  = MakeLabel(L"First name:",  LX, y); txtFirst = MakeBox(FX, y, FW); y += RH;
            lblMid    = MakeLabel(L"Middle name:", LX, y); txtMid   = MakeBox(FX, y, FW); y += RH;
            lblLast   = MakeLabel(L"Surname:",     LX, y); txtLast  = MakeBox(FX, y, FW); y += RH;

            lblGender = MakeLabel(L"Gender:", LX, y);
            cbGender = gcnew ComboBox();
            cbGender->Location = Point(FX, y); cbGender->Size = System::Drawing::Size(FW, 23);
            cbGender->DropDownStyle = ComboBoxStyle::DropDownList;
            cbGender->Items->Add(L"Male");
            cbGender->Items->Add(L"Female");
            cbGender->SelectedIndex = 0;
            y += RH;

            lblAge   = MakeLabel(L"Age:",   LX, y); txtAge   = MakeBox(FX, y, FW); y += RH;
            lblGroup = MakeLabel(L"Group:", LX, y); txtGroup = MakeBox(FX, y, FW); y += RH;
            lblPets  = MakeLabel(L"Pets:",  LX, y); txtPets  = MakeBox(FX, y, FW); y += RH;   // ДЗ5 #1

            btnCreate = gcnew Button();
            btnCreate->Text = L"Create";
            btnCreate->Location = Point(FX, y);
            btnCreate->Size = System::Drawing::Size(145, 30);
            btnCreate->Click += gcnew EventHandler(this, &MainForm::btnCreate_Click);

            // ДЗ5 (Варіант Б): кнопка "Clear" — скидає всі поля та Label.
            btnClear = gcnew Button();
            btnClear->Text = L"Clear";
            btnClear->Location = Point(FX + 155, y);
            btnClear->Size = System::Drawing::Size(145, 30);
            btnClear->Click += gcnew EventHandler(this, &MainForm::btnClear_Click);
            y += 42;

            lblResult = gcnew Label();
            lblResult->Location = Point(LX, y);
            lblResult->Size = System::Drawing::Size(FX + FW - LX, 46);
            lblResult->BorderStyle = BorderStyle::FixedSingle;
            lblResult->TextAlign = ContentAlignment::MiddleLeft;
            lblResult->Text = L"Fill the fields and press \"Create\".";
            y += 56;

            // ДЗ5 #4: реальні дані за замовчуванням.
            txtFirst->Text = L"Maksym"; txtMid->Text = L"O."; txtLast->Text = L"Hodyk";
            txtAge->Text = L"19"; txtGroup->Text = L"PI-21"; txtPets->Text = L"cat Barsyk";

            this->Text = L"StudentPortfolio - Homework 5 (field + validation + Clear)";
            this->ClientSize = System::Drawing::Size(FX + FW + 15, y + 5);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                lblFirst, txtFirst, lblMid, txtMid, lblLast, txtLast,
                lblGender, cbGender, lblAge, txtAge, lblGroup, txtGroup, lblPets, txtPets,
                btnCreate, btnClear, lblResult
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        System::Void btnCreate_Click(Object^ sender, EventArgs^ e) {
            // ДЗ5 #2/#3: ім'я та прізвище не порожні (інакше MessageBox).
            if (String::IsNullOrWhiteSpace(txtFirst->Text) || String::IsNullOrWhiteSpace(txtLast->Text)) {
                MessageBox::Show(L"Enter the first name and surname.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            // ДЗ5 #2: стать має бути обрана.
            if (cbGender->SelectedIndex < 0) {
                MessageBox::Show(L"Select a gender.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            // ДЗ5 #2: валідація віку (16-100) ПЕРЕД створенням об'єкта (new).
            int age = 0;
            if (!Int32::TryParse(txtAge->Text, age) || age < 16 || age > 100) {
                MessageBox::Show(L"Age must be an integer between 16 and 100.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // СР5 #2: правильне перетворення String^ -> std::string через marshal_as.
            std::string f  = marshal_as<std::string>(txtFirst->Text->Trim());
            std::string m  = marshal_as<std::string>(txtMid->Text->Trim());
            std::string l  = marshal_as<std::string>(txtLast->Text->Trim());
            std::string g  = marshal_as<std::string>(cbGender->Text);
            std::string gr = marshal_as<std::string>(txtGroup->Text->Trim());

            // ДЗ5 #1: індивідуальне поле pets потрапляє в об'єкт.
            std::string pet = marshal_as<std::string>(txtPets->Text->Trim());

            // Створення студента (старий об'єкт спершу видаляємо).
            if (currentStudent_) { delete currentStudent_; }
            currentStudent_ = new Student(f, m, l, g, age, gr, "College of IT", pet, "none");

            // Вивід у Label — std::string -> String^ через gcnew String.
            lblResult->Text = gcnew String(currentStudent_->getInfo().c_str());
        }

        // ДЗ5 (Варіант Б): скидання форми — очищаємо всі поля та Label.
        System::Void btnClear_Click(Object^ sender, EventArgs^ e) {
            txtFirst->Clear(); txtMid->Clear(); txtLast->Clear();
            txtAge->Clear(); txtGroup->Clear(); txtPets->Clear();
            cbGender->SelectedIndex = 0;
            lblResult->Text = L"Form cleared.";
        }
    };
}
