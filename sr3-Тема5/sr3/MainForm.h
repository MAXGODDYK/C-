#pragma once

// THEME 7. DataGridView / ListView and synchronization with a vector.
// MainForm keeps a mixed list people_ (vector<Person*>): Student / Teacher / Staff.
// A ListView is synchronized with the vector and supports:
//  - Details / LargeIcon view modes (RadioButton switch);
//  - sorting by the "Surname" column on header click;
//  - context menu (Edit / Delete / Details);
//  - conditional formatting: age < 18 -> red text, experience > 15 -> bold;
//  - role filter (variant A): All / Students / Teachers / Staff.
// UI = managed (gcnew, ^); business logic (Person*) = native (new/delete).

#include <string>
#include <vector>
#include <algorithm>
#include <msclr/marshal_cppstd.h>

#include "Person.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include "StudentEditForm.h"
#include "StatisticsForm.h"

namespace Portfolio {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    // Native comparator for sorting people_ by a chosen ListView column.
    // (A native lambda is not allowed inside a managed member function.)
    struct PersonComparer {
        int col; bool asc;
        bool operator()(Person* a, Person* b) const {
            int c = 0;
            switch (col) {
            case 0: c = a->getLastName().compare(b->getLastName()); break;
            case 1: c = a->getFirstName().compare(b->getFirstName()); break;
            case 2: c = a->getRole().compare(b->getRole()); break;
            case 3: c = a->getAge() - b->getAge(); break;
            case 4: c = a->getExperienceYears() - b->getExperienceYears(); break;
            default: c = a->getInfo().compare(b->getInfo()); break;
            }
            return asc ? (c < 0) : (c > 0);
        }
    };

