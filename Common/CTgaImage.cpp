#include "pch.h"

#include "Common/CTgaImage.h"

UwpEngine::CTgaImage::CTgaImage()
{
	memset(this, 0x00, sizeof(CTgaImage));
}

UwpEngine::CTgaImage::~CTgaImage()
{
	if (m_pixels8)
	{
		free(m_pixels8);
	}

	if (m_pixels24)
	{
		free(m_pixels24);
	}

	if (m_pixels32)
	{
		free(m_pixels32);
	}
}

UwpEngine::CTgaImage::CTgaImage(const char* filename)
{
	memset(this, 0x00, sizeof(CTgaImage));

	errno_t error = fopen_s(&m_file, filename, "rb");

	if (error != 0)
	{
		return;
	}

	Load();

	m_isInitialized = true;

	if (m_file)
	{
		fclose(m_file);
	}
}

bool UwpEngine::CTgaImage::Load()
{
	LoadHeader();
	LoadImageDescription();
	LoadColorTable();
	LoadPixelData();
	LoadExtensions();

	if (m_header.imageDescriptor & 8)
	{
		Flip();
	}

	return true;
}

void UwpEngine::CTgaImage::Flip()
{
	size_t pitch = (size_t)m_header.width * 4;
	size_t height = (size_t)m_header.height;

	BYTE* flip = (BYTE*)malloc(height * pitch);
	BYTE* fliph = flip;

	BYTE* start = m_pixels32 + (height * pitch) - pitch;

	for (UINT i = 0; i < height; i++)
	{
		memcpy(fliph, start, pitch);
		
		start -= pitch;
		fliph += pitch;
	}

	memcpy(m_pixels32, flip, height * pitch);

	free(flip);
}

void UwpEngine::CTgaImage::DumpHeader()
{
	FILE* file;

	errno_t error = fopen_s(&file, "header.txt", "w");

	memset(m_text, 0x00, MAX_TEXT);
	sprintf_s((char*)m_text, MAX_TEXT, "%05d", m_header.width);

	size_t bytesWriten = fwrite(m_text, sizeof(BYTE), strlen((char*)m_text), file);

	memset(m_text, 0x00, MAX_TEXT);
	sprintf_s((char*)m_text, MAX_TEXT, "%05d", m_header.height);

	bytesWriten = fwrite(m_text, sizeof(BYTE), strlen((char*)m_text), file);

	if (file)
	{
		fclose(file);
	}
}

void UwpEngine::CTgaImage::DumpFooter()
{
	FILE* file;

	errno_t error = fopen_s(&file, "footer.txt", "w");

	size_t bytesWriten = fwrite(m_footer.signature, sizeof(BYTE), strlen((char*)m_footer.signature), file);

	if (file)
	{
		fclose(file);
	}
}

void UwpEngine::CTgaImage::DumpColorMap()
{
	FILE* file;

	errno_t error = fopen_s(&file, "colorMap.txt", "wb");

	int i = sizeof(BGR);

	size_t bytesWriten = fwrite(m_colorTable, i, m_header.cMapLength, file);

	if (file)
	{
		fclose(file);
	}
}

void UwpEngine::CTgaImage::DumpRawPixels8()
{
	FILE* file;

	errno_t error = fopen_s(&file, "pixels8.txt", "wb");

	if (m_pixels8 > 0)
	{
		size_t bytesWriten = fwrite(m_pixels8, sizeof(BYTE), m_size8, file);
	}

	if (file)
	{
		fclose(file);
	}
}

void UwpEngine::CTgaImage::DumpRawPixels24()
{
	FILE* file;

	errno_t error = fopen_s(&file, "pixels24.txt", "wb");

	if (m_pixels24 > 0)
	{
		size_t bytesWriten = fwrite(m_pixels24, sizeof(RGB), m_size24, file);
	}

	if (file)
	{
		fclose(file);
	}
}

void UwpEngine::CTgaImage::DumpRawPixels32()
{
	FILE* file;

	errno_t error = fopen_s(&file, "pixels32.txt", "wb");

	if (m_pixels32 > 0)
	{
		size_t bytesWriten = fwrite(m_pixels32, sizeof(RGBA), m_size32, file);
	}

	if (file)
	{
		fclose(file);
	}
}

