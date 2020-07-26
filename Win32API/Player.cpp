#include "stdafx.h"
#include "Player.h"
#include "ObjManager.h"
#include "Effect.h"
#include "UI.h"
#include "InfoButton.h"
#include "ComboUI.h"
#include "TargetUI.h"
#include "CollideSpawner.h"
#include "Item.h"

CPlayer::CPlayer(float x, float y, EX_DIR::TAG dir)
	: m_direction(dir), m_attackTime(GetTickCount()), m_attackSpeed(0)
{
	m_info.xPos = x;
	m_info.yPos = y;

	Init();
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Init()
{
	m_curScene = PLAYER_SCENE::IDLE;
	m_nextScene = PLAYER_SCENE::IDLE;
	m_comboCnt = 0;
	m_atkCnt = 0;
	m_skillNum = 0;

	m_hp = 100;
	m_mp = 100;
	m_sp = 100;

	m_exp = 0;
	m_totalExp = 100;
	m_level = 1;

	m_money = 100000;

	m_isRun = false;
	m_isAttackSpawn = false;
	m_isSkillSpawn = false;
	m_isInvincible = false;
	m_isLive = true;

	m_info.xSize = 50;
	m_info.ySize = 90;
	m_atk = 10;

	m_speed = PLAYER_WALK_SPEED;

	CCamera::SetX(m_info.xPos - WINCX / 2);
	CCamera::SetY(m_info.yPos - WINCY / 2);

	ChangeScene(PLAYER_SCENE::IDLE);

	CreateUI();
	CreateButton();

	UpdateRect();
}

int CPlayer::Update()
{
	if (m_isDead)
	{
		return DEAD_EVENT;
	}

	if (m_isLive)
	{

		FrameProcess();

		DecreaseSP();
		IncreaseSP();

		KeyCheck();
	}

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	CCamera::SetCenterX(m_info.xPos);
	CCamera::SetCenterY(m_info.yPos);

	if (m_hp <= 0)
	{
		if (m_isLive)
		{
			Dead();
			m_isLive = false;
		}
	}
	UpdateRect();
}

void CPlayer::Render(HDC hDC)
{
	float xPos = m_info.xPos - (256 / 2.f);
	float yPos = m_info.yPos - (256 / 2.f);

	HDC memDC = GetDCByDirection(m_direction);

	TransparentBlt(hDC, xPos - CCamera::GetX(), yPos - CCamera::GetY(), 256, 256, memDC, m_frame.startFrame * 256, PLAYER_FRAME::SCENE[m_frame.sceneFrame] * 256, 256, 256, RGB(255, 0, 255));

	memDC = CBitmapManager::GetInstance()->GetDC(__T("Skill_Icon"));

	for (int i = 0; i < 3; ++i)
	{
		int x = 10 + (i * 30);
		int y = WINCY - 86 + 15;

		BitBlt(hDC, x, y, 24, 24, memDC, i * 24, 0, SRCCOPY);
	}

	memDC = CBitmapManager::GetInstance()->GetDC(__T("Items"));

	for (int i = 0; i < 3; ++i)
	{
		if (CInvenManager::GetInstance()->IsItemExist(i))
		{
			int x = 10 + (i * 30);
			int y = WINCY - 86 + 56;

			TransparentBlt(hDC, x, y, 24, 24, memDC, i * 24, 0, 24, 24, RGB(255, 0, 255));
		}
	}
}

void CPlayer::Release()
{
}

void CPlayer::Damaged(int damage)
{
	if (!m_isInvincible && m_isLive)
	{
		if (PLAYER_SCENE::ATTACK != m_frame.sceneFrame && PLAYER_SCENE::READY != m_frame.sceneFrame && PLAYER_SCENE::SKILL != m_frame.sceneFrame)
		{
			m_curScene = PLAYER_SCENE::HIT;
			ChangeScene(m_curScene);
		}

		m_hp -= damage;

		CObj* crashEffect = new CEffect(m_info.xPos, m_info.yPos, 127, 127, __T("Crash2"), 0, 4, 50, RGB(0, 0, 0), true);
		CObjManager::GetInstance()->AddObject(crashEffect, OBJ::EFFECT);

		CDamageFontManager::CreateDamageFont(m_info.xPos, m_info.yPos, damage);

		CSoundManager::GetInstance()->PlayOverlapSound(__T("Hit.wav"), CSoundManager::PLAYER);
	}
}

void CPlayer::SpecialDamaged(int damage)
{
	if (!m_isInvincible && m_isLive)
	{
		m_isAttackSpawn = false;
		m_isSkillSpawn = false;

		ResetCombo();

		m_curScene = PLAYER_SCENE::HIT;
		ExecuteScene(m_curScene);

		m_hp -= damage;

		CObj* crashEffect = new CEffect(m_info.xPos, m_info.yPos, 127, 127, __T("Crash2"), 0, 4, 50, RGB(0, 0, 0), true);
		CObjManager::GetInstance()->AddObject(crashEffect, OBJ::EFFECT);

		CDamageFontManager::CreateDamageFont(m_info.xPos, m_info.yPos, damage);

		CSoundManager::GetInstance()->PlayOverlapSound(__T("Hit.wav"), CSoundManager::PLAYER);
	}
}

void CPlayer::IncreaseEXP(int exp)
{
	m_exp += exp;

	while (m_exp >= m_totalExp)
	{
		LevelUp();
	}
}

void CPlayer::IncreaseMoney(int money)
{
	m_money += money;
}

void CPlayer::Dead()
{
	CSoundManager::GetInstance()->PlayOverlapSound(__T("Failed.wav"), CSoundManager::PLAYER);

	m_direction = EX_DIR::LD;

	ExecuteScene(PLAYER_SCENE::SIT);
}

void CPlayer::CreateUI()
{
	CObj* newUI = new CUI(52, 69, 24, 24, __T("Skill_Icon"), &m_skillNum, false);
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);

	newUI = new CUI(0, 0, 178, 100, __T("Player_Info"));
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);

	newUI = new CUI(94, 16, 84, 14, __T("Player_Hp"), nullptr, true, &m_hp, 100.f, 84);
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);

	newUI = new CUI(94, 31, 84, 14, __T("Player_Mp"), nullptr, true, &m_mp, 100.f, 84);
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);

	newUI = new CUI(94, 46, 84, 14, __T("Player_Sp"), nullptr, true, &m_sp, 100.f, 84);
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);

	newUI = new CUI(27, 58, 12, 11, __T("Player_MoveState"), &m_isRun);
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);

	newUI = new CUI(0, WINCY - 86, 133, 86, __T("Player_Slot"), nullptr, true, nullptr, 0, 0, RGB(0, 0, 0));
	CObjManager::GetInstance()->AddObject(newUI, OBJ::UI);
}

