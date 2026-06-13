#pragma once

// Simple statistics form: shows counts and average age; created for Theme 6

namespace Portfolio {

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	public ref class StatisticsForm : public Form {
	public:
		StatisticsForm(int total, int students, double avgAge) {
			InitializeComponent();
			lblTotal->Text = String::Format(L"Усього: {0}", total);
			lblStudents->Text = String::Format(L"Студентів: {0}", students);
			lblAvgAge->Text = String::Format(L"Середній вік студентів: {0:0.##}", avgAge);
		}

	protected:
		~StatisticsForm() { if (components) delete components; }

	private:
		System::ComponentModel::Container^ components;
		Label^ lblTotal; Label^ lblStudents; Label^ lblAvgAge;

		void InitializeComponent() {
			components = nullptr;
			this->SuspendLayout();

			lblTotal = gcnew Label(); lblTotal->Location = Point(15, 15); lblTotal->AutoSize = true;
			lblStudents = gcnew Label(); lblStudents->Location = Point(15, 45); lblStudents->AutoSize = true;
			lblAvgAge = gcnew Label(); lblAvgAge->Location = Point(15, 75); lblAvgAge->AutoSize = true;

			this->Text = L"Статистика";
			this->ClientSize = System::Drawing::Size(300, 120);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;

			array<Control^>^ controls = gcnew array<Control^>{ lblTotal, lblStudents, lblAvgAge };
			this->Controls->AddRange(controls);

			this->ResumeLayout(false);
			this->PerformLayout();
		}
	};
}
