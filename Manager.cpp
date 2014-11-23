#pragma once
#include <iostream>
namespace Manage {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;



	/// <summary>
	/// Summary for Manager
	/// </summary>
	public ref class Manager : public System::Windows::Forms::Form
	{
		static array<String^>^ Tests = gcnew array<String^>(3){ "Cat Scan", "ER Visit", "Hospitalization" };
	private: System::Windows::Forms::DataGridView^  dashGrid;
	private: static array<bool>^ options;
	private: static array<TimeSpan>^ lengths;
	private: array<Object^, 2>^ TestArray;
	public:
		Manager(void)
		{
			InitializeComponent();
		}

	public:
		Manager(DataGridView^ other, array<Object^, 2>^ TestArray){
			dashGrid = other;
			// this->options = options;
			//this->lengths = lengths;
			this->TestArray = TestArray;
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Manager()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			for (int i = 0; i < this->TestArray->GetUpperBound(0); i++)
			{
				System::Windows::Forms::CheckBox^  checkBox1 = (gcnew System::Windows::Forms::CheckBox());
				System::Windows::Forms::ComboBox^ comboBox1 = (gcnew System::Windows::Forms::ComboBox());
				System::Windows::Forms::Label^ label1 = (gcnew System::Windows::Forms::Label());
				System::Windows::Forms::TextBox^  textBox1 = (gcnew System::Windows::Forms::TextBox());

				checkBox1->AutoSize = true;
				if (dashGrid->Columns[i + 4]->Visible){
					checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
				}
				checkBox1->Location = System::Drawing::Point(12, (i * 80) + 1);
				checkBox1->Name = L"checkBox1";
				checkBox1->Size = System::Drawing::Size(70, 17);
				checkBox1->TabIndex = 0;
				checkBox1->Text = TestArray[i, 0]->ToString();
				checkBox1->UseVisualStyleBackColor = true;
				checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Manager::comboBoxUncheck);




				textBox1->Location = System::Drawing::Point(150, (i * 80) + 1);
				textBox1->Name = TestArray[i, 0]->ToString();
				if (!dashGrid->Columns[i + 4]->Visible){
					textBox1->Enabled = false;
				}
				//textBox1->SelectedItem = ((safe_cast <TimeSpan^>(TestArray[i,1]))->Days).ToString();
				textBox1->Size = System::Drawing::Size(56, 21);
				textBox1->TabIndex = 4;
				textBox1->Text = ((safe_cast <TimeSpan^>(TestArray[i, 1]))->Days).ToString();
				//comboBox1->DropDownStyle = ComboBoxStyle::DropDownList;

				label1->AutoSize = true;
				label1->Location = System::Drawing::Point(210, (i * 80) + 1);
				label1->Name = L"label1";
				label1->Size = System::Drawing::Size(35, 13);
				label1->TabIndex = 0;
				label1->Text = L"Days";

				this->Controls->Add(checkBox1); //evens
				this->Controls->Add(textBox1); //odds
				this->Controls->Add(label1);

			}

			this->button1 = (gcnew System::Windows::Forms::Button());


			//
			// button1
			// 
			this->button1->Location = System::Drawing::Point(171, 227);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 6;
			this->button1->Text = L"OK";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Manager::button1_Click);

			this->Controls->Add(this->button1);

			// 
			// Manager
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(398, 262);


		}
#pragma endregion
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void comboBoxUncheck(System::Object^  sender, System::EventArgs^  e) {
		CheckBox^ clone = safe_cast<CheckBox^>(sender);
		array<Control^>^ options = this->Controls->Find(clone->Text, true);
		if (!clone->Checked)
		{
			options[0]->Enabled = false;
		}
		else{
			options[0]->Enabled = true;
		}
	}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		Console::WriteLine(this->Controls[0]);
		Console::WriteLine(this->Controls->Count);
		for (int i = 0, v = 0; i< this->Controls->Count - 1; i += 3, v++)
		{
			if (safe_cast<CheckBox^>(this->Controls[i])->Checked)
			{
				dashGrid->Columns[v + 4]->Visible = true;
				//this->options[v]= true;
			}
			else{
				dashGrid->Columns[v + 4]->Visible = false;
				//this->options[v] = false;
			}
			try{
				this->TestArray[v, 1] = TimeSpan(Convert::ToInt32(safe_cast<TextBox^>(this->Controls[i + 1])->Text, 10), 0, 0, 0);
			}
			catch (FormatException^ e){
				this->TestArray[v, 1] = TimeSpan(7, 0, 0, 0);
			}
		}
		dashGrid->Enabled = true;
		Manager::Close();
	}

	};
}