#pragma once

// Тема 11 — об'єднано СР11 + ДЗ11. Інтеграція з MS Excel через COM Interop.
// СР11 (основа експорту, маркери // СР11 #k у FillSheet/finally):
//   #1 COM-посилання Microsoft.Office.Interop.Excel; #2 rowIndex 1-based
//   (дані з рядка 2); #3 звільнення COM у finally (ReleaseComObject + GC);
//   #4 автоформат (Bold-заголовок, Borders, AutoFit).
// ДЗ11 (надбудова, маркери // ДЗ11 #k):
//   #1 повний цикл створення/заповнення/форматування/збереження;
//   #2 ProgressBar + Application::DoEvents(); #3 окремі аркуші за типом;
//   #4 перевірка наявності Excel; #5 Варіант А — діаграма за віком студентів.
// ⚠ Потребує встановленого MS Excel + PIA Microsoft.Office.Interop.Excel.

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
    using namespace System::Runtime::InteropServices;
    using namespace msclr::interop;

    namespace Excel = Microsoft::Office::Interop::Excel;

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
        Button^ btnExportExcel;
        StatusStrip^ statusStrip1;
        ToolStripStatusLabel^ lblStatus;
        ToolStripProgressBar^ progress_;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 12);
            dataGridView1->Size = System::Drawing::Size(620, 250);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            btnExportExcel = gcnew Button();
            btnExportExcel->Text = L"Export to Excel (sheets + chart)...";
            btnExportExcel->Location = Point(12, 274);
            btnExportExcel->Size = System::Drawing::Size(260, 32);
            // Індивідуальне оформлення (Марченко К.): зелений акцент.
            btnExportExcel->FlatStyle = FlatStyle::Flat;
            btnExportExcel->BackColor = Color::SeaGreen;
            btnExportExcel->ForeColor = Color::White;
            btnExportExcel->Click += gcnew EventHandler(this, &MainForm::btnExportExcel_Click);

            statusStrip1 = gcnew StatusStrip();
            lblStatus = gcnew ToolStripStatusLabel(L"Ready.");
            progress_ = gcnew ToolStripProgressBar();
            progress_->Visible = false;
            statusStrip1->Items->Add(lblStatus);
            statusStrip1->Items->Add(progress_);

            this->Text = L"StudentPortfolio - Theme 11 - Marchenko Kostiantyn";
            this->ClientSize = System::Drawing::Size(644, 330);
            this->BackColor = Color::FromArgb(232, 245, 233);   // індивідуальний фон (зелений відтінок)
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            this->Controls->Add(dataGridView1);
            this->Controls->Add(btnExportExcel);
            this->Controls->Add(statusStrip1);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void SeedSampleData() {
            for (int k = 0; k < 6; ++k) {
                Student* s = new Student("Student" + std::to_string(k), "O.", "Hodyk", "Male", 17 + k,
                    "PI-2" + std::to_string(k % 3), "College of IT", "cat", "none");
                s->setAverageGrade(3.0 + (k % 3) * 0.5);
                people_->push_back(s);
            }
            people_->push_back(new Teacher("Olena", "I.", "Shevchenko", "Female", 44, "Mathematics", 20));
            people_->push_back(new Teacher("Petro", "V.", "Tkachenko", "Male", 33, "Physics", 8));
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

        // ДЗ11 #4: чи встановлено Excel (зареєстровано COM ProgID).
        bool IsExcelAvailable() {
            return System::Type::GetTypeFromProgID(L"Excel.Application") != nullptr;
        }

        // Заповнити аркуш списком людей одного типу. Повертає к-сть рядків даних.
        int FillSheet(Excel::Worksheet^ ws, std::vector<Person*>& group, int& progressValue) {
            ws->Cells[1, 1] = L"Full name";
            ws->Cells[1, 2] = L"Age";
            ws->Cells[1, 3] = L"Details";

            int rowIndex = 1;                 // СР11 #2: 1-based; дані з рядка 2
            for (Person* p : group) {
                rowIndex++;
                ws->Cells[rowIndex, 1] = gcnew String(p->getFullName().c_str());
                ws->Cells[rowIndex, 2] = p->getAge();
                ws->Cells[rowIndex, 3] = gcnew String(p->getInfo().c_str());
                progressValue++;
                progress_->Value = Math::Min(progressValue, progress_->Maximum);
                Application::DoEvents();      // ДЗ11 #2
            }

            // СР11 #4: автоформат — Bold-заголовок, межі, автоширина.
            Excel::Range^ header = ws->Range[L"A1", L"C1"];
            header->Font->Bold = true;
            Excel::Range^ used = ws->UsedRange;
            used->Borders->LineStyle = Excel::XlLineStyle::xlContinuous;
            used->Columns->AutoFit();
            return rowIndex - 1;
        }

        // ДЗ11 #5 (Варіант А): стовпчикова діаграма за віком (стовпці A:B).
        void AddAgeChart(Excel::Worksheet^ ws, int dataRows) {
            if (dataRows < 1) { return; }
            Excel::ChartObjects^ charts = safe_cast<Excel::ChartObjects^>(ws->ChartObjects(Type::Missing));
            Excel::ChartObject^ chartObj = charts->Add(280, 10, 360, 220);
            Excel::Chart^ chart = chartObj->Chart;
            Excel::Range^ src = ws->Range[L"A1", L"B" + (dataRows + 1).ToString()];
            chart->SetSourceData(src, Type::Missing);
            chart->ChartType = Excel::XlChartType::xlColumnClustered;
            chart->HasTitle = true;
            chart->ChartTitle->Text = L"Students by age";
        }

        System::Void btnExportExcel_Click(System::Object^ sender, EventArgs^ e) {
            // ДЗ11 #4: перевірка наявності Excel ПЕРЕД запуском.
            if (!IsExcelAvailable()) {
                MessageBox::Show(L"Microsoft Excel is not installed on this PC.", L"Excel not found",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"Excel Workbook|*.xlsx";
            dlg->FileName = L"StudentPortfolio.xlsx";
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }

            // Розкласти людей за типом (ДЗ11 #3).
            std::vector<Person*> students, teachers, staff;
            for (Person* p : *people_) {
                if (dynamic_cast<Student*>(p) != nullptr)      { students.push_back(p); }
                else if (dynamic_cast<Teacher*>(p) != nullptr) { teachers.push_back(p); }
                else                                           { staff.push_back(p); }
            }

            progress_->Minimum = 0;
            progress_->Maximum = static_cast<int>(people_->size());
            progress_->Value = 0;
            progress_->Visible = true;
            lblStatus->Text = L"Exporting to Excel...";

            Excel::Application^ excelApp = nullptr;
            Excel::Workbook^   workbook  = nullptr;
            Excel::Worksheet^  wsStudents = nullptr;
            Excel::Worksheet^  wsTeachers = nullptr;
            Excel::Worksheet^  wsStaff    = nullptr;
            try {
                // ДЗ11 #1: повний цикл — створення книги.
                excelApp = gcnew Excel::ApplicationClass();
                excelApp->Visible = false;
                workbook = excelApp->Workbooks->Add(Type::Missing);

                // ДЗ11 #3: три аркуші за типом.
                wsStudents = safe_cast<Excel::Worksheet^>(workbook->ActiveSheet);
                wsStudents->Name = L"Students";
                wsTeachers = safe_cast<Excel::Worksheet^>(
                    workbook->Worksheets->Add(Type::Missing, wsStudents, Type::Missing, Type::Missing));
                wsTeachers->Name = L"Teachers";
                wsStaff = safe_cast<Excel::Worksheet^>(
                    workbook->Worksheets->Add(Type::Missing, wsTeachers, Type::Missing, Type::Missing));
                wsStaff->Name = L"Staff";

                int progressValue = 0;
                int studentRows = FillSheet(wsStudents, students, progressValue);
                FillSheet(wsTeachers, teachers, progressValue);
                FillSheet(wsStaff, staff, progressValue);

                AddAgeChart(wsStudents, studentRows);   // ДЗ11 #5 (Варіант А)

                workbook->SaveCopyAs(dlg->FileName);    // ДЗ11 #1: збереження
                lblStatus->Text = L"Exported " + people_->size().ToString() + L" rows to 3 sheets.";
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Export error: " + ex->Message, L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                lblStatus->Text = L"Export failed.";
            }
            finally {
                // СР11 #3: звільнення COM-об'єктів у finally (без завислого EXCEL.EXE).
                if (wsStudents != nullptr) { Marshal::ReleaseComObject(wsStudents); }
                if (wsTeachers != nullptr) { Marshal::ReleaseComObject(wsTeachers); }
                if (wsStaff    != nullptr) { Marshal::ReleaseComObject(wsStaff); }
                if (workbook   != nullptr) { workbook->Close(false, Type::Missing, Type::Missing); Marshal::ReleaseComObject(workbook); }
                if (excelApp   != nullptr) { excelApp->Quit(); Marshal::ReleaseComObject(excelApp); }
                GC::Collect();
                GC::WaitForPendingFinalizers();
                progress_->Visible = false;
            }
        }
    };
}