void CPlayer::CreateButton()
{
	CObj* newButton = new CInfoButton(WINCX - 18, 100, 18, 51, __T("Stat_Button"), 342, 302, __T("Stat_Info"));
	CObjManager::GetInstance()->AddObject(newButton, OBJ::UI);

	newButton = new CInfoButton(WINCX - 18, 151, 18, 51, __T("Equip_Button"), 195, 182, __T("Equip_Info"));
	CObjManager::GetInstance()->AddObject(newButton, OBJ::UI);

	newButton = new CInfoButton(WINCX - 18, 202, 18, 51, __T("Inven_Button"), 343, 398, __T("Inven_Info"));
	CObjManager::GetInstance()->AddObject(newButton, OBJ::UI);
}

void CPlayer::UpdateRect()
{
	m_rect.left = LONG(m_info.xPos - (m_info.xSize >> 1));
	m_rect.top = LONG(m_info.yPos - (m_info.ySize >> 1));
	m_rect.right = LONG(m_info.xPos + (m_info.xSize >> 1));
	m_rect.bottom = LONG(m_info.yPos + (m_info.ySize >> 1));

	m_moveRect.left = LONG(m_info.xPos - (30 >> 1));
	m_moveRect.top = LONG(m_info.yPos + 32  - (30 >> 1));
	m_moveRect.right = LONG(m_info.xPos + (30 >> 1));
	m_moveRect.bottom = LONG(m_info.yPos + 32 + (30 >> 1));
}

void CPlayer::UsePotion(int id)
{
	if (CInvenManager::GetInstance()->IsItemExist(id))
	{
		switch (id)
		{
		case 0: HealHp(); break;
		case 1: HealMp(); break;
		case 2: HealSp(); break;
		}

		CInvenManager::GetInstance()->DecreaseItem(id);
	}
}

