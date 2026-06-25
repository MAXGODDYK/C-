#pragma once

// Тема 7 — об'єднано СР7 + ДЗ7. Один спільний нативний список
// std::vector<Person*> показано двома способами на вкладках TabControl:
//   • вкладка «Grid (SR7)»  — DataGridView із синхронізацією, деталями та
//     умовним фарбуванням рядка Student (LightBlue). Маркери // СР7 #1..#4.
//   • вкладка «List (HW7)» — ListView з режимами Details/LargeIcon,
//     сортуванням за прізвищем, контекстним меню, умовним форматуванням
//     (age<18 червоний, стаж>15 жирний) і підсумковим рядком. Маркери // ДЗ7 #1..#5.
// Інтерфейс керований (gcnew, ^), бізнес-логіка нативна (Person*, std::vector).

#include <vector>
#include <string>
#include <algorithm>
#include <msclr/marshal_cppstd.h>

#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include "StudentEditForm.h"

namespace Portfolio {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    // Нативний компаратор за прізвищем (далі — за іменем): лямбди в методах
    // керованого класу заборонені, тож виносимо на рівень namespace.
    inline bool LessBySurname(Person* a, Person* b) {
        if (a->getLastName() != b->getLastName()) { return a->getLastName() < b->getLastName(); }
        return a->getFirstName() < b->getFirstName();
    }

    public ref class MainForm : public Form {
    public:
        MainForm() {
            people_ = new std::vector<Person*>();
            sortAscending_ = true;
            InitializeComponent();
            SeedSampleData();
            InitializeGrid();
            RefreshAll();
        }

    protected:
        ~MainForm() { if (components) { delete components; } CleanupNative(); }
        !MainForm() { CleanupNative(); }

    private:
        std::vector<Person*>* people_;
        bool sortAscending_;
        void CleanupNative() {
            if (people_) {
                for (Person* p : *people_) { delete p; }
                delete people_;
                people_ = nullptr;
            }
        }

        System::ComponentModel::Container^ components;
        TabControl^ tabs_;
        // --- вкладка Grid (СР7) ---
        TabPage^ tabGrid_;
        DataGridView^ dataGridView1;
        Label^ lblDetails;
        // --- вкладка List (ДЗ7) ---
        TabPage^ tabList_;
        ListView^ listView1;
        ImageList^ largeIcons_;
        RadioButton^ rbDetails;
        RadioButton^ rbLargeIcon;
        System::Windows::Forms::ContextMenuStrip^ contextMenu_;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            tabs_ = gcnew TabControl();
            tabs_->Location = Point(8, 8);
            tabs_->Size = System::Drawing::Size(648, 410);

            BuildGridTab();
            BuildListTab();
            tabs_->TabPages->Add(tabGrid_);
            tabs_->TabPages->Add(tabList_);

            this->Text = L"StudentPortfolio - Theme 7 (combined SR7 + HW7)";
            this->ClientSize = System::Drawing::Size(664, 426);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Controls->Add(tabs_);

            this->ResumeLayout(false);
        }

        // ======================= Вкладка Grid (СР7) =======================
        void BuildGridTab() {
            tabGrid_ = gcnew TabPage(L"Grid (SR7)");

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(8, 8);
            dataGridView1->Size = System::Drawing::Size(620, 250);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->MultiSelect = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dataGridView1->SelectionChanged += gcnew EventHandler(this, &MainForm::dataGridView1_SelectionChanged);

            lblDetails = gcnew Label();
            lblDetails->Location = Point(8, 268);
            lblDetails->Size = System::Drawing::Size(620, 96);
            lblDetails->BorderStyle = BorderStyle::FixedSingle;
            lblDetails->TextAlign = ContentAlignment::TopLeft;
            lblDetails->Text = L"Select a row to see details.";

            tabGrid_->Controls->Add(dataGridView1);
            tabGrid_->Controls->Add(lblDetails);
        }

