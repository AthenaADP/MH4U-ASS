#pragma once
#include "CharmDatabase.h"
#include "Armor.h"
#include "Crypto/SaveData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MH4GASS {

	public ref class ImportCharms : public System::Windows::Forms::Form
	{
		List_t< Charm^ > charms;
		ColumnHeader^  columnHeader1;

		Button^  btnSelectNone;
		Button^  btnSelectBest;
		Button^  btnSort;

	public:
		unsigned language;

		ImportCharms(void)
		{
			InitializeComponent();

			listView1->Columns[ 0 ]->Width = -1;
			
			DialogResult = System::Windows::Forms::DialogResult::Cancel;

			Text = StaticString( ImportCharmsFromSaveData );
			btnOK->Text = StaticString( Import );
			btnSelectBest->Text = StaticString( SelectBest );
			btnSelectNone->Text = StaticString( SelectNone );
			btnCancel->Text = StaticString( Cancel );
			btnSort->Text = StaticString( Sort );
			chkDeleteExisting->Text = StaticString( DeleteExistingCharms );
			grpCharms->Text = StaticString( Charms );
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ImportCharms()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::GroupBox^  grpCharms;
	private: System::Windows::Forms::CheckBox^  chkDeleteExisting;
	private: System::Windows::Forms::GroupBox^  groupBox2;




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
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->grpCharms = (gcnew System::Windows::Forms::GroupBox());
			this->chkDeleteExisting = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->btnSelectBest = (gcnew System::Windows::Forms::Button());
			this->btnSelectNone = (gcnew System::Windows::Forms::Button());
			this->btnSort = (gcnew System::Windows::Forms::Button());
			this->grpCharms->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(87, 42);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 0;
			this->btnOK->Text = L"&Import";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ImportCharms::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(87, 71);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->TabIndex = 1;
			this->btnCancel->Text = L"&Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &ImportCharms::btnCancel_Click);
			// 
			// listView1
			// 
			this->listView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->listView1->CheckBoxes = true;
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->columnHeader1});
			this->listView1->Location = System::Drawing::Point(6, 19);
			this->listView1->MultiSelect = false;
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(335, 293);
			this->listView1->TabIndex = 2;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::List;
			// 
			// grpCharms
			// 
			this->grpCharms->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->grpCharms->Controls->Add(this->listView1);
			this->grpCharms->Location = System::Drawing::Point(12, 8);
			this->grpCharms->Name = L"grpCharms";
			this->grpCharms->Size = System::Drawing::Size(347, 318);
			this->grpCharms->TabIndex = 3;
			this->grpCharms->TabStop = false;
			this->grpCharms->Text = L"Charms";
			// 
			// chkDeleteExisting
			// 
			this->chkDeleteExisting->AutoSize = true;
			this->chkDeleteExisting->Checked = true;
			this->chkDeleteExisting->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkDeleteExisting->Location = System::Drawing::Point(6, 19);
			this->chkDeleteExisting->Name = L"chkDeleteExisting";
			this->chkDeleteExisting->Size = System::Drawing::Size(134, 17);
			this->chkDeleteExisting->TabIndex = 4;
			this->chkDeleteExisting->Text = L"Delete Existing Charms";
			this->chkDeleteExisting->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->groupBox2->Controls->Add(this->btnSort);
			this->groupBox2->Controls->Add(this->btnSelectBest);
			this->groupBox2->Controls->Add(this->btnSelectNone);
			this->groupBox2->Controls->Add(this->chkDeleteExisting);
			this->groupBox2->Controls->Add(this->btnOK);
			this->groupBox2->Controls->Add(this->btnCancel);
			this->groupBox2->Location = System::Drawing::Point(365, 8);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(174, 129);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			// 
			// btnSelectBest
			// 
			this->btnSelectBest->Location = System::Drawing::Point(6, 71);
			this->btnSelectBest->Name = L"btnSelectBest";
			this->btnSelectBest->Size = System::Drawing::Size(75, 23);
			this->btnSelectBest->TabIndex = 6;
			this->btnSelectBest->Text = L"Select &Best";
			this->btnSelectBest->UseVisualStyleBackColor = true;
			this->btnSelectBest->Click += gcnew System::EventHandler(this, &ImportCharms::btnSelectBest_Click);
			// 
			// btnSelectNone
			// 
			this->btnSelectNone->Location = System::Drawing::Point(6, 42);
			this->btnSelectNone->Name = L"btnSelectNone";
			this->btnSelectNone->Size = System::Drawing::Size(75, 23);
			this->btnSelectNone->TabIndex = 5;
			this->btnSelectNone->Text = L"Select &None";
			this->btnSelectNone->UseVisualStyleBackColor = true;
			this->btnSelectNone->Click += gcnew System::EventHandler(this, &ImportCharms::btnSelectNone_Click);
			// 
			// btnSort
			// 
			this->btnSort->Location = System::Drawing::Point(6, 100);
			this->btnSort->Name = L"btnSort";
			this->btnSort->Size = System::Drawing::Size(75, 23);
			this->btnSort->TabIndex = 7;
			this->btnSort->Text = L"&Sort";
			this->btnSort->UseVisualStyleBackColor = true;
			this->btnSort->Click += gcnew System::EventHandler(this, &ImportCharms::btnSort_Click);
			// 
			// ImportCharms
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(551, 338);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->grpCharms);
			this->Name = L"ImportCharms";
			this->Text = L"Import Charms";
			this->grpCharms->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	
		void btnCancel_Click(System::Object^  sender, System::EventArgs^  e)
		{
			Close();
		}

		void AddCharms()
		{
			for( int i = 0; i < charms.Count; ++i )
			{
				if( listView1->Items[ i ]->Checked && !charms[ i ]->hacked )
				{
					charms[ i ]->custom = true;
					CharmDatabase::mycharms.Add( charms[ i ] );
				}
			}
		}

		void btnOK_Click(System::Object^  sender, System::EventArgs^  e)
		{
			DialogResult = System::Windows::Forms::DialogResult::OK;

			if( chkDeleteExisting )
				CharmDatabase::mycharms.Clear();

			listView1->Enabled = false;

			AddCharms();

			CharmDatabase::SaveCustom();

			Close();
		}

