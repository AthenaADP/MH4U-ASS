#include "stdafx.h"
#include "SaveData.h"
#include "Blowfish.h"
#include "../CharmDatabase.h"

unsigned short GetShort( array< unsigned char >^ a, unsigned pos )
{
	unsigned short b1 = a[pos];
	unsigned short b2 = a[pos+1];
	return b1 + ( b2 << 8 );
}

array< unsigned char >^ DecryptSaveData( array< unsigned char >^ data )
{
	Blowfish::Cypher cypher( "blowfish key iorajegqmrna4itjeangmb agmwgtobjteowhv9mope" );

	array< unsigned char >^ data2 = cypher.Decrypt( data );

	const unsigned short sixteen = GetShort( data2, 0 );

	unsigned short key = GetShort( data2, 2 );

	unsigned tempkey = key, csum1 = 0, csum2 = 0;

	for( int position = 4; position < data2->Length; position += 2 )
	{
		if( tempkey == 0 )
			tempkey = 1;

		tempkey *= 0xB0;
		tempkey %= 0xFF53;
		unsigned short out = GetShort( data2, position );
		out ^= tempkey;
		data2[ position + 0 ] = out & 0xFF;
		data2[ position + 1 ] = out >> 8;

		if( position == 4 )
			csum1 = out;

		if( position == 6 )
			csum2 = out;
	}

	unsigned csum_calc = 0;
	for( int i = 8; i < data2->Length; ++i )
		csum_calc += data2[ i ];

	csum_calc &= 0xFFFFFFFF;

	const unsigned csum = ((csum2 & 0xFFFF) << 16) | (csum1 & 0xFFFF);

	return data2;
}

List_t< Charm^ >^ ExtractCharms( array< unsigned char >^ savedata )
{
	List_t< Charm^ >^ charms = gcnew List_t< Charm^ >();

	unsigned equip_start, equip_end;
	
	if( savedata->Length == 0x13E00)
	{
		equip_start = 0x1746;
		equip_end = 0xBB4E;
	}
	else if( savedata->Length == 0x13DF8 )
	{
		equip_start = 0x173E;
		equip_end = 0xBB46;
	}
	else return charms;

	//iterate over equipment box
	for( unsigned i = equip_start; i < equip_end; i += 0x1C )
	{
		//0x06 is the code for a charm
		if( savedata[i] != 0x06 )
			continue;

		Charm^ charm = gcnew Charm( savedata[ i + 1 ] );
		if( charm->num_slots > 3 )
			continue;

		const unsigned skill1_index = savedata[ i + 12 ] + ( savedata[ i + 13 ] << 8 );
		const unsigned skill1_value = savedata[ i + 14 ] + ( savedata[ i + 15 ] << 8 );

		if( skill1_value > 14 || skill1_index >= (unsigned)Ability::savedata_abilities.Count )
			continue;

		Ability^ ab1 = Ability::savedata_abilities[ skill1_index ];
		if( !ab1 )
			continue;

		charm->abilities.Add( gcnew AbilityPair( ab1, skill1_value ) );

		const unsigned skill2_index = savedata[ i + 16 ] + ( savedata[ i + 17 ] << 8 );
		if( skill2_index > 0 )
		{
			//check for Torso Up
			if( skill2_index == 1 )
				continue;

			const signed short skill2_value = savedata[ i + 18 ] + ( savedata[ i + 19 ] << 8 );

			if( skill2_value > 14 || skill2_value < -10 || skill2_index >= (unsigned)Ability::savedata_abilities.Count )
				continue;

			Ability^ ab2 = Ability::savedata_abilities[ skill2_index ];
			if( !ab2 )
				continue;

			charm->abilities.Add( gcnew AbilityPair( ab2, skill2_value ) );
		}

		if( CharmDatabase::CharmIsLegal( charm ) )
		{
			charm->custom = true;
			charms->Add( charm );
		}
	}

	return charms;
}

List_t< Weapon^ >^ ExtractRelicWeapons( array< unsigned char >^ savedata )
{
	List_t< Weapon^ >^ weapons = gcnew List_t< Weapon^ >();

	return weapons;
}

List_t< Armor^ >^ ExtractRelicArmors( array< unsigned char >^ savedata )
{
	List_t< Armor^ >^ armors = gcnew List_t< Armor^ >();

	return armors;
}