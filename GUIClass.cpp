#include "Manager.cpp"
#include "PurgeItem.h"
#include <time.h>
//#include "TimeComparer.cpp"
namespace dash {
	using namespace System;
	using namespace Manage;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;


	public ref class GUIClass : public System::Windows::Forms::Form, public IComparer
	{
	private: const time_t MINUTE_INTERVAL = 60;
	private: const time_t HOUR_INTERVAL = 60 * MINUTE_INTERVAL;

    // Any records fetched before this interval needs to be removed (in seconds) 
	private: const time_t PURGE_AGE_INTERVAL = 6 * HOUR_INTERVAL;

	// How often we check for old records (in seconds)
	private: const time_t CHECK_PURGE_AGE_INTERVAL = 10 * MINUTE_INTERVAL;

	// How often we check if there are more than max number of records in the grid			 
	private: const time_t CHECK_PURGE_SIZE_INTERVAL = 5 * MINUTE_INTERVAL;

    // Max number of records permitted in the grid
	private: const time_t PURGE_SIZE_MAX_ROWS = 1000;

	private: System::Windows::Forms::DataGridView^  dataGridView1;
			 //private: static array<bool>^ options = gcnew array<bool>(3)
			 //	{ true, true, true};
	private: static array<Object^, 2>^ TestArray = gcnew array<Object^, 2>(4, 2);
			 //private: static array<TimeSpan>^ timeRange = gcnew array<TimeSpan>(3){ TimeSpan(7,0,0,0), TimeSpan(7,0,0,0), TimeSpan(31,0,0,0)};

	private: System::Windows::Forms::Timer^ purgeAgeTimer;
	private: System::Windows::Forms::Timer^ purgeSizeTimer;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private:  System::Windows::Forms::ToolStripMenuItem^ menuToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  visibilityToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  privateModeToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  publicModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  testManagerToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^ addDataButton;

	public:
		GUIClass(array<Object^, 2>^ tests){
			this->TestArray = tests;
			init();
		}

	private:
		void TimerPurgeAgeProcessor(Object^, EventArgs^) {
			time_t currentTime;
			time(&currentTime);

			cout << "Timer Went Off."; //Testing
			for (int i = this->dataGridView1->Rows->Count - 1; i > -1; i--) {
				DataGridViewRow^ row = this->dataGridView1->Rows[i];
				DataGridViewCell^ cell = row->Cells[row->Cells->Count - 1];
				if (cell->Value != nullptr) {
					time_t createTime = (time_t)cell->Value;
					if (currentTime - createTime >= PURGE_AGE_INTERVAL) {
						this->dataGridView1->Rows->RemoveAt(i);
					}
				}
			}

			dataGridView1->PerformLayout();
		}

		virtual int compare(Object^ a, Object^ b) sealed = IComparer::Compare{
			PurgeItem^ pa = (PurgeItem^)a;
			PurgeItem^ pb = (PurgeItem^)b;
			if (pa->getTime() == pb->getTime()) {
				return pa->getIndex() - pb->getIndex();
			}
			else {
				return pa->getTime() - pb->getTime();
			}
		}

		void TimerPurgeSizeProcessor(Object^, EventArgs^) {
			if (this->dataGridView1->Rows->Count <= PURGE_SIZE_MAX_ROWS) {
				return;
			}

			SortedList^ ageToIndex = gcnew SortedList(this);

			for (int i = this->dataGridView1->Rows->Count - 1; i > -1; i--) {
				DataGridViewRow^ row = this->dataGridView1->Rows[i];
				DataGridViewCell^ cell = row->Cells[row->Cells->Count - 1];
				if (cell->Value != nullptr) {
					ageToIndex->Add(gcnew PurgeItem((time_t)cell->Value, cell->RowIndex), nullptr);
				}
			}

			if (ageToIndex->Count < 1) {
				return;
			}

			SortedList^ indexToAge = gcnew SortedList;

			for (int i = 0; i < ageToIndex->Count - (PURGE_SIZE_MAX_ROWS - 1); i++) {
				PurgeItem^ key = (PurgeItem^)ageToIndex->GetKey(i);
				indexToAge->Add(key->getIndex(), key->getTime());
			}

			for (int i = indexToAge->Count - 1; i > -1; i--) {
				this->dataGridView1->Rows->RemoveAt((int)indexToAge->GetKey(i));
			}
			cout << dataGridView1->Rows->Count << endl;
			dataGridView1->PerformLayout();

		}

		void init(){
			this->purgeAgeTimer = (gcnew System::Windows::Forms::Timer());
			this->purgeSizeTimer = (gcnew System::Windows::Forms::Timer());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->menuToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->visibilityToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->privateModeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->publicModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->testManagerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->addDataButton = (gcnew System::Windows::Forms::Button());

			this->menuToolStripMenuItem->Text = L"Menu";
			this->visibilityToolStripMenuItem1->Text = L"Visibility";
			this->privateModeToolStripMenuItem1->Text = L"Private Mode";
			this->publicModeToolStripMenuItem->Text = L"Public Mode";

			this->publicModeToolStripMenuItem->Checked = true;

			this->testManagerToolStripMenuItem->Text = L"Manager";
			this->menuStrip1->Items->Add(menuToolStripMenuItem);
			this->menuToolStripMenuItem->DropDownItems->Add(visibilityToolStripMenuItem1);
			this->menuToolStripMenuItem->DropDownItems->Add(testManagerToolStripMenuItem);
			this->testManagerToolStripMenuItem->Click += gcnew System::EventHandler(this, &GUIClass::testManagerToolStripMenuItem_Click);
			this->visibilityToolStripMenuItem1->DropDownItems->Add(privateModeToolStripMenuItem1);
			this->privateModeToolStripMenuItem1->Click += gcnew System::EventHandler(this, &GUIClass::VisibilityChanged);
			this->visibilityToolStripMenuItem1->DropDownItems->Add(publicModeToolStripMenuItem);
			this->publicModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &GUIClass::VisibilityChanged);
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(844, 439);
			this->Text = L"Dashboard";
			this->SizeChanged += gcnew System::EventHandler(this, &GUIClass::Form1_SizeChanged);
			this->ResumeLayout(false);
			this->PerformLayout();
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;

			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->menuStrip1);