void UwpEngine::CTgaImage::LoadHeader()
{
	size_t bytesRead = fread_s(&m_header.idLength, sizeof(BYTE), sizeof(BYTE), 1, m_file);

	bytesRead = fread_s(&m_header.colorMapType, sizeof(BYTE), sizeof(BYTE), 1, m_file);
	bytesRead = fread_s(&m_header.imageType, sizeof(BYTE), sizeof(BYTE), 1, m_file);
	bytesRead = fread_s(&m_header.cMapStart, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.cMapLength, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.cMapDepth, sizeof(BYTE), sizeof(BYTE), 1, m_file);
	bytesRead = fread_s(&m_header.xOffset, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.yOffset, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.width, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.height, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.pixelDepth, sizeof(BYTE), sizeof(BYTE), 1, m_file);
	bytesRead = fread_s(&m_header.imageDescriptor, sizeof(BYTE), sizeof(BYTE), 1, m_file);

	m_size8  = m_header.width * m_header.height;
	m_size24 = m_header.width * m_header.height * sizeof(RGB);
	m_size32 = m_header.width * m_header.height * sizeof(RGBA);
}

void UwpEngine::CTgaImage::LoadImageDescription()
{
	// max length is 256
	if (m_header.idLength > 0)
	{
		size_t bytesRead = fread_s(&m_imageDescription, sizeof(BYTE) * 256, sizeof(BYTE), m_header.idLength, m_file);
	}
}

void UwpEngine::CTgaImage::LoadColorTable()
{
	if (m_header.cMapLength > 0)
	{
		size_t bytesRead = fread_s(&m_colorTable, sizeof(BGR) * 256, sizeof(BGR), m_header.cMapLength, m_file);
	}
}

