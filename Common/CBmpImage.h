#pragma once

#include "pch.h"

#include "Common/CString.h"

// Copied from wingdi.h
typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER, FAR* LPBITMAPFILEHEADER, * PBITMAPFILEHEADER;

namespace UwpEngine
{
	class CBmpImage
	{
	public:

		CString*			m_filename;
		BITMAPFILEHEADER	m_header;
		BITMAPINFO			m_info;
		PALETTEENTRY		m_paletteEntry[256];
		int					m_bpp;
		BYTE*				m_pixels;
		bool				m_isInitialized;

		CBmpImage();
		CBmpImage(const char* filename);
		~CBmpImage();

		void WriteBitmapHeader(UINT width, UINT height, FILE* file);
		void WriteGreyscalePalette(FILE* file);

	private:

		FILE* m_file;
	};
}