void CPlayer::HealHp()
{
	m_hp += 1000;

	if (m_hp > MAXHP) m_hp = MAXHP;

	CObj* newEffect = new CEffect(m_info.xPos, m_info.yPos, 128, 128, __T("HpPotion"), 0, 6, 100, RGB(255, 255, 255), true);
	CObjManager::GetInstance()->AddObject(newEffect, OBJ::UI);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("PotionUse.wav"), CSoundManager::PLAYER);
}

void CPlayer::HealMp()
{
	m_mp += 1000;

	if (m_mp > MAXMP) m_mp = MAXMP;

	CObj* newEffect = new CEffect(m_info.xPos, m_info.yPos, 128, 128, __T("MpPotion"), 0, 6, 100, RGB(255, 255, 255), true);
	CObjManager::GetInstance()->AddObject(newEffect, OBJ::UI);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("PotionUse.wav"), CSoundManager::PLAYER);
}

void CPlayer::HealSp()
{
	m_sp += 1000;

	if (m_sp > MAXSP) m_sp = MAXSP;

	CObj* newEffect = new CEffect(m_info.xPos, m_info.yPos, 128, 128, __T("HpPotion"), 0, 6, 100, RGB(255, 255, 255), true);
	CObjManager::GetInstance()->AddObject(newEffect, OBJ::UI);

	newEffect = new CEffect(m_info.xPos, m_info.yPos, 128, 128, __T("MpPotion"), 0, 6, 100, RGB(255, 255, 255), true);
	CObjManager::GetInstance()->AddObject(newEffect, OBJ::UI);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("PotionUse.wav"), CSoundManager::PLAYER);
}

void CPlayer::MakeHalfDie()
{
	m_hp >>= 2;
	m_mp >>= 2;
	m_sp >>= 2;
}

void CPlayer::FrameProcess()
{
	bool isEnd = UpdateFrame();

	AttackSpawnCheck();
	SkillSpawnCheck();

	if (isEnd)
	{
		switch (m_curScene)
		{
		case PLAYER_SCENE::IDLE: CComboManager::ResetCombo(); break;
		case PLAYER_SCENE::ATTACK: m_isAttackSpawn = false; break;
		case PLAYER_SCENE::SKILL: m_isSkillSpawn = false; break;
		}

		// 만약 스킬 사전 단계였다면 바로 스킬로 넘어간다.
		if (PLAYER_SCENE::READY == m_curScene)
		{
			m_curScene = PLAYER_SCENE::SKILL;
			m_nextScene = PLAYER_SCENE::IDLE;
			if (m_comboCnt) ExecuteScene(m_curScene);
			else ChangeScene(m_curScene);
		}

		// 스킬 사전 준비 단계가 아닌 경우
		else
		{
			// 공격 콤보 횟수 초기화
			// 공격이 끝났는데
			if (PLAYER_SCENE::ATTACK == m_curScene)
			{
				// 다음 행동이 공격이 아닌 경우
				if (PLAYER_SCENE::ATTACK != m_nextScene) m_atkCnt = 0;
				// 다음 행동이 공격도 스킬도 아닌 경우에는 콤보 초기화
				if (PLAYER_SCENE::ATTACK != m_nextScene && PLAYER_SCENE::READY != m_nextScene) ResetCombo();
			}
			// 스킬이 끝났는데
			if (PLAYER_SCENE::SKILL == m_curScene)
			{
				// 또 공격이 아니라면 combo 초기화
				if (PLAYER_SCENE::ATTACK != m_nextScene) ResetCombo();
			}

			// 공격이 끝났고 연속 공격을 할 경우
			if (PLAYER_SCENE::ATTACK == m_curScene && PLAYER_SCENE::ATTACK == m_nextScene)
			{
				if (PLAYER_MAX_ATT > m_atkCnt)
				{
					++m_atkCnt;
					m_curScene = m_nextScene;
					m_nextScene = PLAYER_SCENE::IDLE;

					ExecuteScene(m_curScene);
				}
				else
				{
					ResetCombo();
				}
				
			}
			// 공격이 끝났고 스킬을 사용할 경우
			else if (PLAYER_SCENE::ATTACK == m_curScene && PLAYER_SCENE::READY == m_nextScene)
			{
				m_curScene = m_nextScene;
				m_nextScene = PLAYER_SCENE::IDLE;

				ExecuteScene(m_curScene);
			}
			// 스킬이 끝났고 공격을 할 경우
			else if (PLAYER_SCENE::SKILL == m_curScene && PLAYER_SCENE::ATTACK == m_nextScene)
			{
				++m_comboCnt;
				m_curScene = m_nextScene;
				m_nextScene = PLAYER_SCENE::IDLE;

				ExecuteScene(m_curScene);
			}
			else
			{
				m_curScene = m_nextScene;
				m_nextScene = PLAYER_SCENE::IDLE;
				ExecuteScene(m_curScene);
			}
		}
	}
}

