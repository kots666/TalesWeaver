#include "stdafx.h"
#include "ShopUI.h"
#include "Item.h"

CShopUI::CShopUI()
{
	Init();
}

CShopUI::~CShopUI()
{
	Release();
}

void CShopUI::Init()
{
	m_buyID = 1;
	m_sellID = 0;
	m_yesID = 0;
	m_cancleID = 0;

	m_shopListVal = 0;
	m_buyerListVal = 0;

	m_maxShopListVal = 7;
	m_maxBuyerListVal = 10;

	m_info.xSize = 384;
	m_info.ySize = 390;

	m_objMgr = CObjManager::GetInstance();
	m_invenMgr = CInvenManager::GetInstance();
	m_keyMgr = CKeyManager::GetInstance();
	m_bmpMgr = CBitmapManager::GetInstance();

	RectInit();
	ItemInit();
}

int CShopUI::Update()
{
	if (m_yesID) m_yesID = 0;

	CalcMaxVal();

	ClickCheck();

	return NO_EVENT;
}

void CShopUI::LateUpdate()
{
	CalcMaxVal();
	if (m_cancleID) m_objMgr->RemoveObject(OBJ::SHOP);
}

void CShopUI::Render(HDC hDC)
{
	UIRender(hDC);
	ItemRender(hDC);

	NumRender(hDC, m_rect.left + 88, m_rect.top + 348, m_objMgr->GetPlayer()->GetMoney());
	
	SumRender(hDC, m_rect.left + 88, m_rect.top + 371);
}

void CShopUI::Release()
{
	if (m_sellID) CopyAllItemToInven();

	ReleaseBuyerList();
}

void CShopUI::CalcMaxVal()
{
	if (m_buyID)
	{
		m_maxShopListVal = 7;
		
		if (4 < m_buyerList.size())
		{
			m_maxBuyerListVal = m_buyerList.size() - 4;
		}
		else
		{
			m_buyerListVal = 0;
			m_maxBuyerListVal = 0;
		}
	}
	if (m_sellID)
	{
		if (7 < CInvenManager::GetInstance()->GetSize())
		{
			m_maxShopListVal = m_invenMgr->GetSize() - 7;
		}
		else
		{
			m_shopListVal = 0;
			m_maxShopListVal = 0;
		}

		if (4 < m_buyerList.size())
		{
			m_maxBuyerListVal = m_buyerList.size() - 4;
		}
		else
		{
			m_buyerListVal = 0;
			m_maxBuyerListVal = 0;
		}
	}
}

