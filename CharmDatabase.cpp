#include "stdafx.h"
#include "CharmDatabase.h"
#include "Solution.h"
#include "Armor.h"
#include "Skill.h"
#include <fstream>
#include <cmath>

using namespace System;

//not sure what this is. ordering?
/*"刀匠",
"射手",
"状態耐性",
"怒",
"細菌学",
"回避術",
"居合",
"頑強",
"剛撃",
"一心",
"盾持",
"潔癖",
"増幅",
"護石収集",
"強欲",
"属性解放",
"剣術",
"匠",
"抜刀会心",
"抜刀減気",
"装填速度",
"反動", 
"速射", 
"本気", 
"闘魂", 
"不動", 
"護石王",
"痛撃",
"属性耐性", 
"攻撃", 
"防御", 
"採取",
"剥ぎ取り",
"運気", 
"捕獲", 
"達人",
"砲術", 
"回避性能", 
"体力", 
"聴覚保護", 
"斬れ味",
"ガード性能", 
"ガード強化",
"研ぎ師",
"底力",
"爆弾強化", 
"特殊攻撃", 
"スタミナ", 
"重撃",
"回避距離", 
"納刀",
"溜め短縮", 
"減気攻撃", 
"無傷", 
"根性", 
"装填数", 
"通常弾強化", 
"貫通弾強化",
"散弾強化",
"KO", 
"対防御DOWN", 
"効果持続", 
"通常弾追加",
"貫通弾追加", 
"散弾追加",
"榴弾追加", 
"拡散弾追加", 
"斬裂弾追加", 
"爆破弾追加", 
"毒瓶追加",
"麻痺瓶追加", 
"睡眠瓶追加",
"強撃瓶追加", 
"接撃瓶追加", 
"減気瓶追加",
"爆破瓶追加", 
"精密射撃",
"火属性攻撃", 
"水属性攻撃", 
"雷属性攻撃",
"氷属性攻撃",
"龍属性攻撃", 
"属性攻撃", 
"火耐性",
"水耐性", 
"雷耐性", 
"氷耐性", 
"龍耐性",
"高速収集",
"千里眼", 
"観察眼",
"回復量", 
"広域", 
"笛", 
"逆境", 
"肉食", 
"食いしん坊", 
"体術", 
"気力回復", 
"風圧", 
"食事",
"調合成功率", 
"調合数", 
"狩人", 
"耐暑",
"耐寒", 
"指揮", 
"采配", 
"号令",
"回復速度", 
"気まぐれ", 
"加護", 
"腹減り",
"運搬", 
"高速設置", 
"ハチミツ", 
"毒",
"麻痺",
"睡眠", 
"気絶", 
"耐泥耐雪", 
"耐震",
"盗み無効", 
"気配", 
"燃鱗", 
"自動防御",
"狂撃耐性", 
"乗り", 
"属性会心",
"特殊会心", 
"射法", 
"茸食", 
"節食", 
"裂傷", 
"耐粘",
"斬術", 
"食欲", 
"北辰納豆流", 
"秘伝",
"職工",
"剛腕", 
"祈願", 
"裏稼業"*/

unsigned CalcMaxCharmType( Query^ query )
{
	int hr_to_charm_type[] = 
	{
		0, // HR0+ mystery
		0,
		0,
		0,
		1, // HR4+ shining
		2, // HR5+ timeworn
		2,
		2,
		4, // HR8+ enduring & distorted (G-rank)
		4, // G1
		4, // G2
		4, // G3
		5, // All (special permit) graven charms (Lv140 Guild Quests etc.)
		5,
		5,
	};

	int effective_hr = query->hr;
	if( effective_hr < 4 && query->elder_star > 6 )
		effective_hr = 4;
	if( effective_hr < 5 && query->elder_star > 7 )
		effective_hr = 5;

	return hr_to_charm_type[ effective_hr ];
}

String^ FixTypos( String^ input )
{
	if( input == L"Faint" )
		return L"Stun";
	else if( input == L"Snow/Mud Res" )
		return L"Mud/Snow";
	else if( input == L"Defense Lock" )
		return L"Def Lock";
	else if( input == L"Fast Charge" )
		return L"FastCharge";
	else if( input == L"Stamina Drain" )
		return L"Stam Drain";
	else if( input == L"Constitution" )
		return L"Constitutn";
	else if( input == L"Stamina Recovery" )
		return L"Stam Recov";
	else if( input == L"Evade Distance" )
		return L"Evade Dist";
	else if( input == L"Elemental Attack" )
		return L"Elemental";
	else if( input == L"Fire Attack" )
		return L"Fire Atk";
	else if( input == L"Water Attack" )
		return L"Water Atk";
	else if( input == L"Thunder Attack" )
		return L"ThunderAtk";
	else if( input == L"Ice Attack" )
		return L"Ice Atk";
	else if( input == L"Dragon Attack" )
		return L"Dragon Atk";
	else if( input == L"Thunder Res" )
		return L"ThunderRes";
	else if( input == L"Sharpening Speed" )
		return L"Sharpener";
	else if( input == L"Punishing Draw" )
		return L"PunishDraw";
	else if( input == L"Critical Draw" )
		return L"Crit Draw";
	else if( input == L"Reload Speed" )
		return L"Reload Spd";
	else if( input == L"Slime S+" )
		return L"Blast S+";
	else if( input == L"Paralysis C+" )
		return L"Para C+";
	else if( input == L"Slime C+" )
		return L"Blast C+";
	else if( input == L"Recovery Level" )
		return L"Rec Level";
	else if( input == L"Wide Range" )
		return L"Wide-Range";
	else if( input == L"Lasting Power" )
		return L"LastingPwr";
	else if( input == L"Speed Gather" )
		return L"Spd Gather";
	else if( input == L"Tranquilizer" )
		return L"Tranquilzr";
	else if( input == L"Transporter" )
		return L"Transportr";
	else if( input == L"Speed Setup" )
		return L"SpeedSetup";
	else if( input == L"Free Element" )
		return L"FreeElemnt";
	else if( input == L"Team Player" )
		return L"TeamPlayer";
	else if( input == L"Team Leader" )
		return L"TeamLeader";
	else if( input == L"Steadyhand" )
		return L"SteadyHand";
	else if( input == L"Single-Minded" )
		return L"Ferocity";
	else if( input == L"Firm Defence" )
		return L"Resilience";
	else if( input == L"Greedy" )
		return L"Avarice";
	else if( input == L"Infection Res" )
		return L"Frenzy Res";
	else if( input == L"Precise Attack" )
		return L"Brutality";
	else if( input == L"Rider" )
		return L"Mounting";
	else if( input == L"Martial Arts" )
		return L"Readiness";
	else if( input == L"Acrobat" )
		return L"Nimbleness";
	else if( input == L"Shielder" )
		return L"Stalwart";
	else if( input == L"Finesse" )
		return L"Prudence";
	else if( input == L"Charm Collector" )
		return L"Hoarding";
	else if( input == L"Amplifier" )
		return L"Amplify";
	else if( input == L"Bleed Res" )
		return L"Bleeding";
	else if( input == L"Diet" )
		return L"LightEater";
	else if( input == L"Mushrooms" )
		return L"Mycology";
	else if( input == L"Appetite" )
		return L"PowerEater";
	else if( input == L"Range Extend" )
		return L"Dead Eye";
	else if( input == L"Tricksy" )
		return L"Covert";
	else if( input == L"Blade" )
		return L"Edge Lore";
	else if( input == L"Status Crit" )
		return L"CritStatus";
	else if( input == L"Elemental Crit" )
		return L"CritElemnt";
	else if( input == L"Sticky Fighter" )
		return L"D. Fencing";
	else if( input == L"Hidden Power" )
		return L"Arcana";
	else if( input == L"Trap Master" )
		return L"Mechanic";
	else if( input == L"Hard Hitting" )
		return L"Brawn";

	else return input;
}

#pragma region Custom charms/gear

#define CUSTOM_GEAR_TXT L"Data/mygear.txt"

