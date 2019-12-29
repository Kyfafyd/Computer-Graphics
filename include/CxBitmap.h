#ifndef GRAY_BITMAP_H
#define GRAY_BITMAP_H

#define BITMAP_ID 0x4D42 // universal id for a bitmap
#define MAX_COLORS_PALETTE 256

#include <windows.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if (p) { delete p; p = NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete [] p; p = NULL; } }
#endif

#define NEIGHBOUR_SIZE (3)

// this builds a 24 bit color value in 8.8.8 format 
#define RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16) )

class CxBitmap
{
	//�����ǲ���λͼ��һЩ����
public:
	CxBitmap();
	~CxBitmap();
	int LoadBitmap(const char* pszFileName = NULL);

	//�����ǻ�ȡ������λͼ�����Ե�һЩ����
public:
	int GetWidth() const { return bitmapinfoheader.biWidth; }
	int GetHeight() const { return bitmapinfoheader.biHeight; }
	int GetBytesPerPixel() const { return bitmapinfoheader.biBitCount / 8; }
	int GetBytesPerLine() const { return m_nBytesPerLine; }
	BYTE* GetBuffer() const { return pbyBuffer; }
	int GetImageSize() const { return bitmapinfoheader.biSizeImage; }


	//�ڲ��õ���һЩ˽�к���
private:
	void FlipBuffer();
	void FreeBitmap();
	void InitPalette();

private:
	char m_szBitmapName[MAX_PATH];
	BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
	BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
	RGBQUAD palette[256];               // we will store the palette here
	BYTE* pbyBuffer;
	BOOL m_bLoad;     //�Ƿ��Ѿ�����ͼƬ��
	int m_nBytesPerLine;
	int m_nBytesPerPixel;
};

#endif