void UwpEngine::CTgaImage::LoadPixelData()
{
	switch (m_header.imageType)
	{
		// Colormapped image data
		case 1:
		{
			if (m_header.pixelDepth == 8)
			{
				Load8BitUncompressed();
			}

			break;
		}

		// Truecolor image data
		case 2:
		{
			if (m_header.pixelDepth == 24)
			{
				Load24BitUncompressed();
			}

			if (m_header.pixelDepth == 32)
			{
				Load32BitUncompressed();
			}

			break;
		}

		// Colormap with RLE Compression
		case 9:
		{
			if (m_header.pixelDepth == 8)
			{
				Load8BitCompressed();
			}

			if (m_header.pixelDepth == 24)
			{
				Load24BitCompressed();
			}

			if (m_header.pixelDepth == 32)
			{
				Load32BitCompressed();
			}

			break;
		}

		// BGR with RLE Compression
		case 10:
		{
			if (m_header.pixelDepth == 8)
			{
				Load8BitCompressed();
			}

			if (m_header.pixelDepth == 24)
			{
				Load24BitCompressed();
			}

			if (m_header.pixelDepth == 32)
			{
				Load32BitCompressed();
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void UwpEngine::CTgaImage::Load8BitUncompressed()
{
	m_pixels8 = (BYTE*)malloc(m_size8);
	m_pixels24 = (BYTE*)malloc(m_size24);
	m_pixels32 = (BYTE*)malloc(m_size32);

	BYTE* pIterator8 = m_pixels8;
	BYTE* pIterator24 = m_pixels24;
	BYTE* pIterator32 = m_pixels32;

	BYTE value;

	int pcount = 0;

	while (m_size32 > pcount)
	{
		size_t bytesRead = fread_s(&value, sizeof(BYTE), sizeof(BYTE), 1, m_file);

		BGR bgr = m_colorTable[value];

		*pIterator8 = value;
		pIterator8 += 1;


		*pIterator24 = bgr.R;
		pIterator24 += 1;

		*pIterator24 = bgr.G;
		pIterator24 += 1;

		*pIterator24 = bgr.B;
		pIterator24 += 1;


		*pIterator32 = bgr.R;
		pIterator32 += 1;

		*pIterator32 = bgr.G;
		pIterator32 += 1;

		*pIterator32 = bgr.B;
		pIterator32 += 1;

		*pIterator32 = 255;
		pIterator32 += 1;

		pcount += 4;
	}
}

void UwpEngine::CTgaImage::Load24BitUncompressed()
{
	m_pixels24 = (BYTE*)malloc(m_size24);
	m_pixels32 = (BYTE*)malloc(m_size32);

	BYTE* pIterator24 = m_pixels24;
	BYTE* pIterator32 = m_pixels32;

	BGR bgr;

	int pcount = 0;

	while (m_size32 > pcount)
	{
		size_t bytesRead = fread_s(&bgr, sizeof(BGR), sizeof(BGR), 1, m_file);

		*pIterator24 = bgr.R;
		pIterator24 += 1;

		*pIterator24 = bgr.G;
		pIterator24 += 1;

		*pIterator24 = bgr.B;
		pIterator24 += 1;


		*pIterator32 = bgr.R;
		pIterator32 += 1;

		*pIterator32 = bgr.G;
		pIterator32 += 1;

		*pIterator32 = bgr.B;
		pIterator32 += 1;

		*pIterator32 = 255;
		pIterator32 += 1;

		pcount += 4;
	}
}

void UwpEngine::CTgaImage::Load32BitUncompressed()
{
	m_pixels24 = (BYTE*)malloc(m_size24);
	m_pixels32 = (BYTE*)malloc(m_size32);

	BYTE* pIterator24 = m_pixels24;
	BYTE* pIterator32 = m_pixels32;

	BGRA bgra;

	int pcount = 0;

	while (m_size32 > pcount)
	{
		size_t bytesRead = fread_s(&bgra, sizeof(BGRA), sizeof(BGRA), 1, m_file);

		*pIterator24 = bgra.R;
		pIterator24 += 1;

		*pIterator24 = bgra.G;
		pIterator24 += 1;

		*pIterator24 = bgra.B;
		pIterator24 += 1;


		*pIterator32 = bgra.R;
		pIterator32 += 1;

		*pIterator32 = bgra.G;
		pIterator32 += 1;

		*pIterator32 = bgra.B;
		pIterator32 += 1;

		*pIterator32 = bgra.A;
		pIterator32 += 1;

		pcount += 4;
	}
}

void UwpEngine::CTgaImage::Load8BitCompressed()
{
	m_pixels8 = (BYTE*)malloc(m_size8);
	m_pixels24 = (BYTE*)malloc(m_size24);
	m_pixels32 = (BYTE*)malloc(m_size32);

	BYTE* pIterator8 = m_pixels8;
	BYTE* pIterator24 = m_pixels24;
	BYTE* pIterator32 = m_pixels32;

	int pcount = 0;

	RLEPacket8* p8 = new RLEPacket8();

	while (m_size32 > pcount)
	{
		p8->ReadCount(m_file);

		int count = p8->GetCount();

		// encoded or raw
		if (p8->IsEncoded())
		{
			p8->ReadValue(m_file);

			BGR bgr = m_colorTable[p8->value];

			for (int c = 0; c < count; c++)
			{
				*pIterator8 = p8->value;
				pIterator8 += 1;


				*pIterator24 = bgr.R;
				pIterator24 += 1;

				*pIterator24 = bgr.G;
				pIterator24 += 1;

				*pIterator24 = bgr.B;
				pIterator24 += 1;


				*pIterator32 = bgr.R;
				pIterator32 += 1;

				*pIterator32 = bgr.G;
				pIterator32 += 1;

				*pIterator32 = bgr.B;
				pIterator32 += 1;

				*pIterator32 = 255;
				pIterator32 += 1;

				pcount += 4;
			}
		}
		else
		{
			for (int c = 0; c < count; c++)
			{
				size_t bytesRead = fread_s(&p8->value, sizeof(BYTE), sizeof(BYTE), 1, m_file);

				BGR bgr = m_colorTable[p8->value];

				*pIterator8 = p8->value;
				pIterator8 += 1;


				*pIterator24 = bgr.R;
				pIterator24 += 1;

				*pIterator24 = bgr.G;
				pIterator24 += 1;

				*pIterator24 = bgr.B;
				pIterator24 += 1;


				*pIterator32 = bgr.R;
				pIterator32 += 1;

				*pIterator32 = bgr.G;
				pIterator32 += 1;

				*pIterator32 = bgr.B;
				pIterator32 += 1;

				*pIterator32 = 255;
				pIterator32 += 1;

				pcount += 4;
			}
		}
	}

	delete p8;
}

void UwpEngine::CTgaImage::Load24BitCompressed()
{
	m_pixels24 = (BYTE*)malloc(m_size24);
	m_pixels32 = (BYTE*)malloc(m_size32);

	BYTE* pIterator24 = m_pixels24;
	BYTE* pIterator32 = m_pixels32;

	int pcount = 0;

	RLEPacket24* p24 = new RLEPacket24();

	while (m_size32 > pcount)
	{
		p24->ReadCount(m_file);

		int count = p24->GetCount();

		// encoded or raw
		if (p24->IsEncoded())
		{
			p24->ReadValue(m_file);

			for (int c = 0; c < count; c++)
			{
				*pIterator24 = p24->value.R;
				pIterator24 += 1;

				*pIterator24 = p24->value.G;
				pIterator24 += 1;

				*pIterator24 = p24->value.B;
				pIterator24 += 1;


				*pIterator32 = p24->value.R;
				pIterator32 += 1;

				*pIterator32 = p24->value.G;
				pIterator32 += 1;

				*pIterator32 = p24->value.B;
				pIterator32 += 1;

				*pIterator32 = 255;
				pIterator32 += 1;

				pcount += 4;
			}
		}
		else
		{
			for (int c = 0; c < count; c++)
			{
				p24->ReadValue(m_file);

				*pIterator24 = p24->value.R;
				pIterator24 += 1;

				*pIterator24 = p24->value.G;
				pIterator24 += 1;

				*pIterator24 = p24->value.B;
				pIterator24 += 1;


				*pIterator32 = p24->value.R;
				pIterator32 += 1;

				*pIterator32 = p24->value.G;
				pIterator32 += 1;

				*pIterator32 = p24->value.B;
				pIterator32 += 1;

				*pIterator32 = 255;
				pIterator32 += 1;

				pcount += 4;
			}
		}
	}
}

void UwpEngine::CTgaImage::Load32BitCompressed()
{
	m_pixels24 = (BYTE*)malloc(m_size24);
	m_pixels32 = (BYTE*)malloc(m_size32);

	BYTE* pIterator24 = m_pixels24;
	BYTE* pIterator32 = m_pixels32;

	int pcount = 0;

	RLEPacket32* p32 = new RLEPacket32();

	while (m_size32 > pcount)
	{
		p32->ReadCount(m_file);

		int count = p32->GetCount();

		// encoded or raw
		if (p32->IsEncoded())
		{
			p32->ReadValue(m_file);

			for (int c = 0; c < count; c++)
			{
				*pIterator24 = p32->value.R;
				pIterator24 += 1;

				*pIterator24 = p32->value.G;
				pIterator24 += 1;

				*pIterator24 = p32->value.B;
				pIterator24 += 1;


				*pIterator32 = p32->value.R;
				pIterator32 += 1;

				*pIterator32 = p32->value.G;
				pIterator32 += 1;

				*pIterator32 = p32->value.B;
				pIterator32 += 1;

				*pIterator32 = p32->value.A;
				pIterator32 += 1;

				pcount += 4;
			}
		}
		else
		{
			for (int c = 0; c < count; c++)
			{
				p32->ReadValue(m_file);

				*pIterator24 = p32->value.R;
				pIterator24 += 1;

				*pIterator24 = p32->value.G;
				pIterator24 += 1;

				*pIterator24 = p32->value.B;
				pIterator24 += 1;


				*pIterator32 = p32->value.R;
				pIterator32 += 1;

				*pIterator32 = p32->value.G;
				pIterator32 += 1;

				*pIterator32 = p32->value.B;
				pIterator32 += 1;

				*pIterator32 = p32->value.A;
				pIterator32 += 1;

				pcount += 4;
			}
		}
	}
}

void UwpEngine::CTgaImage::LoadExtensions()
{
	if (!feof(m_file))
	{
		m_footer.ReadValues(m_file);
	}

	if (m_footer.extensionOffset > 0)
	{

	}

	if (m_footer.developerOffset > 0)
	{

	}
}