			this->dataGridView1->Location = System::Drawing::Point(0, 96);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->Size = System::Drawing::Size(this->Width - 12, this->Height - 130);
			this->dataGridView1->ColumnCount = 8; // Column Counter
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->ColumnHeadersVisible = true;

			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(12, 48);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(259, 20);
			this->textBox1->TabIndex = 2;
			this->Controls->Add(this->textBox1);
			this->textBox1->KeyUp += gcnew KeyEventHandler(this, &dash::GUIClass::pressedKey);
			
			//
			// Button
			//
			this->addDataButton->Text = L"AddSomeData";
			this->addDataButton->Location = System::Drawing::Point(350, 48);
			this->addDataButton->Name = L"addDataButton";
			this->addDataButton->Size = System::Drawing::Size(60, 20);
			this->addDataButton->TabIndex = 5;
			this->Controls->Add(this->addDataButton);
			this->addDataButton->Click += gcnew System::EventHandler(this, &dash::GUIClass::addDataClicked);


			//Set up timer for purging old data
			this->purgeAgeTimer->Tick += gcnew EventHandler(this, &GUIClass::TimerPurgeAgeProcessor);
			this->purgeAgeTimer->Interval = CHECK_PURGE_AGE_INTERVAL * 1000;
			this->purgeAgeTimer->Start();

			//Set up timer for purging old data
			this->purgeSizeTimer->Tick += gcnew EventHandler(this, &GUIClass::TimerPurgeSizeProcessor);
			this->purgeSizeTimer->Interval = CHECK_PURGE_SIZE_INTERVAL * 1000;
			this->purgeSizeTimer->Start();

			this->dataGridView1->Columns[0]->Name = "Name"; //public
			this->dataGridView1->Columns[1]->Name = "Name"; //private
			this->dataGridView1->Columns[1]->Visible = false;
			this->dataGridView1->Columns[2]->Name = "DOB";
			this->dataGridView1->Columns[3]->Name = "Arrival Time";
			this->dataGridView1->Columns[7]->Name = "Fetched";
			//this->dataGridView1->Columns[7]->Visible = false;


			for (int i = 0; i < this->TestArray->GetUpperBound(0); i++){
				this->dataGridView1->Columns[i + 4]->Name = this->TestArray[i, 0]->ToString();
			}
			//this->dataGridView1->TabIndex = 0;

			//ADD TEST ROWS************************
			array<String^>^row1 = gcnew array < String^ > {
				"Fid**,Ja***", "Fiduk, Jason", "19930821", "20140805", "09/20/2014", "10/29/2014", ""
			};
			this->addRow(row1);