public:
		void RefreshCharmList()
		{
			listView1->BeginUpdate();
			listView1->Clear();
			for each( Charm^ charm in charms )
			{
				listView1->Items->Add( charm->GetName() );
			}
			listView1->EndUpdate();
			//select best ones by default
			btnSelectBest_Click( nullptr, nullptr );
		}

		bool DataIsEncrypted( array< unsigned char >^ data )
		{
			if( data->Length < 0x110 )
				return false;

			unsigned num_zeros = 0;
			for( unsigned i = 0; i < 0x100; ++i )
				num_zeros += data[i] == 0;

			return num_zeros < 50;
		}

		void LoadCharms( String^ filename )
		{
			if( filename->EndsWith( L"mycharms.txt" ) )
			{
				charms.AddRange( CharmDatabase::LoadCharms( filename ) );
			}
			else
			{
				array< unsigned char >^ encrypted = IO::File::ReadAllBytes( filename );
				array< unsigned char >^ decrypted;

				if( encrypted->Length % 8 != 0 )
					return;

				if( DataIsEncrypted( encrypted ) )
					decrypted = DecryptSaveData( encrypted );
				else
					decrypted = encrypted;

				charms.AddRange( ExtractCharms( decrypted ) );
			}
			
			RefreshCharmList();
		}

		System::Void btnSelectNone_Click(System::Object^  sender, System::EventArgs^  e)
		{
			listView1->BeginUpdate();
			for each( ListViewItem^ item in listView1->Items )
				item->Checked = false;
			listView1->EndUpdate();
		}

		System::Void btnSelectBest_Click(System::Object^  sender, System::EventArgs^  e)
		{
			listView1->BeginUpdate();

			for each( Ability^ a in Ability::static_abilities )
			{
				a->relevant = true;
			}
			for( int i = 0; i < charms.Count; ++i )
			{
				if( charms[ i ]->hacked )
				{
					listView1->Items[ i ]->ForeColor = Color::Red;
					continue;
				}
				else 
					listView1->Items[ i ]->Checked = true;
				for( int j = 0; j < i; j++ )
				{
					if( listView1->Items[ j ]->Checked )
					{
						if( charms[ i ]->StrictlyBetterThan( charms[ j ] ) )
							listView1->Items[ j ]->Checked = false;
						else if( charms[ j ]->StrictlyBetterThan( charms[ i ] ) ||
							charms[ i ] == charms[ j ] )
							listView1->Items[ i ]->Checked = false;
					}
				}
			}
			listView1->EndUpdate();
		}

		System::Void listView1_ItemChecked(System::Object^  sender, ItemCheckEventArgs^  e)
		{
			if( charms[ e->Index ]->hacked )
				e->NewValue = e->CurrentValue;	
		}

		System::Void btnSort_Click(System::Object^  sender, System::EventArgs^  e)
		{
			btnSort->Enabled = false;
			charms.Sort( gcnew Comparison< Charm^ >( CompareCharms1 ) );
			RefreshCharmList();
		}
};
}