void CPlayer::KeyCheck()
{
	if (CKeyManager::GetInstance()->isKeyDown(KEY_1)) m_skillNum = 0;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_2)) m_skillNum = 1;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_3)) m_skillNum = 2;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_4)) UsePotion(0);
	if (CKeyManager::GetInstance()->isKeyDown(KEY_5)) UsePotion(1);
	if (CKeyManager::GetInstance()->isKeyDown(KEY_6)) UsePotion(2);
	if (CKeyManager::GetInstance()->isKeyDown(KEY_X)) MakeHalfDie();
	if (CKeyManager::GetInstance()->isKeyDown(KEY_BACK)) m_isInvincible ^= true;

	if (CKeyManager::GetInstance()->isKeyDown(KEY_R))
	{
		m_isRun ^= true;
		if (m_isRun) m_speed = PLAYER_RUN_SPEED;
		else m_speed = PLAYER_WALK_SPEED;
	}

	// 일정 시간이 지나야 다시 공격 가능
	if (m_attackTime + m_attackSpeed < GetTickCount())
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_Z))
		{
			if (m_mp - 100 >= 0)
			{
				m_mp -= 100;
				if (PLAYER_SCENE::HIT != m_curScene && PLAYER_SCENE::READY != m_curScene && PLAYER_SCENE::SKILL != m_curScene)
				{
					CSoundManager::GetInstance()->PlayOverlapSound(__T("Skill_Ready.wav"), CSoundManager::PLAYER);
					if (PLAYER_SCENE::ATTACK == m_curScene)
					{
						if (CComboManager::GetCombo()) m_nextScene = PLAYER_SCENE::READY;
					}
					else
					{
						m_curScene = PLAYER_SCENE::READY;
						m_nextScene = PLAYER_SCENE::SKILL;
						ChangeScene(m_curScene);
					}
				}
			}
			else CSoundManager::GetInstance()->PlayOverlapSound(__T("NoMana.wav"), CSoundManager::PLAYER);
		}

		if (CKeyManager::GetInstance()->isKeyDown(KEY_SPACE))
		{
			if (PLAYER_SCENE::HIT != m_curScene && PLAYER_SCENE::READY != m_curScene)
			{
				if (PLAYER_SCENE::ATTACK == m_curScene)
				{
					if (CComboManager::GetCombo()) m_nextScene = PLAYER_SCENE::ATTACK;
				}
				else if (PLAYER_SCENE::SKILL == m_curScene)
				{
					if (CComboManager::GetCombo()) m_nextScene = PLAYER_SCENE::ATTACK;
				}
				else
				{
					m_curScene = PLAYER_SCENE::ATTACK;
					m_nextScene = PLAYER_SCENE::IDLE;
					ChangeScene(m_curScene);
				}
			}
		}
	}

	if (PLAYER_SCENE::IDLE == m_curScene || PLAYER_SCENE::RUN == m_curScene || PLAYER_SCENE::WALK == m_curScene) {
		
		bool isNoAction = true;

		if (CKeyManager::GetInstance()->isKeyPressing(KEY_UP))
		{
			isNoAction = false;

			if (CKeyManager::GetInstance()->isKeyPressing(KEY_LEFT))
			{
				float speed = m_speed / sqrtf(2.f);
				MoveLU(speed);
			}
			else if (CKeyManager::GetInstance()->isKeyPressing(KEY_RIGHT))
			{
				float speed = m_speed / sqrtf(2.f);
				MoveRU(speed);
			}
			else
			{
				MoveUp(m_speed);
			}
		}
		else if (CKeyManager::GetInstance()->isKeyPressing(KEY_DOWN))
		{
			isNoAction = false;

			if (CKeyManager::GetInstance()->isKeyPressing(KEY_LEFT))
			{
				float speed = m_speed / sqrtf(2.f);
				MoveLD(speed);
			}
			else if (CKeyManager::GetInstance()->isKeyPressing(KEY_RIGHT))
			{
				float speed = m_speed / sqrtf(2.f);
				MoveRD(speed);
			}
			else
			{
				MoveDown(m_speed);
			}
		}
		else if (CKeyManager::GetInstance()->isKeyPressing(KEY_LEFT))
		{
			isNoAction = false;
			MoveLeft(m_speed);
		}

		else if (CKeyManager::GetInstance()->isKeyPressing(KEY_RIGHT))
		{
			isNoAction = false;
			MoveRight(m_speed);
		}

		if (isNoAction)
		{
			m_curScene = PLAYER_SCENE::IDLE;
			m_nextScene = PLAYER_SCENE::IDLE;
			ChangeScene(m_curScene);
		}
	}
}

