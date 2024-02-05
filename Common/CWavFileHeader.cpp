#include "pch.h"

#include "CWavFileHeader.h"

UwpEngine::CWavFileHeader::CWavFileHeader()
{
	memset(this, 0x00, sizeof(CWavFileHeader));
}

UwpEngine::CWavFileHeader::~CWavFileHeader()
{
}