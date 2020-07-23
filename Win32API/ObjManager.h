#pragma once

class CObj;

class CObjManager
{
private:
	explicit CObjManager();

public:
	~CObjManager();

	CObjManager(const CObjManager&) = delete;
	CObjManager& operator= (const CObjManager&) = delete;

public:
	static CObjManager* GetInstance();
	static void DestroyInstance();

public:
	void Update();
	void LateUpdate();
	void Render(const HDC& hDC);
	void Release();

public:
	void AddObject(CObj* obj, OBJ::TAG tag);
	void RemoveObject(OBJ::TAG tag);
	list<CObj*>* GetList(OBJ::TAG tag) { return &m_objectList[tag]; }
	CObj* GetPlayer();

private:
	static CObjManager* m_instance;

private:
	list<CObj*> m_objectList[OBJ::END];
};