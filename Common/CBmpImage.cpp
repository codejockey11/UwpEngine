#include "pch.h"

#include "CBmpImage.h"

UwpEngine::CBmpImage::CBmpImage()
{
	memset(this, 0x00, sizeof(CBmpImage));
}

UwpEngine::CBmpImage::CBmpImage(const char* filename)
{
	memset(this, 0x00, sizeof(CBmpImage));

	m_filename = new CString(filename);

	m_isInitialized = false;

	errno_t err = fopen_s(&m_file, filename, "rb");

	if (err)
	{
		return;
	}

	size_t bytesRead = fread_s(&m_header.bfType, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.bfSize, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	bytesRead = fread_s(&m_header.bfReserved1, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.bfReserved2, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_header.bfOffBits, sizeof(DWORD), sizeof(DWORD), 1, m_file);

	bytesRead = fread_s(&m_info.bmiHeader.biSize, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biWidth, sizeof(LONG), sizeof(LONG), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biHeight, sizeof(LONG), sizeof(LONG), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biPlanes, sizeof(WORD), sizeof(WORD), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biBitCount, sizeof(WORD), sizeof(WORD), 1, m_file);

	m_bpp = m_info.bmiHeader.biBitCount / 8;

	bytesRead = fread_s(&m_info.bmiHeader.biCompression, sizeof(DWORD), sizeof(DWORD), 1, m_file);

	// only uncompressed images
	if (m_info.bmiHeader.biCompression != BI_RGB)
	{
		fclose(m_file);

		return;
	}

	bytesRead = fread_s(&m_info.bmiHeader.biSizeImage, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biXPelsPerMeter, sizeof(LONG), sizeof(LONG), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biYPelsPerMeter, sizeof(LONG), sizeof(LONG), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biClrUsed, sizeof(DWORD), sizeof(DWORD), 1, m_file);
	bytesRead = fread_s(&m_info.bmiHeader.biClrImportant, sizeof(DWORD), sizeof(DWORD), 1, m_file);

	if (m_info.bmiHeader.biBitCount == 8)
	{
		bytesRead = fread_s(&m_paletteEntry, 256 * sizeof(PALETTEENTRY), sizeof(PALETTEENTRY), 256, m_file);
	}

	size_t size = (size_t)m_info.bmiHeader.biWidth * m_info.bmiHeader.biHeight * m_bpp;

	m_pixels = (BYTE*)malloc(size);

	if (m_pixels)
	{
		memset(m_pixels, 0x00, size);

		m_pixels += (size - ((size_t)m_info.bmiHeader.biWidth * m_bpp));

		fseek(m_file, 0, SEEK_SET);
		fseek(m_file, m_header.bfOffBits, SEEK_SET);

		size_t count = 0;

		while (count < size)
		{
			bytesRead = fread_s(m_pixels, size, ((size_t)m_info.bmiHeader.biWidth * m_bpp), 1, m_file);

			m_pixels -= ((size_t)m_info.bmiHeader.biWidth * m_bpp);

			count += m_info.bmiHeader.biWidth * m_bpp;
		}

		m_pixels += ((size_t)m_info.bmiHeader.biWidth * m_bpp);
	}

	fclose(m_file);

	m_isInitialized = true;
}

UwpEngine::CBmpImage::~CBmpImage()
{
	delete m_filename;

	if (m_pixels)
	{
		free(m_pixels);
	}
}

void UwpEngine::CBmpImage::WriteBitmapHeader(UINT width, UINT height, FILE* file)
{
	BITMAPFILEHEADER	bmpHeader;
	BITMAPINFO			bmpInfo;

	bmpHeader.bfType = 19778;
	fwrite(&bmpHeader.bfType, sizeof(WORD), 1, file);

	bmpHeader.bfSize = 0;
	fwrite(&bmpHeader.bfSize, sizeof(DWORD), 1, file);

	bmpHeader.bfReserved1 = 0;
	fwrite(&bmpHeader.bfReserved1, sizeof(WORD), 1, file);

	bmpHeader.bfReserved2 = 0;
	fwrite(&bmpHeader.bfReserved2, sizeof(WORD), 1, file);

	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * sizeof(PALETTEENTRY));
	fwrite(&bmpHeader.bfOffBits, sizeof(DWORD), 1, file);

	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	fwrite(&bmpInfo.bmiHeader.biSize, sizeof(DWORD), 1, file);

	bmpInfo.bmiHeader.biWidth = width;
	fwrite(&bmpInfo.bmiHeader.biWidth, sizeof(LONG), 1, file);

	bmpInfo.bmiHeader.biHeight = height;
	fwrite(&bmpInfo.bmiHeader.biHeight, sizeof(LONG), 1, file);

	bmpInfo.bmiHeader.biPlanes = 1;
	fwrite(&bmpInfo.bmiHeader.biPlanes, sizeof(WORD), 1, file);

	bmpInfo.bmiHeader.biBitCount = 8;
	fwrite(&bmpInfo.bmiHeader.biBitCount, sizeof(WORD), 1, file);

	bmpInfo.bmiHeader.biCompression = BI_RGB;
	fwrite(&bmpInfo.bmiHeader.biCompression, sizeof(DWORD), 1, file);

	bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biHeight;
	fwrite(&bmpInfo.bmiHeader.biSizeImage, sizeof(DWORD), 1, file);

	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	fwrite(&bmpInfo.bmiHeader.biXPelsPerMeter, sizeof(LONG), 1, file);

	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	fwrite(&bmpInfo.bmiHeader.biYPelsPerMeter, sizeof(LONG), 1, file);

	bmpInfo.bmiHeader.biClrUsed = 0;
	fwrite(&bmpInfo.bmiHeader.biClrUsed, sizeof(DWORD), 1, file);

	bmpInfo.bmiHeader.biClrImportant = 0;
	fwrite(&bmpInfo.bmiHeader.biClrImportant, sizeof(DWORD), 1, file);
}

void UwpEngine::CBmpImage::WriteGreyscalePalette(FILE* file)
{
	PALETTEENTRY paletteentry;

	for (UINT i = 0; i < 256; i++)
	{
		paletteentry.peRed = i;
		paletteentry.peGreen = i;
		paletteentry.peBlue = i;
		paletteentry.peFlags = 0;

		fwrite(&paletteentry, sizeof(PALETTEENTRY), 1, file);
	}
}