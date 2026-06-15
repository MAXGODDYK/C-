#pragma once

// Simple statistics dialog: shows counts by role and the average age.

namespace Portfolio {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class StatisticsForm : public Form {
    public:
        StatisticsForm(int total, int students, int teachers, int staff, double avgAge) {
            InitializeComponent();
            lblTotal->Text    = String::Format(L"Total people: {0}", total);
            lblStudents->Text = String::Format(L"Students: {0}", students);
            lblTeachers->Text = String::Format(L"Teachers: {0}", teachers);
            lblStaff->Text    = String::Format(L"Staff: {0}", staff);
            lblAvgAge->Text   = String::Format(L"Average age: {0:0.##}", avgAge);
        }

    protected:
        ~StatisticsForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        Label^ lblTotal; Label^ lblStudents; Label^ lblTeachers; Label^ lblStaff; Label^ lblAvgAge;

        void InitializeComponent() {
            components = nullptr;
            this->SuspendLayout();

            int y = 15;
            lblTotal    = gcnew Label(); lblTotal->Location    = Point(15, y);      lblTotal->AutoSize = true;
            lblStudents = gcnew Label(); lblStudents->Location = Point(15, y + 30);  lblStudents->AutoSize = true;
            lblTeachers = gcnew Label(); lblTeachers->Location = Point(15, y + 60);  lblTeachers->AutoSize = true;
            lblStaff    = gcnew Label(); lblStaff->Location    = Point(15, y + 90);  lblStaff->AutoSize = true;
            lblAvgAge   = gcnew Label(); lblAvgAge->Location   = Point(15, y + 120); lblAvgAge->AutoSize = true;

            this->Text = L"Statistics";
            this->ClientSize = System::Drawing::Size(300, 180);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->StartPosition = FormStartPosition::CenterParent;

            array<Control^>^ controls = gcnew array<Control^>{ lblTotal, lblStudents, lblTeachers, lblStaff, lblAvgAge };
            this->Controls->AddRange(controls);

            this->ResumeLayout(false);
            this->PerformLayout();
        }
    };
}