void CPlayer::AttackSpawnCheck()
{
	if (PLAYER_SCENE::ATTACK == m_frame.sceneFrame && PLAYER_FRAME::ATTACK_SPAWNTIME == m_frame.startFrame && !m_isAttackSpawn)
	{
		m_isAttackSpawn = true;

		float xPos = m_info.xPos;
		float yPos = m_info.yPos;

		switch (m_direction)
		{
		case EX_DIR::LEFT:
			xPos -= TILECX;
			break;
		case EX_DIR::LD:
			xPos -= TILECX / sqrtf(2.f);
			yPos += TILECY / sqrtf(2.f);
			break;
		case EX_DIR::DOWN:
			yPos += TILECY;
			break;
		case EX_DIR::RD:
			xPos += TILECX / sqrtf(2.f);
			yPos += TILECY / sqrtf(2.f);
			break;
		case EX_DIR::RIGHT:
			xPos += TILECX;
			break;
		case EX_DIR::RU:
			xPos += TILECX / sqrtf(2.f);
			yPos -= TILECY / sqrtf(2.f);
			break;
		case EX_DIR::UP:
			yPos -= TILECY;
			break;
		case EX_DIR::LU:
			xPos -= TILECX / sqrtf(2.f);
			yPos -= TILECY / sqrtf(2.f);
			break;
		}

		SpawnCollide(xPos, yPos, 50, 50, m_atk);

		CSoundManager::GetInstance()->PlayOverlapSound(__T("Attack.wav"), CSoundManager::PLAYER);
	}
}

void CPlayer::SkillSpawnCheck()
{
	if (PLAYER_SCENE::SKILL == m_frame.sceneFrame && PLAYER_FRAME::SKILL_SPANWTIME == m_frame.startFrame && !m_isSkillSpawn)
	{
		m_isSkillSpawn = true;

		switch (m_skillNum)
		{
		case SKILL::Moon: SkillMoon(); break;
		case SKILL::Boom: SkillBoom(); break;
		case SKILL::Multi: SkillMulti(); break;
		}
	}
}

void CPlayer::SkillMoon()
{
	float xPos = m_info.xPos;
	float yPos = m_info.yPos;

	float mul = 1.5f;

	switch (m_direction)
	{
	case EX_DIR::LEFT:
		xPos -= TILECX * mul;
		break;
	case EX_DIR::LD:
		xPos -= TILECX * mul / sqrtf(2.f);
		yPos += TILECY * mul / sqrtf(2.f);
		break;
	case EX_DIR::DOWN:
		yPos += TILECY * mul;
		break;
	case EX_DIR::RD:
		xPos += TILECX * mul / sqrtf(2.f);
		yPos += TILECY * mul / sqrtf(2.f);
		break;
	case EX_DIR::RIGHT:
		xPos += TILECX * mul;
		break;
	case EX_DIR::RU:
		xPos += TILECX * mul / sqrtf(2.f);
		yPos -= TILECY * mul / sqrtf(2.f);
		break;
	case EX_DIR::UP:
		yPos -= TILECY * mul;
		break;
	case EX_DIR::LU:
		xPos -= TILECX * mul / sqrtf(2.f);
		yPos -= TILECY * mul / sqrtf(2.f);
		break;
	}

	DWORD skillSpeed = 250;

	SpawnCollide(xPos, yPos, 100, 100, m_atk * 3, 4, skillSpeed, OBJ::PLAYER_SKILL);

	int width = SKILL_FRAME::WIDTH[m_skillNum];
	int height = SKILL_FRAME::HEIGHT[m_skillNum];
	int end = SKILL_FRAME::END[m_skillNum];
	DWORD speed = SKILL_FRAME::SPEED[m_skillNum];
	DWORD color = SKILL_FRAME::COLOR[m_skillNum];

	CObj* newEffect = new CEffect(xPos, yPos, width, height, __T("Skill_Moon"), 0, end, speed, color, true);

	CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("Skill_Moon.wav"), CSoundManager::PLAYER);
}