void ExcavatedGear::SaveCustom()
{
	IO::StreamWriter fout( CUSTOM_GEAR_TXT );
	for each( Weapon^ wep in weapons )
	{
		fout.Write( "5," );
		fout.Write( Convert::ToString( (int)wep->type ) + "," );
		fout.Write( Convert::ToString( wep->ability_amount ) + "," );
		fout.Write( Convert::ToString( wep->ability->name ) + "," );
		fout.Write( Convert::ToString( wep->defence ) );
		fout.WriteLine();
	}
	for( int i = 0; i < armor->Length; ++i )
	{
		for( int j = 0; j < armor[ i ]->Count; ++j )
		{
			Armor^ arm = armor[ i ][ j ];
			fout.Write( Convert::ToString( i ) + L"," );
			fout.Write( Convert::ToString( (int)arm->type ) + L"," );
			fout.Write( Convert::ToString( arm->abilities[ 0 ]->amount ) + "," );
			fout.Write( Convert::ToString( arm->abilities[ 0 ]->ability->name ) + L"," );

			fout.Write( Convert::ToString( arm->defence ) + L"," );
			fout.Write( Convert::ToString( arm->fire_res ) + L"," );
			fout.Write( Convert::ToString( arm->water_res ) + L"," );
			fout.Write( Convert::ToString( arm->ice_res ) + L"," );
			fout.Write( Convert::ToString( arm->thunder_res ) + L"," );
			fout.Write( Convert::ToString( arm->dragon_res ) + L"," );
			fout.WriteLine();
		}
	}
}

bool ExcavatedGear::LoadCustom()
{
	weapons.Clear();
	armor = gcnew array< List_t< Armor^ >^ >( (int)Armor::ArmorType::NumArmorTypes );
	for( int i = 0; i < armor->Length; ++i )
		armor[ i ] = gcnew List_t< Armor^ >();

	if( !IO::File::Exists( CUSTOM_GEAR_TXT ) )
	{
		return true;
	}

	IO::StreamReader fin( CUSTOM_GEAR_TXT );
	String^ temp;
	while( !fin.EndOfStream )
	{
		temp = fin.ReadLine();
		if( temp == L"" || temp[ 0 ] == L'#' )
			continue;

		try
		{
			List_t< String^ > split;
			Utility::SplitString( %split, temp, L',' );

			if( StringTable::english )
				split[ 3 ] = FixTypos( split[ 3 ] );

			const int type = Convert::ToUInt32( split[ 0 ] );
			if( type == (int)Armor::ArmorType::NumArmorTypes )
			{
				Weapon^ wep = gcnew Weapon();
				wep->type = (HunterType)Convert::ToUInt32( split[ 1 ] );
				wep->ability_amount = Convert::ToUInt32( split[ 2 ] );
				wep->ability = Ability::FindAbility( split[ 3 ] );
				wep->defence = split.Count > 4 ? Convert::ToUInt32( split[ 4 ] ) : 0;
				wep->index = weapons.Count;
				if( wep->ability )
					weapons.Add( wep );
				else
					return false;
			}
			else
			{
				Armor^ arm = gcnew Armor();
				arm->relic = true;
				arm->type = (HunterType)Convert::ToUInt32( split[ 1 ] );
				int amount = Convert::ToUInt32( split[ 2 ] );
				Ability^ ability = Ability::FindAbility( split[ 3 ] );
				if( ability )
				{
					arm->abilities.Add( gcnew AbilityPair( ability, amount ) );
					arm->defence = arm->max_defence = Convert::ToUInt32( split[ 4 ] );
					arm->fire_res = Convert::ToInt32( split[ 5 ] );
					arm->water_res = Convert::ToInt32( split[ 6 ] );
					arm->ice_res = Convert::ToInt32( split[ 7 ] );
					arm->thunder_res = Convert::ToInt32( split[ 8 ] );
					arm->dragon_res = Convert::ToInt32( split[ 9 ] );
					arm->SetExName( type );
					arm->index = armor[ type ]->Count;
					arm->gender = Gender::BOTH_GENDERS;
					armor[ type ]->Add( arm );
				}
				else return false;
			}
		}
		catch( Exception^ )
		{
			return false;
		}
	}
	return true;
}

#define CUSTOM_CHARM_TXT L"Data/mycharms.txt"

void CharmDatabase::SaveCustom()
{
	//slots,skill1,num,skill2,num
	IO::StreamWriter fout( CUSTOM_CHARM_TXT );
	fout.WriteLine( L"#Format: Slots,Skill1,Points1,Skill2,Points2" );
	for each( Charm^ ch in mycharms )
	{
		fout.Write( Convert::ToString( ch->num_slots ) );
		for( int i = 0; i < 2; ++i )
		{
			if( i < ch->abilities.Count )
				fout.Write( L"," + ch->abilities[ i ]->ability->name + L"," + Convert::ToString( ch->abilities[ i ]->amount ) );
			else fout.Write( L",," );
		}
		fout.WriteLine();
	}
}

bool CharmDatabase::CreateNewCustom()
{
	if( !IO::File::Exists( CUSTOM_CHARM_TXT ) )
	{
		mycharms.Clear();
		Charm^ charm = gcnew Charm( 0 );
		charm->abilities.Add( gcnew AbilityPair( SpecificAbility::auto_guard, 10 ) );
		mycharms.Add( charm );
		return true;
	}
	return false;
}

List_t< Charm^ >^ CharmDatabase::LoadCharms( System::String^ filename )
{
	List_t< Charm^ >^ results = gcnew List_t< Charm^ >();

	IO::StreamReader fin( filename );
	String^ temp;
	while( !fin.EndOfStream )
	{
		temp = fin.ReadLine();
		if( temp == L"" || temp[ 0 ] == L'#' ) continue;
		List_t< String^ > split;
		Utility::SplitString( %split, temp, L',' );
		if( split.Count != 5 )
		{
			//MessageBox::Show( L"Failed to load mycharms.txt: Wrong number of commas" );
			results->Clear();
			return results;
		}

		//slots,skill1,num,skill2,num
		Charm^ charm = gcnew Charm();
		charm->num_slots = Convert::ToUInt32( split[ 0 ] );

		try
		{
			if( split[ 1 ] != L"" )
			{
				if( StringTable::english )
					split[ 1 ] = FixTypos( split[ 1 ] );
				Ability^ ab = Ability::FindAbility( split[ 1 ] );
				if( !ab )
				{
					results->Clear();
					return results;
				}

				charm->abilities.Add( gcnew AbilityPair( ab, Convert::ToInt32( split[ 2 ] ) ) );
			}
			if( split[ 3 ] != L"" )
			{
				if( StringTable::english )
					split[ 3 ] = FixTypos( split[ 3 ] );
				Ability^ ab = Ability::FindAbility( split[ 3 ] );
				if( !ab )
				{
					results->Clear();
					return results;
				}

				charm->abilities.Add( gcnew AbilityPair( ab, Convert::ToInt32( split[ 4 ] ) ) );
			}
		}
		catch( Exception^ )
		{
			results->Clear();
			return results;
		}
		results->Add( charm );
	}

	return results;
}

bool CharmDatabase::LoadCustom()
{
	if( !IO::File::Exists( CUSTOM_CHARM_TXT ) )
	{
		Charm^ charm = gcnew Charm( 0 );
		charm->abilities.Add( gcnew AbilityPair( SpecificAbility::auto_guard, 10 ) );
		mycharms.Clear();
		mycharms.Add( charm );
		return true;
	}

	List_t< Charm^ >^ charms = LoadCharms( CUSTOM_CHARM_TXT );
	if( charms->Count == 0 )
		return false;

	mycharms.Clear();

	bool cheats = false;
	for each( Charm^ charm in charms )
	{
		if( CharmDatabase::CharmIsLegal( charm ) )
		{
			charm->custom = true;
			mycharms.Add( charm );
		}
		else
		{
			cheats = true;
		}
	}
	
	if( cheats )
		System::Windows::Forms::MessageBox::Show( StaticString( Cheater ) );

	return true;
}

#pragma endregion

#pragma region Charm Generation
ref struct StaticData
{
	static array< array< unsigned char >^ >^ skill1_table;
	static array< array<   signed char >^ >^ skill2_table; // [type][skill]
	static array< array< unsigned char >^ >^ slot_table;

	static array< unsigned char >^ skill2_chance_table =
	{
		100,
		35,
		25,
		20,
		15,
		25
	};
};

int rnd( const int n )
{
	Assert( n < 65536 && n >= 0, L"Bad RND" );
	if( n == 0 ) return 176;

	return ( n * 176 ) % 65363;
}

int reverse_rnd( const int n )
{
	return ( n * 7799 ) % 65363;
}

unsigned GetNumSlots( const unsigned charm_type, const int slot_table, const unsigned roll )
{
	Assert( (int)charm_type < StaticData::slot_table->Length, L"Bad charm type" );

	const unsigned table_index = Math::Min( slot_table, StaticData::slot_table[ charm_type ]->Length / 3 ) - 1;
	const unsigned for2 = StaticData::slot_table[ charm_type ][ table_index * 3 + 1 ];
	const unsigned for3 = StaticData::slot_table[ charm_type ][ table_index * 3 + 2 ];
	const unsigned for1 = StaticData::slot_table[ charm_type ][ table_index * 3 + 0 ];
	if( roll >= for2 )
	{
		return ( roll >= for3 ) ? 3 : 2;
	}
	else
	{
		return ( roll >= for1 ) ? 1 : 0;
	}
}

