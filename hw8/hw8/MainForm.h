#pragma once

// ДЗ8 (Тема 8). Повний файловий цикл поверх DataGridView + Settings.
//   #1 Save/Load з парсингом TXT (TYP|first|mid|last|gender|age|extra1|extra2).
//   #2 Автозбереження при закритті форми (подія FormClosing) у autosave.txt.
//   #3 Збереження/відновлення розміру та позиції вікна через Properties::Settings
//      (RestoreWindowState застосовує збережені межі лише в межах екрана).
//   #4 Кнопка «Export CSV» (кома-роздільник, лапки для полів із комами).
//   #5 Варіант Б: лог помилок I/O у error.log з датою/часом.

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
            RestoreWindowState();                 // ДЗ8 #3
            InitializeGrid();
            // ДЗ8 #2: за наявності автозбереження — відновлюємо, інакше демо-дані.
            if (File::Exists(AutosavePath()) && LoadFromFile(AutosavePath()) > 0) {
                lblStatus->Text = L"Restored from autosave.";
            } else {
                SeedSampleData();
                lblStatus->Text = L"Ready (sample data).";
            }
            RefreshGrid();
            this->FormClosing += gcnew FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
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
        Button^ btnExportCsv;
        Label^ lblStatus;

        String^ AutosavePath() { return Path::Combine(Application::StartupPath, L"autosave.txt"); }
        String^ ErrorLogPath() { return Path::Combine(Application::StartupPath, L"error.log"); }

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 12);
            dataGridView1->Size = System::Drawing::Size(620, 250);
            dataGridView1->Anchor = static_cast<AnchorStyles>(
                AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->MultiSelect = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            btnSave = gcnew Button();
            btnSave->Text = L"Save...";
            btnSave->Size = System::Drawing::Size(140, 32);
            btnSave->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Left);
            btnSave->Click += gcnew EventHandler(this, &MainForm::btnSave_Click);

            btnLoad = gcnew Button();
            btnLoad->Text = L"Load...";
            btnLoad->Size = System::Drawing::Size(140, 32);
            btnLoad->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Left);
            btnLoad->Click += gcnew EventHandler(this, &MainForm::btnLoad_Click);

            btnExportCsv = gcnew Button();
            btnExportCsv->Text = L"Export CSV...";
            btnExportCsv->Size = System::Drawing::Size(140, 32);
            btnExportCsv->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Left);
            btnExportCsv->Click += gcnew EventHandler(this, &MainForm::btnExportCsv_Click);

            lblStatus = gcnew Label();
            lblStatus->AutoSize = true;
            lblStatus->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Left);
            lblStatus->Text = L"Ready.";

            // Початкове розташування кнопок (далі тримається якорями).
            btnSave->Location      = Point(12, 274);
            btnLoad->Location      = Point(160, 274);
            btnExportCsv->Location = Point(308, 274);
            lblStatus->Location    = Point(456, 282);

            this->Text = L"StudentPortfolio - Theme 8 (HW8: full file cycle)";
            this->ClientSize = System::Drawing::Size(644, 320);
            this->MinimumSize = System::Drawing::Size(520, 280);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                dataGridView1, btnSave, btnLoad, btnExportCsv, lblStatus
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

        // ===================== Варіант Б: лог помилок =====================
        // ДЗ8 #5: дописати рядок про помилку I/O у error.log з датою/часом.
        void LogError(String^ context, String^ message) {
            try {
                StreamWriter^ log = gcnew StreamWriter(ErrorLogPath(), true, System::Text::Encoding::UTF8);
                log->WriteLine(DateTime::Now.ToString(L"yyyy-MM-dd HH:mm:ss") + L"  [" + context + L"]  " + message);
                log->Close();
            }
            catch (Exception^) { /* лог не повинен валити застосунок */ }
        }

        // ===================== Серіалізація (ДЗ8 #1) =====================
        String^ SerializePerson(Person* p) {
            String^ first  = gcnew String(p->getFirstName().c_str());
            String^ mid    = gcnew String(p->getMidName().c_str());
            String^ last   = gcnew String(p->getLastName().c_str());
            String^ gender = gcnew String(p->getGender().c_str());
            String^ age    = p->getAge().ToString();

            Student* st = dynamic_cast<Student*>(p);
            if (st != nullptr) {
                return String::Join(L"|", gcnew array<String^>{ L"S", first, mid, last, gender, age,
                    gcnew String(st->getGroup().c_str()), gcnew String(st->getHomePets().c_str()) });
            }
            Teacher* t = dynamic_cast<Teacher*>(p);
            if (t != nullptr) {
                return String::Join(L"|", gcnew array<String^>{ L"T", first, mid, last, gender, age,
                    gcnew String(t->getSubject().c_str()), t->getExperience().ToString() });
            }
            Staff* f = dynamic_cast<Staff*>(p);
            String^ e1 = (f != nullptr) ? gcnew String(f->getPosition().c_str())   : L"";
            String^ e2 = (f != nullptr) ? gcnew String(f->getDepartment().c_str()) : L"";
            return String::Join(L"|", gcnew array<String^>{ L"F", first, mid, last, gender, age, e1, e2 });
        }

        Person* DeserializeLine(String^ line) {
            if (String::IsNullOrWhiteSpace(line)) { return nullptr; }
            array<String^>^ f = line->Split('|');
            if (f->Length < 8) { return nullptr; }

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

            if (sType == L"S") { return new Student(first, mid, last, gender, age, e1, "College of IT", e2, "none"); }
            if (sType == L"T") { int exp = 0; Int32::TryParse(sE2, exp); return new Teacher(first, mid, last, gender, age, e1, exp); }
            if (sType == L"F") { return new Staff(first, mid, last, gender, age, e1, e2, "n/a"); }
            return nullptr;
        }

        // Запис vector у файл (без діалогу). true = успіх.
        bool SaveToFile(String^ path) {
            StreamWriter^ sw = nullptr;
            try {
                sw = gcnew StreamWriter(path, false, System::Text::Encoding::UTF8);
                for (Person* p : *people_) { sw->WriteLine(SerializePerson(p)); }
                return true;
            }
            catch (Exception^ ex) { LogError(L"Save", ex->Message); return false; }
            finally { if (sw != nullptr) { sw->Close(); } }
        }

        // Завантаження vector із файлу (без діалогу). Повертає к-сть записів (-1 = помилка).
        int LoadFromFile(String^ path) {
            if (!File::Exists(path)) { return -1; }
            ClearPeople();
            StreamReader^ sr = nullptr;
            int loaded = 0;
            try {
                sr = gcnew StreamReader(path, System::Text::Encoding::UTF8);
                String^ line;
                while ((line = sr->ReadLine()) != nullptr) {
                    Person* p = DeserializeLine(line);
                    if (p != nullptr) { people_->push_back(p); ++loaded; }
                }
                return loaded;
            }
            catch (Exception^ ex) { LogError(L"Load", ex->Message); return -1; }
            finally { if (sr != nullptr) { sr->Close(); } }
        }

        // ===================== Кнопки Save / Load =====================
        System::Void btnSave_Click(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"Text Files|*.txt|All Files|*.*";
            dlg->InitialDirectory = Properties::Settings::Default->LastPath;
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }
            if (SaveToFile(dlg->FileName)) {
                Properties::Settings::Default->LastPath = Path::GetDirectoryName(dlg->FileName);
                Properties::Settings::Default->Save();
                lblStatus->Text = L"Saved " + people_->size().ToString() + L" record(s).";
            } else {
                MessageBox::Show(L"Save failed (see error.log).", L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnLoad_Click(System::Object^ sender, EventArgs^ e) {
            OpenFileDialog^ dlg = gcnew OpenFileDialog();
            dlg->Filter = L"Text Files|*.txt|All Files|*.*";
            dlg->InitialDirectory = Properties::Settings::Default->LastPath;
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }
            int loaded = LoadFromFile(dlg->FileName);
            RefreshGrid();
            if (loaded >= 0) {
                Properties::Settings::Default->LastPath = Path::GetDirectoryName(dlg->FileName);
                Properties::Settings::Default->Save();
                lblStatus->Text = L"Loaded " + loaded.ToString() + L" record(s).";
            } else {
                MessageBox::Show(L"Load failed (see error.log).", L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // ===================== ДЗ8 #4: Export CSV =====================
        String^ CsvEscape(String^ field) {
            if (field == nullptr) { return L""; }
            if (field->Contains(L",") || field->Contains(L"\"") || field->Contains(L"\n")) {
                return L"\"" + field->Replace(L"\"", L"\"\"") + L"\"";   // подвоєння лапок
            }
            return field;
        }

        System::Void btnExportCsv_Click(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"CSV Files|*.csv|All Files|*.*";
            dlg->InitialDirectory = Properties::Settings::Default->LastPath;
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }

            StreamWriter^ sw = nullptr;
            try {
                sw = gcnew StreamWriter(dlg->FileName, false, System::Text::Encoding::UTF8);
                sw->WriteLine(L"Role,Surname,FirstName,MiddleName,Gender,Age,Details");
                for (Person* p : *people_) {
                    array<String^>^ cells = gcnew array<String^>{
                        CsvEscape(gcnew String(p->getRole().c_str())),
                        CsvEscape(gcnew String(p->getLastName().c_str())),
                        CsvEscape(gcnew String(p->getFirstName().c_str())),
                        CsvEscape(gcnew String(p->getMidName().c_str())),
                        CsvEscape(gcnew String(p->getGender().c_str())),
                        p->getAge().ToString(),
                        CsvEscape(gcnew String(p->getInfo().c_str()))
                    };
                    sw->WriteLine(String::Join(L",", cells));
                }
                lblStatus->Text = L"Exported CSV (" + people_->size().ToString() + L" rows).";
            }
            catch (Exception^ ex) {
                LogError(L"ExportCSV", ex->Message);
                MessageBox::Show(L"CSV export failed (see error.log).", L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally { if (sw != nullptr) { sw->Close(); } }
        }

        // ===================== Вікно: збереження/відновлення =====================
        // ДЗ8 #3: відновити збережені межі ЛИШЕ якщо вони в межах якогось екрана.
        void RestoreWindowState() {
            System::Drawing::Size sz = Properties::Settings::Default->WindowSize;
            if (sz.Width >= this->MinimumSize.Width && sz.Height >= this->MinimumSize.Height) {
                System::Drawing::Point loc = Properties::Settings::Default->WindowLocation;
                System::Drawing::Rectangle bounds(loc, sz);
                bool onScreen = false;
                for each (Screen^ scr in Screen::AllScreens) {
                    if (scr->WorkingArea.IntersectsWith(bounds)) { onScreen = true; break; }
                }
                if (onScreen) {
                    this->StartPosition = FormStartPosition::Manual;
                    this->Location = loc;
                    this->Size = sz;
                    return;
                }
            }
            this->StartPosition = FormStartPosition::CenterScreen;
        }

        void SaveWindowState() {
            // Для згорнутого/розгорнутого вікна беремо «нормальні» межі.
            System::Drawing::Rectangle b = (this->WindowState == FormWindowState::Normal)
                ? this->Bounds : this->RestoreBounds;
            Properties::Settings::Default->WindowLocation = b.Location;
            Properties::Settings::Default->WindowSize = b.Size;
        }

        // ДЗ8 #2: автозбереження + межі вікна при закритті форми.
        System::Void MainForm_FormClosing(System::Object^ sender, FormClosingEventArgs^ e) {
            SaveToFile(AutosavePath());
            SaveWindowState();
            Properties::Settings::Default->Save();
        }
    };
}
