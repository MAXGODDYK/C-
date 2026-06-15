#pragma once

// THEME 6. Modal dialog for editing a student.
// Input  -> via the parameterized constructor.
// Output -> via read-only properties (EditedAge, etc.).
// Communication with MainForm is only through ShowDialog() + DialogResult,
// WITHOUT direct access to the dialog controls from the outside.

namespace Portfolio {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    public ref class StudentEditForm : public Form {
    public:
        // Constructor receives the current student data and fills the fields.
        StudentEditForm(String^ first, String^ mid, String^ last, String^ gender,
                        int age, String^ group, String^ pets, String^ sport) {
            InitializeComponent();
            txtFirst->Text = first;
            txtMid->Text   = mid;
            txtLast->Text  = last;
            cbGender->SelectedIndex = (gender == L"Female") ? 1 : 0;
            txtAge->Text   = age.ToString();
            txtGroup->Text = group;
            txtPets->Text  = pets;
            txtSport->Text = sport;
        }

    protected:
        ~StudentEditForm() { if (components) { delete components; } }

    private:
        System::ComponentModel::Container^ components;

        String^ editedFirst_;  String^ editedMid_;   String^ editedLast_;
        String^ editedGender_; int     editedAge_;
        String^ editedGroup_;  String^ editedPets_;  String^ editedSport_;

        Label^   lblFirst;  TextBox^  txtFirst;
        Label^   lblMid;    TextBox^  txtMid;
        Label^   lblLast;   TextBox^  txtLast;
        Label^   lblGender; ComboBox^ cbGender;
        Label^   lblAge;    TextBox^  txtAge;
        Label^   lblGroup;  TextBox^  txtGroup;
        Label^   lblPets;   TextBox^  txtPets;
        Label^   lblSport;  TextBox^  txtSport;
        Button^  btnSave;   Button^   btnCancel;

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

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            const int LX = 15, FX = 150, FW = 360, RH = 33;
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

            lblAge   = MakeLabel(L"Age:",    LX, y); txtAge   = MakeBox(FX, y, FW); y += RH;
            lblGroup = MakeLabel(L"Group:",  LX, y); txtGroup = MakeBox(FX, y, FW); y += RH;
            lblPets  = MakeLabel(L"Pets:",   LX, y); txtPets  = MakeBox(FX, y, FW); y += RH;
            lblSport = MakeLabel(L"Sport:",  LX, y); txtSport = MakeBox(FX, y, FW); y += RH + 8;

            btnSave = gcnew Button();
            btnSave->Text = L"Save";
            btnSave->Location = Point(FX, y);
            btnSave->Size = System::Drawing::Size(175, 30);
            btnSave->Click += gcnew EventHandler(this, &StudentEditForm::btnSave_Click);

            btnCancel = gcnew Button();
            btnCancel->Text = L"Cancel";
            btnCancel->Location = Point(FX + 185, y);
            btnCancel->Size = System::Drawing::Size(175, 30);
            btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            btnCancel->Click += gcnew EventHandler(this, &StudentEditForm::btnCancel_Click);
            y += 45;

            this->Text = L"Edit student";
            this->ClientSize = System::Drawing::Size(FX + FW + 15, y);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->StartPosition = FormStartPosition::CenterParent;
            this->AcceptButton = btnSave;     // Enter = Save
            this->CancelButton = btnCancel;   // Esc   = Cancel

            array<Control^>^ controls = gcnew array<Control^>{
                lblFirst, txtFirst, lblMid, txtMid, lblLast, txtLast,
                lblGender, cbGender, lblAge, txtAge, lblGroup, txtGroup,
                lblPets, txtPets, lblSport, txtSport, btnSave, btnCancel
            };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        // Validate -> store result -> DialogResult::OK -> close.
        System::Void btnSave_Click(Object^ sender, EventArgs^ e) {
            if (String::IsNullOrWhiteSpace(txtFirst->Text) ||
                String::IsNullOrWhiteSpace(txtLast->Text)) {
                MessageBox::Show(L"Enter the first name and surname.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            if (cbGender->SelectedIndex < 0) {
                MessageBox::Show(L"Select a gender.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            int age = 0;
            if (!Int32::TryParse(txtAge->Text, age) || age < 16 || age > 100) {
                MessageBox::Show(L"Age must be an integer between 16 and 100.", L"Input error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            editedFirst_  = txtFirst->Text->Trim();
            editedMid_    = txtMid->Text->Trim();
            editedLast_   = txtLast->Text->Trim();
            editedGender_ = cbGender->Text;
            editedAge_    = age;
            editedGroup_  = txtGroup->Text->Trim();
            editedPets_   = txtPets->Text->Trim();
            editedSport_  = txtSport->Text->Trim();

            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }

        System::Void btnCancel_Click(Object^ sender, EventArgs^ e) {
            this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Close();
        }

    public:
        // Read-only properties returning the edited data.
        property String^ EditedFirst  { String^ get() { return editedFirst_;  } }
        property String^ EditedMid    { String^ get() { return editedMid_;    } }
        property String^ EditedLast   { String^ get() { return editedLast_;   } }
        property String^ EditedGender { String^ get() { return editedGender_; } }
        property int     EditedAge    { int     get() { return editedAge_;    } }
        property String^ EditedGroup  { String^ get() { return editedGroup_;  } }
        property String^ EditedPets   { String^ get() { return editedPets_;   } }
        property String^ EditedSport  { String^ get() { return editedSport_;  } }
    };
}