        System::Void InitializeGrid() {
            dataGridView1->Columns->Clear();
            dataGridView1->Columns->Add(L"colName",   L"Full name");
            dataGridView1->Columns->Add(L"colType",   L"Role");
            dataGridView1->Columns->Add(L"colDetail", L"Details");
            dataGridView1->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dataGridView1->AllowUserToAddRows = false;   // СР7 #1
        }

        System::Void RefreshGrid() {
            dataGridView1->Rows->Clear();
            for (std::size_t i = 0; i < people_->size(); ++i) {
                Person* p = (*people_)[i];
                String^ name   = gcnew String(p->getFullName().c_str());
                String^ type   = gcnew String(p->getRole().c_str());
                String^ detail = gcnew String(p->getInfo().c_str());   // СР7 #2
                int rowIndex = dataGridView1->Rows->Add(name, type, detail);
                if (dynamic_cast<Student*>(p) != nullptr) {            // СР7 #4
                    dataGridView1->Rows[rowIndex]->DefaultCellStyle->BackColor = Color::LightBlue;
                }
            }
            dataGridView1->ClearSelection();
            lblDetails->Text = L"Select a row to see details.";
        }

        System::Void dataGridView1_SelectionChanged(System::Object^ sender, EventArgs^ e) {
            if (dataGridView1->SelectedRows->Count == 0) {            // СР7 #3
                lblDetails->Text = L"Select a row to see details.";
                return;
            }
            int idx = dataGridView1->SelectedRows[0]->Index;
            if (idx < 0 || idx >= static_cast<int>(people_->size())) { return; }
            Person* selected = (*people_)[idx];
            lblDetails->Text = gcnew String(selected->getFormalGreeting().c_str())
                + Environment::NewLine + gcnew String(selected->getInfo().c_str());
        }

        // ======================= Вкладка List (ДЗ7) =======================
        void BuildListTab() {
            tabList_ = gcnew TabPage(L"List (HW7)");

            largeIcons_ = gcnew ImageList();
            largeIcons_->ImageSize = System::Drawing::Size(32, 32);
            largeIcons_->Images->Add(MakeIcon(Color::SteelBlue));   // 0 = Student
            largeIcons_->Images->Add(MakeIcon(Color::SeaGreen));    // 1 = Teacher
            largeIcons_->Images->Add(MakeIcon(Color::DarkOrange));  // 2 = Staff
            largeIcons_->Images->Add(MakeIcon(Color::Gray));        // 3 = summary

            contextMenu_ = gcnew System::Windows::Forms::ContextMenuStrip();
            ToolStripMenuItem^ miEdit    = gcnew ToolStripMenuItem(L"Edit");
            ToolStripMenuItem^ miDelete  = gcnew ToolStripMenuItem(L"Delete");
            ToolStripMenuItem^ miDetails = gcnew ToolStripMenuItem(L"Details");
            miEdit->Click    += gcnew EventHandler(this, &MainForm::miEdit_Click);
            miDelete->Click  += gcnew EventHandler(this, &MainForm::miDelete_Click);
            miDetails->Click += gcnew EventHandler(this, &MainForm::miDetails_Click);
            contextMenu_->Items->Add(miEdit);
            contextMenu_->Items->Add(miDelete);
            contextMenu_->Items->Add(miDetails);

            rbDetails = gcnew RadioButton();
            rbDetails->Text = L"Details"; rbDetails->Location = Point(8, 8);
            rbDetails->AutoSize = true; rbDetails->Checked = true;
            rbDetails->CheckedChanged += gcnew EventHandler(this, &MainForm::rbView_CheckedChanged);

            rbLargeIcon = gcnew RadioButton();
            rbLargeIcon->Text = L"Large icons"; rbLargeIcon->Location = Point(106, 8);
            rbLargeIcon->AutoSize = true;
            rbLargeIcon->CheckedChanged += gcnew EventHandler(this, &MainForm::rbView_CheckedChanged);

            listView1 = gcnew ListView();
            listView1->Location = Point(8, 40);
            listView1->Size = System::Drawing::Size(620, 324);
            listView1->View = System::Windows::Forms::View::Details;
            listView1->FullRowSelect = true;
            listView1->MultiSelect = false;
            listView1->HideSelection = false;
            listView1->LargeImageList = largeIcons_;
            listView1->SmallImageList = largeIcons_;
            listView1->ContextMenuStrip = contextMenu_;
            listView1->Columns->Add(L"Surname / full name", 260);
            listView1->Columns->Add(L"Role", 110);
            listView1->Columns->Add(L"Age", 60);
            listView1->Columns->Add(L"Details", 180);
            listView1->ColumnClick += gcnew ColumnClickEventHandler(this, &MainForm::listView1_ColumnClick);
            listView1->MouseDown   += gcnew MouseEventHandler(this, &MainForm::listView1_MouseDown);

            tabList_->Controls->Add(rbDetails);
            tabList_->Controls->Add(rbLargeIcon);
            tabList_->Controls->Add(listView1);
        }

