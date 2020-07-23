#include "stdafx.h"
#include "ComboUI.h"

CComboUI::CComboUI(CObj * target) : m_target(target)
{
}

CComboUI::~CComboUI()
{
}

void CComboUI::Init()
{
}

int CComboUI::Update()
{
	if (nullptr == m_target) m_isDead = true;

	if (m_isDead) return DEAD_EVENT;

	cout << CComboManager::GetCombo() << endl;

	return NO_EVENT;
}

void CComboUI::LateUpdate()
{
	if (nullptr == m_target) m_isDead = true;

	if (0 == CComboManager::GetCombo()) m_isDead = true;
}

void CComboUI::Render(HDC hDC)
{
	if (nullptr == m_target) return;
	INFO playerInfo = m_target->GetInfo();

	if (!m_isDead)
	{
		float offset = 50.f;

		float x = playerInfo.xPos - 50;
		float y = playerInfo.yPos - offset - 46;

		HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Combo"));

		TransparentBlt(hDC, x - CCamera::GetX(), y - CCamera::GetY(), 100, 46, memDC, 0, 0, 100, 46, RGB(255, 0, 255));

		RenderCombo(hDC, x, y);
	}
}

void CComboUI::Release()
{
}

void CComboUI::RenderCombo(HDC hDC, float x, float y)
{
	HDC numDC = CBitmapManager::GetInstance()->GetDC(__T("Number"));

	int combo = CComboManager::GetCombo();

	int tempCombo = combo;

	int length = 0;

	while (tempCombo > 0)
	{
		tempCombo /= 10;
		++length;
	}

	float xPos = 0.f;
	float yPos = 0.f;

	// �汝鶺� 唳辦
	if (length & 1)
	{
		int gap = (length - 1) / 2;
		xPos = x + 70.f - (gap * 23) - 12.f;
		yPos = y;
	}
	// 礎熱檣 唳辦
	else
	{
		int gap = length >> 1;
		xPos = x + 60.f - (gap * 23);
		yPos = y;
	}

	if (1 == length)
	{
		TransparentBlt(hDC, xPos - CCamera::GetX(), yPos - CCamera::GetY(), 23, 30, numDC, combo * 23, 0, 23, 30, RGB(255, 0, 255));
	}
	else
	{
		int tenPow = length;
		int i = 0;

		while (0 < tenPow)
		{
			int powNum = pow(10, --tenPow);
			int num = combo / powNum;
			combo %= powNum;

			TransparentBlt(hDC, xPos - CCamera::GetX() + (i++ * 23), yPos - CCamera::GetY(), 23, 30, numDC, num * 23, 0, 23, 30, RGB(255, 0, 255));
		}
	}
}