bool TryTwoSkillCharm( const unsigned charm_type, int n, int m, array< List_t< unsigned >^ >^ charms )
{
	array< unsigned char >^ skill1_table = StaticData::skill1_table[ charm_type ];
	array< signed char >^	skill2_table = StaticData::skill2_table[ charm_type ];
	const unsigned num_skills1 = skill1_table->Length / 3;
	const unsigned num_skills2 = skill2_table == nullptr ? 0 : skill2_table->Length / 3;

	//skill 1
	const int skill1_index = n % num_skills1;
	const int skill1_name = skill1_table[ skill1_index * 3 ];
	const int skill1_min = skill1_table[ skill1_index * 3 + 1 ];
	const int skill1_max = skill1_table[ skill1_index * 3 + 2 ];

	//skill 1 point
	n = rnd( n );
	const int point1 = skill1_min + (n^m) % ( skill1_max - skill1_min + 1 );

	//has skill 2?
	int skill2_index = -1, point2 = 0, skill2_min = 0, skill2_max = 0, skill2_name = 0;
	n = rnd( n );
	m = rnd( m );
	if( (n^m) % 100 >= StaticData::skill2_chance_table[ charm_type ] )
	{
		//skill 2
		m = rnd( m );
		skill2_index = m % num_skills2;
		skill2_name = skill2_table[ skill2_index * 3 ];
		skill2_min = skill2_table[ skill2_index * 3 + 1 ];
		skill2_max = skill2_table[ skill2_index * 3 + 2 ];

		//skill 2 point
		if( skill2_min > 0 ) //positive number
		{
			n = rnd( n );
			m = rnd( m );
			point2 = ( (n^m) % ( skill2_max - skill2_min + 1 ) ) + skill2_min;
		}
		else //check for negative
		{
			n = rnd( n );
			if( n % 2 == 1 ) //positive
			{
				n = rnd( n );
				m = rnd( m );
				point2 = (n^m) % skill2_max + 1;
			}
			else //negative
			{
				n = rnd( n );
				m = rnd( m );
				point2 = skill2_min + (n^m) % ( -skill2_min );
			}
		}
		if( point2 < 0 )
			return false;

		if( skill1_name == skill2_name )
		{
			skill2_min = skill2_max = point2 = 0;
			skill2_name = skill2_index = Ability::static_abilities.Count;
		}
	}

	const int slot_table = (int)Math::Floor( point1*10.0 / skill1_max + ( (point2 > 0) ? point2*10.0 / skill2_max : 0 ) );

	//slots
	n = rnd( n );
	int slot_roll = 0;
	if( n & 128 )
	{
		n = rnd( n );
		slot_roll = n % 100;
	}
	else
	{
		m = rnd( m );
		slot_roll = m % 100;
	}

	const int num_slots = GetNumSlots( charm_type, slot_table, slot_roll );

	List_t< unsigned >^ list = charms[ num_slots ];
	for( int i = 0; i < list->Count; ++i )
	{
		const unsigned hash = list[i];
		const int p1 = hash >> 16;
		const int p2 = hash & 0xFFFF;
		if( p1 >= point1 && p2 >= point2 )
			return false;
	}
	list->Add( point1 << 16 | point2 );

	return num_slots == Math::Min( 3u, charm_type + 1 ) && slot_table == 20;
}

String^ CanGenerateCharm1( const unsigned charm_type, int n, int m, Charm^ charm )
{
	Assert( charm->abilities.Count == 1, "Too many abilities for charm" );
	Assert( (int)charm_type < StaticData::skill1_table->Length && (int)charm_type < StaticData::skill2_table->Length, "Charm type is invalid" );
	array< unsigned char >^ skill1_table = StaticData::skill1_table[ charm_type ];
	array< signed char >^	skill2_table = StaticData::skill2_table[ charm_type ];
	const unsigned num_skills1 = skill1_table->Length / 3;
	const unsigned num_skills2 = skill2_table->Length / 3;

	//skill 1
	const int skill1_index = n % num_skills1;
	const int skill1_name = skill1_table[ skill1_index * 3 ];
	const int skill1_min = skill1_table[ skill1_index * 3 + 1 ];
	const int skill1_max = skill1_table[ skill1_index * 3 + 2 ];

	//skill 1 point
	n = rnd( n );
	const int point1 = skill1_min + (n^m) % ( skill1_max - skill1_min + 1 );

	if( skill1_name == charm->abilities[0]->ability->static_index && point1 < charm->abilities[0]->amount )
		return nullptr;

	//has skill 2?
	int skill2_index = -1, point2 = 0, skill2_min = 0, skill2_max = 0, skill2_name = 0;
	n = rnd( n );
	m = rnd( m );
	if( (n^m) % 100 >= StaticData::skill2_chance_table[ charm_type ] )
	{
		//skill 2
		m = rnd( m );
		skill2_index = m % num_skills2;
		skill2_name = skill2_table[ skill2_index * 3 ];
		skill2_min = skill2_table[ skill2_index * 3 + 1 ];
		skill2_max = skill2_table[ skill2_index * 3 + 2 ];

		//skill 2 point
		if( skill2_min > 0 ) //positive number
		{
			n = rnd( n );
			m = rnd( m );
			point2 = ( (n^m) % ( skill2_max - skill2_min + 1 ) ) + skill2_min;
		}
		else //check for negative
		{
			n = rnd( n );
			if( n % 2 == 1 ) //positive
			{
				n = rnd( n );
				m = rnd( m );
				point2 = (n^m) % skill2_max + 1;
			}
			else //negative
			{
				n = rnd( n );
				m = rnd( m );
				point2 = skill2_min + (n^m) % ( -skill2_min );
			}
		}

		if( skill1_name == skill2_name )
		{
			skill2_name = point2 = 0;
			skill2_index = -1;
		}

		if( skill1_name != charm->abilities[0]->ability->static_index && skill2_name != charm->abilities[0]->ability->static_index ||
			point2 < charm->abilities[0]->amount )
			return nullptr;
	}

	const int slot_table = (int)Math::Floor( point1*10.0 / skill1_max + ( (point2 > 0) ? point2*10.0 / skill2_max : 0 ) );

	//slots
	n = rnd( n );
	int slot_roll = 0;
	if( n & 128 )
	{
		n = rnd( n );
		slot_roll = n % 100;
	}
	else
	{
		m = rnd( m );
		slot_roll = m % 100;
	}

	const unsigned num_slots = GetNumSlots( charm_type, slot_table, slot_roll );

	if( num_slots < charm->num_slots )
		return nullptr;

	Charm c( num_slots );
	c.abilities.Add( gcnew AbilityPair( Ability::static_abilities[ skill1_name ], point1 ) );
	if( point2 )
		c.abilities.Add( gcnew AbilityPair( Ability::static_abilities[ skill2_name ], point2 ) );

	return c.GetName();
}

String^ CanGenerateCharm2( const unsigned charm_type, int n, int m, Charm^ charm )
{
	Assert( charm->abilities.Count == 2, "Too few abilities for charm" );
	Assert( (int)charm_type < StaticData::skill1_table->Length && (int)charm_type < StaticData::skill2_table->Length, "Charm type is invalid" );
	array< unsigned char >^ skill1_table = StaticData::skill1_table[ charm_type ];
	array< signed char >^	skill2_table = StaticData::skill2_table[ charm_type ];
	const unsigned num_skills1 = skill1_table->Length / 3;
	const unsigned num_skills2 = skill2_table->Length / 3;

	//skill 1
	const int skill1_index = n % num_skills1;
	const int skill1_name = skill1_table[ skill1_index * 3 ];
	const int skill1_min = skill1_table[ skill1_index * 3 + 1 ];
	const int skill1_max = skill1_table[ skill1_index * 3 + 2 ];

	//skill 1 point
	n = rnd( n );
	const int point1 = skill1_min + (n^m) % ( skill1_max - skill1_min + 1 );

	if( point1 < charm->abilities[0]->amount )
		return nullptr;

	//has skill 2?
	int skill2_index = -1, point2 = 0, skill2_min = 0, skill2_max = 0, skill2_name = 0;
	n = rnd( n );
	m = rnd( m );
	if( (n^m) % 100 >= StaticData::skill2_chance_table[ charm_type ] )
	{
		//skill 2
		m = rnd( m );
		skill2_index = m % num_skills2;
		skill2_name = skill2_table[ skill2_index * 3 ];
		skill2_min = skill2_table[ skill2_index * 3 + 1 ];
		skill2_max = skill2_table[ skill2_index * 3 + 2 ];

		//skill 2 point
		if( skill2_min > 0 ) //positive number
		{
			n = rnd( n );
			m = rnd( m );
			point2 = ( (n^m) % ( skill2_max - skill2_min + 1 ) ) + skill2_min;
		}
		else //check for negative
		{
			n = rnd( n );
			if( n % 2 == 1 ) //positive
			{
				n = rnd( n );
				m = rnd( m );
				point2 = (n^m) % skill2_max + 1;
			}
			else //negative
			{
				n = rnd( n );
				m = rnd( m );
				point2 = skill2_min + (n^m) % ( -skill2_min );
			}
		}

		if( skill1_name == skill2_name || point2 < charm->abilities[1]->amount )
			return nullptr;
	}
	else return nullptr;

	const int slot_table = (int)Math::Floor( point1*10.0 / skill1_max + ( (point2 > 0) ? point2*10.0 / skill2_max : 0 ) );

	//slots
	n = rnd( n );
	int slot_roll = 0;
	if( n & 128 )
	{
		n = rnd( n );
		slot_roll = n % 100;
	}
	else
	{
		m = rnd( m );
		slot_roll = m % 100;
	}

	const unsigned num_slots = GetNumSlots( charm_type, slot_table, slot_roll );

	if( num_slots < charm->num_slots )
		return nullptr;

	Charm c( num_slots );
	c.abilities.Add( gcnew AbilityPair( Ability::static_abilities[ skill1_name ], point1 ) );
	c.abilities.Add( gcnew AbilityPair( Ability::static_abilities[ skill2_name ], point2 ) );

	return c.GetName();
}

