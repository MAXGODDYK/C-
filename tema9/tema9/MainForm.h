#pragma once

// Тема 9 — об'єднано СР9 + ДЗ9. Розширені елементи інтерфейсу та навігація.
// СР9 (маркери // СР9 #k): TabControl як фільтр (All/Students/Teachers/Staff)
//   з підпискою SelectedIndexChanged (#1), порівняння == (#2), очищення
//   ErrorProvider при успіху (#3), FilterByType() через dynamic_cast (#4).
// ДЗ9 (маркери // ДЗ9 #k): MenuStrip File/Edit (#1), ToolStrip зі спільними
//   обробниками (#2), ContextMenuStrip на таблиці (#3), валідація ПІБ через
//   ErrorProvider (#4), StatusStrip — час збереження + к-сть виділених (#5).
// Під фільтром рядок таблиці != індекс у векторі, тож тримаємо мапу shownIndex_.

#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include "StudentEditForm.h"

namespace Portfolio {

    using namespace System;
    using namespace System::IO;
    using namespace System::Collections::Generic;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class MainForm : public Form {
    public:
        MainForm() {
            people_ = new std::vector<Person*>();
            shownIndex_ = gcnew List<int>();
            currentFilter_ = L"All";
            InitializeComponent();
            SeedSampleData();
            InitializeGrid();
            SetupNavigation();
            FilterByType(L"All");
            UpdateSelectedCount();
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

        List<int>^ shownIndex_;       // рядок таблиці -> індекс у векторі (під фільтром)
        String^ currentFilter_;

        System::ComponentModel::Container^ components;
        MenuStrip^ menuStrip1;
        ToolStrip^ toolStrip1;
        TabControl^ tabControl1;
        StatusStrip^ statusStrip1;
        ToolStripStatusLabel^ lblSaved;
        ToolStripStatusLabel^ lblSelected;
        DataGridView^ dataGridView1;
        System::Windows::Forms::ContextMenuStrip^ gridContextMenu_;
        ErrorProvider^ errorProvider1;

        Label^ lblFirst; TextBox^ txtFirst;
        Label^ lblMid;   TextBox^ txtMid;
        Label^ lblLast;  TextBox^ txtLast;
        Label^ lblAge;   TextBox^ txtAge;
        Label^ lblGender; ComboBox^ cbGender;
        Label^ lblGroup; TextBox^ txtGroup;
        Label^ lblPets;  TextBox^ txtPets;

        Label^ MakeLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label(); l->Text = text; l->Location = Point(x, y + 3); l->AutoSize = true; return l;
        }
        TextBox^ MakeBox(int x, int y, int w) {
            TextBox^ b = gcnew TextBox(); b->Location = Point(x, y); b->Size = System::Drawing::Size(w, 23); return b;
        }
        Bitmap^ MakeIcon(Color c) {
            Bitmap^ bmp = gcnew Bitmap(16, 16);
            Graphics^ g = Graphics::FromImage(bmp);
            g->Clear(c); g->DrawRectangle(Pens::Black, 0, 0, 15, 15);
            delete g; return bmp;
        }

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            // ---- ДЗ9 #1: MenuStrip ----
            menuStrip1 = gcnew MenuStrip();
            ToolStripMenuItem^ miFile = gcnew ToolStripMenuItem(L"File");
            ToolStripMenuItem^ miFileSave = gcnew ToolStripMenuItem(L"Save");
            ToolStripMenuItem^ miFileExit = gcnew ToolStripMenuItem(L"Exit");
            miFileSave->Click += gcnew EventHandler(this, &MainForm::OnSave);
            miFileExit->Click += gcnew EventHandler(this, &MainForm::OnExit);
            miFile->DropDownItems->Add(miFileSave);
            miFile->DropDownItems->Add(miFileExit);
            ToolStripMenuItem^ miEdit = gcnew ToolStripMenuItem(L"Edit");
            ToolStripMenuItem^ miEditAdd = gcnew ToolStripMenuItem(L"Add");
            ToolStripMenuItem^ miEditDel = gcnew ToolStripMenuItem(L"Delete");
            miEditAdd->Click += gcnew EventHandler(this, &MainForm::OnAdd);
            miEditDel->Click += gcnew EventHandler(this, &MainForm::OnDelete);
            miEdit->DropDownItems->Add(miEditAdd);
            miEdit->DropDownItems->Add(miEditDel);
            menuStrip1->Items->Add(miFile);
            menuStrip1->Items->Add(miEdit);
            this->MainMenuStrip = menuStrip1;

            // ---- ДЗ9 #2: ToolStrip (спільні обробники) ----
            toolStrip1 = gcnew ToolStrip();
            ToolStripButton^ tbSave = gcnew ToolStripButton(L"Save", MakeIcon(Color::SteelBlue));
            ToolStripButton^ tbAdd  = gcnew ToolStripButton(L"Add",  MakeIcon(Color::SeaGreen));
            ToolStripButton^ tbDel  = gcnew ToolStripButton(L"Delete", MakeIcon(Color::IndianRed));
            tbSave->Click += gcnew EventHandler(this, &MainForm::OnSave);
            tbAdd->Click  += gcnew EventHandler(this, &MainForm::OnAdd);
            tbDel->Click  += gcnew EventHandler(this, &MainForm::OnDelete);
            toolStrip1->Items->Add(tbSave);
            toolStrip1->Items->Add(tbAdd);
            toolStrip1->Items->Add(tbDel);

            // ---- СР9: TabControl-фільтр ----
            tabControl1 = gcnew TabControl();
            tabControl1->Location = Point(12, 60);
            tabControl1->Size = System::Drawing::Size(620, 26);
            tabControl1->TabPages->Add(gcnew TabPage(L"All"));
            tabControl1->TabPages->Add(gcnew TabPage(L"Students"));
            tabControl1->TabPages->Add(gcnew TabPage(L"Teachers"));
            tabControl1->TabPages->Add(gcnew TabPage(L"Staff"));

            // ---- ДЗ9 #3: ContextMenuStrip ----
            gridContextMenu_ = gcnew System::Windows::Forms::ContextMenuStrip();
            ToolStripMenuItem^ cmEdit = gcnew ToolStripMenuItem(L"Edit");
            ToolStripMenuItem^ cmDel  = gcnew ToolStripMenuItem(L"Delete");
            cmEdit->Click += gcnew EventHandler(this, &MainForm::OnEdit);
            cmDel->Click  += gcnew EventHandler(this, &MainForm::OnDelete);
            gridContextMenu_->Items->Add(cmEdit);
            gridContextMenu_->Items->Add(cmDel);

            // ---- DataGridView ----
            dataGridView1 = gcnew DataGridView();
            dataGridView1->Location = Point(12, 92);
            dataGridView1->Size = System::Drawing::Size(620, 180);
            dataGridView1->ReadOnly = true;
            dataGridView1->RowHeadersVisible = false;
            dataGridView1->MultiSelect = true;
            dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dataGridView1->ContextMenuStrip = gridContextMenu_;
            dataGridView1->SelectionChanged += gcnew EventHandler(this, &MainForm::grid_SelectionChanged);
            dataGridView1->MouseDown += gcnew MouseEventHandler(this, &MainForm::grid_MouseDown);

            // ---- Поля вводу ----
            const int LX = 12, FX = 90, RH = 30; int y = 284;
            lblFirst = MakeLabel(L"First:", LX, y); txtFirst = MakeBox(FX, y, 150);
            lblLast  = MakeLabel(L"Surname:", 250, y); txtLast = MakeBox(330, y, 150);
            lblMid   = MakeLabel(L"Mid:", 490, y); txtMid = MakeBox(540, y, 90); y += RH;
            lblGender = MakeLabel(L"Gender:", LX, y);
            cbGender = gcnew ComboBox(); cbGender->Location = Point(FX, y); cbGender->Size = System::Drawing::Size(150, 23);
            cbGender->DropDownStyle = ComboBoxStyle::DropDownList;
            cbGender->Items->Add(L"Male"); cbGender->Items->Add(L"Female"); cbGender->SelectedIndex = 0;
            lblAge   = MakeLabel(L"Age:", 250, y); txtAge = MakeBox(330, y, 150); y += RH;
            lblGroup = MakeLabel(L"Group:", LX, y); txtGroup = MakeBox(FX, y, 150);
            lblPets  = MakeLabel(L"Pets:", 250, y); txtPets = MakeBox(330, y, 150);

            txtFirst->Text = L"Maksym"; txtLast->Text = L"Hodyk"; txtMid->Text = L"O.";
            txtAge->Text = L"19"; txtGroup->Text = L"PI-21"; txtPets->Text = L"cat Barsyk";

            // ---- ДЗ9 #4 / СР9 #3: ErrorProvider ----
            errorProvider1 = gcnew ErrorProvider();
            errorProvider1->BlinkStyle = ErrorBlinkStyle::NeverBlink;
            txtFirst->Validating += gcnew CancelEventHandler(this, &MainForm::name_Validating);
            txtLast->Validating  += gcnew CancelEventHandler(this, &MainForm::name_Validating);
            txtAge->Validating   += gcnew CancelEventHandler(this, &MainForm::txtAge_Validating);

            // ---- ДЗ9 #5 (Варіант Б): StatusStrip ----
            statusStrip1 = gcnew StatusStrip();
            lblSaved = gcnew ToolStripStatusLabel(L"Not saved yet");
            lblSelected = gcnew ToolStripStatusLabel(L"Selected: 0");
            lblSelected->Spring = true;
            lblSelected->TextAlign = ContentAlignment::MiddleRight;
            statusStrip1->Items->Add(lblSaved);
            statusStrip1->Items->Add(lblSelected);

            this->Text = L"StudentPortfolio - Theme 9 (combined SR9 + HW9)";
            this->ClientSize = System::Drawing::Size(644, 450);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            this->Controls->Add(tabControl1);
            this->Controls->Add(dataGridView1);
            array<Control^>^ inputs = gcnew array<Control^>{
                lblFirst, txtFirst, lblMid, txtMid, lblLast, txtLast,
                lblGender, cbGender, lblAge, txtAge, lblGroup, txtGroup, lblPets, txtPets
            };
            this->Controls->AddRange(inputs);
            this->Controls->Add(statusStrip1);
            this->Controls->Add(toolStrip1);
            this->Controls->Add(menuStrip1);

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

        System::Void SetupNavigation() {
            // СР9 #1: підписка на зміну активної вкладки.
            tabControl1->SelectedIndexChanged +=
                gcnew EventHandler(this, &MainForm::tabControl1_SelectedIndexChanged);
            errorProvider1->Icon = SystemIcons::Warning;
        }

        System::Void tabControl1_SelectedIndexChanged(System::Object^ sender, EventArgs^ e) {
            int idx = tabControl1->SelectedIndex;
            // СР9 #2: порівняння (==), а не присвоєння (=).
            if (idx == 0)      { FilterByType(L"All"); }
            else if (idx == 1) { FilterByType(L"Student"); }
            else if (idx == 2) { FilterByType(L"Teacher"); }
            else if (idx == 3) { FilterByType(L"Staff"); }
        }

        // СР9 #4: фільтрація переходом по vector + dynamic_cast; будуємо мапу рядків.
        System::Void FilterByType(String^ role) {
            currentFilter_ = role;
            dataGridView1->Rows->Clear();
            shownIndex_->Clear();
            for (std::size_t i = 0; i < people_->size(); ++i) {
                Person* p = (*people_)[i];
                bool show =
                    (role == L"All") ||
                    (role == L"Student" && dynamic_cast<Student*>(p) != nullptr) ||
                    (role == L"Teacher" && dynamic_cast<Teacher*>(p) != nullptr) ||
                    (role == L"Staff"   && dynamic_cast<Staff*>(p)   != nullptr);
                if (show) {
                    dataGridView1->Rows->Add(
                        gcnew String(p->getFullName().c_str()),
                        gcnew String(p->getRole().c_str()),
                        gcnew String(p->getInfo().c_str()));
                    shownIndex_->Add(static_cast<int>(i));
                }
            }
            dataGridView1->ClearSelection();
        }

        // ---- Валідація ----
        bool IsValidName(String^ s) {
            if (String::IsNullOrWhiteSpace(s)) { return false; }
            for each (Char ch in s) {
                if (!Char::IsLetter(ch) && ch != L' ' && ch != L'-') { return false; }
            }
            return true;
        }
        System::Void name_Validating(System::Object^ sender, CancelEventArgs^ e) {
            TextBox^ box = safe_cast<TextBox^>(sender);
            errorProvider1->SetError(box, IsValidName(box->Text) ? L"" : L"Only letters, spaces and hyphen; not empty.");
        }
        System::Void txtAge_Validating(System::Object^ sender, CancelEventArgs^ e) {
            int age;
            if (!Int32::TryParse(txtAge->Text, age) || age < 16 || age > 100) {
                errorProvider1->SetError(txtAge, L"Age must be 16-100");
                e->Cancel = true;
            } else {
                errorProvider1->SetError(txtAge, L"");   // СР9 #3
            }
        }

        // Поточний індекс у векторі через мапу shownIndex_.
        int SelectedVectorIndex() {
            if (dataGridView1->SelectedRows->Count == 0) { return -1; }
            int row = dataGridView1->SelectedRows[0]->Index;
            if (row < 0 || row >= shownIndex_->Count) { return -1; }
            return shownIndex_[row];
        }

        // ===================== СПІЛЬНІ ОБРОБНИКИ =====================
        System::Void OnAdd(System::Object^ sender, EventArgs^ e) {
            if (!IsValidName(txtFirst->Text) || !IsValidName(txtLast->Text)) {
                errorProvider1->SetError(txtFirst, IsValidName(txtFirst->Text) ? L"" : L"Invalid name");
                errorProvider1->SetError(txtLast,  IsValidName(txtLast->Text)  ? L"" : L"Invalid name");
                MessageBox::Show(L"Fix the name fields first.", L"Validation",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            int age = 0;
            if (!Int32::TryParse(txtAge->Text, age) || age < 16 || age > 100) {
                MessageBox::Show(L"Age must be an integer between 16 and 100.", L"Validation",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            std::string f   = marshal_as<std::string>(txtFirst->Text->Trim());
            std::string m   = marshal_as<std::string>(txtMid->Text->Trim());
            std::string l   = marshal_as<std::string>(txtLast->Text->Trim());
            std::string g   = marshal_as<std::string>(cbGender->Text);
            std::string gr  = marshal_as<std::string>(txtGroup->Text->Trim());
            std::string pet = marshal_as<std::string>(txtPets->Text->Trim());
            people_->push_back(new Student(f, m, l, g, age, gr, "College of IT", pet, "none"));
            FilterByType(currentFilter_);
        }

        System::Void OnDelete(System::Object^ sender, EventArgs^ e) {
            int idx = SelectedVectorIndex();
            if (idx < 0) {
                MessageBox::Show(L"Select a row to delete.", L"Delete",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            delete (*people_)[idx];
            people_->erase(people_->begin() + idx);
            FilterByType(currentFilter_);
            UpdateSelectedCount();
        }

        System::Void OnEdit(System::Object^ sender, EventArgs^ e) {
            int idx = SelectedVectorIndex();
            if (idx < 0) { return; }
            Student* st = dynamic_cast<Student*>((*people_)[idx]);
            if (st == nullptr) {
                MessageBox::Show(L"The edit dialog is available for students only.", L"Edit",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            StudentEditForm^ dlg = gcnew StudentEditForm(
                gcnew String(st->getFirstName().c_str()), gcnew String(st->getMidName().c_str()),
                gcnew String(st->getLastName().c_str()),  gcnew String(st->getGender().c_str()),
                st->getAge(), gcnew String(st->getGroup().c_str()),
                gcnew String(st->getHomePets().c_str()),  gcnew String(st->getSport().c_str()));
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
                FilterByType(currentFilter_);
            }
        }

        String^ SerializePerson(Person* p) {
            String^ first = gcnew String(p->getFirstName().c_str());
            String^ mid   = gcnew String(p->getMidName().c_str());
            String^ last  = gcnew String(p->getLastName().c_str());
            String^ gen   = gcnew String(p->getGender().c_str());
            String^ age   = p->getAge().ToString();
            Student* st = dynamic_cast<Student*>(p);
            if (st != nullptr) return String::Join(L"|", gcnew array<String^>{ L"S", first, mid, last, gen, age,
                gcnew String(st->getGroup().c_str()), gcnew String(st->getHomePets().c_str()) });
            Teacher* t = dynamic_cast<Teacher*>(p);
            if (t != nullptr) return String::Join(L"|", gcnew array<String^>{ L"T", first, mid, last, gen, age,
                gcnew String(t->getSubject().c_str()), t->getExperience().ToString() });
            Staff* f = dynamic_cast<Staff*>(p);
            String^ e1 = (f != nullptr) ? gcnew String(f->getPosition().c_str())   : L"";
            String^ e2 = (f != nullptr) ? gcnew String(f->getDepartment().c_str()) : L"";
            return String::Join(L"|", gcnew array<String^>{ L"F", first, mid, last, gen, age, e1, e2 });
        }

        System::Void OnSave(System::Object^ sender, EventArgs^ e) {
            SaveFileDialog^ dlg = gcnew SaveFileDialog();
            dlg->Filter = L"Text Files|*.txt|All Files|*.*";
            if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK) { return; }
            StreamWriter^ sw = nullptr;
            try {
                sw = gcnew StreamWriter(dlg->FileName, false, System::Text::Encoding::UTF8);
                for (Person* p : *people_) { sw->WriteLine(SerializePerson(p)); }
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Save error: " + ex->Message, L"Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }
            finally { if (sw != nullptr) { sw->Close(); } }
            lblSaved->Text = L"Last saved: " + DateTime::Now.ToString(L"HH:mm:ss");   // ДЗ9 #5
        }

        System::Void OnExit(System::Object^ sender, EventArgs^ e) { this->Close(); }

        System::Void UpdateSelectedCount() {
            lblSelected->Text = L"Selected: " + dataGridView1->SelectedRows->Count.ToString();
        }
        System::Void grid_SelectionChanged(System::Object^ sender, EventArgs^ e) { UpdateSelectedCount(); }

        System::Void grid_MouseDown(System::Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                DataGridView::HitTestInfo^ hit = dataGridView1->HitTest(e->X, e->Y);
                if (hit->RowIndex >= 0) {
                    dataGridView1->ClearSelection();
                    dataGridView1->Rows[hit->RowIndex]->Selected = true;
                }
            }
        }
    };
}
