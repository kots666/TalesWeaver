#pragma once
class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();

public:
	HDC GetMemDC() { return m_memDC; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

public:
	void Load(const TCHAR* path);
	void Release();

private:
	HDC m_memDC;
	HBITMAP m_bitmap;
	HBITMAP m_oldBitmap;
	int m_width;
	int m_height;
};

