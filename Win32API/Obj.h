#pragma once

class CObj abstract
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Init() = 0;
	virtual int Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;
	virtual void Damaged(int damage) {}
	virtual void SpecialDamaged(int damage) {}
	virtual void IncreaseEXP(int exp) {}
	virtual void IncreaseMoney(int money) {}

public:
	void UpdateRect();
	int UpdateFrame();

public:
	// setter
	void SetRect(const RECT& rect) { m_rect = rect; }
	void SetInfo(const INFO& info) { m_info = info; }
	void SetXY(float x, float y) { m_info.xPos = x; m_info.yPos = y; }
	void SetFrame(const FRAME& frame) { m_frame = frame; }
	void SetSpeed(float speed) { m_speed = speed; }
	void SetIsDead(bool dead) { m_isDead = dead; }
	void SetHP(int hp) { m_hp = hp; }
	void SetATK(int atk) { m_atk = atk; }
	void SetEXP(int exp) { m_exp = exp; }
	void SetTotalEXP(int totalExp) { m_totalExp = totalExp; }
	void SetLV(int level) { m_level = level; }
	void SetMoney(int money) { m_money = money; }

	// getter
	RECT GetRect() const { return m_rect; }
	INFO GetInfo() const { return m_info; }
	INFO* GetInfoAddress() { return &m_info; }
	CObj* GetThis() { return this; }
	FRAME GetFrame() const { return m_frame; }
	float GetSpeed() const { return m_speed; }
	bool GetIsDead() const { return m_isDead; }
	int GetHP() const { return m_hp; }
	int GetATK() const { return m_atk; }
	int GetEXP() const { return m_exp; }
	int GetTotalEXP() const { return m_totalExp; }
	int GetLV() const { return m_level; }
	int GetMoney() const { return m_money; }

protected:
	RECT m_rect;
	INFO m_info;
	FRAME m_frame;
	float m_speed;
	bool m_isDead;
	int m_hp;
	int m_atk;
	int m_exp;
	int m_totalExp;
	int m_level;
	int m_money;
};

