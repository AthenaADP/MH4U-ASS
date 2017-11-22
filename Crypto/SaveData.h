#pragma once
#include "../Armor.h"

array< unsigned char >^ DecryptSaveData( array< unsigned char >^ data );

List_t< Charm^ >^ ExtractCharms( array< unsigned char >^ savedata );
List_t< Weapon^ >^ ExtractRelicWeapons( array< unsigned char >^ savedata );
List_t< Armor^ >^ ExtractRelicArmors( array< unsigned char >^ savedata );