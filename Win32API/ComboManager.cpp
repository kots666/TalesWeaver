#include "stdafx.h"
#include "ComboManager.h"
#include "ComboUI.h"
#include "ComboLine.h"

int CComboManager::m_combo = 0;

void CComboManager::IncreaseCombo()
{
	++m_combo;

	if (1 == m_combo)
	{
		CObjManager::GetInstance()->RemoveObject(OBJ::COMBO);

		CObj* newUI = new CComboUI(CObjManager::GetInstance()->GetPlayer());
		CObjManager::GetInstance()->AddObject(newUI, OBJ::COMBO);
	}

	CObj* newComLine = new CComboLine;
	CObjManager::GetInstance()->AddObject(newComLine, OBJ::COMBO);
}