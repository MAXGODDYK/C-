#pragma once

// ДЗ10 (Тема 10). Розвиток друку (номери сторінок) та HTML-звіт.
//   #1 Колонтитул-пагінація: поле pageNumber_ -> «Page X» унизу праворуч кожної
//      сторінки; інкрементується щосторінки, скидається в 1 у BeginPrint.
//   #2 HTML-звіт із числовим полем варіанта (GPA — рейтинговий бал студента).
//   #3 Підсумковий рядок <tfoot>: середній GPA по студентах (агрегація).
//   #4 StatusStrip із ToolStripProgressBar під час експорту великого списку.
// Базується на СР10 (printStartIndex_, перевірка межі сторінки, CSV-екранування).

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
        Button^ btnExportHtml;
        StatusStrip^ statusStrip1;
        ToolStripStatusLabel^ lblStatus;
        ToolStripProgressBar^ progress_;
        PrintDocument^ printDocument1;

        int printStartIndex_;   // СР10: індекс рядка друку (скид у BeginPrint)
        int pageNumber_;        // ДЗ10 #1: номер поточної сторінки (скид у BeginPrint)

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
            btnExportCsv->Size = System::Drawing::Size(150, 32);
            btnExportCsv->Click += gcnew EventHandler(this, &MainForm::btnExportCsv_Click);

            btnExportHtml = gcnew Button();
            btnExportHtml->Text = L"Export HTML...";
            btnExportHtml->Location = Point(346, 274);
            btnExportHtml->Size = System::Drawing::Size(150, 32);
            btnExportHtml->Click += gcnew EventHandler(this, &MainForm::btnExportHtml_Click);

            // ДЗ10 #4: StatusStrip із прогрес-баром.
            statusStrip1 = gcnew StatusStrip();
            lblStatus = gcnew ToolStripStatusLabel(L"Ready.");
            progress_ = gcnew ToolStripProgressBar();
            progress_->Visible = false;
            statusStrip1->Items->Add(lblStatus);
            statusStrip1->Items->Add(progress_);

            printDocument1 = gcnew PrintDocument();
            printDocument1->DocumentName = L"StudentPortfolio list";
            printDocument1->BeginPrint += gcnew PrintEventHandler(this, &MainForm::printDocument1_BeginPrint);
            printDocument1->PrintPage  += gcnew PrintPageEventHandler(this, &MainForm::printDocument1_PrintPage);

            this->Text = L"StudentPortfolio - Theme 10 (HW10: page numbers + HTML report)";
            this->ClientSize = System::Drawing::Size(644, 340);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            this->Controls->Add(dataGridView1);
            this->Controls->Add(btnPrint);
            this->Controls->Add(btnExportCsv);
            this->Controls->Add(btnExportHtml);
            this->Controls->Add(statusStrip1);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void SeedSampleData() {
            for (int k = 0; k < 12; ++k) {
                Student* s = new Student("Maksym" + std::to_string(k), "O.", "Hodyk", "Male", 18 + (k % 5),
                    "PI-2" + std::to_string(k % 3), "College of IT", "cat", "none");
                s->setAverageGrade(3.0 + (k % 3) * 0.5);   // рейтинговий бал
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
            printStartIndex_ = 0;
            pageNumber_ = 1;     // ДЗ10 #1: нумерація сторінок з 1
        }

        // ДЗ10 #1: підвал «Page X» унизу праворуч поточної сторінки.
        void DrawPageFooter(PrintPageEventArgs^ e) {
            System::Drawing::Font^ footFont = gcnew System::Drawing::Font(L"Arial", 9, FontStyle::Italic);
            String^ text = L"Page " + pageNumber_.ToString();
            SizeF sz = e->Graphics->MeasureString(text, footFont);
            float fx = static_cast<float>(e->MarginBounds.Right) - sz.Width;
            float fy = static_cast<float>(e->MarginBounds.Bottom) - sz.Height;
            e->Graphics->DrawString(text, footFont, Brushes::Gray, fx, fy);
        }

        System::Void printDocument1_PrintPage(System::Object^ sender, PrintPageEventArgs^ e) {
            System::Drawing::Font^ headerFont = gcnew System::Drawing::Font(L"Arial", 12, FontStyle::Bold);
            System::Drawing::Font^ dataFont   = gcnew System::Drawing::Font(L"Arial", 10);
            float x = static_cast<float>(e->MarginBounds.Left);
            float y = static_cast<float>(e->MarginBounds.Top);

            e->Graphics->DrawString(L"STUDENT PORTFOLIO - LIST", headerFont, Brushes::Black, x, y);
            y += headerFont->GetHeight(e->Graphics) + 6.0f;

            float lineHeight = dataFont->GetHeight(e->Graphics);
            // Залишаємо місце знизу під номер сторінки.
            float bottomLimit = static_cast<float>(e->MarginBounds.Bottom) - lineHeight - 4.0f;

            while (printStartIndex_ < static_cast<int>(people_->size())) {
                if (y + lineHeight > bottomLimit) {
                    DrawPageFooter(e);          // ДЗ10 #1: номер на поточній сторінці
                    pageNumber_++;              // наступна сторінка отримає +1
                    e->HasMorePages = true;
                    return;
                }
                Person* p = (*people_)[printStartIndex_];
                String^ line = gcnew String(
                    (p->getFullName() + "  |  " + p->getRole() + "  |  " + p->getInfo()).c_str());
                e->Graphics->DrawString(line, dataFont, Brushes::Black, x, y);
                y += lineHeight;
                printStartIndex_++;
            }
            DrawPageFooter(e);                  // підвал останньої сторінки
            e->HasMorePages = false;
        }

        System::Void btnPrint_Click(System::Object^ sender, EventArgs^ e) {
            PrintPreviewDialog^ preview = gcnew PrintPreviewDialog();
            preview->Document = printDocument1;
            preview->ShowDialog();
        }

        // ===================== CSV =====================
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
            File::WriteAllText(dlg->FileName, sb->ToString(), System::Text::Encoding::UTF8);
            lblStatus->Text = L"CSV exported.";
        }

        // ===================== HTML-звіт =====================
        String^ HtmlEscape(String^ s) {
            if (s == nullptr) { return L""; }
            return s->Replace(L"&", L"&amp;")->Replace(L"<", L"&lt;")->Replace(L">", L"&gt;");
        }

        System::Void btnExportHtml_Click(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"HTML Files|*.html|All Files|*.*";
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }

            // ДЗ10 #4: показуємо прогрес під час експорту.
            progress_->Minimum = 0;
            progress_->Maximum = static_cast<int>(people_->size());
            progress_->Value = 0;
            progress_->Visible = true;
            lblStatus->Text = L"Exporting HTML...";

            System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
            sb->AppendLine(L"<!DOCTYPE html>");
            sb->AppendLine(L"<html><head><meta charset=\"utf-8\"><title>Rating report</title>");
            sb->AppendLine(L"<style>table{border-collapse:collapse}th,td{border:1px solid #888;padding:4px 8px}"
                           L"tfoot td{font-weight:bold;background:#eee}</style></head><body>");
            sb->AppendLine(L"<h2>Student Portfolio - rating report (GPA)</h2>");
            sb->AppendLine(L"<table><thead><tr><th>Surname</th><th>First name</th><th>Role</th>"
                           L"<th>Age</th><th>GPA</th></tr></thead><tbody>");

            double gpaSum = 0.0; int studentCount = 0;
            for (Person* p : *people_) {
                // ДЗ10 #2: числове поле варіанта — GPA (лише для студентів).
                String^ gpaCell = L"-";
                Student* st = dynamic_cast<Student*>(p);
                if (st != nullptr) {
                    double gpa = st->getAverageGrade();
                    gpaSum += gpa; ++studentCount;
                    gpaCell = gpa.ToString("F2");
                }
                sb->AppendLine(L"<tr><td>" + HtmlEscape(gcnew String(p->getLastName().c_str())) +
                               L"</td><td>" + HtmlEscape(gcnew String(p->getFirstName().c_str())) +
                               L"</td><td>" + HtmlEscape(gcnew String(p->getRole().c_str())) +
                               L"</td><td>" + p->getAge().ToString() +
                               L"</td><td>" + gpaCell + L"</td></tr>");
                progress_->Value = Math::Min(progress_->Value + 1, progress_->Maximum);
                Application::DoEvents();
            }

            // ДЗ10 #3: підсумковий рядок <tfoot> — середній GPA по студентах.
            double avg = (studentCount > 0) ? (gpaSum / studentCount) : 0.0;
            sb->AppendLine(L"</tbody><tfoot><tr><td colspan=\"4\">Average GPA over " +
                           studentCount.ToString() + L" student(s)</td><td>" + avg.ToString("F2") +
                           L"</td></tr></tfoot></table></body></html>");

            File::WriteAllText(dlg->FileName, sb->ToString(), System::Text::Encoding::UTF8);

            progress_->Visible = false;
            lblStatus->Text = L"HTML exported (avg GPA " + avg.ToString("F2") + L").";
        }
    };
}