bool CanGenerate2SkillCharm( const unsigned charm_type, int n, int m, Charm^ charm )
{
	Assert( charm->abilities.Count == 2, "Charm has too few abilities" );
	Assert( charm_type > 0 && (int)charm_type < StaticData::skill1_table->Length && (int)charm_type < StaticData::skill2_table->Length, "Charm type is invalid" );
	array< unsigned char >^ skill1_table = StaticData::skill1_table[ charm_type ];
	array< signed char >^	skill2_table = StaticData::skill2_table[ charm_type ];
	const unsigned num_skills1 = skill1_table->Length / 3;
	const unsigned num_skills2 = skill2_table->Length / 3;

	//skill 1
	//n = rnd( n );
	const int skill1_index = n % num_skills1;
	const int skill1_name = skill1_table[ skill1_index * 3 ];
	Assert( skill1_name == charm->abilities[ 0 ]->ability->static_index, "Skill1 failed" );
	const int skill1_min = skill1_table[ skill1_index * 3 + 1 ];
	const int skill1_max = skill1_table[ skill1_index * 3 + 2 ];

	//skill 1 point
	n = rnd( n );
	//m = rnd( m );
	const int point1 = skill1_min + (n^m) % ( skill1_max - skill1_min + 1 );

	if( point1 < charm->abilities[0]->amount )
		return false;

	//has skill 2?
	int skill2_index = -1, point2 = 0, skill2_min = 0, skill2_max = 0, skill2_name = 0;
	n = rnd( n );
	m = rnd( m );
	if( (n^m) % 100 >= StaticData::skill2_chance_table[ charm_type ] )
	{
		//skill 2
		m = rnd( m );
		skill2_index = m % num_skills2;
		skill2_name = (unsigned char)skill2_table[ skill2_index * 3 ];
		Assert( skill2_name == charm->abilities[1]->ability->static_index, "Skill2 failed" );
		skill2_min = skill2_table[ skill2_index * 3 + 1 ];
		skill2_max = skill2_table[ skill2_index * 3 + 2 ];

		//skill 2 point
		if( skill2_min > 0 ) //positive number
		{
			n = rnd( n );
			m = rnd( m );
			point2 = ( (n^m) % ( skill2_max - skill2_min + 1 ) ) + skill2_min;
		}
		else //check for negative
		{
			n = rnd( n );
			if( n % 2 == 1 ) //positive
			{
				n = rnd( n );
				m = rnd( m );
				point2 = (n^m) % skill2_max + 1;
			}
			else //negative
			{
				n = rnd( n );
				m = rnd( m );
				point2 = skill2_min + (n^m) % ( -skill2_min );
			}
		}

		if( skill1_name == skill2_name || point2 < charm->abilities[1]->amount )
		{
			return false;
		}
	}
	else return false;

	const int slot_table = (int)Math::Floor( point1*10.0 / skill1_max + ( (point2 > 0) ? point2*10.0 / skill2_max : 0 ) );

	//slots
	n = rnd( n );
	int slot_roll = 0;
	if( n & 128 )
	{
		n = rnd( n );
		slot_roll = n % 100;
	}
	else
	{
		m = rnd( m );
		slot_roll = m % 100;
	}

	const unsigned num_slots = GetNumSlots( charm_type, slot_table, slot_roll );

	return num_slots >= charm->num_slots;
}

bool GenerateCharm3( const unsigned charm_type, const unsigned table, int n, int m, Charm^ charm )
{
	//check charm_type < StaticData::skill1_table->Length
	array< unsigned char >^ skill1_table = StaticData::skill1_table[ charm_type ];
	const unsigned num_skills1 = skill1_table->Length / 3;
	//check charm_type < StaticData::skill2_table->Length
	array< signed char >^ skill2_table = StaticData::skill2_table[ charm_type ];
	const unsigned num_skills2 = skill2_table == nullptr ? 0 : skill2_table->Length / 3;

	//skill 1
	//n = rnd( n );
	const int skill1_index = n % num_skills1;
	const int skill1_name = skill1_table[ skill1_index * 3 ];
	const int skill1_min = skill1_table[ skill1_index * 3 + 1 ];
	const int skill1_max = skill1_table[ skill1_index * 3 + 2 ];

	//skill 1 point
	n = rnd( n );
	//m = rnd( m );
	const int point1 = skill1_min + (n^m) % ( skill1_max - skill1_min + 1 );

	//has skill 2?
	int skill2_index = -1, point2 = 0, skill2_min = 0, skill2_max = 0, skill2_name = 0;
	n = rnd( n );
	m = rnd( m );
	if( (n^m) % 100 >= StaticData::skill2_chance_table[ charm_type ] )
	{
		//skill 2
		m = rnd( m );
		skill2_index = m % num_skills2;
		skill2_name = skill2_table[ skill2_index * 3 ];
		skill2_min = skill2_table[ skill2_index * 3 + 1 ];
		skill2_max = skill2_table[ skill2_index * 3 + 2 ];

		//skill 2 point
		if( skill2_min > 0 ) //positive number
		{
			n = rnd( n );
			m = rnd( m );
			point2 = ( (n^m) % ( skill2_max - skill2_min + 1 ) ) + skill2_min;
		}
		else //check for negative
		{
			n = rnd( n );
			if( n % 2 == 1 ) //positive
			{
				n = rnd( n );
				m = rnd( m );
				point2 = (n^m) % skill2_max + 1;
			}
			else //negative
			{
				n = rnd( n );
				m = rnd( m );
				point2 = skill2_min + (n^m) % ( -skill2_min );
			}
		}		

		if( skill1_name == skill2_name )
		{
			skill2_min = skill2_max = point2 = 0;
			skill2_name = skill2_index = Ability::static_abilities.Count;
		}
	}

	const int slot_table = (int)Math::Floor( point1*10.0 / skill1_max + ( (point2 > 0) ? point2*10.0 / skill2_max : 0 ) );

	//slots
	n = rnd( n );
	int slot_roll = 0;
	if( n & 128 )
	{
		n = rnd( n );
		slot_roll = n % 100;
	}
	else
	{
		m = rnd( m );
		slot_roll = m % 100;
	}

	const int num_slots = GetNumSlots( charm_type, slot_table, slot_roll );

	if( num_slots != charm->num_slots )
		return false;

	if( point2 == 0 )
	{
		if( charm->abilities.Count == 2 ||
			skill1_name != charm->abilities[ 0 ]->ability->static_index ||
			point1 != charm->abilities[ 0 ]->amount )
			return false;
	}
	else
	{
		if( charm->abilities.Count != 2 ||
			skill1_name != charm->abilities[ 0 ]->ability->static_index ||
			skill2_name != charm->abilities[ 1 ]->ability->static_index ||
			point1 != charm->abilities[ 0 ]->amount ||
			point2 != charm->abilities[ 1 ]->amount )
			return false;
	}
	return true;
}

array< unsigned char >^ LoadSlotTable( String^ filename )
{
	array< String^ >^ lines = IO::File::ReadAllLines( filename );
	array< unsigned char >^ result = gcnew array< unsigned char >( lines->Length * 3 - 3 );
	for( int i = 1, index = 0; i < lines->Length; ++i )
	{
		array< String^ >^ tokens = lines[ i ]->Split( ',' );
		for( int j = 1; j < tokens->Length; ++j )
			result[ index++ ] = (unsigned char)Convert::ToUInt16( tokens[ j ] );
	}
	return result;
}

