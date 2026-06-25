#pragma once

// СР9 (Тема 9). Навігація вкладками + ErrorProvider.
// TabControl діє як перемикач фільтра (All/Students/Teachers/Staff); знизу —
// DataGridView, що показує відфільтрований std::vector<Person*>. Є поле віку з
// перевіркою через ErrorProvider. Виправлено «закладені» помилки:
//   #1 підписка на tabControl1->SelectedIndexChanged;
//   #2 оператор порівняння == (а не присвоєння =) у виборі вкладки;
//   #3 errorProvider1->SetError(txtAge, "") для очищення при успіху;
//   #4 FilterByType() — перебір vector + dynamic_cast;
//   #5 перевірено перемикання вкладок та валідацію.

#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

namespace Portfolio {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class MainForm : public Form {
    public:
        MainForm() {
            people_ = new std::vector<Person*>();
            InitializeComponent();
            SeedSampleData();
            InitializeGrid();
            SetupNavigation();
            FilterByType(L"All");
        }

    protected:
        ~MainForm() { if (components) { delete components; } CleanupNative(); }
        !MainForm() { CleanupNative(); }

    private:
        std::vector<Person*>* people_;
        void CleanupNative() {
            if (people_) {
                for (Person* p : *people_) { delete p; }
                delete people_;
                people_ = nullptr;
            }
        }

        System::ComponentModel::Container^ components;
        TabControl^ tabControl1;
        DataGridView^ dataGridView1;
        ErrorProvider^ errorProvider1;
        Label^ lblAge;  TextBox^ txtAge;
        Label^ lblHint;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            tabControl1 = gcnew TabControl();
            tabControl1->Location = Point(12, 12);
            tabControl1->Size = System::Drawing::Size(620, 28);
            tabControl1->TabPages->Add(gcnew TabPage(L"All"));
            tabControl1->TabPages->Add(gcnew TabPage(L"Students"));
            tabControl1->TabPages->Add(gcnew TabPage(L"Teachers"));
            tabControl1->TabPages->Add(gcnew TabPage(L"Staff"));

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 46);
            dataGridView1->Size = System::Drawing::Size(620, 240);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->MultiSelect = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            lblAge = gcnew Label();
            lblAge->Text = L"Age filter check:";
            lblAge->Location = Point(12, 300);
            lblAge->AutoSize = true;

            txtAge = gcnew TextBox();
            txtAge->Location = Point(130, 297);
            txtAge->Size = System::Drawing::Size(90, 23);
            txtAge->Text = L"18";
            // СР9 #5: валідація поля при втраті фокуса.
            txtAge->Validating += gcnew CancelEventHandler(this, &MainForm::txtAge_Validating);

            lblHint = gcnew Label();
            lblHint->Location = Point(240, 300);
            lblHint->AutoSize = true;
            lblHint->Text = L"Switch tabs to filter by role; age must be 16-100.";

            errorProvider1 = gcnew ErrorProvider();

            this->Text = L"StudentPortfolio - Theme 9 (SR9: tabs navigation + ErrorProvider)";
            this->ClientSize = System::Drawing::Size(644, 340);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                tabControl1, dataGridView1, lblAge, txtAge, lblHint
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void SeedSampleData() {
            people_->push_back(new Student("Maksym", "O.", "Hodyk", "Male", 19,
                "PI-21", "College of IT", "cat Barsyk", "none"));
            people_->push_back(new Student("Anna", "S.", "Bondarenko", "Female", 18,
                "PI-11", "College of IT", "dog Rex", "swimming"));
            people_->push_back(new Teacher("Olena", "I.", "Shevchenko", "Female", 44,
                "Mathematics", 20));
            people_->push_back(new Staff("Ihor", "P.", "Koval", "Male", 38,
                "Lab engineer", "IT department", "2015-09-01"));
        }

        System::Void InitializeGrid() {
            dataGridView1->Columns->Clear();
            dataGridView1->Columns->Add(L"colName",   L"Full name");
            dataGridView1->Columns->Add(L"colType",   L"Role");
            dataGridView1->Columns->Add(L"colDetail", L"Details");
            dataGridView1->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dataGridView1->AllowUserToAddRows = false;
        }

        // --- ІНІЦІАЛІЗАЦІЯ НАВІГАЦІЇ ---
        System::Void SetupNavigation() {
            // СР9 #1: підписка на зміну активної вкладки.
            tabControl1->SelectedIndexChanged +=
                gcnew EventHandler(this, &MainForm::tabControl1_SelectedIndexChanged);
            // Налаштування ErrorProvider.
            errorProvider1->Icon = SystemIcons::Warning;
            errorProvider1->BlinkStyle = ErrorBlinkStyle::NeverBlink;
        }

        // --- ФІЛЬТРАЦІЯ ЗА ВКЛАДКАМИ ---
        System::Void tabControl1_SelectedIndexChanged(System::Object^ sender, EventArgs^ e) {
            int idx = tabControl1->SelectedIndex;
            // СР9 #2: саме порівняння (==), а не присвоєння (=) у кожній гілці.
            if (idx == 0)      { FilterByType(L"All"); }
            else if (idx == 1) { FilterByType(L"Student"); }
            else if (idx == 2) { FilterByType(L"Teacher"); }
            else if (idx == 3) { FilterByType(L"Staff"); }
        }

        // СР9 #4: фільтрація переходом по vector із перевіркою типу через dynamic_cast.
        System::Void FilterByType(String^ role) {
            dataGridView1->Rows->Clear();
            for (Person* p : *people_) {
                bool show =
                    (role == L"All") ||
                    (role == L"Student" && dynamic_cast<Student*>(p) != nullptr) ||
                    (role == L"Teacher" && dynamic_cast<Teacher*>(p) != nullptr) ||
                    (role == L"Staff"   && dynamic_cast<Staff*>(p)   != nullptr);
                if (show) {
                    dataGridView1->Rows->Add(
                        gcnew String(p->getFullName().c_str()),
                        gcnew String(p->getRole().c_str()),
                        gcnew String(p->getInfo().c_str()));
                }
            }
            dataGridView1->ClearSelection();
        }

        // --- ВАЛІДАЦІЯ ПОЛІВ ---
        System::Void txtAge_Validating(System::Object^ sender, CancelEventArgs^ e) {
            int age;
            bool ok = Int32::TryParse(txtAge->Text, age);
            if (!ok || age < 16 || age > 100) {
                errorProvider1->SetError(txtAge, L"Age must be 16-100");
                e->Cancel = true;     // забороняє вихід з поля
            } else {
                // СР9 #3: очистити попередню помилку при успішній валідації.
                errorProvider1->SetError(txtAge, L"");
            }
        }
    };
}