        Bitmap^ MakeIcon(Color c) {
            Bitmap^ bmp = gcnew Bitmap(32, 32);
            Graphics^ g = Graphics::FromImage(bmp);
            g->Clear(c);
            g->DrawRectangle(Pens::Black, 0, 0, 31, 31);
            delete g;
            return bmp;
        }

        int IconIndexFor(Person* p) {
            if (dynamic_cast<Student*>(p) != nullptr) { return 0; }
            if (dynamic_cast<Teacher*>(p) != nullptr) { return 1; }
            return 2;
        }

        System::Void RefreshListView() {
            listView1->BeginUpdate();
            listView1->Items->Clear();
            int ageSum = 0;
            for (std::size_t i = 0; i < people_->size(); ++i) {
                Person* p = (*people_)[i];
                ListViewItem^ item = gcnew ListViewItem(
                    gcnew String(p->getFullName().c_str()), IconIndexFor(p));
                item->SubItems->Add(gcnew String(p->getRole().c_str()));
                item->SubItems->Add(p->getAge().ToString());
                item->SubItems->Add(gcnew String(p->getInfo().c_str()));
                if (p->getAge() < 18) { item->ForeColor = Color::Red; }           // ДЗ7 #4
                Teacher* t = dynamic_cast<Teacher*>(p);
                if (t != nullptr && t->getExperience() > 15) {                    // ДЗ7 #4
                    item->Font = gcnew System::Drawing::Font(listView1->Font, FontStyle::Bold);
                }
                listView1->Items->Add(item);
                ageSum += p->getAge();
            }
            int n = static_cast<int>(people_->size());
            double avg = (n > 0) ? (static_cast<double>(ageSum) / n) : 0.0;
            ListViewItem^ summary = gcnew ListViewItem(L"TOTAL", 3);             // ДЗ7 #5 (Варіант Б)
            summary->SubItems->Add(L"");
            summary->SubItems->Add(L"");
            summary->SubItems->Add(L"Count: " + n.ToString() + L"   Avg age: " + avg.ToString("F1"));
            summary->BackColor = Color::Gainsboro;
            summary->Font = gcnew System::Drawing::Font(listView1->Font, FontStyle::Bold);
            listView1->Items->Add(summary);
            listView1->EndUpdate();
        }

        // Перемалювати обидва подання після будь-якої зміни вектора.
        System::Void RefreshAll() { RefreshGrid(); RefreshListView(); }

        bool IsSummaryIndex(int idx) { return idx == static_cast<int>(people_->size()); }

        int SelectedVectorIndex() {
            if (listView1->SelectedIndices->Count == 0) { return -1; }
            int idx = listView1->SelectedIndices[0];
            if (idx < 0 || IsSummaryIndex(idx)) { return -1; }
            return idx;
        }

        System::Void rbView_CheckedChanged(Object^ sender, EventArgs^ e) {       // ДЗ7 #1
            listView1->View = rbLargeIcon->Checked
                ? System::Windows::Forms::View::LargeIcon
                : System::Windows::Forms::View::Details;
        }