template< typename T >
array< T >^ LoadSkillTable( String^ filename )
{
	array< String^ >^ lines = IO::File::ReadAllLines( filename );
	array< T >^ result = gcnew array< T >( lines->Length * 3 - 3 );
	for( int i = 1, index = 0; i < lines->Length; ++i )
	{
		array< String^ >^ tokens = lines[ i ]->Split( ',' );
		
		Ability^ ab = Ability::FindCharmAbility( tokens[ 0 ] );
		result[ index++ ] = ab->static_index;
		result[ index++ ] = (T)Convert::ToInt16( tokens[ 1 ] );
		result[ index++ ] = (T)Convert::ToInt16( tokens[ 2 ] );
	}
	return result;
}

void LoadCharmTableData()
{
	array< String^ >^ charm_names =
	{
		"mystery",
		"shining",
		"ancient",
		"distorted",
		"enduring",
		"graven"
	};

	const unsigned NumCharmTypes = charm_names->Length;

	StaticData::skill1_table = gcnew array< array< unsigned char >^ >( NumCharmTypes );
	StaticData::skill2_table = gcnew array< array< signed char >^ >( NumCharmTypes );
	StaticData::slot_table = gcnew array< array< unsigned char >^ >( NumCharmTypes );

	for( unsigned i = 0; i < NumCharmTypes; ++i )
	{
		StaticData::slot_table[ i ] = LoadSlotTable( "Data/Charm Generation/" + charm_names[ i ] + "_slots.csv" );
		StaticData::skill1_table[ i ] = LoadSkillTable< unsigned char >( "Data/Charm Generation/" + charm_names[ i ] + "_skill1.csv" );
		if( i > 0 )
			StaticData::skill2_table[ i ] = LoadSkillTable< signed char >( "Data/Charm Generation/" + charm_names[ i ] + "_skill2.csv" );
	}
}

unsigned char GetMaxSingleSkill( const int index, const unsigned charm_type )
{
	unsigned char res = 0;
	for( int i = 0; i < StaticData::skill1_table[ charm_type ]->Length; i += 3 )
	{
		if( StaticData::skill1_table[ charm_type ][ i ] == index )
			res = Math::Max( res, StaticData::skill1_table[ charm_type ][ i + 2 ] );
	}
	if( StaticData::skill2_table[ charm_type ] )
	{
		for( int i = 0; i < StaticData::skill2_table[ charm_type ]->Length; i += 3 )
		{
			if( StaticData::skill2_table[ charm_type ][ i ] == index )
				res = Math::Max( res, (unsigned char)StaticData::skill2_table[ charm_type ][ i + 2 ] );
		}
	}
	return res;
}

void SetupSingleSkillMaxs()
{
	for( int i = 0; i < Ability::static_abilities.Count; ++i )
	{
		Ability^ a = Ability::static_abilities[ i ];
		a->max_vals1 = gcnew array< unsigned char >( CharmDatabase::NumCharmTypes );
		for( unsigned charm_type = 0; charm_type < CharmDatabase::NumCharmTypes; charm_type++ )
		{
			a->max_vals1[ charm_type ]= GetMaxSingleSkill( a->static_index, charm_type );
		}
	}
}

void CreateTableSeedList()
{
	CharmDatabase::table_seed_list = gcnew array< List_t< unsigned short >^ >( CharmDatabase::table_seeds->Length );
	for( int i = 0; i < CharmDatabase::table_seeds->Length; ++i )
	{
		CharmDatabase::table_seed_list[ i ] = gcnew List_t< unsigned short >();

		int n = CharmDatabase::table_seeds[ i ];
		do 
		{
			CharmDatabase::table_seed_list[ i ]->Add( (unsigned short)(n & 0xFFFF) );
			n = rnd( n );
		}
		while( n != CharmDatabase::table_seeds[ i ] );

		CharmDatabase::table_seed_list[ i ]->Sort();
	}
}

int FindTable( const int n )
{
	for( int i = 0; i < CharmDatabase::table_seed_list->Length; ++i )
	{
		if( CharmDatabase::table_seed_list[i]->BinarySearch( n ) >= 0 )
			return i;
	}
	return -1;
}