			array<String^>^rowA = gcnew array < String^ > {
				"Pen**,Cra***", "Pennell, Craig", "19891225", "20140203", "10/23/2014", "09/21/2013", ""
			};
			this->addRow(rowA);
			array<String^>^rowB = gcnew array < String^ > {
				"Sch***,Joe", "Schmoe, Joe", "19551011", "20141012", "", "10/21/2014", "10/20/2014"
			};
			this->addRow(rowB);
			array<String^>^rowC = gcnew array < String^ > {
				"Mon***,Bur***", "Montgomery, Burns", "18060606", "20140923", "10/20/1989", "", "02/01/1900"
			};
			this->addRow(rowC);
			array<String^>^rowD = gcnew array < String^ > {
				"Fuc***, You***", "Fuchs,Youra", "19990612", "20140830", "10/20/2014", "", ""
			};
			this->addRow(rowD);
			array<String^>^rowE = gcnew array < String^ > {
				"Cla***, Jer***", "Clarion, Jerry", "19730418", "20100917", "09/20/2014", "", ""
			};
			this->addRow(rowE);

			array<String^>^rowF = gcnew array < String^ > {
				"McP***, Gil***", "McPherrson, Gilbert", "19991102", "20110313", "", "02/28/2013", "10/01/2014"
			};
			this->addRow(rowF);
			array<String^>^rowG = gcnew array < String^ > {
				"Til***, Sho***", "Tiller, Shonda", "19820714", "20070413", "", "06/30/2013", ""
			};
			this->addRow(rowG);
			array<String^>^rowH = gcnew array < String^ > {
				"O'D***, Mic***", "O'Donald, Micheal", "19560313", "20130701", "", "", "08/21/2014"
			};
			this->addRow(rowH);
			array<String^>^rowI = gcnew array < String^ > {
				"Por***, Sus***", "Porter, Susan", "19671208", "20140412", "01/31/2000", "02/01/2000", "08/21/2014"
			};
			this->addRow(rowI);
			array<String^>^rowJ = gcnew array < String^ > {
				"Hun***, Wil***", "Huntington, Willard", "19540627", "20131010", "09/30/2012", "", ""
			};
			this->addRow(rowJ);
			array<String^>^rowK = gcnew array < String^ > {
				"DeM***, Bea***", "DeMarco, Beatrice", "19950122", "20140530", "09/30/2012", "", "11/21/2014"
			};
			this->addRow(rowK);
			array<String^>^rowL = gcnew array < String^ > {
				"Lem***, Ran***", "Lemire, Randolf", "19780401", "20090822", "", "06/12/2010", ""
			};
			this->addRow(rowL);
			array<String^>^rowM = gcnew array < String^ > {
				"Gre***, Dar***", "Greenfield, Dareen", "19850229", "20101216", "", "", "10/10/2010"
			};
			this->addRow(rowM);
			array<String^>^rowN = gcnew array < String^ > {
				"Dra***, Han***", "Drake, Hannah", "19970915", "20110615", "", "", ""
			};
			this->addRow(rowN);
			//	dataGridView1->DataSource = rowN;

