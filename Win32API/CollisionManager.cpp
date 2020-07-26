#include "stdafx.h"
#include "CollisionManager.h"
#include "Obj.h"
#include "Player.h"
#include "Tile.h"
#include "CollideRect.h"

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::RectCollision(list<CObj*>& dstList, list<CObj*>& srcList)
{
	RECT rc = {};

	for (auto& dst : dstList)
	{
		for (auto& src : srcList)
		{
			if (IntersectRect(&rc, &dst->GetRect(), &src->GetRect()))
			{
				dst->SetIsDead(true);
				src->SetIsDead(true);
			}
		}
	}
}

void CCollisionManager::SphereCollision(list<CObj*>& dstList, list<CObj*>& srcList)
{
	for (auto& dst : dstList)
	{
		for (auto& src : srcList)
		{
			if (IntersectSphere(dst, src))
			{
				dst->SetIsDead(true);
				src->SetIsDead(true);
			}
		}
	}
}

int CCollisionManager::GetCollideTileState(int index)
{
	if (0 > index || index >= CTileManager::GetInstance()->Size()) return -1;

	return CTileManager::GetInstance()->GetTile(index)->GetState();
}

void CCollisionManager::PlayerCollision(CObj * player, list<CObj*>& collList)
{
	if (nullptr != player)
	{
		RECT rc = {};

		RECT playerRect = player->GetRect();

		for (auto& elem : collList)
		{
			RECT collRect = elem->GetRect();
			int damage = elem->GetATK();

			if (IntersectRect(&rc, &playerRect, &collRect))
			{
				player->Damaged(damage);
				dynamic_cast<CCollideRect*>(elem)->SetIsHit();
				elem->SetIsDead(true);
			}
		}
	}
}

void CCollisionManager::PlayerCollision_Mul(CObj * player, list<CObj*>& collList)
{
	if (nullptr != player)
	{
		RECT rc = {};

		RECT playerRect = player->GetRect();

		for (auto& elem : collList)
		{
			RECT collRect = elem->GetRect();
			int damage = elem->GetATK();

			if (IntersectRect(&rc, &playerRect, &collRect))
			{
				player->SpecialDamaged(damage);
				dynamic_cast<CCollideRect*>(elem)->SetIsHit();
				elem->SetIsDead(true);
			}
		}
	}
}

void CCollisionManager::EnemyCollision(list<CObj*>& enemyList, list<CObj*>& collList)
{
	RECT rc = {};

	for (auto& collision : collList)
	{
		bool isHit = false;

		RECT collRC = collision->GetRect();
		for (auto& enemy : enemyList)
		{
			RECT enemyRC = enemy->GetRect();
			if (IntersectRect(&rc, &collRC, &enemyRC))
			{
				enemy->Damaged(collision->GetATK());
				dynamic_cast<CCollideRect*>(collision)->SetIsHit();

				isHit = true;
			}
		}
		if (isHit) collision->SetIsDead(true);
	}
}

void CCollisionManager::EnemyCollision_Mul(list<CObj*>& enemyList, list<CObj*>& collList)
{
	RECT rc = {};

	for (auto& collision : collList)
	{
		RECT collRC = collision->GetRect();
		for (auto& enemy : enemyList)
		{
			RECT enemyRC = enemy->GetRect();
			if (IntersectRect(&rc, &collRC, &enemyRC))
			{
				enemy->Damaged(collision->GetATK());
				dynamic_cast<CCollideRect*>(collision)->SetIsHit();
			}
		}
	}
}

bool CCollisionManager::IntersectSphere(const CObj * dst, const CObj * src)
{
	float disX = dst->GetInfo().xPos - src->GetInfo().xPos;
	float disY = dst->GetInfo().yPos - src->GetInfo().yPos;

	float radiusSum = (dst->GetInfo().xSize / 2) + (src->GetInfo().xSize / 2);

	float distance = sqrtf(pow(disX, 2) + pow(disY, 2));


	return distance < radiusSum;
}

bool CCollisionManager::IsCollide(const INFO& info, const INFO& info2)
{
	float minX = info.xPos - info.xSize / 2;
	float minY = info.yPos - info.ySize / 2;
	float maxX = info.xPos + info.xSize / 2;
	float maxY = info.yPos + info.ySize / 2;

	float minX2 = info2.xPos - info2.xSize / 2;
	float minY2 = info2.yPos - info2.ySize / 2;
	float maxX2 = info2.xPos + info2.xSize / 2;
	float maxY2 = info2.yPos + info2.ySize / 2;

	if (maxX < minX2 || minX > maxX2) return false;
	if (maxY < minY2 || minY > maxY2) return false;
	return true;
}
