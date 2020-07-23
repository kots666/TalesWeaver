#pragma once
#include "Obj.h"
class CBoss :
	public CObj
{
public:
	explicit CBoss(float x, float y);
	virtual ~CBoss();

	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Damaged(int damage) override;

public:
	void CalcDistance();
	void Attack();
	void Portal();
	void AttackProcess();
	void PortalProcess();
	void LookAtTarget();
	void WalkToTarget();

	void SpawnDeadTrail();

	void ChangeAction(EX_DIR::TAG dir, int scene);
	void ChangeScene(int scene);
	void ExecuteScene(int scene);

	HDC GetDCByDirection();

private:
	EX_DIR::TAG m_direction;
	CObj* target;
	bool m_isHit;
	bool m_isSkill;
	bool m_isAttack;
	bool m_isAttackSpawn;
	bool m_isDeadAnimation;
	DWORD m_actionTime;
	DWORD m_actionSpeed;
	DWORD m_hitTime;
};