void CPlayer::SkillBoom()
{
	int width = SKILL_FRAME::WIDTH[m_skillNum];
	int height = SKILL_FRAME::HEIGHT[m_skillNum];
	int end = SKILL_FRAME::END[m_skillNum];
	DWORD speed = SKILL_FRAME::SPEED[m_skillNum];
	if (m_comboCnt) speed /= 2;
	DWORD color = SKILL_FRAME::COLOR[m_skillNum];

	CObj* newEffect = nullptr;

	for (int i = 0; i < 3; ++i)
	{
		float yPos = m_info.yPos + (i - 1) * height;
		for (int j = 0; j < 3; ++j)
		{
			float xPos = m_info.xPos + (j - 1) * width;

			CObj* newEffect = new CEffect(xPos, yPos, width, height, __T("Skill_Boom"), 0, end, speed, color, true);
			CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);
		}
	}

	SpawnCollide(m_info.xPos, m_info.yPos, width * 3, height * 3, m_atk * 5, 1, 0, OBJ::PLAYER_SKILL);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("Skill_Boom.wav"), CSoundManager::PLAYER);
}

void CPlayer::SkillMulti()
{
	float xPos = m_info.xPos;
	float yPos = m_info.yPos;

	float mul = 1.5f;

	switch (m_direction)
	{
	case EX_DIR::LEFT:
		xPos -= TILECX * mul;
		break;
	case EX_DIR::LD:
		xPos -= TILECX * mul / sqrtf(2.f);
		yPos += TILECY * mul / sqrtf(2.f);
		break;
	case EX_DIR::DOWN:
		yPos += TILECY * mul;
		break;
	case EX_DIR::RD:
		xPos += TILECX * mul / sqrtf(2.f);
		yPos += TILECY * mul / sqrtf(2.f);
		break;
	case EX_DIR::RIGHT:
		xPos += TILECX * mul;
		break;
	case EX_DIR::RU:
		xPos += TILECX * mul / sqrtf(2.f);
		yPos -= TILECY * mul / sqrtf(2.f);
		break;
	case EX_DIR::UP:
		yPos -= TILECY * mul;
		break;
	case EX_DIR::LU:
		xPos -= TILECX * mul / sqrtf(2.f);
		yPos -= TILECY * mul / sqrtf(2.f);
		break;
	}

	SpawnCollide(xPos, yPos, 100, 100, m_atk * 2, 10, 100, OBJ::PLAYER_SKILL);

	int width = SKILL_FRAME::WIDTH[m_skillNum];
	int height = SKILL_FRAME::HEIGHT[m_skillNum];
	int end = SKILL_FRAME::END[m_skillNum];
	DWORD speed = SKILL_FRAME::SPEED[m_skillNum];
	DWORD color = SKILL_FRAME::COLOR[m_skillNum];

	CObj* newEffect = new CEffect(xPos, yPos, width, height, __T("Skill_Multi"), 0, end, speed, color, true);

	CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("Skill_Multi.wav"), CSoundManager::PLAYER);
}

void CPlayer::SpawnCollide(float x, float y, int cx, int cy, int damage, int spawnCount, DWORD gapTime, OBJ::TAG type)
{
	CObj* newSpawner = new CCollideSpawner(x, y, cx, cy, damage, spawnCount, gapTime, type);
	CObjManager::GetInstance()->AddObject(newSpawner, OBJ::SPAWNER);
}

void CPlayer::DecreaseSP()
{
	if (m_sp <= 0)
	{
		m_isRun = 0;
		m_speed = PLAYER_WALK_SPEED;
	}

	if (m_frame.sceneFrame == PLAYER_SCENE::RUN) --m_sp;
}

void CPlayer::IncreaseSP()
{
	if (m_frame.sceneFrame == PLAYER_SCENE::IDLE)
	{
		++m_mp;
		++m_sp;
	}
	if (m_mp > MAXMP) m_mp = MAXMP;

	if (m_sp > MAXSP) m_sp = MAXSP;
}