        System::Void listView1_ColumnClick(Object^ sender, ColumnClickEventArgs^ e) {  // ДЗ7 #2
            std::sort(people_->begin(), people_->end(), LessBySurname);
            if (!sortAscending_) { std::reverse(people_->begin(), people_->end()); }
            sortAscending_ = !sortAscending_;
            RefreshAll();
        }

        System::Void listView1_MouseDown(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                ListViewHitTestInfo^ hit = listView1->HitTest(e->Location);
                if (hit->Item != nullptr) { hit->Item->Selected = true; listView1->Select(); }
            }
        }

        System::Void miEdit_Click(Object^ sender, EventArgs^ e) {                // ДЗ7 #3
            int idx = SelectedVectorIndex();
            if (idx < 0) { return; }
            Student* st = dynamic_cast<Student*>((*people_)[idx]);
            if (st == nullptr) {
                MessageBox::Show(L"The edit dialog is available for students only.", L"Edit",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            StudentEditForm^ dlg = gcnew StudentEditForm(
                gcnew String(st->getFirstName().c_str()),
                gcnew String(st->getMidName().c_str()),
                gcnew String(st->getLastName().c_str()),
                gcnew String(st->getGender().c_str()),
                st->getAge(),
                gcnew String(st->getGroup().c_str()),
                gcnew String(st->getHomePets().c_str()),
                gcnew String(st->getSport().c_str()));
            if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                std::string f   = marshal_as<std::string>(dlg->EditedFirst);
                std::string m   = marshal_as<std::string>(dlg->EditedMid);
                std::string l   = marshal_as<std::string>(dlg->EditedLast);
                std::string g   = marshal_as<std::string>(dlg->EditedGender);
                int         age = dlg->EditedAge;
                std::string gr  = marshal_as<std::string>(dlg->EditedGroup);
                std::string pet = marshal_as<std::string>(dlg->EditedPets);
                std::string sp  = marshal_as<std::string>(dlg->EditedSport);
                delete (*people_)[idx];
                (*people_)[idx] = new Student(f, m, l, g, age, gr, "College of IT", pet, sp);
                RefreshAll();
            }
        }

        System::Void miDelete_Click(Object^ sender, EventArgs^ e) {              // ДЗ7 #3
            int idx = SelectedVectorIndex();
            if (idx < 0) { return; }
            String^ name = gcnew String((*people_)[idx]->getFullName().c_str());
            if (MessageBox::Show(L"Delete \"" + name + L"\"?", L"Confirm delete",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) {
                return;
            }
            delete (*people_)[idx];
            people_->erase(people_->begin() + idx);
            RefreshAll();
        }

        System::Void miDetails_Click(Object^ sender, EventArgs^ e) {             // ДЗ7 #3
            int idx = SelectedVectorIndex();
            if (idx < 0) { return; }
            Person* p = (*people_)[idx];
            String^ text = gcnew String(p->getFormalGreeting().c_str())
                + Environment::NewLine + Environment::NewLine + gcnew String(p->getInfo().c_str());
            MessageBox::Show(text, L"Details", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        // ======================= Спільні демо-дані =======================
        void SeedSampleData() {
            people_->push_back(new Student("Maksym", "O.", "Hodyk", "Male", 19,
                "PI-21", "College of IT", "cat Barsyk", "none"));
            people_->push_back(new Student("Anna", "S.", "Bondarenko", "Female", 17,
                "PI-11", "College of IT", "dog Rex", "swimming"));
            people_->push_back(new Teacher("Olena", "I.", "Shevchenko", "Female", 44,
                "Mathematics", 20));
            people_->push_back(new Teacher("Petro", "V.", "Tkachenko", "Male", 33,
                "Physics", 8));
            people_->push_back(new Staff("Ihor", "P.", "Koval", "Male", 38,
                "Lab engineer", "IT department", "2015-09-01"));
        }
    };
}
