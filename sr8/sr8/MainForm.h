#pragma once

// СР8 (Тема 8). Файлове введення/виведення + Settings.
// DataGridView показує нативний std::vector<Person*>; кнопки Save/Load
// серіалізують список у текстовий файл і відновлюють його назад.
// Виправлено «закладені» помилки із завдання:
//   #1 маркер типу (S/T/F) перед полями — інакше неможливо відновити тип;
//   #2 потоки явно закриваються (StreamWriter/StreamReader -> Close);
//   #3 збереження Settings (LastPath) через Settings::Save();
//   #4 очищення vector (із delete) перед завантаженням;
//   #5 перевірено цикл Save -> Load.
// Формат рядка: TYP|first|mid|last|gender|age|extra1|extra2.

#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include "Settings.h"

namespace Portfolio {

    using namespace System;
    using namespace System::IO;
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
            RefreshGrid();
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
        void ClearPeople() {
            for (Person* p : *people_) { delete p; }
            people_->clear();
        }

        System::ComponentModel::Container^ components;
        DataGridView^ dataGridView1;
        Button^ btnSave;
        Button^ btnLoad;
        Label^ lblStatus;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 12);
            dataGridView1->Size = System::Drawing::Size(620, 260);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->MultiSelect = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            btnSave = gcnew Button();
            btnSave->Text = L"Save...";
            btnSave->Location = Point(12, 284);
            btnSave->Size = System::Drawing::Size(150, 32);
            btnSave->Click += gcnew EventHandler(this, &MainForm::btnSave_Click);

            btnLoad = gcnew Button();
            btnLoad->Text = L"Load...";
            btnLoad->Location = Point(172, 284);
            btnLoad->Size = System::Drawing::Size(150, 32);
            btnLoad->Click += gcnew EventHandler(this, &MainForm::btnLoad_Click);

            lblStatus = gcnew Label();
            lblStatus->Location = Point(338, 290);
            lblStatus->AutoSize = true;
            lblStatus->Text = L"Ready.";

            this->Text = L"StudentPortfolio - Theme 8 (SR8: file save/load)";
            this->ClientSize = System::Drawing::Size(644, 330);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                dataGridView1, btnSave, btnLoad, lblStatus
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void SeedSampleData() {
            people_->push_back(new Student("Maksym", "O.", "Hodyk", "Male", 19,
                "PI-21", "College of IT", "cat Barsyk", "none"));
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

        System::Void RefreshGrid() {
            dataGridView1->Rows->Clear();
            for (Person* p : *people_) {
                dataGridView1->Rows->Add(
                    gcnew String(p->getFullName().c_str()),
                    gcnew String(p->getRole().c_str()),
                    gcnew String(p->getInfo().c_str()));
            }
            dataGridView1->ClearSelection();
        }

        // ---- Серіалізація одного об'єкта у рядок TYP|...  (СР8 #1) ----
        String^ SerializePerson(Person* p) {
            String^ first  = gcnew String(p->getFirstName().c_str());
            String^ mid    = gcnew String(p->getMidName().c_str());
            String^ last   = gcnew String(p->getLastName().c_str());
            String^ gender = gcnew String(p->getGender().c_str());
            String^ age    = p->getAge().ToString();

            Student* st = dynamic_cast<Student*>(p);
            if (st != nullptr) {
                String^ e1 = gcnew String(st->getGroup().c_str());
                String^ e2 = gcnew String(st->getHomePets().c_str());
                return String::Join(L"|", gcnew array<String^>{ L"S", first, mid, last, gender, age, e1, e2 });
            }
            Teacher* t = dynamic_cast<Teacher*>(p);
            if (t != nullptr) {
                String^ e1 = gcnew String(t->getSubject().c_str());
                String^ e2 = t->getExperience().ToString();
                return String::Join(L"|", gcnew array<String^>{ L"T", first, mid, last, gender, age, e1, e2 });
            }
            Staff* f = dynamic_cast<Staff*>(p);
            String^ e1 = (f != nullptr) ? gcnew String(f->getPosition().c_str())   : L"";
            String^ e2 = (f != nullptr) ? gcnew String(f->getDepartment().c_str()) : L"";
            return String::Join(L"|", gcnew array<String^>{ L"F", first, mid, last, gender, age, e1, e2 });
        }

        // ---- Розбір рядка -> новий Person* (або nullptr, якщо формат хибний) ----
        Person* DeserializeLine(String^ line) {
            if (String::IsNullOrWhiteSpace(line)) { return nullptr; }
            array<String^>^ f = line->Split('|');
            if (f->Length < 8) { return nullptr; }

            // Елемент керованого масиву не можна напряму передати в marshal_as
            // (потрібне native-посилання) — копіюємо в локальні String^.
            String^ sType = f[0];
            String^ sFirst = f[1], ^sMid = f[2], ^sLast = f[3], ^sGender = f[4];
            String^ sAge = f[5], ^sE1 = f[6], ^sE2 = f[7];

            std::string first  = marshal_as<std::string>(sFirst);
            std::string mid    = marshal_as<std::string>(sMid);
            std::string last   = marshal_as<std::string>(sLast);
            std::string gender = marshal_as<std::string>(sGender);
            int age = 0; Int32::TryParse(sAge, age);
            std::string e1 = marshal_as<std::string>(sE1);
            std::string e2 = marshal_as<std::string>(sE2);

            if (sType == L"S") {
                return new Student(first, mid, last, gender, age, e1, "College of IT", e2, "none");
            }
            if (sType == L"T") {
                int exp = 0; Int32::TryParse(sE2, exp);
                return new Teacher(first, mid, last, gender, age, e1, exp);
            }
            if (sType == L"F") {
                return new Staff(first, mid, last, gender, age, e1, e2, "n/a");
            }
            return nullptr;
        }

        // ===================== ЗБЕРЕЖЕННЯ В ФАЙЛ =====================
        System::Void btnSave_Click(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"Text Files|*.txt|All Files|*.*";
            dlg->InitialDirectory = Properties::Settings::Default->LastPath;
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }

            StreamWriter^ sw = nullptr;
            try {
                sw = gcnew StreamWriter(dlg->FileName, false, System::Text::Encoding::UTF8);
                for (Person* p : *people_) {
                    sw->WriteLine(SerializePerson(p));   // СР8 #1: маркер типу + поля
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Save error: " + ex->Message, L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }
            finally {
                if (sw != nullptr) { sw->Close(); }      // СР8 #2: потік закрито (файл не заблоковано)
            }

            // СР8 #3: запам'ятати каталог і ЗБЕРЕГТИ налаштування.
            Properties::Settings::Default->LastPath = Path::GetDirectoryName(dlg->FileName);
            Properties::Settings::Default->Save();
            lblStatus->Text = L"Saved " + people_->size().ToString() + L" record(s).";
        }

        // ===================== ЗАВАНТАЖЕННЯ З ФАЙЛУ =====================
        System::Void btnLoad_Click(System::Object^ sender, EventArgs^ e) {
            OpenFileDialog^ dlg = gcnew OpenFileDialog();
            dlg->Filter = L"Text Files|*.txt|All Files|*.*";
            dlg->InitialDirectory = Properties::Settings::Default->LastPath;
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }
            if (!File::Exists(dlg->FileName)) { return; }

            // СР8 #4: очистити vector (із delete) ПЕРЕД завантаженням.
            ClearPeople();

            StreamReader^ sr = nullptr;
            int loaded = 0;
            try {
                sr = gcnew StreamReader(dlg->FileName, System::Text::Encoding::UTF8);
                String^ line;
                while ((line = sr->ReadLine()) != nullptr) {
                    Person* p = DeserializeLine(line);
                    if (p != nullptr) { people_->push_back(p); ++loaded; }
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Load error: " + ex->Message, L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally {
                if (sr != nullptr) { sr->Close(); }      // СР8 #2: читач закрито
            }

            Properties::Settings::Default->LastPath = Path::GetDirectoryName(dlg->FileName);
            Properties::Settings::Default->Save();
            RefreshGrid();
            lblStatus->Text = L"Loaded " + loaded.ToString() + L" record(s).";
        }
    };
}