/*void temp()
{
	array< String^ >^ skill_names = { L"なし", L"胴系統倍化", L"毒", L"麻痺", L"睡眠", L"気絶", L"耐泥耐雪", L"対防御DOWN", L"細菌学", L"攻撃", L"防御", L"体力", L"回復速度", L"加護", L"納刀", L"溜め短縮", L"達人", L"痛撃", L"重撃", L"KO", L"減気攻撃", L"体術", L"聴覚保護", L"風圧", L"耐震", L"耐暑", L"耐寒", L"スタミナ", L"気力回復", L"回避性能", L"回避距離", L"気配", L"ガード性能", L"ガード強化", L"特殊攻撃", L"属性攻撃", L"火属性攻撃", L"水属性攻撃", L"雷属性攻撃", L"氷属性攻撃", L"龍属性攻撃", L"火耐性", L"水耐性", L"雷耐性", L"氷耐性", L"龍耐性", L"斬れ味", L"匠", L"研ぎ師", L"抜刀減気", L"抜刀会心", L"剣術", L"装填速度", L"装填数", L"速射", L"反動", L"精密射撃", L"通常弾強化", L"貫通弾強化", L"散弾強化", L"通常弾追加", L"貫通弾追加", L"散弾追加", L"榴弾追加", L"拡散弾追加", L"斬裂弾追加", L"爆破弾追加", L"毒瓶追加", L"麻痺瓶追加", L"睡眠瓶追加", L"強撃瓶追加", L"接撃瓶追加", L"減気瓶追加", L"爆破瓶追加", L"笛", L"砲術", L"爆弾強化", L"回復量", L"広域", L"効果持続", L"腹減り", L"食いしん坊", L"食事", L"肉食", L"調合成功率", L"調合数", L"採取", L"高速収集", L"気まぐれ", L"ハチミツ", L"護石王", L"運気", L"剥ぎ取り", L"捕獲", L"観察眼", L"千里眼", L"運搬", L"狩人", L"盗み無効", L"高速設置", L"燃鱗", L"底力", L"逆境", L"本気", L"闘魂", L"無傷", L"属性解放", L"根性", L"采配", L"号令", L"自動防御", L"属性耐性", L"状態耐性", L"怒", L"刀匠", L"射手", L"不動", L"一心", L"頑強", L"強欲", L"狂撃耐性", L"剛撃", L"指揮", L"乗り", L"居合", L"回避術", L"盾持", L"潔癖", L"護石収集", L"増幅", L"裂傷", L"節食", L"茸食", L"食欲", L"耐粘", L"射法", L"裏稼業", L"斬術", L"特殊会心", L"属性会心", L"北辰納豆流", L"秘伝", L"職工", L"剛腕", L"祈願" };
	array< array< int >^ >^ vals = { { 2, 1, 5, 1, 5, -10, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -5, 10 }, { 3, 1, 5, 1, 5, -10, 7 }, { 4, 1, 5, 1, 5, -10, 7 }, { 5, 1, 8, 1, 8, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -5, 14 }, { 6, 1, 7, 1, 7, -10, 8 }, { 7, 1, 7, 1, 7, -10, 8 }, { 8 }, { 9, 1, 4, 1, 4, -7, 7, 0, 0, -10, 10, 0, 0, -8, 14, 0, 0, -3, 14, 0, 0, 2, 14 }, { 10, 1, 4, 1, 4, -7, 7, 0, 0, -10, 10, 0, 0, -8, 10, 0, 0, -3, 10, 0, 0, 2, 10 }, { 11, 1, 8, 1, 8, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -3, 11, 0, 0, 3, 11 }, { 12, 0, 0, 1, 7, -4, 4, 0, 0, -10, 12, 0, 0, -8, 11, 0, 0, -3, 12 }, { 13, 0, 0, 1, 7, 0, 0, 1, 7, -10, 9, 3, 7, -8, 9, 4, 8, -5, 9, 0, 0, 2, 9 }, { 14, 0, 0, 1, 6, 0, 0, 1, 6, -10, 4, 3, 6, -8, 4, 3, 8, -5, 4, 0, 0, 2, 4 }, { 15, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 3, 6, 1, 5 }, { 16, 1, 4, 1, 4, -7, 7, 0, 0, -10, 10, 0, 0, -8, 14, 0, 0, -5, 14, 0, 0, 3, 14 }, { 17, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 4, 6, 2, 5 }, { 18, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 3, 8, 2, 5 }, { 19, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 5 }, { 20, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 6 }, { 21, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 5 }, { 22, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 3, 8, 1, 5 }, { 23, 0, 0, 1, 4, 0, 0, 1, 5, -10, 3, 3, 5, -8, 3, 3, 6, -5, 4, 0, 0, 2, 4 }, { 24, 1, 7, 1, 7, -10, 8 }, { 25, 1, 10, 0, 0, -10, 10 }, { 26, 1, 10, 0, 0, -10, 10 }, { 27, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 3, 8, 2, 5 }, { 28, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 5 }, { 29, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 9, 3, 6, -1, 9, 0, 0, 2, 9 }, { 30, 0, 0, 0, 0, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 6 }, { 31, 1, 8, 0, 0, -10, 10 }, { 32, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 6 }, { 33, 0, 0, 1, 4, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 6 }, { 34, 0, 0, 1, 4, -4, 4, 1, 6, -10, 4, 3, 6, -8, 4, 3, 8, -5, 4, 0, 0, 2, 4 }, { 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 6, 2, 6, 1, 4 }, { 36, 0, 0, 1, 7, 0, 0, 1, 7, -10, 13, 3, 7, -8, 10, 4, 8, -3, 14 }, { 37, 0, 0, 1, 7, 0, 0, 1, 7, -10, 13, 3, 7, -8, 10, 4, 8, -3, 14 }, { 38, 0, 0, 1, 7, 0, 0, 1, 7, -10, 13, 3, 7, -8, 10, 4, 8, -3, 14 }, { 39, 0, 0, 1, 7, 0, 0, 1, 7, -10, 13, 3, 7, -8, 10, 4, 8, -3, 14 }, { 40, 0, 0, 1, 7, 0, 0, 1, 7, -10, 13, 3, 7, -8, 10, 4, 8, -3, 14 }, { 41, 1, 6, 1, 6, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -8, 14 }, { 42, 1, 6, 1, 6, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -8, 14 }, { 43, 1, 6, 1, 6, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -8, 14 }, { 44, 1, 6, 1, 6, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -8, 14 }, { 45, 1, 6, 1, 6, -10, 10, 0, 0, -10, 13, 0, 0, -8, 11, 0, 0, -8, 14 }, { 46, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 3, 8, 1, 5 }, { 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 7, 2, 6, 1, 4 }, { 48, 0, 0, 1, 4, 0, 0, 0, 0, -10, 8, 0, 0, -8, 8, 0, 0, -3, 10, 0, 0, 2, 8 }, { 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 7, 0, 0, 2, 7 }, { 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 7, 0, 0, 2, 7 }, { 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 7, 2, 6 }, { 52, 0, 0, 1, 4, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 6 }, { 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 7, 2, 6, 1, 4 }, { 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 4, 0, 0, 1, 6 }, { 55, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 4, 6, 2, 5 }, { 56, 1, 6, 1, 6, -10, 10 }, { 57, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 8, 0, 0, 2, 8 }, { 58, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 8, 0, 0, 2, 8 }, { 59, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 8, 0, 0, 2, 8 }, { 60, 1, 6, 1, 6, -8, 8 }, { 61, 1, 6, 1, 6, -10, 10 }, { 62, 1, 6, 1, 6, -10, 10 }, { 63, 1, 6, 1, 6, -10, 10 }, { 64, 1, 6, 1, 6, -10, 10 }, { 65, 1, 6, 1, 6, -10, 10 }, { 66, 1, 6, 1, 6, -10, 10 }, { 67, 1, 8, 0, 0, -10, 10 }, { 68, 1, 7, 0, 0, -10, 10 }, { 69, 1, 8, 0, 0, -10, 10 }, { 70, 1, 7, 0, 0, -10, 10 }, { 71, 1, 8, 0, 0, -10, 10 }, { 72, 1, 8, 0, 0, -10, 10 }, { 73, 1, 6, 1, 6, -10, 10 }, { 74, 1, 6, 1, 6, -8, 8, 0, 0, -10, 10, 0, 0, -8, 10, 0, 0, -5, 10, 0, 0, 4, 10 }, { 75, 1, 6, 1, 6, -8, 8, 0, 0, -10, 10, 0, 0, -8, 14, 0, 0, -5, 14, 0, 0, 3, 14 }, { 76, 1, 6, 1, 6, -8, 8, 0, 0, -10, 10, 0, 0, -8, 10, 0, 0, -3, 10, 0, 0, 3, 10 }, { 77, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 0, 0, 2, 6 }, { 78, 1, 8, 1, 8, -10, 10, 0, 0, -10, 12, 0, 0, -8, 10, 0, 0, -5, 12, 0, 0, 4, 10 }, { 79, 1, 8, 0, 0, -10, 10 }, { 80, 1, 8, 1, 8, -10, 10, 0, 0, -9, 10, 0, 0, -10, 10 }, { 81, 1, 10, 0, 0, -10, 13 }, { 82, 0, 0, 1, 4, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 0, 0, 2, 4 }, { 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, -5, 5, 2, 5, -2, 4 }, { 84, 1, 10, 0, 0, -10, 13 }, { 85, 1, 8, 0, 0, -10, 10 }, { 86, 1, 10, 0, 0, -10, 13 }, { 87, 1, 8, 0, 0, -10, 10 }, { 88, 1, 10, 0, 0, -10, 13 }, { 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, -5, 5 }, { 90, 0, 0, 1, 3, -5, 5, 3, 7, -8, 10 }, { 91, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -5, 7, 2, 5, -5, 7, 2, 6, 1, 7 }, { 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -5, 7, 2, 5, -5, 7, 2, 6, 1, 7 }, { 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -5, 7, 2, 5, -5, 7, 2, 6, 1, 7 }, { 94, 1, 8, 0, 0, -10, 10 }, { 95, 1, 8, 1, 8, -10, 10, 0, 0, -10, 12, 0, 0, -8, 10, 0, 0, -5, 14 }, { 96, 1, 8, 1, 8, -10, 10 }, { 97, 1, 8, 0, 0, -10, 10 }, { 98, 1, 10, 0, 0, -10, 10 }, { 99, 1, 8, 1, 8, -10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10 }, { 100, 1, 3, 1, 5, -7, 7, 1, 7, -10, 10, 3, 7, 1, 10 }, { 101, 0, 0, 1, 6, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 4, 0, 0, 2, 4 }, { 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 4, 2, 6, 1, 4 }, { 103, 0, 0, 0, 0, 0, 0, 1, 6, -3, 4, 3, 6, -1, 4, 3, 8, -1, 6, 4, 7, 2, 6 }, { 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, -3, 3, 2, 5, -2, 6, 2, 4, 1, 4 }, { 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, -3, 3, 2, 5, -2, 4, 2, 6, 1, 4 }, { 106, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, -5, 7, 2, 5, -2, 7, 2, 6 }, { 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 2, 5, -2, 4, 2, 6, 1, 4 }, { 108, 1, 3, 1, 5, -7, 7, 1, 7, -10, 10, 3, 7, 1, 10, 4, 8, 5, 10 }, { 109, 1, 3, 1, 5, -7, 7, 1, 7, -10, 10, 3, 7, 1, 10, 4, 8, 5, 10 }, { 110 }, { 111, 0, 0, 0, 0, 0, 0, 1, 5, -3, 3, 3, 5, -1, 3, 3, 6, -1, 5, 4, 8, 1, 5 }, { 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 3, 0, 0, 0, 0, 1, 4 }, { 113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 3, 0, 0, 0, 0, 1, 4 }, { 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 3, 0, 0, 0, 0, 1, 4 }, { 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 3, 0, 0, 0, 0, 1, 4 }, { 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 3, 0, 0, 0, 0, 1, 4, 1, 4 }, { 117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4 }, { 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4 }, { 119, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6 }, { 120, 1, 3, 0, 0, -5, 5, 1, 5, 0, 0, 1, 7, 0, 0, 2, 7 }, { 121, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4 }, { 122 }, { 123, 1, 10, 0, 0, -8, 8, 0, 0, -5, 5, 1, 8, 0, 0, 2, 8 }, { 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4 }, { 125, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4 }, { 126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4 }, { 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4 }, { 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6 }, { 129, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4 }, { 130 }, { 131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 3, 6 }, { 132, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 3, 6 }, { 133 }, { 134 }, { 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 2, 5 }, { 136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 1, 3 }, { 137, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 1, 3 }, { 138, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 2, 7, 0, 0, 2, 7 }, { 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 2, 7, 0, 0, 2, 7 }, { 140 }, { 141, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 1, 3 }, { 142 }, { 143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 1, 3 }, { 144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 2, 5 } };

	//0: name
	//1-2: mystery
	//3-6: shining
	//7-10: timeworn
	//11-14: weathered
	//15-18: carved
		
	System::Collections::Generic::List< String^ > output1, output2;
	output1.Add(L"スキル系統,最小,最大");
	output2.Add(L"スキル系統,最小,最大");
	for( int i = 0; i < vals->Length; ++i )
	{
		array< int >^ s = vals[ i ];
		if( s->Length < 19 )
			continue;

		if( s[16] > 0 )
			output1.Add( skill_names[ s[0] ] + L"," + Convert::ToString(s[15]) + L"," + Convert::ToString(s[16]) );

		if( s[18] > 0 )
			output2.Add( skill_names[ s[0] ] + L"," + Convert::ToString(s[17]) + L"," + Convert::ToString(s[18]) );
	}

	IO::File::WriteAllLines( L"carved_skill1.csv", output1.ToArray() );
	IO::File::WriteAllLines( L"carved_skill2.csv", output2.ToArray() );
}*/

