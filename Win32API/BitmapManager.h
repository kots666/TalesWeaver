#pragma once

class CMyBitmap;

class CBitmapManager
{
private:
	CBitmapManager();
	~CBitmapManager();

public:
	static CBitmapManager* GetInstance();
	static void DestroyInstance();

public:
	void InsertBitmap(const TCHAR* key, const TCHAR* path);
	CMyBitmap* GetMyBitmap(const TCHAR* target);
	HBITMAP GetBitmap(const TCHAR* target);
	HDC GetDC(const TCHAR* key);

	void Release();

private:
	static CBitmapManager* m_instance;
	map<const TCHAR*, CMyBitmap*> m_bitmap;
};