void CPlayer::LevelUp()
{
	m_exp -= m_totalExp;

	m_totalExp = 100 * ++m_level;

	m_hp = MAXHP;
	m_mp = MAXMP;
	m_sp = MAXSP;

	// LevelUp Effect
	CObj* LVUP = new CTargetUI(this, 128, 128, __T("LvUp"), 256, 256, 0, 30, 30, RGB(255, 0, 255), true);
	CObjManager::GetInstance()->AddObject(LVUP, OBJ::EFFECT);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("lvup.wav"), CSoundManager::PLAYER);
}

void CPlayer::RefreshStat()
{
	m_hp = 100;
	m_mp = 100;
	m_sp = 100;
	m_atk = 10;

	MAXHP = 100;
	MAXMP = 100;
	MAXSP = 100;
	ATK = 10;

	// HEAD
	const CItem* head = CEquipManager::GetInstance()->GetHead();
	if (nullptr != head)
	{
		int id = head->GetID();
		if (ITEM::HIGH_HAT == id)
		{
			MAXMP += 20000;
		}
		else if (ITEM::LOW_HAT == id)
		{
			MAXMP += 1000;
		}
	}

	// HEAD
	const CItem* armor = CEquipManager::GetInstance()->GetArmor();
	if (nullptr != armor)
	{
		int id = armor->GetID();
		if (ITEM::HIGH_ARMOR == id)
		{
			MAXHP += 20000;
		}
		else if (ITEM::LOW_ARMOR == id)
		{
			MAXHP += 1000;
		}
	}

	// ARMS
	const CItem* arms = CEquipManager::GetInstance()->GetArms();
	if (nullptr != arms)
	{
		int id = arms->GetID();
		if (ITEM::HIGH_SWORD == id)
		{
			ATK += 500;
		}
		else if (ITEM::LOW_SWORD == id)
		{
			ATK += 100;
		}
	}

	// ARMS
	const CItem* hand = CEquipManager::GetInstance()->GetHand();
	if (nullptr != hand)
	{
		int id = hand->GetID();
		if (ITEM::HIGH_RING == id)
		{
			ATK += 100;
			MAXHP += 1000;
			MAXMP += 500;
			MAXSP += 500;
		}
		else if (ITEM::LOW_RING == id)
		{
			ATK += 20;
			MAXHP += 200;
			MAXMP += 100;
			MAXSP += 100;
		}
	}

	// BOOT
	const CItem* foot = CEquipManager::GetInstance()->GetFoot();
	if (nullptr != foot)
	{
		int id = foot->GetID();
		if (ITEM::HIGH_BOOT == id)
		{
			MAXSP += 5000;
		}
		else if (ITEM::LOW_BOOT == id)
		{
			MAXSP += 1000;
		}
	}

	m_hp = MAXHP;
	m_mp = MAXMP;
	m_sp = MAXSP;
	m_atk = ATK;
}

void CPlayer::ResetCombo()
{
	CComboManager::ResetCombo();

	m_isAttackSpawn = false;
	m_isSkillSpawn = false;

	m_atkCnt = 0;

	m_attackTime = GetTickCount();
	m_attackSpeed = 500;

	m_curScene = PLAYER_SCENE::IDLE;
	m_nextScene = PLAYER_SCENE::IDLE;

	ChangeScene(m_curScene);
}

void CPlayer::MoveUp(float speed)
{
	if (0.f <= m_moveRect.top - speed)
	{
		int tileX = m_moveRect.left / TILECX;
		int tileY = (m_moveRect.top - speed) / TILECY;

		int index = tileX + (tileY * TILEX);

		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		tileX = m_moveRect.right / TILECX;
		index = tileX + (tileY * TILEX);

		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		m_info.yPos -= speed;

		if (m_isRun) ChangeAction(EX_DIR::UP, PLAYER_SCENE::RUN);
		else ChangeAction(EX_DIR::UP, PLAYER_SCENE::WALK);
	}
}

void CPlayer::MoveLeft(float speed)
{
	if (0.f <= m_moveRect.left - speed)
	{
		int tileX = (m_moveRect.left - speed) / TILECX;
		int tileY = m_moveRect.top / TILECY;

		int index = tileX + (tileY * TILEX);

		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		tileY = m_moveRect.bottom / TILECY;
		index = tileX + (tileY * TILEX);
		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		m_info.xPos -= speed;

		if (m_isRun) ChangeAction(EX_DIR::LEFT, PLAYER_SCENE::RUN);
		else ChangeAction(EX_DIR::LEFT, PLAYER_SCENE::WALK);
	}
}