void CharmDatabase::GenerateCharmTable()
{
	location_cache = gcnew Map_t< System::String^, CharmLocationDatum^ >();
	LoadCharmTableData();
	CreateTableSeedList();
	SetupSingleSkillMaxs();

	return;
}

#pragma endregion

bool IsAutoguardCharm( Charm^ charm )
{
	return charm->num_slots == 0 &&
		charm->abilities.Count == 1 &&
		charm->abilities[ 0 ]->amount == 10 &&
		charm->abilities[ 0 ]->ability == SpecificAbility::auto_guard;
}

bool IsRiderInfectionResCharm( Charm^ charm )
{
	return
		charm->num_slots == 0 &&
		charm->abilities.Count == 2 &&
		(
			(
				charm->abilities[ 0 ]->amount == 10 &&
				charm->abilities[ 0 ]->ability == SpecificAbility::rider &&
				charm->abilities[ 1 ]->amount == 2 &&
				charm->abilities[ 1 ]->ability == SpecificAbility::infection_res
			)
		  ||
			(
				charm->abilities[ 1 ]->amount == 10 &&
				charm->abilities[ 1 ]->ability == SpecificAbility::rider &&
				charm->abilities[ 0 ]->amount == 2 &&
				charm->abilities[ 0 ]->ability == SpecificAbility::infection_res
			)
		);
}

bool CanFind2SkillCharm( Charm^ charm )
{
	if( charm->abilities.Count < 2 )
		return false;

	const unsigned start = ( charm->num_slots == 3 ) ? 2 : 1;

	array< int >^ skill1_index = gcnew array< int >( CharmDatabase::NumCharmTypes );
	array< int >^ skill2_index = gcnew array< int >( CharmDatabase::NumCharmTypes );

	//quick check first
	for( unsigned charm_type = start; charm_type < CharmDatabase::NumCharmTypes; ++charm_type )
	{
		skill1_index[ charm_type ] = -1;
		skill2_index[ charm_type ] = -1;

		array< unsigned char >^ t1 = StaticData::skill1_table[ charm_type ];
		bool t1_tight = false;
		for( int i = 0; i < t1->Length; i += 3 )
		{
			if( t1[i] == charm->abilities[0]->ability->static_index )
			{
				if( charm->abilities[0]->amount <= t1[i+2] )
					skill1_index[ charm_type ] = i;
				t1_tight = charm->abilities[0]->amount == t1[i+2];
				break;
			}
		}
		if( skill1_index[ charm_type ] == -1 )
			continue;

		array< signed char >^ t2 = StaticData::skill2_table[ charm_type ];
		bool t2_tight = false;
		for( int i = 0; i < t2->Length; i += 3 )
		{
			if( (unsigned char)t2[i] == charm->abilities[1]->ability->static_index )
			{
				if( charm->abilities[1]->amount <= t2[i+2] )
					skill2_index[ charm_type ] = i;
				t2_tight = charm->abilities[1]->amount == t2[i+2];
				break;
			}
		}
		if( skill2_index[ charm_type ] == -1 )
			continue;

		if( charm->num_slots == 0 )
			return true;

		if( !t1_tight && !t2_tight )
			return true;
	}

	//slow check if needed

	for( unsigned charm_type = start; charm_type < CharmDatabase::NumCharmTypes; ++charm_type )
	{
		if( skill1_index[ charm_type ] < 0 ||
			skill2_index[ charm_type ] < 0 )
			continue;

		const unsigned skill1_table_index = skill1_index[ charm_type ];
		const unsigned skill2_table_index = skill2_index[ charm_type ];
		
		const unsigned num1 = StaticData::skill1_table[ charm_type ]->Length / 3;
		const unsigned num2 = StaticData::skill2_table[ charm_type ]->Length / 3;

		for( int n = skill1_table_index / 3; n < 65363; n += num1 )
		{
			for( int m = skill2_table_index / 3; m < 65363; m += num2 )
			{
				const int initm = reverse_rnd(reverse_rnd(m == 0 ? num2 : m));
				if( CanGenerate2SkillCharm( charm_type, n, initm, charm ) )
					return true;
			}
		}
	}

	return false;
}

bool CharmDatabase::CharmIsLegal( Charm^ charm )
{
	if( charm->num_slots >= 4 )
		return false;
	else if( charm->abilities.Count == 0 || IsAutoguardCharm( charm ) || IsRiderInfectionResCharm( charm ) )
		return true;
	else if( charm->abilities.Count == 1 )
	{
		AbilityPair^ ap = charm->abilities[0];
		const unsigned start[4] =
		{
			0,
			0,
			1,
			2
		};
		for( unsigned c = start[ charm->num_slots ]; c < CharmDatabase::NumCharmTypes; ++c )
			if( ap->amount <= ap->ability->max_vals1[c] )
				return true;
	}
	else if( charm->abilities.Count == 2 )
	{
		if( !CanFind2SkillCharm( charm ) )
		{
			AbilityPair^ temp = charm->abilities[0];
			charm->abilities[0] = charm->abilities[1];
			charm->abilities[1] = temp;
			if( CanFind2SkillCharm( charm ) )
			{
				temp = charm->abilities[0];
				charm->abilities[0] = charm->abilities[1];
				charm->abilities[1] = temp;
				return true;
			}
			else return false;
		}
		else return true;
	}
	return false;
}

void FindTwoSkillCharms( array< List_t< unsigned >^ >^ charms, const int n0, const int m0, const int num1, const int num2, const unsigned t )
{
	for( int n = n0; n < 65363; n += num1 )
	{
		for( int m = m0; m < 65363; m += num2 )
		{
			const int initm = reverse_rnd(reverse_rnd(m == 0 ? num2 : m));
			if( TryTwoSkillCharm( t, n, initm, charms ) )
				return;
		}
	}
}

void FindTwoSkillCharms( array< List_t< unsigned >^ >^ charms, const unsigned index1, const unsigned index2, const unsigned max_charm_type )
{
	for( unsigned charm_type = 1; charm_type <= max_charm_type; ++charm_type )
	{
		int skill1_table_index;
		for( skill1_table_index = 0; skill1_table_index < StaticData::skill1_table[ charm_type ]->Length; skill1_table_index += 3 )
		{
			if( StaticData::skill1_table[ charm_type ][ skill1_table_index ] == index1 )
				break;
		}
		if( skill1_table_index >= StaticData::skill1_table[ charm_type ]->Length )
			continue;

		int skill2_table_index = -1;
		for( skill2_table_index = 0; skill2_table_index < StaticData::skill2_table[ charm_type ]->Length; skill2_table_index += 3 )
		{
			if( StaticData::skill2_table[ charm_type ][ skill2_table_index ] == index2 )
				break;
		}
		if( skill2_table_index >= StaticData::skill2_table[ charm_type ]->Length )
			continue;

		const unsigned num_skills1 = StaticData::skill1_table[ charm_type ]->Length / 3;
		const unsigned num_skills2 = StaticData::skill2_table[ charm_type ]->Length / 3;

		FindTwoSkillCharms( charms, skill1_table_index / 3, skill2_table_index / 3, num_skills1, num_skills2, charm_type );
	}
}

bool ContainsBetterCharm( List_t< Charm^ >^ charms, const int p1, const int p2, Ability^ ab1, Ability^ ab2 )
{
	for( int i = 0; i < charms->Count; ++i )
	{
		if( charms[i]->abilities[0]->ability == ab1 &&
			charms[i]->abilities[1]->ability == ab2 &&
			charms[i]->abilities[0]->amount >= p1 &&
			charms[i]->abilities[1]->amount >= p2 )
			return true;
	}
	return false;
}