    public ref class MainForm : public Form {
    public:
        MainForm() {
            people_ = new std::vector<Person*>();
            SeedPeople();
            sortColumn_ = -1; sortAsc_ = true;
            InitializeComponent();
            RefreshList();
            LoadDefaults();
        }

    protected:
        ~MainForm() {
            if (components) { delete components; }
            CleanupNative();
        }
        !MainForm() { CleanupNative(); }

    private:
        // --- native list (managed manually) ---
        std::vector<Person*>* people_;
        int  sortColumn_;
        bool sortAsc_;

        void CleanupNative() {
            if (people_) {
                for (Person* p : *people_) { delete p; }   // virtual ~Person
                delete people_;
                people_ = nullptr;
            }
        }

        void SeedPeople() {
            people_->push_back(new Student("Maksym", "O.", "Hodyk", "Male", 19, "PI-21", "cat Barsyk", "football"));
            people_->push_back(new Student("Ivan", "P.", "Ivanenko", "Male", 17, "PI-22", "none", "chess")); // age < 18 -> red
            people_->push_back(new Teacher("Olena", "I.", "Kovalenko", "Female", 44, "OOP C++", "Senior Lecturer", 20)); // exp > 15 -> bold
            people_->push_back(new Teacher("Petro", "M.", "Shevchuk", "Male", 33, "Algorithms", "Lecturer", 8));
            people_->push_back(new Staff("Andriy", "M.", "Shevchenko", "Male", 45, "Lab assistant", "day", 18)); // exp > 15 -> bold
        }

        // std::string (ASCII/English) -> System::String^
        String^ Str(const std::string& s) { return gcnew String(s.c_str()); }

        int RoleIndex(const std::string& role) {
            if (role == "Student") return 0;
            if (role == "Teacher") return 1;
            return 2; // Staff
        }

        Student* MakeStudentFromInputs(int age) {
            std::string f  = marshal_as<std::string>(txtFirst->Text->Trim());
            std::string m  = marshal_as<std::string>(txtMid->Text->Trim());
            std::string l  = marshal_as<std::string>(txtLast->Text->Trim());
            std::string g  = marshal_as<std::string>(cbGender->Text);
            std::string gr = marshal_as<std::string>(txtGroup->Text->Trim());
            std::string pet= marshal_as<std::string>(txtPets->Text->Trim());
            std::string sp = marshal_as<std::string>(txtSport->Text->Trim());
            return new Student(f, m, l, g, age, gr, pet, sp);
        }

        bool ValidateInputs(int% age) {
            if (String::IsNullOrWhiteSpace(txtFirst->Text) ||
                String::IsNullOrWhiteSpace(txtLast->Text)) {
                MessageBox::Show(L"Enter the first name and surname.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return false;
            }
            if (cbGender->SelectedIndex < 0) {
                MessageBox::Show(L"Select a gender.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return false;
            }
            int a = 0;
            if (!Int32::TryParse(txtAge->Text, a) || a < 16 || a > 100) {
                MessageBox::Show(L"Age must be an integer between 16 and 100.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return false;
            }
            age = a;
            return true;
        }

        int SelectedPersonIndex() {
            if (lvPeople->SelectedItems->Count == 0) return -1;
            return safe_cast<int>(lvPeople->SelectedItems[0]->Tag);
        }

        // --- managed controls ---
        System::ComponentModel::Container^ components;

        Label^   lblFirst;  TextBox^  txtFirst;
        Label^   lblMid;    TextBox^  txtMid;
        Label^   lblLast;   TextBox^  txtLast;
        Label^   lblGender; ComboBox^ cbGender;
        Label^   lblAge;    TextBox^  txtAge;
        Label^   lblGroup;  TextBox^  txtGroup;
        Label^   lblPets;   TextBox^  txtPets;
        Label^   lblSport;  TextBox^  txtSport;

        Button^  btnCreate; Button^ btnEdit; Button^ btnDelete; Button^ btnClear;
        Label^   lblResult;
        TextBox^ txtSearch; Button^ btnSearch;
        RadioButton^ rbDetails; RadioButton^ rbLargeIcon;
        Label^   lblFilter;  ComboBox^ cbFilter;
        ListView^ lvPeople;
        Button^  btnExport;  Button^  btnStats;
        System::Windows::Forms::ContextMenuStrip^ ctxMenu;
        ImageList^ imgLarge; ImageList^ imgSmall;
        System::Drawing::Font^ boldFont_;

        Label^ MakeLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text; l->Location = Point(x, y + 3); l->AutoSize = true;
            return l;
        }
        TextBox^ MakeBox(int x, int y, int w) {
            TextBox^ t = gcnew TextBox();
            t->Location = Point(x, y); t->Size = System::Drawing::Size(w, 23);
            return t;
        }

        Bitmap^ MakeIcon(Color c, String^ letter, int size) {
            Bitmap^ bmp = gcnew Bitmap(size, size);
            Graphics^ g = Graphics::FromImage(bmp);
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
            g->Clear(Color::Transparent);
            g->FillEllipse(gcnew SolidBrush(c), 1, 1, size - 2, size - 2);
            System::Drawing::Font^ f = gcnew System::Drawing::Font("Segoe UI", (float)size * 0.42f, FontStyle::Bold);
            StringFormat^ sf = gcnew StringFormat();
            sf->Alignment = StringAlignment::Center; sf->LineAlignment = StringAlignment::Center;
            g->DrawString(letter, f, Brushes::White, RectangleF(0, 0, (float)size, (float)size), sf);
            delete g;
            return bmp;
        }

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            const int LX = 15, FX = 150, FW = 380, RH = 33;
            int y = 15;

            lblFirst  = MakeLabel(L"First name:",  LX, y); txtFirst = MakeBox(FX, y, FW); y += RH;
            lblMid    = MakeLabel(L"Middle name:", LX, y); txtMid   = MakeBox(FX, y, FW); y += RH;
            lblLast   = MakeLabel(L"Surname:",     LX, y); txtLast  = MakeBox(FX, y, FW); y += RH;

            lblGender = MakeLabel(L"Gender:", LX, y);
            cbGender  = gcnew ComboBox();
            cbGender->Location = Point(FX, y);
            cbGender->Size = System::Drawing::Size(FW, 23);
            cbGender->DropDownStyle = ComboBoxStyle::DropDownList;
            cbGender->Items->Add(L"Male");
            cbGender->Items->Add(L"Female");
            y += RH;

            lblAge   = MakeLabel(L"Age:",   LX, y); txtAge   = MakeBox(FX, y, FW); y += RH;
            lblGroup = MakeLabel(L"Group:", LX, y); txtGroup = MakeBox(FX, y, FW); y += RH;
            lblPets  = MakeLabel(L"Pets:",  LX, y); txtPets  = MakeBox(FX, y, FW); y += RH;
            lblSport = MakeLabel(L"Sport:", LX, y); txtSport = MakeBox(FX, y, FW); y += RH;

            const int BW = 120;
            btnCreate = gcnew Button(); btnCreate->Text = L"Add";
            btnCreate->Location = Point(FX, y); btnCreate->Size = System::Drawing::Size(BW, 30);
            btnCreate->Click += gcnew EventHandler(this, &MainForm::btnCreate_Click);

            btnEdit = gcnew Button(); btnEdit->Text = L"Edit";
            btnEdit->Location = Point(FX + BW + 10, y); btnEdit->Size = System::Drawing::Size(BW, 30);
            btnEdit->Click += gcnew EventHandler(this, &MainForm::btnEdit_Click);

            btnDelete = gcnew Button(); btnDelete->Text = L"Delete";
            btnDelete->Location = Point(FX + 2 * (BW + 10), y); btnDelete->Size = System::Drawing::Size(BW, 30);
            btnDelete->Click += gcnew EventHandler(this, &MainForm::btnDelete_Click);

            btnClear = gcnew Button(); btnClear->Text = L"Clear";
            btnClear->Location = Point(FX + 3 * (BW + 10), y); btnClear->Size = System::Drawing::Size(BW, 30);
            btnClear->Click += gcnew EventHandler(this, &MainForm::btnClear_Click);
            y += 42;

            lblResult = gcnew Label();
            lblResult->Location = Point(LX, y);
            lblResult->Size = System::Drawing::Size(775, 40);
            lblResult->BorderStyle = BorderStyle::FixedSingle;
            lblResult->TextAlign = ContentAlignment::MiddleLeft;
            y += 50;

            // --- search + view mode + role filter ---
            txtSearch = MakeBox(LX, y, 230);
            btnSearch = gcnew Button(); btnSearch->Text = L"Search";
            btnSearch->Location = Point(LX + 235, y - 1); btnSearch->Size = System::Drawing::Size(85, 25);
            btnSearch->Click += gcnew EventHandler(this, &MainForm::btnSearch_Click);

            rbDetails = gcnew RadioButton(); rbDetails->Text = L"Details";
            rbDetails->Location = Point(360, y); rbDetails->Size = System::Drawing::Size(80, 24);
            rbLargeIcon = gcnew RadioButton(); rbLargeIcon->Text = L"Large icons";
            rbLargeIcon->Location = Point(445, y); rbLargeIcon->Size = System::Drawing::Size(110, 24);

            lblFilter = MakeLabel(L"Role:", 575, y);
            cbFilter = gcnew ComboBox();
            cbFilter->Location = Point(615, y); cbFilter->Size = System::Drawing::Size(175, 23);
            cbFilter->DropDownStyle = ComboBoxStyle::DropDownList;
            cbFilter->Items->Add(L"All");
            cbFilter->Items->Add(L"Students");
            cbFilter->Items->Add(L"Teachers");
            cbFilter->Items->Add(L"Staff");
            y += 34;

            // --- ListView ---
            lvPeople = gcnew ListView();
            lvPeople->Location = Point(LX, y);
            lvPeople->Size = System::Drawing::Size(775, 235);
            lvPeople->View = System::Windows::Forms::View::Details;
            lvPeople->FullRowSelect = true;
            lvPeople->MultiSelect = false;
            lvPeople->HideSelection = false;
            lvPeople->GridLines = true;
            lvPeople->Columns->Add(L"Surname", 120);
            lvPeople->Columns->Add(L"First name", 120);
            lvPeople->Columns->Add(L"Role", 80);
            lvPeople->Columns->Add(L"Age", 50);
            lvPeople->Columns->Add(L"Experience", 90);
            lvPeople->Columns->Add(L"Info", 295);

            boldFont_ = gcnew System::Drawing::Font(lvPeople->Font, FontStyle::Bold);

            imgLarge = gcnew ImageList(); imgLarge->ImageSize = System::Drawing::Size(48, 48); imgLarge->ColorDepth = ColorDepth::Depth32Bit;
            imgLarge->Images->Add(MakeIcon(Color::FromArgb(46, 134, 193), L"S", 48));
            imgLarge->Images->Add(MakeIcon(Color::FromArgb(230, 126, 34), L"T", 48));
            imgLarge->Images->Add(MakeIcon(Color::FromArgb(39, 174, 96), L"P", 48));
            imgSmall = gcnew ImageList(); imgSmall->ImageSize = System::Drawing::Size(16, 16); imgSmall->ColorDepth = ColorDepth::Depth32Bit;
            imgSmall->Images->Add(MakeIcon(Color::FromArgb(46, 134, 193), L"S", 16));
            imgSmall->Images->Add(MakeIcon(Color::FromArgb(230, 126, 34), L"T", 16));
            imgSmall->Images->Add(MakeIcon(Color::FromArgb(39, 174, 96), L"P", 16));
            lvPeople->LargeImageList = imgLarge;
            lvPeople->SmallImageList = imgSmall;

            lvPeople->ColumnClick += gcnew ColumnClickEventHandler(this, &MainForm::lvPeople_ColumnClick);
            lvPeople->MouseDown += gcnew MouseEventHandler(this, &MainForm::lvPeople_MouseDown);
            lvPeople->DoubleClick += gcnew EventHandler(this, &MainForm::btnEdit_Click);
            y += 245;

            // --- context menu (right click on a row) ---
            ctxMenu = gcnew System::Windows::Forms::ContextMenuStrip();
            ToolStripMenuItem^ miEdit = gcnew ToolStripMenuItem(L"Edit");
            miEdit->Click += gcnew EventHandler(this, &MainForm::btnEdit_Click);
            ToolStripMenuItem^ miDelete = gcnew ToolStripMenuItem(L"Delete");
            miDelete->Click += gcnew EventHandler(this, &MainForm::btnDelete_Click);
            ToolStripMenuItem^ miDetails = gcnew ToolStripMenuItem(L"Details");
            miDetails->Click += gcnew EventHandler(this, &MainForm::details_Click);
            ctxMenu->Items->Add(miEdit);
            ctxMenu->Items->Add(miDelete);
            ctxMenu->Items->Add(miDetails);
            lvPeople->ContextMenuStrip = ctxMenu;

            // --- export + statistics ---
            btnExport = gcnew Button(); btnExport->Text = L"Export TXT";
            btnExport->Location = Point(LX, y); btnExport->Size = System::Drawing::Size(130, 30);
            btnExport->Click += gcnew EventHandler(this, &MainForm::btnExport_Click);
            btnStats = gcnew Button(); btnStats->Text = L"Statistics";
            btnStats->Location = Point(LX + 140, y); btnStats->Size = System::Drawing::Size(130, 30);
            btnStats->Click += gcnew EventHandler(this, &MainForm::btnStats_Click);
            y += 40;

            // initial states BEFORE wiring change events (so RefreshList isn't called too early)
            rbDetails->Checked = true;
            cbFilter->SelectedIndex = 0;
            rbDetails->CheckedChanged += gcnew EventHandler(this, &MainForm::view_Changed);
            rbLargeIcon->CheckedChanged += gcnew EventHandler(this, &MainForm::view_Changed);
            cbFilter->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::filter_Changed);

            this->Text = L"StudentPortfolio — Theme 7 (ListView + vector)";
            this->ClientSize = System::Drawing::Size(805, y + 10);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->StartPosition = FormStartPosition::CenterScreen;

            array<Control^>^ controls = gcnew array<Control^>{
                lblFirst, txtFirst, lblMid, txtMid, lblLast, txtLast,
                lblGender, cbGender, lblAge, txtAge, lblGroup, txtGroup,
                lblPets, txtPets, lblSport, txtSport,
                btnCreate, btnEdit, btnDelete, btnClear, lblResult,
                txtSearch, btnSearch, rbDetails, rbLargeIcon, lblFilter, cbFilter,
                lvPeople, btnExport, btnStats
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void LoadDefaults() {
            txtFirst->Text = L"Maksym";
            txtMid->Text   = L"O.";
            txtLast->Text  = L"Hodyk";
            cbGender->SelectedIndex = 0;     // Male
            txtAge->Text   = L"19";
            txtGroup->Text = L"PI-21";
            txtPets->Text  = L"cat Barsyk";
            txtSport->Text = L"football";
            lblResult->Text = L"Fill the fields and press \"Add\".";
        }

        // --- vector -> ListView (with current filter, search and conditional formatting) ---
        void RefreshList() {
            if (lvPeople == nullptr || boldFont_ == nullptr) { return; }
            lvPeople->BeginUpdate();
            lvPeople->Items->Clear();

            String^ query = (txtSearch != nullptr) ? txtSearch->Text->Trim() : L"";
            int filter = (cbFilter != nullptr) ? cbFilter->SelectedIndex : 0; // 0 All,1 Students,2 Teachers,3 Staff

            for (size_t i = 0; i < people_->size(); ++i) {
                Person* p = (*people_)[i];
                std::string role = p->getRole();
                if (filter == 1 && role != "Student") { continue; }
                if (filter == 2 && role != "Teacher") { continue; }
                if (filter == 3 && role != "Staff")   { continue; }

                String^ info = Str(p->getInfo());
                if (!String::IsNullOrWhiteSpace(query) &&
                    info->IndexOf(query, StringComparison::OrdinalIgnoreCase) < 0) {
                    continue;
                }

                ListViewItem^ item = gcnew ListViewItem(Str(p->getLastName()));
                item->SubItems->Add(Str(p->getFirstName()));
                item->SubItems->Add(Str(p->getRole()));
                item->SubItems->Add(System::Convert::ToString(p->getAge()));
                item->SubItems->Add(System::Convert::ToString(p->getExperienceYears()));
                item->SubItems->Add(info);
                item->ImageIndex = RoleIndex(role);
                item->Tag = (int)i;

                // conditional formatting
                if (p->getAge() < 18) { item->ForeColor = Color::Red; }          // minor -> red
                if (p->getExperienceYears() > 15) { item->Font = boldFont_; }     // senior -> bold

                lvPeople->Items->Add(item);
            }
            lvPeople->EndUpdate();
        }

        // ADD: create a Student from inputs and add to vector + list
        System::Void btnCreate_Click(Object^ sender, EventArgs^ e) {
            int age = 0;
            if (!ValidateInputs(age)) { return; }
            Student* s = MakeStudentFromInputs(age);
            people_->push_back(s);
            RefreshList();
            lblResult->Text = L"Added: " + Str(s->getInfo());
        }

        // EDIT: open dialog for the selected student, update vector by DialogResult
        System::Void btnEdit_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedPersonIndex();
            if (idx < 0) {
                MessageBox::Show(L"Select a person in the list.", L"Edit",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            Student* st = dynamic_cast<Student*>((*people_)[idx]);
            if (!st) {
                MessageBox::Show(L"Only students can be edited.", L"Edit",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            StudentEditForm^ dlg = gcnew StudentEditForm(
                Str(st->getFirstName()), Str(st->getMidName()), Str(st->getLastName()),
                Str(st->getGender()), st->getAge(),
                Str(st->getGroup()), Str(st->getHomePets()), Str(st->getSport()));

            if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                std::string f  = marshal_as<std::string>(dlg->EditedFirst);
                std::string m  = marshal_as<std::string>(dlg->EditedMid);
                std::string l  = marshal_as<std::string>(dlg->EditedLast);
                std::string g  = marshal_as<std::string>(dlg->EditedGender);
                int age        = dlg->EditedAge;
                std::string gr = marshal_as<std::string>(dlg->EditedGroup);
                std::string pet= marshal_as<std::string>(dlg->EditedPets);
                std::string sp = marshal_as<std::string>(dlg->EditedSport);

                Student* updated = new Student(f, m, l, g, age, gr, pet, sp);
                delete (*people_)[idx];
                (*people_)[idx] = updated;
                RefreshList();
                lblResult->Text = L"Edited: " + Str(updated->getInfo());
            }
            else {
                lblResult->Text = L"Editing canceled.";
            }
        }

        // DELETE: remove selected person from vector and list
        System::Void btnDelete_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedPersonIndex();
            if (idx < 0) {
                MessageBox::Show(L"Select a person in the list.", L"Delete",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            if (MessageBox::Show(L"Delete the selected person?", L"Confirm",
                MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) {
                return;
            }
            delete (*people_)[idx];
            people_->erase(people_->begin() + idx);
            RefreshList();
            lblResult->Text = L"Person deleted.";
        }

        // DETAILS (context menu): show info + formal greeting of the selected person
        System::Void details_Click(Object^ sender, EventArgs^ e) {
            int idx = SelectedPersonIndex();
            if (idx < 0) {
                MessageBox::Show(L"Select a person in the list.", L"Details",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }
            Person* p = (*people_)[idx];
            String^ msg = Str(p->getInfo()) + Environment::NewLine + Str(p->getFormalGreeting());
            MessageBox::Show(msg, L"Details", MessageBoxButtons::OK, MessageBoxIcon::Information);
            lblResult->Text = Str(p->getFormalGreeting());
        }

        // CLEAR: reset only the input fields (the list stays)
        System::Void btnClear_Click(Object^ sender, EventArgs^ e) {
            txtFirst->Clear(); txtMid->Clear(); txtLast->Clear();
            txtAge->Clear();   txtGroup->Clear(); txtPets->Clear(); txtSport->Clear();
            cbGender->SelectedIndex = -1;
            lblResult->Text = L"Fields cleared.";
        }

        // SEARCH: filter list by name/info substring
        System::Void btnSearch_Click(Object^ sender, EventArgs^ e) {
            RefreshList();
            lblResult->Text = L"Search applied.";
        }

        // VIEW switch: Details / LargeIcon
        System::Void view_Changed(Object^ sender, EventArgs^ e) {
            lvPeople->View = rbLargeIcon->Checked
                ? System::Windows::Forms::View::LargeIcon
                : System::Windows::Forms::View::Details;
        }

        // ROLE FILTER (variant A)
        System::Void filter_Changed(Object^ sender, EventArgs^ e) {
            RefreshList();
        }

        // SORT by clicked column ("Surname" required; other columns supported too)
        System::Void lvPeople_ColumnClick(Object^ sender, ColumnClickEventArgs^ e) {
            int col = e->Column;
            if (col == sortColumn_) { sortAsc_ = !sortAsc_; }
            else { sortColumn_ = col; sortAsc_ = true; }
            bool asc = sortAsc_;
            PersonComparer cmp; cmp.col = col; cmp.asc = asc;
            std::sort(people_->begin(), people_->end(), cmp);
            RefreshList();
            lblResult->Text = L"Sorted by column: " + lvPeople->Columns[col]->Text +
                (asc ? L" (ascending)" : L" (descending)");
        }

        // select row under cursor on right click (so the context menu acts on it)
        System::Void lvPeople_MouseDown(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                ListViewHitTestInfo^ hit = lvPeople->HitTest(e->Location);
                if (hit->Item != nullptr) { hit->Item->Selected = true; }
            }
        }

        // EXPORT: save all people to a TXT file
        System::Void btnExport_Click(Object^ sender, EventArgs^ e) {
            try {
                SaveFileDialog^ dlg = gcnew SaveFileDialog();
                dlg->Filter = L"Text files (*.txt)|*.txt|All files (*.*)|*.*";
                dlg->FileName = L"people.txt";
                if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) { return; }
                System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(dlg->FileName, false, System::Text::Encoding::UTF8);
                for (Person* p : *people_) { sw->WriteLine(Str(p->getInfo())); }
                sw->Close();
                lblResult->Text = L"Export finished.";
            }
            catch (Exception^ ex) {
                MessageBox::Show(ex->Message, L"Export error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // STATISTICS: counts by role + average age
        System::Void btnStats_Click(Object^ sender, EventArgs^ e) {
            int total = (int)people_->size();
            int students = 0, teachers = 0, staff = 0, sum = 0;
            for (Person* p : *people_) {
                std::string r = p->getRole();
                if (r == "Student") students++;
                else if (r == "Teacher") teachers++;
                else staff++;
                sum += p->getAge();
            }
            double avg = (total > 0) ? (double)sum / total : 0.0;
            StatisticsForm^ sf = gcnew StatisticsForm(total, students, teachers, staff, avg);
            sf->ShowDialog(this);
        }
    };
}
