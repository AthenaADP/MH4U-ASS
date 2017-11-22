#pragma once

namespace Blowfish
{
	ref class Cypher
	{
	public:
		Cypher( char key[56] );
		array< unsigned char >^ Decrypt( array< unsigned char >^ src );

	private:
		void SetKey( array< unsigned char >^ key );
		void EncryptBlock( unsigned long% left, unsigned long% right );
		void DecryptBlock( unsigned long% left, unsigned long% right );
		unsigned long Feistel( unsigned long val );

		array< unsigned long >^ pary;
		array< array< unsigned long >^ >^ sbox;
	};
}
