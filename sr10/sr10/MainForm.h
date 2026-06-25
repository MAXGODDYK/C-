#pragma once

// СР10 (Тема 10). Багатосторінковий друк + коректний CSV-експорт.
// DataGridView показує std::vector<Person*>; кнопка Print відкриває
// PrintPreviewDialog, кнопка Export CSV пише екранований UTF-8 файл.
// Виправлено «закладені» помилки:
//   #1 індекс друку — ПОЛЕ форми printStartIndex_, що обнуляється лише в BeginPrint
//      (а не локальна змінна, що скидалась на кожній сторінці);
//   #2 перевірка межі сторінки: y+lineHeight>MarginBounds.Bottom -> HasMorePages=true;
//   #3 заголовок друкується поза циклом даних (на початку сторінки);
//   #4 CSV: екранування (подвоєння лапок, обгортання полів із комами) + UTF8;
//   #5 перевірено PrintPreview та відкриття CSV.

#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

#include "Student.h"
#include "Teacher.h"
#include "Staff.h"

namespace Portfolio {

    using namespace System;
    using namespace System::IO;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Drawing::Printing;
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

        System::ComponentModel::Container^ components;
        DataGridView^ dataGridView1;
        Button^ btnPrint;
        Button^ btnExportCsv;
        PrintDocument^ printDocument1;

        // СР10 #1: індекс поточного рядка друку — ПОЛЕ форми (зберігається між
        // викликами PrintPage), обнуляється виключно у BeginPrint.
        int printStartIndex_;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 12);
            dataGridView1->Size = System::Drawing::Size(620, 250);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            btnPrint = gcnew Button();
            btnPrint->Text = L"Print preview...";
            btnPrint->Location = Point(12, 274);
            btnPrint->Size = System::Drawing::Size(160, 32);
            btnPrint->Click += gcnew EventHandler(this, &MainForm::btnPrint_Click);

            btnExportCsv = gcnew Button();
            btnExportCsv->Text = L"Export CSV...";
            btnExportCsv->Location = Point(184, 274);
            btnExportCsv->Size = System::Drawing::Size(160, 32);
            btnExportCsv->Click += gcnew EventHandler(this, &MainForm::btnExportCsv_Click);

            // PrintDocument + підписки.
            printDocument1 = gcnew PrintDocument();
            printDocument1->DocumentName = L"StudentPortfolio list";
            printDocument1->BeginPrint += gcnew PrintEventHandler(this, &MainForm::printDocument1_BeginPrint);
            printDocument1->PrintPage  += gcnew PrintPageEventHandler(this, &MainForm::printDocument1_PrintPage);

            this->Text = L"StudentPortfolio - Theme 10 (SR10: printing + CSV)";
            this->ClientSize = System::Drawing::Size(644, 320);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{ dataGridView1, btnPrint, btnExportCsv };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void SeedSampleData() {
            // Достатньо рядків, щоб друк зайняв більше однієї сторінки.
            for (int k = 0; k < 12; ++k) {
                Student* s = new Student("Maksym" + std::to_string(k), "O.", "Hodyk", "Male", 18 + (k % 5),
                    "PI-2" + std::to_string(k % 3), "College of IT", "cat", "none");
                s->setAverageGrade(3.0 + (k % 3) * 0.5);
                people_->push_back(s);
            }
            people_->push_back(new Teacher("Olena", "I.", "Shevchenko", "Female", 44, "Mathematics", 20));
            people_->push_back(new Staff("Ihor", "P.", "Koval", "Male", 38, "Lab engineer", "IT department", "2015-09-01"));
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

        // ===================== ДРУК =====================
        System::Void printDocument1_BeginPrint(System::Object^ sender, PrintEventArgs^ e) {
            printStartIndex_ = 0;   // СР10 #1: обнуляємо саме тут, раз на завдання друку
        }

        System::Void printDocument1_PrintPage(System::Object^ sender, PrintPageEventArgs^ e) {
            System::Drawing::Font^ headerFont = gcnew System::Drawing::Font(L"Arial", 12, FontStyle::Bold);
            System::Drawing::Font^ dataFont   = gcnew System::Drawing::Font(L"Arial", 10);
            float x = static_cast<float>(e->MarginBounds.Left);
            float y = static_cast<float>(e->MarginBounds.Top);

            // СР10 #3: заголовок — ПОЗА циклом даних, на початку кожної сторінки.
            e->Graphics->DrawString(L"STUDENT PORTFOLIO - LIST", headerFont, Brushes::Black, x, y);
            y += headerFont->GetHeight(e->Graphics) + 6.0f;

            float lineHeight = dataFont->GetHeight(e->Graphics);
            while (printStartIndex_ < static_cast<int>(people_->size())) {
                // СР10 #2: якщо наступний рядок виходить за нижню межу — нова сторінка.
                if (y + lineHeight > static_cast<float>(e->MarginBounds.Bottom)) {
                    e->HasMorePages = true;
                    return;            // printStartIndex_ лишається на поточному рядку
                }
                Person* p = (*people_)[printStartIndex_];
                String^ line = gcnew String(
                    (p->getFullName() + "  |  " + p->getRole() + "  |  " + p->getInfo()).c_str());
                e->Graphics->DrawString(line, dataFont, Brushes::Black, x, y);
                y += lineHeight;
                printStartIndex_++;
            }
            e->HasMorePages = false;   // усі рядки надруковано
        }

        System::Void btnPrint_Click(System::Object^ sender, EventArgs^ e) {
            PrintPreviewDialog^ preview = gcnew PrintPreviewDialog();
            preview->Document = printDocument1;
            preview->ShowDialog();      // СР10 #5
        }

        // ===================== CSV =====================
        // СР10 #4: екранування — обгортання в лапки та подвоєння внутрішніх лапок.
        String^ CsvEscape(String^ field) {
            if (field == nullptr) { return L""; }
            if (field->Contains(L",") || field->Contains(L"\"") || field->Contains(L"\n")) {
                return L"\"" + field->Replace(L"\"", L"\"\"") + L"\"";
            }
            return field;
        }

        System::Void btnExportCsv_Click(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"CSV Files|*.csv|All Files|*.*";
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }

            System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
            sb->AppendLine(L"Role,FullName,Details");
            for (Person* p : *people_) {
                sb->AppendLine(String::Join(L",", gcnew array<String^>{
                    CsvEscape(gcnew String(p->getRole().c_str())),
                    CsvEscape(gcnew String(p->getFullName().c_str())),
                    CsvEscape(gcnew String(p->getInfo().c_str()))
                }));
            }
            // СР10 #4: явно вказуємо UTF8, інакше кирилиця -> «кракозябри».
            File::WriteAllText(dlg->FileName, sb->ToString(), System::Text::Encoding::UTF8);
            MessageBox::Show(L"Exported " + people_->size().ToString() + L" rows.", L"CSV",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