void CPlayer::MoveDown(float speed)
{
	if (CCamera::GetMaxY() + WINCY >= m_moveRect.bottom + speed)
	{
		int tileX = m_moveRect.left / TILECX;
		int tileY = (m_moveRect.bottom + speed) / TILECY;

		int index = tileX + (tileY * TILEX);

		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		tileX = m_moveRect.right / TILECX;
		index = tileX + (tileY * TILEX);
		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		m_info.yPos += speed;

		if (m_isRun) ChangeAction(EX_DIR::DOWN, PLAYER_SCENE::RUN);
		else ChangeAction(EX_DIR::DOWN, PLAYER_SCENE::WALK);
	}
}

void CPlayer::MoveRight(float speed)
{
	if (CCamera::GetMaxX() + WINCX >= m_moveRect.right + speed)
	{
		int tileX = (m_moveRect.right + speed) / TILECX;
		int tileY = m_moveRect.top / TILECY;

		int index = tileX + (tileY * TILEX);

		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		tileY = m_moveRect.bottom / TILECY;
		index = tileX + (tileY * TILEX);
		if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) return;

		m_info.xPos += speed;

		if (m_isRun) ChangeAction(EX_DIR::RIGHT, PLAYER_SCENE::RUN);
		else ChangeAction(EX_DIR::RIGHT, PLAYER_SCENE::WALK);
	}
}

void CPlayer::MoveLU(float speed)
{
	MoveUp(speed);
	MoveLeft(speed);
	if (m_isRun) ChangeAction(EX_DIR::LU, PLAYER_SCENE::RUN);
	else ChangeAction(EX_DIR::LU, PLAYER_SCENE::WALK);
}

void CPlayer::MoveLD(float speed)
{
	MoveDown(speed);
	MoveLeft(speed);
	if (m_isRun) ChangeAction(EX_DIR::LD, PLAYER_SCENE::RUN);
	else ChangeAction(EX_DIR::LD, PLAYER_SCENE::WALK);
}

void CPlayer::MoveRU(float speed)
{
	MoveUp(speed);
	MoveRight(speed);
	if (m_isRun) ChangeAction(EX_DIR::RU, PLAYER_SCENE::RUN);
	else ChangeAction(EX_DIR::RU, PLAYER_SCENE::WALK);
}

void CPlayer::MoveRD(float speed)
{
	MoveDown(speed);
	MoveRight(speed);
	if (m_isRun) ChangeAction(EX_DIR::RD, PLAYER_SCENE::RUN);
	else ChangeAction(EX_DIR::RD, PLAYER_SCENE::WALK);
}

void CPlayer::ChangeAction(EX_DIR::TAG dir, int scene)
{
	if (m_direction != dir) m_direction = dir;
	ChangeScene(scene);
}

void CPlayer::ChangeScene(int scene)
{
	if (m_frame.sceneFrame != scene)
	{
		m_frame.sceneFrame = scene;
		m_frame.startFrame = PLAYER_FRAME::START[scene];
		m_frame.endFrame = PLAYER_FRAME::END[scene];
		m_frame.frameTime = GetTickCount();
		m_frame.frameSpeed = PLAYER_FRAME::SPEED[scene];
	}
	if (EX_DIR::DOWN == m_direction && PLAYER_SCENE::WALK == scene) m_frame.endFrame = PLAYER_FRAME::END[scene] - 1;
}

void CPlayer::ExecuteScene(int scene)
{
	m_frame.sceneFrame = scene;
	m_frame.startFrame = PLAYER_FRAME::START[scene];
	m_frame.endFrame = PLAYER_FRAME::END[scene];
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = PLAYER_FRAME::SPEED[scene];
}

HDC CPlayer::GetDCByDirection(EX_DIR::TAG dir)
{
	HDC returnDC = nullptr;

	switch (dir)
	{
	case EX_DIR::LEFT: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_Left")); break;
	case EX_DIR::LD: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_LD")); break;
	case EX_DIR::DOWN: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_Down")); break;
	case EX_DIR::RD: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_RD")); break;
	case EX_DIR::RIGHT: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_Right")); break;
	case EX_DIR::RU: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_RU")); break;
	case EX_DIR::LU: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_LU")); break;
	case EX_DIR::UP: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Player_Up")); break;
	}

	return returnDC;
}