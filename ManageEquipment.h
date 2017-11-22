#pragma once
#include "Common.h"
#include "Armor.h"
#include "CharmDatabase.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MH4GASS {

	public ref class ManageEquipment : public System::Windows::Forms::Form
	{
		Generic::List< Ability^ > wep_abilities, arm_abilities;
		Generic::List< unsigned > wep_max, arm_max;
		unsigned weapon_max, armor_max;
		bool listening_to_list;
		Ability^ last_ability;
		
		static const int minmaxres = 8;
	public:
		ManageEquipment(void)
			: listening_to_list( false )
		{
			InitializeComponent();

			SetLanguage();
			
			SetDefaultValues();
			AddAbilities();
			AddGear();
			UpdateEnables();

			listening_to_list = true;
		}

	private:

		void SetLanguage()
		{
			this->Text = StaticString( ManageEquipment );
			btnAddNew->Text = StaticString( AddNewCharm );
			btnDelete->Text = StaticString( DeleteCharm );
			btnSave->Text = StaticString( SaveCharms );
			btnTrim->Text = StaticString( TrimCharms );
			grpEquipment->Text = StaticString( Equipment );
			lblSkill->Text = StaticString( Skill );
			lblHunterType->Text = StaticString( HunterType );
			lblItemType->Text = StaticString( ItemType );
			lblDefence->Text = StaticString( Defence );
			lblIceRes->Text = StaticString( IceRes );
			lblFireRes->Text = StaticString( FireRes );
			lblWaterRes->Text = StaticString( WaterRes );
			lblDragonRes->Text = StaticString( DragonRes );
			lblThunderRes->Text = StaticString( ThunderRes );
			cmbHunterType->Items->Add( StaticString( Blademaster ) );
			cmbHunterType->Items->Add( StaticString( Gunner ) );
			cmbHunterType->Items->Add( StaticString( BothHunterTypes ) );
			cmbItemType->Items->Add( StaticString( Weapon ) );
			cmbItemType->Items->Add( StaticString( Helm ) );
			cmbItemType->Items->Add( StaticString( Torso ) );
			cmbItemType->Items->Add( StaticString( Arms ) );
			cmbItemType->Items->Add( StaticString( Waist ) );
			cmbItemType->Items->Add( StaticString( Legs ) );
		}

		void SetDefaultValues()
		{
			txtDefence->Text = L"0";

			for( int i = -minmaxres; i <= minmaxres; i++ )
			{
				String^ num = ( i <= 0 ? L"" : L"+" ) + Convert::ToString( i );
				cmbFireRes->Items->Add( num );
				cmbWaterRes->Items->Add( num );
				cmbIceRes->Items->Add( num );
				cmbThunderRes->Items->Add( num );
				cmbDragonRes->Items->Add( num );
			}

			cmbFireRes->SelectedIndex = minmaxres;
			cmbWaterRes->SelectedIndex = minmaxres;
			cmbIceRes->SelectedIndex = minmaxres;
			cmbThunderRes->SelectedIndex = minmaxres;
			cmbDragonRes->SelectedIndex = minmaxres;
		}

		void AddGear()
		{
			lstEquipment->BeginUpdate();

			lstEquipment->Items->Clear();
			if( ExcavatedGear::armor == nullptr )
			{
				ExcavatedGear::armor = gcnew array< List_t< Armor^ >^ >( 5 );
				for( int i = 0; i < 5; ++i )
					ExcavatedGear::armor[ i ] = gcnew List_t< Armor^ >();
			}

			for each( Weapon^ weapon in ExcavatedGear::weapons )
			{
				lstEquipment->Items->Add( weapon->GetName() );
			}
			
			for each( List_t< Armor^ >^ lst in ExcavatedGear::armor )
			{
				for each( Armor^ armor in lst )
				{
					lstEquipment->Items->Add( armor->name );
				}
			}

			lstEquipment->EndUpdate();

			CalcCanTrim();
		}

		void AddWeapon( Weapon^ nu )
		{
			int index = ExcavatedGear::weapons.Count;
			for( int i = 0; i < ExcavatedGear::weapons.Count; ++i )
			{
				Weapon^ weapon = ExcavatedGear::weapons[ i ];
				if( weapon->ability->static_index > nu->ability->static_index ||
					weapon->ability->static_index == nu->ability->static_index && weapon->ability_amount > nu->ability_amount )
				{
					index = i;
					break;
				}
			}
			lstEquipment->BeginUpdate();
			lstEquipment->Items->Insert( index, nu->GetName() );
			listening_to_list = false;
			lstEquipment->SelectedIndex = index;
			listening_to_list = true;
			lstEquipment->EndUpdate();
			ExcavatedGear::weapons.Insert( index, nu );
			ExcavatedGear::SaveCustom();
		}

		void AddArmor( Armor^ nu, const int armor_type )
		{
			int list_index = ExcavatedGear::weapons.Count;
			for( int i = 0; i <= armor_type; ++i )
				list_index += ExcavatedGear::armor[ i ]->Count;
			int index = ExcavatedGear::armor[ armor_type ]->Count;

			for( int i = 0; i < ExcavatedGear::armor[ armor_type ]->Count; ++i )
			{
				Armor^ armor = ExcavatedGear::armor[ armor_type ][ i ];
				if( armor->abilities[ 0 ]->ability->static_index > nu->abilities[ 0 ]->ability->static_index ||
					armor->abilities[ 0 ]->ability->static_index == nu->abilities[ 0 ]->ability->static_index && armor->abilities[ 0 ]->amount > nu->abilities[ 0 ]->amount )
				{
					index = i;
					list_index += i - ExcavatedGear::armor[ armor_type ]->Count;
					break;
				}
			}
			lstEquipment->BeginUpdate();
			lstEquipment->Items->Insert( list_index, nu->name );
			listening_to_list = false;
			lstEquipment->SelectedIndex = list_index;
			listening_to_list = true;
			lstEquipment->EndUpdate();
			ExcavatedGear::armor[ armor_type ]->Insert( index, nu );
			ExcavatedGear::SaveCustom();
		}

		bool CanTrimArmor( List_t< Armor^ >^ lst )
		{
			for( int i = 0; i < lst->Count; ++i )
			{
				if( lst[ i ]->abilities.Count != 1 )
					continue;

				for( int j = i + 1; j < lst->Count; ++j )
				{
					if( lst[ j ]->abilities.Count != 1 )
						continue;

					if( lst[ j ]->abilities[ 0 ]->ability == lst[ i ]->abilities[ 0 ]->ability &&
						lst[ j ]->abilities[ 0 ]->amount >= lst[ i ]->abilities[ 0 ]->amount )
					{
						return true;
					}
				}
			}
			return false;
		}

		void CalcCanTrim()
		{
			for( int i = 0; i < ExcavatedGear::weapons.Count; ++i )
			{
				for( int j = i + 1; j < ExcavatedGear::weapons.Count; ++j )
				{
					if( ExcavatedGear::weapons[ j ]->type == ExcavatedGear::weapons[ i ]->type &&
						ExcavatedGear::weapons[ j ]->ability == ExcavatedGear::weapons[ i ]->ability &&
						ExcavatedGear::weapons[ j ]->ability_amount >= ExcavatedGear::weapons[ i ]->ability_amount )
					{
						btnTrim->Enabled = true;
						return;
					}
				}
			}
			for( int i = 0; i < 5; ++i )
			{
				if( CanTrimArmor( ExcavatedGear::armor[ i ] ) )
				{
					btnTrim->Enabled = true;
					return;
				}
			}
			btnTrim->Enabled = false;
		}

		void AddAbilities()
		{
			wep_abilities.Clear();
			arm_abilities.Clear();
			wep_max.Clear();
			arm_max.Clear();
			weapon_max = armor_max = 0;

			for each( Ability^ ability in Ability::static_abilities )
			{
				if( ability->excavated_wep_max > 0 )
				{
					wep_abilities.Add( ability );
					wep_max.Add( ability->excavated_wep_max );
					weapon_max = Math::Max( weapon_max, ability->excavated_wep_max );
				}

				if( ability->excavated_arm_max > 0 )
				{
					arm_abilities.Add( ability );
					arm_max.Add( ability->excavated_arm_max );
					armor_max = Math::Max( armor_max, ability->excavated_arm_max );
				}
			}
		}

	protected:

		~ManageEquipment()
		{
			if( components )
			{
				delete components;
			}
		}
#pragma region Components
	private: System::Windows::Forms::GroupBox^  grpEquipment;
	private: System::Windows::Forms::ListBox^  lstEquipment;
	private: System::Windows::Forms::GroupBox^  grpDetails;
	private: System::Windows::Forms::ComboBox^  cmbItemType;
	private: System::Windows::Forms::Label^  lblHunterType;
	private: System::Windows::Forms::ComboBox^  cmbHunterType;
	private: System::Windows::Forms::ComboBox^  cmbAbility;
	private: System::Windows::Forms::ComboBox^  cmbSkillPoint;
	private: System::Windows::Forms::Label^  lblItemType;
	private: System::Windows::Forms::ComboBox^  cmbDragonRes;
	private: System::Windows::Forms::ComboBox^  cmbIceRes;
	private: System::Windows::Forms::ComboBox^  cmbWaterRes;
	private: System::Windows::Forms::ComboBox^  cmbThunderRes;
	private: System::Windows::Forms::ComboBox^  cmbFireRes;
	private: System::Windows::Forms::TextBox^  txtDefence;
	private: System::Windows::Forms::Label^  lblDragonRes;
	private: System::Windows::Forms::Label^  lblIceRes;
	private: System::Windows::Forms::Label^  lblThunderRes;
	private: System::Windows::Forms::Label^  lblFireRes;
	private: System::Windows::Forms::Label^  lblWaterRes;
	private: System::Windows::Forms::Label^  lblDefence;
	private: System::Windows::Forms::Label^  lblSkill;
	private: System::Windows::Forms::Button^  btnSave;
	private: System::Windows::Forms::Button^  btnAddNew;
	private: System::Windows::Forms::Button^  btnTrim;
	private: System::Windows::Forms::Button^  btnDelete;

	private:

		System::ComponentModel::Container ^components;
#pragma endregion

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->grpEquipment = (gcnew System::Windows::Forms::GroupBox());
			this->lstEquipment = (gcnew System::Windows::Forms::ListBox());
			this->grpDetails = (gcnew System::Windows::Forms::GroupBox());
			this->lblHunterType = (gcnew System::Windows::Forms::Label());
			this->btnTrim = (gcnew System::Windows::Forms::Button());
			this->cmbHunterType = (gcnew System::Windows::Forms::ComboBox());
			this->btnSave = (gcnew System::Windows::Forms::Button());
			this->btnDelete = (gcnew System::Windows::Forms::Button());
			this->btnAddNew = (gcnew System::Windows::Forms::Button());
			this->lblDragonRes = (gcnew System::Windows::Forms::Label());
			this->lblIceRes = (gcnew System::Windows::Forms::Label());
			this->lblThunderRes = (gcnew System::Windows::Forms::Label());
			this->lblFireRes = (gcnew System::Windows::Forms::Label());
			this->lblWaterRes = (gcnew System::Windows::Forms::Label());
			this->lblDefence = (gcnew System::Windows::Forms::Label());
			this->lblSkill = (gcnew System::Windows::Forms::Label());
			this->lblItemType = (gcnew System::Windows::Forms::Label());
			this->cmbDragonRes = (gcnew System::Windows::Forms::ComboBox());
			this->cmbIceRes = (gcnew System::Windows::Forms::ComboBox());
			this->cmbWaterRes = (gcnew System::Windows::Forms::ComboBox());
			this->cmbThunderRes = (gcnew System::Windows::Forms::ComboBox());
			this->cmbFireRes = (gcnew System::Windows::Forms::ComboBox());
			this->txtDefence = (gcnew System::Windows::Forms::TextBox());
			this->cmbAbility = (gcnew System::Windows::Forms::ComboBox());
			this->cmbSkillPoint = (gcnew System::Windows::Forms::ComboBox());
			this->cmbItemType = (gcnew System::Windows::Forms::ComboBox());
			this->grpEquipment->SuspendLayout();
			this->grpDetails->SuspendLayout();
			this->SuspendLayout();
			// 
			// grpEquipment
			// 
			this->grpEquipment->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->grpEquipment->Controls->Add(this->lstEquipment);
			this->grpEquipment->Location = System::Drawing::Point(12, 12);
			this->grpEquipment->Name = L"grpEquipment";
			this->grpEquipment->Size = System::Drawing::Size(353, 321);
			this->grpEquipment->TabIndex = 0;
			this->grpEquipment->TabStop = false;
			this->grpEquipment->Text = L"Equipment";
			// 
			// lstEquipment
			// 
			this->lstEquipment->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->lstEquipment->FormattingEnabled = true;
			this->lstEquipment->Location = System::Drawing::Point(9, 16);
			this->lstEquipment->Name = L"lstEquipment";
			this->lstEquipment->Size = System::Drawing::Size(338, 290);
			this->lstEquipment->TabIndex = 0;
			this->lstEquipment->SelectedIndexChanged += gcnew System::EventHandler(this, &ManageEquipment::lstEquipment_SelectedIndexChanged);
			// 
			// grpDetails
			// 
			this->grpDetails->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->grpDetails->Controls->Add(this->lblHunterType);
			this->grpDetails->Controls->Add(this->btnTrim);
			this->grpDetails->Controls->Add(this->cmbHunterType);
			this->grpDetails->Controls->Add(this->btnSave);
			this->grpDetails->Controls->Add(this->btnDelete);
			this->grpDetails->Controls->Add(this->btnAddNew);
			this->grpDetails->Controls->Add(this->lblDragonRes);
			this->grpDetails->Controls->Add(this->lblIceRes);
			this->grpDetails->Controls->Add(this->lblThunderRes);
			this->grpDetails->Controls->Add(this->lblFireRes);
			this->grpDetails->Controls->Add(this->lblWaterRes);
			this->grpDetails->Controls->Add(this->lblDefence);
			this->grpDetails->Controls->Add(this->lblSkill);
			this->grpDetails->Controls->Add(this->lblItemType);
			this->grpDetails->Controls->Add(this->cmbDragonRes);
			this->grpDetails->Controls->Add(this->cmbIceRes);
			this->grpDetails->Controls->Add(this->cmbWaterRes);
			this->grpDetails->Controls->Add(this->cmbThunderRes);
			this->grpDetails->Controls->Add(this->cmbFireRes);
			this->grpDetails->Controls->Add(this->txtDefence);
			this->grpDetails->Controls->Add(this->cmbAbility);
			this->grpDetails->Controls->Add(this->cmbSkillPoint);
			this->grpDetails->Controls->Add(this->cmbItemType);
			this->grpDetails->Location = System::Drawing::Point(371, 12);
			this->grpDetails->Name = L"grpDetails";
			this->grpDetails->Size = System::Drawing::Size(259, 321);
			this->grpDetails->TabIndex = 1;
			this->grpDetails->TabStop = false;
			// 
			// lblHunterType
			// 
			this->lblHunterType->AutoSize = true;
			this->lblHunterType->Location = System::Drawing::Point(6, 70);
			this->lblHunterType->Name = L"lblHunterType";
			this->lblHunterType->Size = System::Drawing::Size(66, 13);
			this->lblHunterType->TabIndex = 5;
			this->lblHunterType->Text = L"Hunter Type";
			// 
			// btnTrim
			// 
			this->btnTrim->Enabled = false;
			this->btnTrim->Location = System::Drawing::Point(87, 290);
			this->btnTrim->Name = L"btnTrim";
			this->btnTrim->Size = System::Drawing::Size(75, 23);
			this->btnTrim->TabIndex = 22;
			this->btnTrim->Text = L"&Trim";
			this->btnTrim->UseVisualStyleBackColor = true;
			this->btnTrim->Click += gcnew System::EventHandler(this, &ManageEquipment::btnTrim_Click);
			// 
			// cmbHunterType
			// 
			this->cmbHunterType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbHunterType->FormattingEnabled = true;
			this->cmbHunterType->Location = System::Drawing::Point(88, 67);
			this->cmbHunterType->Name = L"cmbHunterType";
			this->cmbHunterType->Size = System::Drawing::Size(121, 21);
			this->cmbHunterType->TabIndex = 6;
			this->cmbHunterType->SelectedIndexChanged += gcnew System::EventHandler(this, &ManageEquipment::cmbHunterType_SelectedIndexChanged);
			// 
			// btnSave
			// 
			this->btnSave->Location = System::Drawing::Point(87, 261);
			this->btnSave->Name = L"btnSave";
			this->btnSave->Size = System::Drawing::Size(75, 23);
			this->btnSave->TabIndex = 20;
			this->btnSave->Text = L"&Save";
			this->btnSave->UseVisualStyleBackColor = true;
			this->btnSave->Click += gcnew System::EventHandler(this, &ManageEquipment::btnSave_Click);
			// 
			// btnDelete
			// 
			this->btnDelete->Enabled = false;
			this->btnDelete->Location = System::Drawing::Point(6, 290);
			this->btnDelete->Name = L"btnDelete";
			this->btnDelete->Size = System::Drawing::Size(75, 23);
			this->btnDelete->TabIndex = 21;
			this->btnDelete->Text = L"&Delete";
			this->btnDelete->UseVisualStyleBackColor = true;
			this->btnDelete->Click += gcnew System::EventHandler(this, &ManageEquipment::btnDelete_Click);
			// 
			// btnAddNew
			// 
			this->btnAddNew->Location = System::Drawing::Point(6, 261);
			this->btnAddNew->Name = L"btnAddNew";
			this->btnAddNew->Size = System::Drawing::Size(75, 23);
			this->btnAddNew->TabIndex = 19;
			this->btnAddNew->Text = L"&Add New";
			this->btnAddNew->UseVisualStyleBackColor = true;
			this->btnAddNew->Click += gcnew System::EventHandler(this, &ManageEquipment::btnAddNew_Click);
			// 
			// lblDragonRes
			// 
			this->lblDragonRes->AutoSize = true;
			this->lblDragonRes->Location = System::Drawing::Point(6, 232);
			this->lblDragonRes->Name = L"lblDragonRes";
			this->lblDragonRes->Size = System::Drawing::Size(64, 13);
			this->lblDragonRes->TabIndex = 17;
			this->lblDragonRes->Text = L"Dragon Res";
			// 
			// lblIceRes
			// 
			this->lblIceRes->AutoSize = true;
			this->lblIceRes->Location = System::Drawing::Point(6, 205);
			this->lblIceRes->Name = L"lblIceRes";
			this->lblIceRes->Size = System::Drawing::Size(44, 13);
			this->lblIceRes->TabIndex = 15;
			this->lblIceRes->Text = L"Ice Res";
			// 
			// lblThunderRes
			// 
			this->lblThunderRes->AutoSize = true;
			this->lblThunderRes->Location = System::Drawing::Point(6, 178);
			this->lblThunderRes->Name = L"lblThunderRes";
			this->lblThunderRes->Size = System::Drawing::Size(69, 13);
			this->lblThunderRes->TabIndex = 13;
			this->lblThunderRes->Text = L"Thunder Res";
			// 
			// lblFireRes
			// 
			this->lblFireRes->AutoSize = true;
			this->lblFireRes->Location = System::Drawing::Point(6, 124);
			this->lblFireRes->Name = L"lblFireRes";
			this->lblFireRes->Size = System::Drawing::Size(46, 13);
			this->lblFireRes->TabIndex = 9;
			this->lblFireRes->Text = L"Fire Res";
			// 
			// lblWaterRes
			// 
			this->lblWaterRes->AutoSize = true;
			this->lblWaterRes->Location = System::Drawing::Point(6, 151);
			this->lblWaterRes->Name = L"lblWaterRes";
			this->lblWaterRes->Size = System::Drawing::Size(58, 13);
			this->lblWaterRes->TabIndex = 11;
			this->lblWaterRes->Text = L"Water Res";
			// 
			// lblDefence
			// 
			this->lblDefence->AutoSize = true;
			this->lblDefence->Location = System::Drawing::Point(6, 98);
			this->lblDefence->Name = L"lblDefence";
			this->lblDefence->Size = System::Drawing::Size(48, 13);
			this->lblDefence->TabIndex = 7;
			this->lblDefence->Text = L"Defence";
			// 
			// lblSkill
			// 
			this->lblSkill->AutoSize = true;
			this->lblSkill->Location = System::Drawing::Point(6, 43);
			this->lblSkill->Name = L"lblSkill";
			this->lblSkill->Size = System::Drawing::Size(26, 13);
			this->lblSkill->TabIndex = 2;
			this->lblSkill->Text = L"Skill";
			// 
			// lblItemType
			// 
			this->lblItemType->AutoSize = true;
			this->lblItemType->Location = System::Drawing::Point(6, 16);
			this->lblItemType->Name = L"lblItemType";
			this->lblItemType->Size = System::Drawing::Size(54, 13);
			this->lblItemType->TabIndex = 0;
			this->lblItemType->Text = L"Item Type";
			// 
			// cmbDragonRes
			// 
			this->cmbDragonRes->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbDragonRes->FormattingEnabled = true;
			this->cmbDragonRes->Location = System::Drawing::Point(88, 229);
			this->cmbDragonRes->Name = L"cmbDragonRes";
			this->cmbDragonRes->Size = System::Drawing::Size(41, 21);
			this->cmbDragonRes->TabIndex = 18;
			// 
			// cmbIceRes
			// 
			this->cmbIceRes->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbIceRes->FormattingEnabled = true;
			this->cmbIceRes->Location = System::Drawing::Point(88, 202);
			this->cmbIceRes->Name = L"cmbIceRes";
			this->cmbIceRes->Size = System::Drawing::Size(41, 21);
			this->cmbIceRes->TabIndex = 16;
			// 
			// cmbWaterRes
			// 
			this->cmbWaterRes->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbWaterRes->FormattingEnabled = true;
			this->cmbWaterRes->Location = System::Drawing::Point(88, 148);
			this->cmbWaterRes->Name = L"cmbWaterRes";
			this->cmbWaterRes->Size = System::Drawing::Size(41, 21);
			this->cmbWaterRes->TabIndex = 12;
			// 
			// cmbThunderRes
			// 
			this->cmbThunderRes->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbThunderRes->FormattingEnabled = true;
			this->cmbThunderRes->Location = System::Drawing::Point(88, 175);
			this->cmbThunderRes->Name = L"cmbThunderRes";
			this->cmbThunderRes->Size = System::Drawing::Size(41, 21);
			this->cmbThunderRes->TabIndex = 14;
			// 
			// cmbFireRes
			// 
			this->cmbFireRes->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbFireRes->FormattingEnabled = true;
			this->cmbFireRes->Location = System::Drawing::Point(88, 121);
			this->cmbFireRes->Name = L"cmbFireRes";
			this->cmbFireRes->Size = System::Drawing::Size(41, 21);
			this->cmbFireRes->TabIndex = 10;
			// 
			// txtDefence
			// 
			this->txtDefence->Location = System::Drawing::Point(88, 95);
			this->txtDefence->Name = L"txtDefence";
			this->txtDefence->Size = System::Drawing::Size(41, 20);
			this->txtDefence->TabIndex = 8;
			this->txtDefence->TextChanged += gcnew System::EventHandler(this, &ManageEquipment::txtDefence_TextChanged);
			// 
			// cmbAbility
			// 
			this->cmbAbility->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->cmbAbility->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->cmbAbility->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbAbility->FormattingEnabled = true;
			this->cmbAbility->Location = System::Drawing::Point(134, 40);
			this->cmbAbility->Name = L"cmbAbility";
			this->cmbAbility->Size = System::Drawing::Size(121, 21);
			this->cmbAbility->TabIndex = 4;
			this->cmbAbility->SelectedIndexChanged += gcnew System::EventHandler(this, &ManageEquipment::cmbAbility_SelectedIndexChanged);
			// 
			// cmbSkillPoint
			// 
			this->cmbSkillPoint->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbSkillPoint->FormattingEnabled = true;
			this->cmbSkillPoint->Location = System::Drawing::Point(88, 40);
			this->cmbSkillPoint->Name = L"cmbSkillPoint";
			this->cmbSkillPoint->Size = System::Drawing::Size(41, 21);
			this->cmbSkillPoint->TabIndex = 3;
			this->cmbSkillPoint->SelectedIndexChanged += gcnew System::EventHandler(this, &ManageEquipment::cmbSkillPoint_SelectedIndexChanged);
			// 
			// cmbItemType
			// 
			this->cmbItemType->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->cmbItemType->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->cmbItemType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbItemType->FormattingEnabled = true;
			this->cmbItemType->Location = System::Drawing::Point(88, 13);
			this->cmbItemType->Name = L"cmbItemType";
			this->cmbItemType->Size = System::Drawing::Size(121, 21);
			this->cmbItemType->TabIndex = 1;
			this->cmbItemType->SelectedIndexChanged += gcnew System::EventHandler(this, &ManageEquipment::cmbItemType_SelectedIndexChanged);
			// 
			// ManageEquipment
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(642, 345);
			this->Controls->Add(this->grpDetails);
			this->Controls->Add(this->grpEquipment);
			this->Name = L"ManageEquipment";
			this->Text = L"Manage Equipment";
			this->grpEquipment->ResumeLayout(false);
			this->grpDetails->ResumeLayout(false);
			this->grpDetails->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	System::Void lstEquipment_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( !listening_to_list || lstEquipment->SelectedIndex < 0 )
			return;

		if( lstEquipment->SelectedIndex < ExcavatedGear::weapons.Count )
		{
			Weapon^ wep = ExcavatedGear::weapons[ lstEquipment->SelectedIndex ];
			cmbItemType->SelectedIndex = 0;
			SetSelectedHunterType( wep->type );
			for( int i = 0; i < wep_abilities.Count; ++i )
			{
				if( wep_abilities[ i ] == wep->ability )
				{
					cmbAbility->SelectedIndex = i;
					break;
				}
			}
			cmbSkillPoint->SelectedIndex = wep->ability_amount - 1;
			txtDefence->Text = Convert::ToString( wep->defence );
		}
		else
		{
			int index = lstEquipment->SelectedIndex - ExcavatedGear::weapons.Count;
			for( int i = 0; i < 5; ++i )
			{
				if( index < ExcavatedGear::armor[ i ]->Count )
				{
					cmbItemType->SelectedIndex = i + 1;

					Armor^ arm = ExcavatedGear::armor[ i ][ index ];
					Assert( arm->abilities.Count == 1, "Excavated armor doesn't have 1 ability!?" );
					for( int a = 0; a < arm_abilities.Count; ++a )
					{
						if( arm_abilities[ a ] == arm->abilities[ 0 ]->ability )
						{
							cmbAbility->SelectedIndex = a;
							break;
						}
					}
					SetSelectedHunterType( arm->type );
					txtDefence->Text = Convert::ToString( arm->defence );
					cmbFireRes->SelectedIndex = arm->fire_res + minmaxres;
					cmbWaterRes->SelectedIndex = arm->water_res + minmaxres;
					cmbIceRes->SelectedIndex = arm->ice_res + minmaxres;
					cmbThunderRes->SelectedIndex = arm->thunder_res + minmaxres;
					cmbDragonRes->SelectedIndex = arm->dragon_res + minmaxres;

					cmbSkillPoint->SelectedIndex = arm->abilities[ 0 ]->amount - 1;
					break;
				}
				else index -= ExcavatedGear::armor[ i ]->Count;
			}
		}
		UpdateEnables();
	}
	
	System::Void txtDefence_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		try
		{
			const unsigned val = Convert::ToUInt32( txtDefence->Text );
		}
		catch( Exception^ )
		{
			txtDefence->Text = L"0";
		}
	}

	System::Void cmbItemType_SelectedIndexChanged( System::Object^ sender, System::EventArgs^ e )
	{
		UpdateEnables();

		const bool enable_def = cmbItemType->SelectedIndex != 0;

		cmbFireRes->Enabled = enable_def;
		cmbWaterRes->Enabled = enable_def;
		cmbIceRes->Enabled = enable_def;
		cmbThunderRes->Enabled = enable_def;
		cmbDragonRes->Enabled = enable_def;

		const HunterType selected_type = GetSelectedHunterType();
		if( cmbItemType->SelectedIndex == 1 ) //helm
		{
			if( cmbHunterType->Items->Count != 1 )
			{
				cmbHunterType->BeginUpdate();
				cmbHunterType->Items->Clear();
				cmbHunterType->Items->Add( StaticString( BothHunterTypes ) );
				cmbHunterType->SelectedIndex = 0;
				cmbHunterType->EndUpdate();
			}
		}
		else //weapon or non-helm armor
		{
			if( cmbHunterType->Items->Count != 2 )
			{
				cmbHunterType->BeginUpdate();
				cmbHunterType->Items->Clear();
				cmbHunterType->Items->Add( StaticString( Blademaster ) );
				cmbHunterType->Items->Add( StaticString( Gunner ) );
				cmbHunterType->EndUpdate();
			}
		}
		SetSelectedHunterType( selected_type );

		//update ability list
		{
			cmbAbility->BeginUpdate();
			cmbAbility->Items->Clear();
			
			Generic::List< Ability^ >^ abs = cmbItemType->SelectedIndex == 0 ? %wep_abilities : %arm_abilities;
			int new_index = -1;
			for( int i = 0; i < abs->Count; ++i )
			{
				if( abs[ i ] == last_ability )
					new_index = i;
				cmbAbility->Items->Add( abs[ i ]->name );
			}
			cmbAbility->SelectedIndex = new_index;

			cmbAbility->EndUpdate();

			//update skill point maximums
			UpdateSkillPoints();
		}
	}

	int GetWepMax()
	{
		return cmbAbility->SelectedIndex < 0 ? weapon_max : wep_max[ cmbAbility->SelectedIndex ];
	}

	int GetArmMax()
	{
		return cmbAbility->SelectedIndex < 0 ? armor_max : arm_max[ cmbAbility->SelectedIndex ];
	}

	HunterType GetSelectedHunterType()
	{
		if( cmbItemType->SelectedIndex == 1 ) //helm
			return HunterType::BOTH_TYPES;
		else
			return (HunterType)( cmbHunterType->SelectedIndex + 1 );
	}

	void SetSelectedHunterType( const HunterType type )
	{
		if( (int)type == 0 )
			cmbHunterType->SelectedIndex = -1;
		else if( cmbItemType->SelectedIndex == 1 ) //helm
			cmbHunterType->SelectedIndex = 0;
		else if( cmbItemType->SelectedIndex == -1 ) //unknown
			cmbHunterType->SelectedIndex = (int)type - 1 ;
		else
			cmbHunterType->SelectedIndex = Math::Min( (int)type - 1, 1 );
	}

	void UpdateSkillPoints()
	{
		const int new_max = cmbItemType->SelectedIndex == 0 ? GetWepMax() : GetArmMax();
		if( new_max != cmbSkillPoint->Items->Count )
		{
			cmbSkillPoint->BeginUpdate();

			const int skillpoint_index = Math::Min( cmbSkillPoint->SelectedIndex, new_max - 1 );
			cmbSkillPoint->Items->Clear();
			for( int i = 1; i <= new_max; ++i )
				cmbSkillPoint->Items->Add( L"+" + Convert::ToString( i ) );
			cmbSkillPoint->SelectedIndex = skillpoint_index;

			cmbSkillPoint->EndUpdate();
		}
	}

	void UpdateEnables()
	{
		btnAddNew->Enabled = cmbSkillPoint->SelectedIndex >= 0 && cmbAbility->SelectedIndex >= 0 && cmbItemType->SelectedIndex >= 0 &&
						   ( cmbItemType->SelectedIndex == 0 || cmbHunterType->SelectedIndex >= 0 );

		cmbAbility->Enabled = cmbItemType->SelectedIndex >= 0;
		cmbSkillPoint->Enabled = cmbAbility->Enabled;

		btnDelete->Enabled = lstEquipment->SelectedIndex >= 0;
		btnSave->Enabled = lstEquipment->SelectedIndex >= 0;
	}

	System::Void cmbAbility_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		last_ability = cmbItemType->SelectedIndex < 0 ? nullptr : cmbItemType->SelectedIndex == 0 ? wep_abilities[ cmbAbility->SelectedIndex ] : arm_abilities[ cmbAbility->SelectedIndex ];
		UpdateEnables();
		UpdateSkillPoints();
	}

	System::Void cmbSkillPoint_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		UpdateEnables();
	}

	System::Void cmbHunterType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		UpdateEnables();
	}

	System::Void btnAddNew_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( cmbItemType->SelectedIndex == 0 )
		{
			Weapon^ nu = gcnew Weapon();
			nu->type = GetSelectedHunterType();
			nu->ability = wep_abilities[ cmbAbility->SelectedIndex ];
			nu->ability_amount = cmbSkillPoint->SelectedIndex + 1;
			nu->defence = Convert::ToUInt32( txtDefence->Text );
			AddWeapon( nu );
		}
		else
		{
			Armor^ nu = gcnew Armor();
			nu->abilities.Add( gcnew AbilityPair( arm_abilities[ cmbAbility->SelectedIndex ], cmbSkillPoint->SelectedIndex + 1 ) );
			nu->gender = Gender::BOTH_GENDERS;
			nu->type = GetSelectedHunterType();

			nu->SetExName( cmbItemType->SelectedIndex - 1 );

			nu->defence = nu->max_defence = Convert::ToUInt32( txtDefence->Text );
			nu->fire_res = cmbFireRes->SelectedIndex - minmaxres;
			nu->water_res = cmbWaterRes->SelectedIndex - minmaxres;
			nu->ice_res = cmbIceRes->SelectedIndex - minmaxres;
			nu->thunder_res = cmbThunderRes->SelectedIndex - minmaxres;
			nu->dragon_res = cmbDragonRes->SelectedIndex - minmaxres;
			AddArmor( nu, cmbItemType->SelectedIndex - 1 );
		}

		UpdateEnables();
		CalcCanTrim();
		ExcavatedGear::SaveCustom();
	}

	System::Void btnSave_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( lstEquipment->SelectedIndex < 0 )
			return;

		lstEquipment->BeginUpdate();
		
		if( lstEquipment->SelectedIndex < ExcavatedGear::weapons.Count )
		{
			ExcavatedGear::weapons.RemoveAt( lstEquipment->SelectedIndex );
			lstEquipment->Items->RemoveAt( lstEquipment->SelectedIndex );
			
			btnAddNew_Click( sender, e );
		}
		else
		{
			int index = lstEquipment->SelectedIndex - ExcavatedGear::weapons.Count;
			int list_index = ExcavatedGear::weapons.Count;
			for( int i = 0; i < 5; i++ )
			{
				if( index < ExcavatedGear::armor[ i ]->Count )
				{
					ExcavatedGear::armor[ i ]->RemoveAt( index );
					lstEquipment->Items->RemoveAt( list_index );

					btnAddNew_Click( sender, e );
					break;
				}
				index -= ExcavatedGear::armor[ i ]->Count;
				list_index += ExcavatedGear::armor[ i ]->Count;
			}
		}

		lstEquipment->EndUpdate();
	}

	System::Void btnDelete_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( lstEquipment->SelectedIndex < 0 )
			return;

		if( lstEquipment->SelectedIndex < ExcavatedGear::weapons.Count )
		{
			ExcavatedGear::weapons.RemoveAt( lstEquipment->SelectedIndex );
			lstEquipment->Items->RemoveAt( lstEquipment->SelectedIndex );
		}
		else
		{
			int index = lstEquipment->SelectedIndex - ExcavatedGear::weapons.Count;
			int list_index = ExcavatedGear::weapons.Count;
			for( int i = 0; i < 5; i++ )
			{
				if( index < ExcavatedGear::armor[ i ]->Count )
				{
					ExcavatedGear::armor[ i ]->RemoveAt( index );
					lstEquipment->Items->RemoveAt( list_index );
					break;
				}
				index -= ExcavatedGear::armor[ i ]->Count;
				list_index += ExcavatedGear::armor[ i ]->Count;
			}
		}

		if( lstEquipment->SelectedIndex == -1 && lstEquipment->Items->Count > 0 )
			lstEquipment->SelectedIndex = 0;

		UpdateEnables();
		CalcCanTrim();
		ExcavatedGear::SaveCustom();
	}

	System::Void btnTrim_Click(System::Object^  sender, System::EventArgs^  e)
	{
		for( int i = 0; i < ExcavatedGear::weapons.Count; ++i )
		{
			for( int j = i + 1; j < ExcavatedGear::weapons.Count; ++j )
			{
				if( ExcavatedGear::weapons[ j ]->type == ExcavatedGear::weapons[ i ]->type &&
					ExcavatedGear::weapons[ j ]->ability == ExcavatedGear::weapons[ i ]->ability &&
					ExcavatedGear::weapons[ j ]->ability_amount >= ExcavatedGear::weapons[ i ]->ability_amount )
				{
					ExcavatedGear::weapons.RemoveAt( i-- );
					break;
				}
			}
		}
		for( int i = 0; i < 5; ++i )
		{
			for( int j = 0; j < ExcavatedGear::armor[ i ]->Count; ++j )
			{
				for( int k = j + 1; k < ExcavatedGear::armor[ i ]->Count; ++k )
				{
					if( ExcavatedGear::armor[ i ][ k ]->type == ExcavatedGear::armor[ i ][ j ]->type &&
						ExcavatedGear::armor[ i ][ k ]->abilities[ 0 ]->ability == ExcavatedGear::armor[ i ][ j ]->abilities[ 0 ]->ability &&
						ExcavatedGear::armor[ i ][ k ]->abilities[ 0 ]->amount >= ExcavatedGear::armor[ i ][ j ]->abilities[ 0 ]->amount )
					{
						ExcavatedGear::armor[ i ]->RemoveAt( j-- );
						break;
					}
				}
			}
		}

		AddGear();
		ExcavatedGear::SaveCustom();
		UpdateEnables();
	}
};
}
