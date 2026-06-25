#pragma once

// Тема 6 — об'єднано СР6 + ДЗ6.
// СР6: модальний StudentEditForm через ShowDialog()/DialogResult із передачею
//      даних у конструктор і зчитуванням результату через Edited*-властивості.
// ДЗ6: повний цикл Add / Edit / Delete, валідація у формі та в діалозі,
//      Варіант Б — пошук за ПІБ (TextBox, що фільтрує ListBox у реальному часі).
// Інтерфейс керований (gcnew, ^), бізнес-логіка нативна (Student*, std::vector).

#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

#include "Student.h"
#include "StudentEditForm.h"

namespace Portfolio {

    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class MainForm : public Form {
    public:
        MainForm() {
            students_ = new std::vector<Student*>();
            shown_ = gcnew List<int>();   // ДЗ6 #4: рядок списку -> індекс у векторі
            InitializeComponent();
            RefreshList();
        }

    protected:
        ~MainForm() { if (components) { delete components; } CleanupNative(); }
        !MainForm() { CleanupNative(); }

    private:
        // Нативний список студентів — пам'яттю керуємо вручну.
        std::vector<Student*>* students_;
        void CleanupNative() {
            if (students_) {
                for (Student* s : *students_) { delete s; }
                delete students_;
                students_ = nullptr;
            }
        }

        // Відповідність "видимий рядок ListBox -> індекс у векторі" (через фільтр).
        List<int>^ shown_;

        System::ComponentModel::Container^ components;
        Label^ lblFirst;  TextBox^ txtFirst;
        Label^ lblMid;    TextBox^ txtMid;
        Label^ lblLast;   TextBox^ txtLast;
        Label^ lblGender; ComboBox^ cbGender;
        Label^ lblAge;    TextBox^ txtAge;
        Label^ lblGroup;  TextBox^ txtGroup;
        Label^ lblPets;   TextBox^ txtPets;
        Button^ btnAdd;
        Label^ lblSearch; TextBox^ txtSearch;
        Label^ lblList;   ListBox^ lstStudents;
        Button^ btnEdit;  Button^ btnDelete;

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
            lblPets  = MakeLabel(L"Pets:",  LX, y); txtPets  = MakeBox(FX, y, FW); y += RH;

            btnAdd = gcnew Button();
            btnAdd->Text = L"Add";
            btnAdd->Location = Point(FX, y);
            btnAdd->Size = System::Drawing::Size(FW, 30);
            btnAdd->Click += gcnew EventHandler(this, &MainForm::btnAdd_Click);
            y += 42;

            // ДЗ6 #4 (Варіант Б): поле пошуку за ПІБ.
            lblSearch = MakeLabel(L"Search:", LX, y);
            txtSearch = MakeBox(FX, y, FW);
            txtSearch->TextChanged += gcnew EventHandler(this, &MainForm::txtSearch_TextChanged);
            y += RH;

            lblList = MakeLabel(L"Students:", LX, y); y += 22;
            lstStudents = gcnew ListBox();
            lstStudents->Location = Point(LX, y);
            lstStudents->Size = System::Drawing::Size(FX + FW - LX, 150);
            lstStudents->DoubleClick += gcnew EventHandler(this, &MainForm::btnEdit_Click);
            y += 158;

            btnEdit = gcnew Button();
            btnEdit->Text = L"Edit selected";
            btnEdit->Location = Point(LX, y);
            btnEdit->Size = System::Drawing::Size((FX + FW - LX - 10) / 2, 30);
            btnEdit->Click += gcnew EventHandler(this, &MainForm::btnEdit_Click);

            btnDelete = gcnew Button();
            btnDelete->Text = L"Delete selected";
            btnDelete->Location = Point(LX + (FX + FW - LX - 10) / 2 + 10, y);
            btnDelete->Size = System::Drawing::Size((FX + FW - LX - 10) / 2, 30);
            btnDelete->Click += gcnew EventHandler(this, &MainForm::btnDelete_Click);
            y += 42;

            // Реальні дані за замовчуванням для зручного тесту.
            txtFirst->Text = L"Maksym"; txtMid->Text = L"O."; txtLast->Text = L"Hodyk";
            txtAge->Text = L"19"; txtGroup->Text = L"PI-21"; txtPets->Text = L"cat Barsyk";

            this->Text = L"StudentPortfolio - Theme 6 (combined SR6 + HW6)";
            this->ClientSize = System::Drawing::Size(FX + FW + 15, y + 5);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                lblFirst, txtFirst, lblMid, txtMid, lblLast, txtLast,
                lblGender, cbGender, lblAge, txtAge, lblGroup, txtGroup, lblPets, txtPets,
                btnAdd, lblSearch, txtSearch, lblList, lstStudents, btnEdit, btnDelete
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        // Перемалювати список з урахуванням фільтра пошуку за ПІБ.
        System::Void RefreshList() {
            int keep = lstStudents->SelectedIndex;
            lstStudents->BeginUpdate();
            lstStudents->Items->Clear();
            shown_->Clear();

            String^ q = (txtSearch != nullptr) ? txtSearch->Text->Trim()->ToLower() : L"";
            for (std::size_t i = 0; i < students_->size(); ++i) {
                Student* s = (*students_)[i];
                String^ full = gcnew String(s->getFullName().c_str());
                // ДЗ6 #4: фільтр за ПІБ (пусто = показати всіх).
                if (q->Length == 0 || full->ToLower()->Contains(q)) {
                    String^ line = gcnew String(
                        (s->getFullName() + "  |  " + s->getGroup()
                         + "  |  pets: " + s->getHomePets()
                         + "  |  Age: " + std::to_string(s->getAge())).c_str());
                    lstStudents->Items->Add(line);
                    shown_->Add(static_cast<int>(i));
                }
            }
            lstStudents->EndUpdate();
            if (keep >= 0 && keep < lstStudents->Items->Count) { lstStudents->SelectedIndex = keep; }
        }

        // Спільна валідація полів форми (ім'я/стать/вік). Повертає вік через out.
        bool ValidateInputs(int% age) {
            if (String::IsNullOrWhiteSpace(txtFirst->Text) || String::IsNullOrWhiteSpace(txtLast->Text)) {
                MessageBox::Show(L"Enter the first name and surname.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return false;
            }
            if (cbGender->SelectedIndex < 0) {
                MessageBox::Show(L"Select a gender.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return false;
            }
            int parsed = 0;
            if (!Int32::TryParse(txtAge->Text, parsed) || parsed < 16 || parsed > 100) {
                MessageBox::Show(L"Age must be an integer between 16 and 100.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return false;
            }
            age = parsed;
            return true;
        }

        // Поточний індекс обраного студента у векторі (через мапу shown_), або -1.
        int SelectedVectorIndex() {
            int row = lstStudents->SelectedIndex;
            if (row < 0 || row >= shown_->Count) { return -1; }
            return shown_[row];
        }

        // ДЗ6 #1: Add — валідація -> новий Student* -> у vector -> RefreshList.
        System::Void btnAdd_Click(Object^ sender, EventArgs^ e) {
            int age = 0;
            if (!ValidateInputs(age)) { return; }

            std::string f   = marshal_as<std::string>(txtFirst->Text->Trim());
            std::string m   = marshal_as<std::string>(txtMid->Text->Trim());
            std::string l   = marshal_as<std::string>(txtLast->Text->Trim());
            std::string g   = marshal_as<std::string>(cbGender->Text);
            std::string gr  = marshal_as<std::string>(txtGroup->Text->Trim());
            std::string pet = marshal_as<std::string>(txtPets->Text->Trim());

            students_->push_back(new Student(f, m, l, g, age, gr, "College of IT", pet, "none"));
            RefreshList();
        }

        // СР6: Edit — модальний StudentEditForm, зміни лише за DialogResult::OK.
        System::Void btnEdit_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedVectorIndex();
            if (idx < 0) {
                MessageBox::Show(L"Please select a student to edit.", L"No selection",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            Student* cur = (*students_)[idx];

            // СР6 #1: передаємо поточні дані студента в діалог через конструктор.
            StudentEditForm^ dlg = gcnew StudentEditForm(
                gcnew String(cur->getFirstName().c_str()),
                gcnew String(cur->getMidName().c_str()),
                gcnew String(cur->getLastName().c_str()),
                gcnew String(cur->getGender().c_str()),
                cur->getAge(),
                gcnew String(cur->getGroup().c_str()),
                gcnew String(cur->getHomePets().c_str()),
                gcnew String(cur->getSport().c_str()));

            // СР6 #2: зміни застосовуємо лише за DialogResult::OK.
            if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                // СР6 #3: зчитуємо результат через Edited*-властивості (read-only).
                std::string f   = marshal_as<std::string>(dlg->EditedFirst);
                std::string m   = marshal_as<std::string>(dlg->EditedMid);
                std::string l   = marshal_as<std::string>(dlg->EditedLast);
                std::string g   = marshal_as<std::string>(dlg->EditedGender);
                int         age = dlg->EditedAge;
                std::string gr  = marshal_as<std::string>(dlg->EditedGroup);
                std::string pet = marshal_as<std::string>(dlg->EditedPets);
                std::string sp  = marshal_as<std::string>(dlg->EditedSport);

                // СР6 #4: оновлюємо vector — старий об'єкт замінюємо новим.
                delete cur;
                (*students_)[idx] = new Student(f, m, l, g, age, gr, "College of IT", pet, sp);
                RefreshList();
            }
        }

        // ДЗ6 #3: Delete — підтвердження -> видалення з vector -> RefreshList.
        System::Void btnDelete_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedVectorIndex();
            if (idx < 0) {
                MessageBox::Show(L"Please select a student to delete.", L"No selection",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            String^ name = gcnew String((*students_)[idx]->getFullName().c_str());
            if (MessageBox::Show(L"Delete \"" + name + L"\"?", L"Confirm delete",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) {
                return;
            }
            delete (*students_)[idx];
            students_->erase(students_->begin() + idx);
            lstStudents->SelectedIndex = -1;
            RefreshList();
        }

        // ДЗ6 #4 (Варіант Б): живий фільтр списку при зміні тексту пошуку.
        System::Void txtSearch_TextChanged(Object^ sender, EventArgs^ e) {
            lstStudents->SelectedIndex = -1;
            RefreshList();
        }
    };
}
