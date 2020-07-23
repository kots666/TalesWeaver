#pragma once
#include "Obj.h"
class CJelly :
	public CObj
{
public:
	explicit CJelly(float x, float y);
	virtual ~CJelly();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void CalcDistance();
	void Attack();
	void AttackProcess();
	void SpawnMotionTrail();
	virtual void Damaged(int damage) override;

	void ChangeAction(DIR::TAG dir, int scene);
	void ChangeScene(int scene);
	void ExecuteScene(int scene);

private:
	DIR::TAG m_direction;
	CObj* target;
	bool m_isAttack;
	bool m_isHit;
	bool m_isDeadAnimation;
	DWORD m_actionTime;
	DWORD m_actionSpeed;
};

