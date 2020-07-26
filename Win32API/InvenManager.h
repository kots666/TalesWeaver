#pragma once

class CItem;

class CInvenManager
{
private:
	explicit CInvenManager();
	~CInvenManager();

public:
	static CInvenManager* GetInstance();
	static void DestroyInstance();

public:
	void AddItem(CItem* item) { m_inventory.emplace_back(item); }
	void ReturnItem(CItem* item);
	void IncreaseItem(int id);
	void DecreaseItem(int id);

	bool IsItemExist(int id);

	int GetSize() { return m_inventory.size(); }
	vector<CItem*>* GetInventory() { return &m_inventory; }

private:
	static CInvenManager* m_instance;
	vector<CItem*> m_inventory;
};

