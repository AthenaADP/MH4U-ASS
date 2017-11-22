#pragma once
#include "Common.h"
#include "Armor.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MH4GASS
{

	public ref class PreviewImage : public System::Windows::Forms::Form
	{
		Bitmap^ image;
		String^ path;
		bool female;

	public:
		PreviewImage( const bool female ) : image( nullptr ), female( female )
		{
			InitializeComponent();

			if( female )
				path = L"Data/Images/Female/";
			else
				path = L"Data/Images/Male/";
			
			String^ base_filename = path + L"0.png";
			if( IO::File::Exists( base_filename ) )
				image = gcnew Bitmap( base_filename );
		}

#pragma warning( disable: 4677 )

		bool SetData( List_t< Armor^ >% armors )
		{
			if( !image )
				return false;

			MergeImage( L"Waist/", armors[ (int)Armor::ArmorType::WAIST ], true );
			MergeImage( L"Body/",  armors[ (int)Armor::ArmorType::BODY ], false );
			MergeImage( L"Legs/",  armors[ (int)Armor::ArmorType::LEGS ], false );
			MergeImage( L"Head/",  armors[ (int)Armor::ArmorType::HEAD ], false );
			MergeImage( L"Waist/", armors[ (int)Armor::ArmorType::WAIST ], false );
			MergeImage( L"Arms/",  armors[ (int)Armor::ArmorType::ARMS ], false );

			pictureBox1->Image = image;

			return true;
		}

		bool CanShow()
		{
			return !!image;
		}

	protected:

		void MergeImage( String^ subpath, Armor^ armor, bool use_mid_alpha_only )
		{
			if( !armor )
				return;

			const int index = female ? armor->female_image : armor->male_image;
			if( index < 1 )
				return;

			String^ filename = path + subpath + Convert::ToString( index ) + L".png";
			if( !IO::File::Exists( filename ) )
				return;

			Bitmap^ b = gcnew Bitmap( filename );
			for( int x = 0; x < b->Width; ++x )
			{
				for( int y = 0; y < b->Height; ++y )
				{
					Drawing::Color c = b->GetPixel( x, y );
					if( c.A == 0 )
						continue;

					if( ( c.A < 255 ) == use_mid_alpha_only )
					{
						image->SetPixel( x, y, Drawing::Color::FromArgb( 255, c ) );
					}
				}
			}
		}
		
		~PreviewImage()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::PictureBox^  pictureBox1;
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
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox1->Location = System::Drawing::Point(2, 2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(175, 215);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// PreviewImage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(179, 199);
			this->ControlBox = false;
			this->Controls->Add(this->pictureBox1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"PreviewImage";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->TopMost = true;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
