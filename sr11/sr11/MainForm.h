#pragma once

// СР11 (Тема 11). Інтеграція з MS Excel через COM Interop.
// Кнопка Export Excel створює книгу, заповнює аркуш зі std::vector<Person*>,
// форматує та зберігає. Виправлено «закладені» помилки:
//   #1 додано COM-посилання Microsoft.Office.Interop.Excel (у .vcxproj);
//   #2 rowIndex 1-based — дані з рядка 2 (рядок 1 = заголовок);
//   #3 звільнення COM-об'єктів у finally (Marshal::ReleaseComObject + GC),
//      інакше EXCEL.EXE лишається у фоні;
//   #4 автоформат: Bold-заголовок, Borders, AutoFit;
//   #5 перевірено експорт (у Диспетчері завдань немає EXCEL.EXE).
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

    // Псевдонім, щоб уникнути конфлікту Excel::Application з Forms::Application.
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
        Label^ lblStatus;

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
            btnExportExcel->Text = L"Export to Excel...";
            btnExportExcel->Location = Point(12, 274);
            btnExportExcel->Size = System::Drawing::Size(180, 32);
            btnExportExcel->Click += gcnew EventHandler(this, &MainForm::btnExportExcel_Click);

            lblStatus = gcnew Label();
            lblStatus->Location = Point(204, 282);
            lblStatus->AutoSize = true;
            lblStatus->Text = L"Ready.";

            this->Text = L"StudentPortfolio - Theme 11 (SR11: Excel COM Interop)";
            this->ClientSize = System::Drawing::Size(644, 320);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{ dataGridView1, btnExportExcel, lblStatus };
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

        System::Void btnExportExcel_Click(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"Excel Workbook|*.xlsx";
            dlg->FileName = L"StudentPortfolio.xlsx";
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }

            Excel::Application^ excelApp = nullptr;
            Excel::Workbook^   workbook  = nullptr;
            Excel::Worksheet^  worksheet = nullptr;
            try {
                excelApp = gcnew Excel::ApplicationClass();
                excelApp->Visible = false;        // приховуємо Excel під час експорту
                workbook  = excelApp->Workbooks->Add(Type::Missing);
                worksheet = dynamic_cast<Excel::Worksheet^>(workbook->ActiveSheet);

                // Рядок 1 — заголовок.
                worksheet->Cells[1, 1] = L"Full name";
                worksheet->Cells[1, 2] = L"Role";
                worksheet->Cells[1, 3] = L"Details";

                // СР11 #2: rowIndex 1-based, дані з рядка 2 (пропускаємо заголовок).
                int rowIndex = 1;
                for (Person* p : *people_) {
                    rowIndex++;
                    worksheet->Cells[rowIndex, 1] = gcnew String(p->getFullName().c_str());
                    worksheet->Cells[rowIndex, 2] = gcnew String(p->getRole().c_str());
                    worksheet->Cells[rowIndex, 3] = gcnew String(p->getInfo().c_str());
                }

                // СР11 #4: автоформат — Bold-заголовок, межі, автоширина.
                Excel::Range^ header = worksheet->Range[L"A1", L"C1"];
                header->Font->Bold = true;
                Excel::Range^ used = worksheet->UsedRange;
                used->Borders->LineStyle = Excel::XlLineStyle::xlContinuous;
                used->Columns->AutoFit();

                workbook->SaveCopyAs(dlg->FileName);
                lblStatus->Text = L"Exported " + people_->size().ToString() + L" rows.";
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Export error: " + ex->Message, L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally {
                // СР11 #3: звільнити COM-об'єкти, інакше EXCEL.EXE лишається у фоні.
                if (worksheet != nullptr) { Marshal::ReleaseComObject(worksheet); }
                if (workbook  != nullptr) { workbook->Close(false, Type::Missing, Type::Missing); Marshal::ReleaseComObject(workbook); }
                if (excelApp  != nullptr) { excelApp->Quit(); Marshal::ReleaseComObject(excelApp); }
                GC::Collect();
                GC::WaitForPendingFinalizers();
            }
        }
    };
}
