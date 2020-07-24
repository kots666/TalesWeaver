#pragma once
#include "Obj.h"

class CPlayer :
	public CObj
{
public:
	CPlayer(float x, float y, EX_DIR::TAG dir = EX_DIR::LD);
	virtual ~CPlayer();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Damaged(int damage) override;
	virtual void IncreaseEXP(int exp) override;
	virtual void IncreaseMoney(int money) override;

public:
	void CreateUI();
	void CreateButton();
	void UpdateRect();

	void FrameProcess();
	void KeyCheck();
	void AttackSpawnCheck();
	void SkillSpawnCheck();
	void SkillMoon();
	void SkillBoom();
	void SpawnCollide(float xPos, float yPos, int cx, int cy, int atk, OBJ::TAG type, DWORD lifeTime = 0);

	void LevelUp();

	// Combo
	void ResetCombo();

	// 4방향
	void MoveUp(float speed);
	void MoveLeft(float speed);
	void MoveDown(float speed);
	void MoveRight(float speed);

	// 8방향
	void MoveLU(float speed);
	void MoveLD(float speed);
	void MoveRU(float speed);
	void MoveRD(float speed);

	void ChangeAction(EX_DIR::TAG dir, int scene);
	void ChangeScene(int scene);
	void ExecuteScene(int scene);
	void ExecuteSpeedScene(int scene);

	HDC GetDCByDirection(EX_DIR::TAG dir);

public:
	// Setter
	void SetDirection(EX_DIR::TAG dir) { m_direction = dir; }

	// Getter
	EX_DIR::TAG GetDirection() const { return m_direction; }
	int GetMP() const { return m_mp; }
	int GetSP() const { return m_sp; }

private:
	EX_DIR::TAG m_direction;
	RECT m_moveRect;

	int m_curScene;
	int m_nextScene;
	int m_comboCnt;
	int m_atkCnt;
	int m_skillNum;

	int m_mp;
	int m_sp;
	int m_isRun;

	DWORD m_attackTime;
	DWORD m_attackSpeed;

	bool m_isHit;
	bool m_isAttackSpawn;
	bool m_isSkillSpawn;
};