bool ContainsBetterCharm( List_t< Charm^ >^ charms, Charm^ charm )
{
	for each( Charm^ c in charms )
	{
		if( c->abilities[0]->ability == charm->abilities[0]->ability &&
			c->abilities[0]->amount >= charm->abilities[0]->amount )
			return true;
		if( c->abilities.Count == 2 &&
			c->abilities[1]->ability == charm->abilities[0]->ability &&
			c->abilities[1]->amount >= charm->abilities[0]->amount )
			return true;
	}
	return false;
}

void GetDoubleSkillCharms( List_t< Charm^ >^ list, List_t< Skill^ >% skills, const unsigned max_charm_type )
{
	const unsigned max_slots = Math::Min( 3u, max_charm_type + 1 );
	
	array< List_t< Charm^ >^ >^ two_skills = gcnew array< List_t< Charm^ >^ >( max_slots + 1 );
	for( unsigned k = 0; k <= max_slots; ++k )
		two_skills[ k ] = gcnew List_t< Charm^ >();

	for( int i = 1; i < skills.Count; ++i )
	{
		Ability^ ab1 = skills[ i ]->ability;
		for( int j = 0; j < i; ++j )
		{
			Ability^ ab2 = skills[ j ]->ability;

			array< List_t< unsigned >^ >^ charms = gcnew array< List_t< unsigned >^ >( max_slots + 1 );
			for( unsigned k = 0; k <= max_slots; ++k )
				charms[ k ] = gcnew List_t< unsigned >();

			FindTwoSkillCharms( charms, ab1->static_index, ab2->static_index, max_charm_type );

			for( unsigned k = 0; k <= max_slots; ++k )
			{
				for( unsigned l = charms[k]->Count; l --> 0; )
				{
					const int p1 = charms[k][l] >> 16;
					const int p2 = charms[k][l] & 0xFFFF;
					if( !ContainsBetterCharm( two_skills[k], p1, p2, ab1, ab2 ) )
					{
						Charm^ c = gcnew Charm( k );
						c->abilities.Add( gcnew AbilityPair( ab1, p1 ) );
						c->abilities.Add( gcnew AbilityPair( ab2, p2 ) );

						two_skills[k]->Add( c );
					}
				}
			}

			for( unsigned k = 0; k <= max_slots; ++k )
				charms[ k ]->Clear();

			FindTwoSkillCharms( charms, ab2->static_index, ab1->static_index, max_charm_type );

			for( unsigned k = 0; k <= max_slots; ++k )
			{
				for( unsigned l = charms[k]->Count; l --> 0; )
				{
					const int p1 = charms[k][l] >> 16;
					const int p2 = charms[k][l] & 0xFFFF;
					if( !ContainsBetterCharm( two_skills[k], p1, p2, ab2, ab1 ) )
					{
						Charm^ c = gcnew Charm( k );
						c->abilities.Add( gcnew AbilityPair( ab2, p1 ) );
						c->abilities.Add( gcnew AbilityPair( ab1, p2 ) );

						two_skills[k]->Add( c );
					}
				}
			}
		}
	}

	for each( Charm^ c in list )
	{
		if( !ContainsBetterCharm( two_skills[ c->num_slots ], c ) )
			two_skills[ c->num_slots ]->Add( c );
	}

	list->Clear();
	for( unsigned k = 0; k <= max_slots; ++k )
		list->AddRange( two_skills[ k ] );
}

void GetSingleSkillCharms( List_t< Charm^ >^ list, List_t< Skill^ >% skills, const unsigned max_charm_type )
{
	const unsigned max_slots = Math::Min( 3u, max_charm_type + 1 );
	for each( Skill^ skill in skills )
	{
		if( skill->ability->max_vals1 == nullptr )
			continue;

		for( unsigned slots = 1; slots <= max_slots; ++slots )
		{
			Charm^ ct = gcnew Charm;
			ct->num_slots = slots;
			ct->abilities.Add( gcnew AbilityPair( skill->ability, skill->ability->max_vals1[ max_charm_type ] ) );
			list->Add( ct );
		}
	}
}

void AddSlotOnlyCharms( List_t< Charm^ >^ res, Query^ query, const unsigned max_charm_type )
{
	bool have[ 4 ] = { false, false, false, false };
	for each( Charm^ charm in res )
	{
		have[ charm->num_slots ] = true;
	}

	for( unsigned slots = Math::Min( 3u, max_charm_type + 1 ); slots > 0; --slots )
	{
		if( !have[ slots ] )
		{
			res->Add( gcnew Charm( slots ) );
			break;
		}
	}
}

List_t< Charm^ >^ CharmDatabase::GetCharms( Query^ query, const bool use_two_skill_charms )
{
	List_t< Charm^ >^ res = gcnew List_t< Charm^ >;
	const unsigned max_charm_type = CalcMaxCharmType( query );

	GetSingleSkillCharms( res, query->skills, max_charm_type );

	if( use_two_skill_charms && max_charm_type > 0 )
		GetDoubleSkillCharms( res, query->skills, max_charm_type );

	AddSlotOnlyCharms( res, query, max_charm_type );

	return res;
}

CharmLocationDatum^ CharmDatabase::FindCharmLocations( Charm^ charm )
{
	CharmLocationDatum^ result = gcnew CharmLocationDatum();
	result->table = gcnew array< unsigned, 2 >( 17, 6 );
	result->examples = gcnew array< System::String^ >( 17 );
	const unsigned limit = 64000;

	unsigned num_found = 0;
	for( unsigned charm_type = 0; charm_type < NumCharmTypes; ++charm_type )
	{
		if( charm->num_slots > charm_type + 1 )
			continue;

		if( charm->abilities.Count == 0 )
		{
			for( int t = 0; t < table_seeds->Length; ++t )
				result->table[t, charm_type] = limit;
			continue;
		}

		int skill1_table_index = -1;
		for( skill1_table_index = 0; skill1_table_index < StaticData::skill1_table[ charm_type ]->Length; skill1_table_index += 3 )
		{
			if( StaticData::skill1_table[ charm_type ][ skill1_table_index ] == charm->abilities[0]->ability->static_index &&
				StaticData::skill1_table[ charm_type ][ skill1_table_index + 2 ] >= charm->abilities[0]->amount )
				break;
		}

		const unsigned num1 = StaticData::skill1_table[ charm_type ]->Length / 3;

		if( charm->abilities.Count == 1 )
		{
			int skill2_table_index = -1;
			if( StaticData::skill2_table[ charm_type ] )
			{
				for( skill2_table_index = 0; skill2_table_index < StaticData::skill2_table[ charm_type ]->Length; skill2_table_index += 3 )
				{
					if( StaticData::skill2_table[ charm_type ][ skill2_table_index ] == charm->abilities[0]->ability->static_index &&
						StaticData::skill2_table[ charm_type ][ skill2_table_index + 2 ] >= charm->abilities[0]->amount )
						break;
				}
			}
			if( skill1_table_index >= StaticData::skill1_table[ charm_type ]->Length &&
				( skill2_table_index < 0 || skill2_table_index >= StaticData::skill2_table[ charm_type ]->Length ) )
				continue;

			for( int t = 0; t < table_seeds->Length; ++t )
			{
				if( t == 10 || t == 11  || t == 14 || t == 15 || t == 16 )
				{
					//do nothing. see MH4 implementation
				}
				else
				{
					result->table[t, charm_type] = limit;
				}
			}
		}
		else if( StaticData::skill2_table[ charm_type ] )
		{
			if( skill1_table_index >= StaticData::skill1_table[ charm_type ]->Length )
				continue;

			int skill2_table_index = -1;
			for( skill2_table_index = 0; skill2_table_index < StaticData::skill2_table[ charm_type ]->Length; skill2_table_index += 3 )
			{
				if( StaticData::skill2_table[ charm_type ][ skill2_table_index ] == charm->abilities[1]->ability->static_index &&
					StaticData::skill2_table[ charm_type ][ skill2_table_index + 2 ] >= charm->abilities[1]->amount )
					break;
			}
			if( skill2_table_index >= StaticData::skill2_table[ charm_type ]->Length )
				continue;

			const unsigned num2 = StaticData::skill2_table[ charm_type ]->Length / 3;

			for( int n = skill1_table_index / 3; n < 65363; n += num1 )
			{
				const int table = FindTable( n );
				if( table == -1 )
					continue;

				for( int m = skill2_table_index / 3; m < 65363; m += num2 )
				{
					const int initm = reverse_rnd(reverse_rnd(m == 0 ? num2 : m));
					String^ str = CanGenerateCharm2( charm_type, n, initm, charm );
					if( str )
					{
						if( result->table[table, charm_type]++ == 0 )
							result->examples[ table ] = str;
						else
							result->examples[ table ] = nullptr;

						if( num_found++ == 0 )
							result->example = str;
						else result->example = nullptr;
					}
				}
			}
		}
	}
	return result;
}
