#pragma once
#include "Obj.h"
class CCollideSpawner :
	public CObj
{
public:
	explicit CCollideSpawner(float x, float y, int cx, int cy, int damage, int spawnCount = 1, DWORD gapTime = 0, OBJ::TAG type = OBJ::PLAYER_ATTACK, bool isPlayer = true);
	virtual ~CCollideSpawner();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	float m_x;
	float m_y;
	int m_cx;
	int m_cy;
	int m_damage;
	int m_spawnCount;

	DWORD m_spawnTime;
	DWORD m_gapTime;
	OBJ::TAG m_type;
	bool m_isPlayer;
};

