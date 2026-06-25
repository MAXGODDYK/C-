#pragma once

// ДЗ7 (Тема 7). ListView поверх нативного std::vector<Person*>.
// 1) Режими View: Details / LargeIcon (перемикач RadioButton).
// 2) Сортування за прізвищем при кліку на заголовок (toggle asc/desc).
// 3) Контекстне меню (ПКМ): Edit / Delete / Details.
// 4) Умовне форматування: age < 18 -> червоний текст; стаж викладача > 15 -> жирний.
// 5) Варіант Б: підсумковий рядок — загальна кількість і середній вік.
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

    // Нативний компаратор за прізвищем (а далі — за іменем). Винесено на рівень
    // namespace, бо C++/CLI забороняє лямбди в методах керованого класу.
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
            RefreshList();
        }

    protected:
        ~MainForm() { if (components) { delete components; } CleanupNative(); }
        !MainForm() { CleanupNative(); }

    private:
        std::vector<Person*>* people_;
        bool sortAscending_;            // напрям сортування за прізвищем
        void CleanupNative() {
            if (people_) {
                for (Person* p : *people_) { delete p; }
                delete people_;
                people_ = nullptr;
            }
        }

        System::ComponentModel::Container^ components;
        ListView^ listView1;
        ImageList^ largeIcons_;
        RadioButton^ rbDetails;
        RadioButton^ rbLargeIcon;
        System::Windows::Forms::ContextMenuStrip^ contextMenu_;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            // --- ImageList для режиму LargeIcon (кольорові квадрати за роллю) ---
            largeIcons_ = gcnew ImageList();
            largeIcons_->ImageSize = System::Drawing::Size(32, 32);
            largeIcons_->Images->Add(MakeIcon(Color::SteelBlue));   // 0 = Student
            largeIcons_->Images->Add(MakeIcon(Color::SeaGreen));    // 1 = Teacher
            largeIcons_->Images->Add(MakeIcon(Color::DarkOrange));  // 2 = Staff
            largeIcons_->Images->Add(MakeIcon(Color::Gray));        // 3 = summary

            // --- Контекстне меню (ДЗ7 #3) ---
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

            // --- ListView ---
            listView1 = gcnew ListView();
            listView1->Location = Point(12, 50);
            listView1->Size = System::Drawing::Size(620, 320);
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

            // --- Перемикач режимів (ДЗ7 #1) ---
            rbDetails = gcnew RadioButton();
            rbDetails->Text = L"Details";
            rbDetails->Location = Point(12, 16);
            rbDetails->AutoSize = true;
            rbDetails->Checked = true;
            rbDetails->CheckedChanged += gcnew EventHandler(this, &MainForm::rbView_CheckedChanged);

            rbLargeIcon = gcnew RadioButton();
            rbLargeIcon->Text = L"Large icons";
            rbLargeIcon->Location = Point(110, 16);
            rbLargeIcon->AutoSize = true;
            rbLargeIcon->CheckedChanged += gcnew EventHandler(this, &MainForm::rbView_CheckedChanged);

            this->Text = L"StudentPortfolio - Theme 7 (HW7: ListView, sort, context menu)";
            this->ClientSize = System::Drawing::Size(644, 382);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                rbDetails, rbLargeIcon, listView1
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        // Згенерувати простий кольоровий значок 32x32 для LargeIcon-режиму.
        Bitmap^ MakeIcon(Color c) {
            Bitmap^ bmp = gcnew Bitmap(32, 32);
            Graphics^ g = Graphics::FromImage(bmp);
            g->Clear(c);
            g->DrawRectangle(Pens::Black, 0, 0, 31, 31);
            delete g;
            return bmp;
        }

        // Демо-дані: змішані типи, зокрема неповнолітній (червоний)
        // та викладач зі стажем > 15 (жирний).
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

        int IconIndexFor(Person* p) {
            if (dynamic_cast<Student*>(p) != nullptr) { return 0; }
            if (dynamic_cast<Teacher*>(p) != nullptr) { return 1; }
            return 2;
        }

        // СИНХРОНІЗАЦІЯ vector -> ListView (+ форматування та підсумковий рядок).
        System::Void RefreshList() {
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

                // ДЗ7 #4: умовне форматування.
                if (p->getAge() < 18) {
                    item->ForeColor = Color::Red;                 // неповнолітній -> червоний
                }
                Teacher* t = dynamic_cast<Teacher*>(p);
                if (t != nullptr && t->getExperience() > 15) {
                    item->Font = gcnew System::Drawing::Font(listView1->Font, FontStyle::Bold); // стаж>15 -> жирний
                }
                listView1->Items->Add(item);
                ageSum += p->getAge();
            }

            // ДЗ7 #5 (Варіант Б): підсумковий рядок — кількість і середній вік.
            int n = static_cast<int>(people_->size());
            double avg = (n > 0) ? (static_cast<double>(ageSum) / n) : 0.0;
            ListViewItem^ summary = gcnew ListViewItem(L"TOTAL", 3);
            summary->SubItems->Add(L"");
            summary->SubItems->Add(L"");
            summary->SubItems->Add(L"Count: " + n.ToString() + L"   Avg age: " + avg.ToString("F1"));
            summary->BackColor = Color::Gainsboro;
            summary->Font = gcnew System::Drawing::Font(listView1->Font, FontStyle::Bold);
            listView1->Items->Add(summary);

            listView1->EndUpdate();
        }

        // Чи є цей рядок підсумковим (останній, без backing Person*)?
        bool IsSummaryIndex(int idx) { return idx == static_cast<int>(people_->size()); }

        // Поточний обраний індекс у векторі, або -1 (включно з підсумковим рядком).
        int SelectedVectorIndex() {
            if (listView1->SelectedIndices->Count == 0) { return -1; }
            int idx = listView1->SelectedIndices[0];
            if (idx < 0 || IsSummaryIndex(idx)) { return -1; }
            return idx;
        }

        // ДЗ7 #1: перемикання режиму відображення.
        System::Void rbView_CheckedChanged(Object^ sender, EventArgs^ e) {
            listView1->View = rbLargeIcon->Checked
                ? System::Windows::Forms::View::LargeIcon
                : System::Windows::Forms::View::Details;
        }

        // ДЗ7 #2: сортування за прізвищем при кліку на заголовок (toggle напряму).
        System::Void listView1_ColumnClick(Object^ sender, ColumnClickEventArgs^ e) {
            std::sort(people_->begin(), people_->end(), LessBySurname);
            if (!sortAscending_) { std::reverse(people_->begin(), people_->end()); }
            sortAscending_ = !sortAscending_;
            RefreshList();
        }

        // ПКМ: виділяємо рядок під курсором, щоб контекстне меню діяло на нього.
        System::Void listView1_MouseDown(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                ListViewHitTestInfo^ hit = listView1->HitTest(e->Location);
                if (hit->Item != nullptr) { hit->Item->Selected = true; listView1->Select(); }
            }
        }

        // ДЗ7 #3: Edit — для Student відкриваємо StudentEditForm, інакше повідомлення.
        System::Void miEdit_Click(Object^ sender, EventArgs^ e) {
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
                RefreshList();
            }
        }

        // ДЗ7 #3: Delete — підтвердження і видалення з vector.
        System::Void miDelete_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedVectorIndex();
            if (idx < 0) { return; }
            String^ name = gcnew String((*people_)[idx]->getFullName().c_str());
            if (MessageBox::Show(L"Delete \"" + name + L"\"?", L"Confirm delete",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) {
                return;
            }
            delete (*people_)[idx];
            people_->erase(people_->begin() + idx);
            RefreshList();
        }

        // ДЗ7 #3: Details — повний getInfo() обраного об'єкта.
        System::Void miDetails_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedVectorIndex();
            if (idx < 0) { return; }
            Person* p = (*people_)[idx];
            String^ text = gcnew String(p->getFormalGreeting().c_str());
            text += Environment::NewLine + Environment::NewLine + gcnew String(p->getInfo().c_str());
            MessageBox::Show(text, L"Details", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
