#pragma once

// СР7 (Тема 7). DataGridView, синхронізований із нативним std::vector<Person*>.
// Таблиця показує ПІБ / Роль / Деталі для змішаного списку (Student/Teacher/Staff).
// Виправлено 4 «закладені» помилки із завдання:
//   #1 AllowUserToAddRows = false (порожній UI-рядок ламав синхронізацію з vector);
//   #2 коректний виклик getInfo() для колонки «Деталі»;
//   #3 перевірка SelectedRows->Count перед доступом до SelectedRows[0];
//   #4 умовне фарбування: рядок Student -> фон LightBlue.
// Інтерфейс керований (gcnew, ^), бізнес-логіка нативна (Person*, std::vector).

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
            RefreshGrid();
        }

    protected:
        ~MainForm() { if (components) { delete components; } CleanupNative(); }
        !MainForm() { CleanupNative(); }

    private:
        // Нативний поліморфний список — пам'яттю керуємо вручну.
        std::vector<Person*>* people_;
        void CleanupNative() {
            if (people_) {
                for (Person* p : *people_) { delete p; }
                delete people_;
                people_ = nullptr;
            }
        }

        System::ComponentModel::Container^ components;
        DataGridView^ dataGridView1;
        Label^ lblDetailsCaption;
        Label^ lblDetails;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 12);
            dataGridView1->Size = System::Drawing::Size(620, 250);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->MultiSelect = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dataGridView1->SelectionChanged += gcnew EventHandler(this, &MainForm::dataGridView1_SelectionChanged);

            lblDetailsCaption = gcnew Label();
            lblDetailsCaption->Location = Point(12, 272);
            lblDetailsCaption->AutoSize = true;
            lblDetailsCaption->Text = L"Details of selected:";

            lblDetails = gcnew Label();
            lblDetails->Location = Point(12, 294);
            lblDetails->Size = System::Drawing::Size(620, 70);
            lblDetails->BorderStyle = BorderStyle::FixedSingle;
            lblDetails->TextAlign = ContentAlignment::TopLeft;
            lblDetails->Text = L"Select a row to see details.";

            this->Text = L"StudentPortfolio - Theme 7 (SR7: DataGridView sync)";
            this->ClientSize = System::Drawing::Size(644, 376);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                dataGridView1, lblDetailsCaption, lblDetails
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        // Демо-дані: по одному об'єкту кожного типу (видно поліморфізм і фарбування).
        void SeedSampleData() {
            people_->push_back(new Student("Maksym", "O.", "Hodyk", "Male", 19,
                "PI-21", "College of IT", "cat Barsyk", "none"));
            people_->push_back(new Teacher("Olena", "I.", "Shevchenko", "Female", 44,
                "Mathematics", 20));
            people_->push_back(new Staff("Ihor", "P.", "Koval", "Male", 38,
                "Lab engineer", "IT department", "2015-09-01"));
        }

        // Налаштування колонок таблиці.
        System::Void InitializeGrid() {
            dataGridView1->Columns->Clear();
            dataGridView1->Columns->Add(L"colName",   L"Full name");
            dataGridView1->Columns->Add(L"colType",   L"Role");
            dataGridView1->Columns->Add(L"colDetail", L"Details");
            dataGridView1->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            // СР7 #1: забороняємо додавання рядків із UI, інакше з'являється
            // порожній «новий рядок», який не відповідає жодному Person* у vector.
            dataGridView1->AllowUserToAddRows = false;
        }

        // СИНХРОНІЗАЦІЯ vector -> UI. Викликати після будь-якої зміни people_.
        System::Void RefreshGrid() {
            dataGridView1->Rows->Clear();
            for (std::size_t i = 0; i < people_->size(); ++i) {
                Person* p = (*people_)[i];
                String^ name = gcnew String(p->getFullName().c_str());
                String^ type = gcnew String(p->getRole().c_str());
                // СР7 #2: для деталей беремо повний getInfo() конкретного типу.
                String^ detail = gcnew String(p->getInfo().c_str());

                int rowIndex = dataGridView1->Rows->Add(name, type, detail);

                // СР7 #4: умовне фарбування — рядок Student робимо світло-блакитним.
                if (dynamic_cast<Student*>(p) != nullptr) {
                    dataGridView1->Rows[rowIndex]->DefaultCellStyle->BackColor = Color::LightBlue;
                }
            }
            dataGridView1->ClearSelection();
            lblDetails->Text = L"Select a row to see details.";
        }

        // ОБРОБКА ВИДІЛЕННЯ РЯДКА -> показ деталей у lblDetails.
        System::Void dataGridView1_SelectionChanged(System::Object^ sender, EventArgs^ e) {
            // СР7 #3: без цієї перевірки доступ до SelectedRows[0] кидав би виняток
            // при порожньому виборі (наприклад, після ClearSelection()).
            if (dataGridView1->SelectedRows->Count == 0) {
                lblDetails->Text = L"Select a row to see details.";
                return;
            }
            int idx = dataGridView1->SelectedRows[0]->Index;
            if (idx < 0 || idx >= static_cast<int>(people_->size())) { return; }

            Person* selected = (*people_)[idx];
            String^ greeting = gcnew String(selected->getFormalGreeting().c_str());
            String^ info     = gcnew String(selected->getInfo().c_str());
            lblDetails->Text = greeting + Environment::NewLine + info;
        }
    };
}
