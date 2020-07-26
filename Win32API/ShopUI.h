#pragma once
#include "Obj.h"

class CItem;

class CShopUI :
	public CObj
{
public:
	explicit CShopUI();
	virtual ~CShopUI();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void CalcMaxVal();
	void ClickCheck();

	void AddShopListVal(int val);
	void AddBuyerListVal(int val);

	void AddItemToList(int index);
	void RemoveItemFromList(int index);

	void ChangeToBuy();
	void ChangeToSell();
	void CopyAllItemToInven();
	void ReleaseBuyerList();

	void PayToBuyerList();

public:
	void NameRender(HDC hDC, float x, float y, const TCHAR* buffer);
	void NumRender(HDC hDC, float x, float y, int n);
	void SumRender(HDC hDC, float x, float y);
	void ItemRender(HDC hDC);
	void UIRender(HDC hDC);
	void RectInit();
	void ItemInit();
	
private:
	list<CItem*> m_buyerList;

	RECT m_shopPart[7];
	RECT m_buyerPart[4];

	RECT m_shopUpRect;
	RECT m_shopDownRect;
	RECT m_buyerUpRect;
	RECT m_buyerDownRect;

	RECT m_buyRect;
	RECT m_sellRect;
	RECT m_yesRect;
	RECT m_cancleRect;

	int m_buyID;
	int m_sellID;
	int m_yesID;
	int m_cancleID;

	int m_shopListVal;
	int m_buyerListVal;
	int m_maxShopListVal;
	int m_maxBuyerListVal;

	TCHAR* m_itemName[14];

	CObjManager* m_objMgr;
	CInvenManager* m_invenMgr;
	CKeyManager* m_keyMgr;
	CBitmapManager* m_bmpMgr;
};