void CShopUI::ClickCheck()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_buyRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			ChangeToBuy();
		}
	if (PtInRect(&m_sellRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			ChangeToSell();
		}
	if (PtInRect(&m_yesRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			CSoundManager::GetInstance()->PlayOverlapSound(__T("Buy.wav"), CSoundManager::UI);
			PayToBuyerList();
		}

	if (PtInRect(&m_cancleRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			CSoundManager::GetInstance()->PlayOverlapSound(__T("CloseWnd.wav"), CSoundManager::UI);
			m_cancleID ^= 1;
		}

	if (PtInRect(&m_shopUpRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			//CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			AddShopListVal(-1);
		}
	if (PtInRect(&m_shopDownRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			//CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			AddShopListVal(1);
		}
	if (PtInRect(&m_buyerUpRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			//CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			AddBuyerListVal(-1);
		}
	if (PtInRect(&m_buyerDownRect, pt))
		if (m_keyMgr->isKeyDown(KEY_LBUTTON))
		{
			//CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			AddBuyerListVal(1);
		}


	for (int i = 0; i < 7; ++i)
	{
		if (PtInRect(&m_shopPart[i], pt))
			if (m_keyMgr->isKeyDown(KEY_LBUTTON))
			{
				CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
				AddItemToList(i);
			}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (PtInRect(&m_buyerPart[i], pt))
			if (m_keyMgr->isKeyDown(KEY_LBUTTON))
			{
				CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
				RemoveItemFromList(i);
			}
	}
}

void CShopUI::AddShopListVal(int val)
{
	if (0 <= m_shopListVal + val && m_shopListVal + val <= m_maxShopListVal) m_shopListVal += val;
}

void CShopUI::AddBuyerListVal(int val)
{
	if (0 <= m_buyerListVal + val && m_buyerListVal + val <= m_maxBuyerListVal) m_buyerListVal += val;
}

void CShopUI::AddItemToList(int index)
{
	if (m_buyID)
	{
		int ID = index + m_shopListVal;

		auto iter = find_if(m_buyerList.begin(), m_buyerList.end(), [&](CItem*& item)
		{
			return ID == item->GetID();
		});

		if (m_buyerList.end() == iter)
		{
			CItem* newItem = new CItem(ID);
			m_buyerList.emplace_back(newItem);
		}
		else
		{
			(*iter)->IncreaseCount();
		}
	}

	if (m_sellID)
	{
		int at = index + m_shopListVal;

		if (at >= m_invenMgr->GetSize()) return;

		int ID = m_invenMgr->GetInventory()->at(at)->GetID();

		auto iter = find_if(m_buyerList.begin(), m_buyerList.end(), [&](CItem*& item)
		{
			return ID == item->GetID();
		});

		m_invenMgr->DecreaseItem(ID);

		if (m_buyerList.end() == iter)
		{
			CItem* newItem = new CItem(ID);
			m_buyerList.emplace_back(newItem);
		}
		else
		{
			(*iter)->IncreaseCount();
		}
	}
}

void CShopUI::RemoveItemFromList(int index)
{
	if (m_buyID)
	{
		int ID = index + m_buyerListVal;

		if (ID < m_buyerList.size())
		{
			auto iter = m_buyerList.begin();

			while (ID > 0)
			{
				--ID;
				++iter;
			}

			(*iter)->DecreaseCount();

			if (1 > (*iter)->GetCount())
			{
				SafeDelete(*iter);
				m_buyerList.erase(iter);
			}
		}
	}

	if (m_sellID)
	{
		int ID = index + m_buyerListVal;

		if (ID < m_buyerList.size())
		{
			auto iter = m_buyerList.begin();

			while (ID > 0)
			{
				--ID;
				++iter;
			}

			int itemID = (*iter)->GetID();

			if (m_invenMgr->IsItemExist(itemID))
			{
				m_invenMgr->IncreaseItem(itemID);
			}
			else
			{
				CItem* newItem = new CItem(itemID);
				m_invenMgr->AddItem(newItem);
			}

			(*iter)->DecreaseCount();

			if (1 > (*iter)->GetCount())
			{
				SafeDelete(*iter);
				m_buyerList.erase(iter);
			}
		}
	}
}

void CShopUI::ChangeToBuy()
{
	if (m_buyID) return;

	CopyAllItemToInven();
	ReleaseBuyerList();

	m_buyID = 1;
	m_sellID = 0;
	m_shopListVal = 0;
	m_buyerListVal = 0;
}

void CShopUI::ChangeToSell()
{
	if (m_sellID) return;

	ReleaseBuyerList();

	m_buyID = 0;
	m_sellID = 1;
	m_shopListVal = 0;
	m_buyerListVal = 0;
}

void CShopUI::CopyAllItemToInven()
{
	for (auto& elem : m_buyerList)
	{
		int id = elem->GetID();
		int count = elem->GetCount();

		if (m_invenMgr->IsItemExist(id))
		{
			for (int i = 0; i < count; ++i) m_invenMgr->IncreaseItem(id);
		}
		else
		{
			CItem* newItem = new CItem(id, count);
			m_invenMgr->AddItem(newItem);
		}
	}
}

void CShopUI::ReleaseBuyerList()
{
	for (auto& elem : m_buyerList)
	{
		SafeDelete(elem);
	}
	m_buyerList.clear();
}

void CShopUI::PayToBuyerList()
{
	m_yesID = 1;

	int sum = 0;

	CObj* player = m_objMgr->GetPlayer();
	if (nullptr == player) return;

	int playerMoney = player->GetMoney();

	if (m_buyID)
	{
		for (auto& elem : m_buyerList)
		{
			sum += elem->GetMoney() * elem->GetCount();
		}

		if (playerMoney >= sum)
		{
			playerMoney -= sum;

			player->SetMoney(playerMoney);

			for (auto& elem : m_buyerList)
			{
				CItem* newItem = new CItem(elem->GetID(), elem->GetCount());
				m_invenMgr->AddItem(newItem);
			}

			ReleaseBuyerList();
		}
	}
	if (m_sellID)
	{
		for (auto& elem : m_buyerList)
		{
			sum += (elem->GetMoney() * elem->GetCount() / 2);
		}

		playerMoney += sum;

		player->SetMoney(playerMoney);

		ReleaseBuyerList();
	}
}

void CShopUI::NameRender(HDC hDC, float x, float y, const TCHAR * buffer)
{
	TCHAR writeBuffer[30];

	wsprintf(writeBuffer, buffer);
	TextOut(hDC, x, y, writeBuffer, lstrlen(writeBuffer));
}

void CShopUI::NumRender(HDC hDC, float x, float y, int n)
{
	TCHAR writeBuffer[30];

	wsprintf(writeBuffer, __T("%d"), n);
	TextOut(hDC, x, y, writeBuffer, lstrlen(writeBuffer));
}

void CShopUI::SumRender(HDC hDC, float x, float y)
{
	int sum = 0;

	for (auto& elem : m_buyerList)
	{
		int cost = elem->GetCount() * elem->GetMoney();

		if (m_sellID) cost >> 1;

		sum += cost;
	}
	
	NumRender(hDC, x, y, sum);
}

void CShopUI::ItemRender(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Items"));

	if (m_buyID)
	{
		int shopMax = m_shopListVal + 7;

		int partIndex = 0;

		for (int i = m_shopListVal; i < shopMax; ++i)
		{
			int x = m_rect.left + 16;
			int y = m_rect.top + (25 * partIndex++) + 24;

			TransparentBlt(hDC, x, y, 24, 24, memDC, i * 24, 0, 24, 24, RGB(255, 0, 255));

			NameRender(hDC, x + 36, y + 6, m_itemName[i]);
			NumRender(hDC, x + 294, y + 6, ITEM::COST[i]);
		}
	}

	if (m_sellID)
	{
		int shopMax = 0;

		if (7 < m_invenMgr->GetSize()) shopMax = m_shopListVal + 7;
		else  shopMax = m_invenMgr->GetSize();

		int partIndex = 0;

		for (int i = m_shopListVal; i < shopMax; ++i)
		{
			int id = m_invenMgr->GetInventory()->at(i)->GetID();
			int x = m_rect.left + 16;
			int y = m_rect.top + (25 * partIndex++) + 24;

			TransparentBlt(hDC, x, y, 24, 24, memDC, id * 24, 0, 24, 24, RGB(255, 0, 255));

			NameRender(hDC, x + 36, y + 6, m_itemName[id]);
			NumRender(hDC, x + 294, y + 6, m_invenMgr->GetInventory()->at(i)->GetCount());
		}
	}

	int buyerMax = m_buyerListVal + 4;

	auto iter = m_buyerList.begin();

	for (int i = 0; i < m_buyerListVal; ++i) ++iter;

	int partIndex = 0;

	for (int i = m_buyerListVal; i < buyerMax; ++i)
	{
		if (m_buyerList.end() == iter) break;

		int x = m_rect.left + 17;
		int y = m_rect.top + (25 * partIndex++) + 232;

		int ID = (*iter)->GetID();

		TransparentBlt(hDC, x, y, 24, 24, memDC, ID * 24, 0, 24, 24, RGB(255, 0, 255));

		NameRender(hDC, x + 36, y + 6, (*iter)->GetKey());
		if (m_buyID) NumRender(hDC, x + 294, y + 6, (*iter)->GetMoney() * (*iter)->GetCount());
		if (m_sellID) NumRender(hDC, x + 294, y + 6, (*iter)->GetMoney() * (*iter)->GetCount() / 2);

		++iter;
	}
}

void CShopUI::UIRender(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("ShopWnd"));

	float startX = m_rect.left;
	float startY = m_rect.top;
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, 0, 0, width, height, RGB(255, 0, 255));

	memDC = CBitmapManager::GetInstance()->GetDC(__T("WndUp"));
	startX = m_shopUpRect.left;
	startY = m_shopUpRect.top;
	width = m_shopUpRect.right - m_shopUpRect.left;
	height = m_shopUpRect.bottom - m_shopUpRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, 0, 0, 12, 12, RGB(255, 0, 255));

	startX = m_buyerUpRect.left;
	startY = m_buyerUpRect.top;
	width = m_buyerUpRect.right - m_buyerUpRect.left;
	height = m_buyerUpRect.bottom - m_buyerUpRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, 0, 0, 12, 12, RGB(255, 0, 255));

	memDC = m_bmpMgr->GetDC(__T("WndDown"));
	startX = m_shopDownRect.left;
	startY = m_shopDownRect.top;
	width = m_shopDownRect.right - m_shopDownRect.left;
	height = m_shopDownRect.bottom - m_shopDownRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, 0, 0, 12, 12, RGB(255, 0, 255));

	startX = m_buyerDownRect.left;
	startY = m_buyerDownRect.top;
	width = m_buyerDownRect.right - m_buyerDownRect.left;
	height = m_buyerDownRect.bottom - m_buyerDownRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, 0, 0, 12, 12, RGB(255, 0, 255));

	memDC = m_bmpMgr->GetDC(__T("Buy"));
	startX = m_buyRect.left;
	startY = m_buyRect.top;
	width = m_buyRect.right - m_buyRect.left;
	height = m_buyRect.bottom - m_buyRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_buyID * width, 0, width, height, RGB(255, 0, 255));

	memDC = m_bmpMgr->GetDC(__T("Sell"));
	startX = m_sellRect.left;
	startY = m_sellRect.top;
	width = m_sellRect.right - m_sellRect.left;
	height = m_sellRect.bottom - m_sellRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_sellID * width, 0, width, height, RGB(255, 0, 255));

	memDC = m_bmpMgr->GetDC(__T("OK"));
	startX = m_yesRect.left;
	startY = m_yesRect.top;
	width = m_yesRect.right - m_yesRect.left;
	height = m_yesRect.bottom - m_yesRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_yesID * width, 0, width, height, RGB(255, 0, 255));

	memDC = m_bmpMgr->GetDC(__T("Cancle"));
	startX = m_cancleRect.left;
	startY = m_cancleRect.top;
	width = m_cancleRect.right - m_cancleRect.left;
	height = m_cancleRect.bottom - m_cancleRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_cancleID * width, 0, width, height, RGB(255, 0, 255));
}