			//***************************************************
		}
	public: System::Void addRow(array<String^> ^data)
	{
		array<Object^> ^objarray = gcnew array < Object^ >(data->Length + 1);
		System::Array::Copy(data, objarray, data->Length);
		time_t fetchTime;
		time(&fetchTime);
		objarray[data->Length] = fetchTime;
		dataGridView1->Rows->Add(objarray);
		checkTests(this->dataGridView1, dataGridView1->Rows->Count - 2);
		dataGridView1->PerformLayout();
	}


	private: System::Void Form1_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
		this->dataGridView1->Size = System::Drawing::Size(this->Width - 12, this->Height - 130);
	}

	private: System::Void testManagerToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Manager^ test = gcnew Manager(this->dataGridView1, TestArray);
		test->FormClosed += gcnew FormClosedEventHandler(this, &dash::GUIClass::Form1_FormClosed);
		this->dataGridView1->Enabled = false;
		test->ShowDialog();
	}
			 void Form1_FormClosed(System::Object^ sender, FormClosedEventArgs^ e)
			 {
				 checkAll();
			 }
	private:  void pressedKey(System::Object^  sender, KeyEventArgs^  e) {
		System::Text::RegularExpressions::Regex^ state = gcnew System::Text::RegularExpressions::Regex("\\b(" + textBox1->Text + ")([^ ]*)", System::Text::RegularExpressions::RegexOptions::IgnoreCase);
		for (int i = 0; i <= dataGridView1->Rows->Count - 2; i++){
			if (e->KeyCode == Keys::Back){
				if (state->IsMatch(dataGridView1->Rows[i]->Cells[1]->Value->ToString()))
				{
					this->dataGridView1->Rows[i]->Visible = true;
				}
			}
			else{
				Console::WriteLine(textBox1->Text);
				if (!state->IsMatch(dataGridView1->Rows[i]->Cells[1]->Value->ToString()))
				{
					this->dataGridView1->Rows[i]->Visible = false;
				}
				// Console::WriteLine("Test");
			}
		}
	}

	private:  void addDataClicked(System::Object^  sender, System::EventArgs^  e) {
		srand((unsigned)time(NULL));
		int n = rand();
		
		for (int i = 0; i < (n % 20) + 1; i++) {
			this->addRow(gcnew array < String^ > {
				"Pe***,Br***", "Pennell, Brian", "20140422", "20140805", "09/20/2014", "10/29/2014", ""
			});
		}
		cout << dataGridView1->Rows->Count << endl;
	}

	private: System::Void VisibilityChanged(System::Object^  sender, System::EventArgs^  e) {
		if (sender->Equals(this->privateModeToolStripMenuItem1)){
			this->publicModeToolStripMenuItem->Checked = false;
			this->privateModeToolStripMenuItem1->Checked = true;
			this->dataGridView1->Columns[0]->Visible = false;
			this->dataGridView1->Columns[1]->Visible = true;
		}
		else{
			this->privateModeToolStripMenuItem1->Checked = false;
			this->publicModeToolStripMenuItem->Checked = true;
			this->dataGridView1->Columns[0]->Visible = true;
			this->dataGridView1->Columns[1]->Visible = false;
		}
	}
			 //Seperate into seperate Time calculate file if possible ***************************************
			 DateTime^ calculateDate(System::String^ s){
				 try{
					 int month = Convert::ToInt32(s->Substring(0, 2));
					 int day = Convert::ToInt32(s->Substring(3, 2));
					 int year = Convert::ToInt32(s->Substring(6, 4));
					 return gcnew DateTime(year, month, day);
				 }
				 catch (Exception^ e){
					 return nullptr;
				 }

			 }
			 void checkAll(){
				 for (int i = 0; i <= dataGridView1->Rows->Count - 2; i++)
				 {
					 checkTests(dataGridView1, i);
				 }
			 }
			 //add rows using datagridview add method
			 //pass grid reference, get latest row, check tests
			 void checkTests(DataGridView^ grid, int row_number)
			 {
				 //todays date
				 DateTime^ today = gcnew DateTime();
				 today = today->Today;
				 for (int i = 0; i < 3; i++)
				 {
					 System::Object^ dateValue = grid->Rows[row_number]->Cells[i + 4]->Value;
					 if (dateValue == "")
					 {
						 grid->Rows[row_number]->Cells[i + 4]->Style->ForeColor = System::Drawing::Color::Red;
						 grid->Rows[row_number]->Cells[i + 4]->Style->BackColor = System::Drawing::Color::Red;
					 }
					 else{
						 DateTime^ date = calculateDate(dateValue->ToString());
						 if (date == nullptr)
						 {
							 grid->Rows[row_number]->Cells[i + 4]->Style->ForeColor = System::Drawing::Color::Red;
							 grid->Rows[row_number]->Cells[i + 4]->Style->BackColor = System::Drawing::Color::Red;
						 }
						 else{

							 TimeSpan^ s = today->Subtract(*date);
							 TimeSpan^ length = safe_cast<TimeSpan^>(this->TestArray[i, 1]);
							 if (length->CompareTo(s) == 1) //length is shorter than s.
							 {
								 grid->Rows[row_number]->Cells[i + 4]->Style->ForeColor = System::Drawing::Color::Green;
								 grid->Rows[row_number]->Cells[i + 4]->Style->BackColor = System::Drawing::Color::Green;
							 }

							 else{ //Exists but not in range.
								 grid->Rows[row_number]->Cells[i + 4]->Style->ForeColor = System::Drawing::Color::Yellow;
								 grid->Rows[row_number]->Cells[i + 4]->Style->BackColor = System::Drawing::Color::Yellow;
							 }
						 }
					 }
				 }
			 }
			 //******************************************************************************************************
	};
}