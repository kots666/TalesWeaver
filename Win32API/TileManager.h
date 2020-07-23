#pragma once

class CTile;

class CTileManager
{
private:
	CTileManager();
	~CTileManager();

public:
	static CTileManager* GetInstance();
	static void DestroyInstance();

public:
	void Init();
	void ChangeTile(int index, int state);
	void SaveData(EDIT::STATE map);
	void LoadData(EDIT::STATE map);

	CTile* GetTile(int index);
	int Size() { return m_tileVec.size(); }
	
	void Render(HDC hDC);
	void Release();

private:
	static CTileManager* m_instance;
	vector<CTile*> m_tileVec;
};

