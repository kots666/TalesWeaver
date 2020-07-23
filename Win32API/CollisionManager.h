#pragma once

class CObj;

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

public:
	// ��ü �������� ����ϱ� ����
	static void RectCollision(list<CObj*>& dstList, list<CObj*>& srcList);
	static void SphereCollision(list<CObj*>& dstList, list<CObj*>& srcList);
	
	static int GetCollideTileState(int index);

	static void PlayerCollision(CObj* player, list<CObj*>& collList);
	static void EnemyCollision(list<CObj*>& enemyList, list<CObj*>& collList);
	static void EnemyCollision_Mul(list<CObj*>& enemyList, list<CObj*>& collList);

private:
	static bool IntersectSphere(const CObj* dst, const CObj* src);
	static bool IsCollide(const INFO& info, const INFO& info2);
};