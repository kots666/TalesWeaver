#pragma once
#include "Obj.h"
class CShopUI :
	public CObj
{
public:
	explicit CShopUI(int ID);
	virtual ~CShopUI();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void ClickCheck();


public:
	void TextRender(HDC hDC);
	void ItemRender(HDC hDC);
	void UIRender(HDC hDC);
	void RectInit();

private:
	list<CObj*> buyerList;

	RECT m_closeRect;
	RECT m_shopUpRect;
	RECT m_shopDownRect;
	RECT m_buyerUpRect;
	RECT m_buyerDownRect;

	RECT m_buyRect;
	RECT m_sellRect;
	RECT m_yesRect;
	RECT m_cancleRect;

	int m_ID;
	int m_buyID;
	int m_sellID;
	int m_yesID;
	int m_cancleID;

	int m_shopListVal;
	int m_buyerListVal;
};

