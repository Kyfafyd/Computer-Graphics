#pragma warning(disable:4996)

#include "CxBitmap.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <Windows.h>
using namespace std;

CxBitmap::CxBitmap()
{
	m_bLoad = FALSE;
	pbyBuffer = NULL;
}

CxBitmap::~CxBitmap()
{
	FreeBitmap();
}

void CxBitmap::FreeBitmap()
{
	//如果是调用SetBuffer得到的图片,则不需要释放内存,否则会造成内存错误
	SAFE_DELETE_ARRAY(pbyBuffer);
	m_bLoad = FALSE;
}

void CxBitmap::InitPalette()
{
	for (int i = 0; i < MAX_COLORS_PALETTE; ++i)
	{
		palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = i;
	}
}

void CxBitmap::FlipBuffer()
{
	int nLenOfLine = (bitmapinfoheader.biWidth * bitmapinfoheader.biBitCount) / 8;
	BYTE* pbyTemp = new BYTE[nLenOfLine];
	int nEnd = bitmapinfoheader.biHeight / 2 - 1;

	for (int i = 0; i <= nEnd; ++i)
	{
		memcpy(pbyTemp, pbyBuffer + i * nLenOfLine, nLenOfLine);
		memcpy(pbyBuffer + i * nLenOfLine, pbyBuffer + (bitmapinfoheader.biWidth - i - 1)
			* nLenOfLine, nLenOfLine);
		memcpy(pbyBuffer + (bitmapinfoheader.biWidth - i - 1) * nLenOfLine, pbyTemp, nLenOfLine);
	}
	delete[] pbyTemp;
}

int CxBitmap::LoadBitmap(const char* pszFileName)
{
	int i;
	int nUnAlignBytesPerLine;

	FreeBitmap();
	if (pszFileName != NULL)
	{
		strcpy_s(m_szBitmapName, pszFileName);
	}

	FILE* fpBitmap = fopen(m_szBitmapName, "rb");

	if (fpBitmap == NULL)
	{
		return FALSE;
	}

	int nSizeOfHeader = sizeof(bitmapfileheader);
	if (nSizeOfHeader != 14)//字节对齐可能会导致其变成16
	{
		nSizeOfHeader = 14;
	}
	fread(&bitmapfileheader, nSizeOfHeader, 1, fpBitmap);
	if (bitmapfileheader.bfType != BITMAP_ID)//检测是否是位图
	{
		fclose(fpBitmap);
		return FALSE;
	}
	fread(&bitmapinfoheader, sizeof(bitmapinfoheader), 1, fpBitmap);
	if (bitmapinfoheader.biBitCount == 8)//8位位图,那么需要读取调色板
	{
		fread(palette, sizeof(PALETTEENTRY) * MAX_COLORS_PALETTE, 1, fpBitmap);
	}
	if (bitmapinfoheader.biSizeImage == 0)
	{
		bitmapinfoheader.biSizeImage = bitmapinfoheader.biWidth * bitmapinfoheader.biHeight
			* bitmapinfoheader.biBitCount / 8;
	}
	pbyBuffer = new BYTE[bitmapinfoheader.biSizeImage];
	if (pbyBuffer == NULL)
	{
		return FALSE;
	}
	fread(pbyBuffer, bitmapinfoheader.biSizeImage, 1, fpBitmap);
	if (bitmapinfoheader.biHeight < 0)
	{
		bitmapinfoheader.biHeight = -bitmapinfoheader.biHeight;
		FlipBuffer();
	}
	fclose(fpBitmap);

	m_nBytesPerLine = (bitmapinfoheader.biWidth * bitmapinfoheader.biBitCount + 31) / 32 * 4;
	m_nBytesPerPixel = bitmapinfoheader.biBitCount / 8;

	m_bLoad = TRUE;

	return TRUE;
}