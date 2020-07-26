#pragma once

class CItem;

class CEquipManager
{
private:
	explicit CEquipManager();
	~CEquipManager();

public:
	static CEquipManager* GetInstance();
	static void DestroyInstance();

public:
	void Refresh();
	void EquipItem(int ID);
	void UnequipItem(int ID);

	void ReleaseItem(CItem*& item);
	void EquipAndReturn(CItem*& item, int ID);
	bool CanEquip(int ID);

	CItem* GetHead() { return m_head; }
	CItem* GetArmor() { return m_armor; }
	CItem* GetArms() { return m_arms; }
	CItem* GetHand() { return m_hand; }
	CItem* GetFoot() { return m_foot; }

private:
	static CEquipManager* m_instance;
	CItem* m_head;
	CItem* m_armor;
	CItem* m_arms;
	CItem* m_hand;
	CItem* m_foot;
};