void CShopUI::RectInit()
{
	// ShopWnd
	m_rect.left = 200;
	m_rect.top = 100;
	m_rect.right = m_rect.left + m_info.xSize;
	m_rect.bottom = m_rect.top + m_info.ySize;

	// shop Up
	m_shopUpRect.left = m_rect.left + 362;
	m_shopUpRect.top = m_rect.top + 29;
	m_shopUpRect.right = m_shopUpRect.left + 12;
	m_shopUpRect.bottom = m_shopUpRect.top + 12;

	// shop Down
	m_shopDownRect.left = m_rect.left + 362;
	m_shopDownRect.top = m_rect.top + 186;
	m_shopDownRect.right = m_shopDownRect.left + 12;
	m_shopDownRect.bottom = m_shopDownRect.top + 12;

	// buyer Up
	m_buyerUpRect.left = m_rect.left + 362;
	m_buyerUpRect.top = m_rect.top + 232;
	m_buyerUpRect.right = m_buyerUpRect.left + 12;
	m_buyerUpRect.bottom = m_buyerUpRect.top + 12;

	// buyer Down
	m_buyerDownRect.left = m_rect.left + 362;
	m_buyerDownRect.top = m_rect.top + 319;
	m_buyerDownRect.right = m_buyerDownRect.left + 12;
	m_buyerDownRect.bottom = m_buyerDownRect.top + 12;

	// 구매 버튼
	m_buyRect.left = m_rect.left + 5 - 21;
	m_buyRect.top = m_rect.top + 6;
	m_buyRect.right = m_buyRect.left + 21;
	m_buyRect.bottom = m_buyRect.top + 70;

	// 판매 버튼
	m_sellRect.left = m_buyRect.left;
	m_sellRect.top = m_buyRect.bottom + 1;
	m_sellRect.right = m_sellRect.left + 21;
	m_sellRect.bottom = m_sellRect.top + 70;

	// 예 버튼
	m_yesRect.left = m_rect.left + 236;
	m_yesRect.top = m_rect.top + 347;
	m_yesRect.right = m_yesRect.left + 32;
	m_yesRect.bottom = m_yesRect.top + 14;

	// 취소 버튼
	m_cancleRect.left = m_yesRect.right + 42;
	m_cancleRect.top = m_yesRect.top;
	m_cancleRect.right = m_cancleRect.left + 32;
	m_cancleRect.bottom = m_cancleRect.top + 14;

	// shop Part
	for (int i = 0; i < 7; ++i)
	{
		m_shopPart[i].left = m_rect.left + 16;
		m_shopPart[i].top = m_rect.top + 24 + (25 * i);
		m_shopPart[i].right = m_shopPart[i].left + 24;
		m_shopPart[i].bottom = m_shopPart[i].top + 24;
	}

	// buyer Part
	for (int i = 0; i < 4; ++i)
	{
		m_buyerPart[i].left = m_rect.left + 17;
		m_buyerPart[i].top = m_rect.top + 232 + (25 * i);
		m_buyerPart[i].right = m_buyerPart[i].left + 24;
		m_buyerPart[i].bottom = m_buyerPart[i].top + 24;
	}
}

void CShopUI::ItemInit()
{
	m_itemName[0] = __T("HP 회복 포션");
	m_itemName[1] = __T("MP 회복 포션");
	m_itemName[2] = __T("SP 회복 포션");
	m_itemName[3] = __T("고급 갑옷");
	m_itemName[4] = __T("하급 갑옷");
	m_itemName[5] = __T("고급 검");
	m_itemName[6] = __T("하급 검");
	m_itemName[7] = __T("고급 신발");
	m_itemName[8] = __T("하급 신발");
	m_itemName[9] = __T("고급 반지");
	m_itemName[10] = __T("하급 반지");
	m_itemName[11] = __T("고급 모자");
	m_itemName[12] = __T("하급 모자");
	m_itemName[13] = __T("수상한 물건");
}
