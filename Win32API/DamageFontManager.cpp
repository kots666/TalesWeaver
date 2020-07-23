#include "stdafx.h"
#include "DamageFontManager.h"
#include "DamageFont.h"

void CDamageFontManager::CreateDamageFont(float x, float y, int damage, OBJ::TAG tag)
{
	int dmg = damage;

	int length = 0;

	while (dmg > 0)
	{
		dmg /= 10;
		++length;
	}

	float offset = 10.f;
	float xPos = 0.f, yPos = 0.f;

	// �汝鶺� 唳辦
	if (length & 1)
	{
		int gap = (length - 1) / 2;
		xPos = x - (gap * 23) - 12.f;
		yPos = y - offset - 30.f;
	}
	// 礎熱檣 唳辦
	else
	{
		int gap = length >> 1;
		xPos = x - (gap * 23);
		yPos = y - offset - 30.f;
	}

	if (1 == length)
	{
		CObj* newFont = new CDamageFont(xPos, yPos, damage);
		CObjManager::GetInstance()->AddObject(newFont, tag);
	}
	else
	{
		int tenPow = length;
		int i = 0;

		while (0 < tenPow)
		{
			int powNum = pow(10, --tenPow);
			int num = damage / powNum;
			damage %= powNum;

			CObj* newFont = new CDamageFont(xPos + (i++ * 23), yPos, num);
			CObjManager::GetInstance()->AddObject(newFont, tag);
		}
	}
}
