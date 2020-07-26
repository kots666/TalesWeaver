#include "stdafx.h"
#include "Boss.h"
#include "CollideRect.h"
#include "CollideSpawner.h"
#include "MotionTrail.h"
#include "Player.h"
#include "Effect.h"

CBoss::CBoss(float x, float y)
	: m_isHit(false), m_isSkill(false), m_isAttack(false), m_isDeadAnimation(false), m_actionTime(GetTickCount()), m_actionSpeed(1500), m_hitTime(GetTickCount())
{
	m_info.xPos = x;
	m_info.yPos = y;

	Init();
}


CBoss::~CBoss()
{
	Release();
}

void CBoss::Init()
{
	m_info.xSize = 70;
	m_info.ySize = 100;

	m_phase = 1;
	m_hp = BOSS_HP;
	m_atk = 100;

	m_speed = 5.f;

	ExecuteScene(BOSS_SCENE::IDLE);

	target = CObjManager::GetInstance()->GetPlayer();

	m_lightSpawnTime = GetTickCount();

	UpdateRect();
}

int CBoss::Update()
{
	if (1 < m_phase) SpawnLightning();

	if (m_isDeadAnimation)
	{
		bool isEnd = UpdateFrame();

		if (isEnd)
		{
			m_isDead = true;

			SpawnDeadTrail();
		}
	}
	else
	{
		if (m_isSkill)
		{
			PortalProcess();
		}
		else
		{
			if (m_isAttack) AttackProcess();

			UpdateFrame();

			if (nullptr != target) CalcDistance();
		}
	}

	if (m_isDead) return DEAD_EVENT;

	return NO_EVENT;
}

void CBoss::LateUpdate()
{
	if (m_hp <= 0)
	{
		m_isDeadAnimation = true;
		ChangeAction(EX_DIR::DOWN, BOSS_SCENE::DIE);
	}

	ChangePhase();

	UpdateRect();
}

void CBoss::Render(HDC hDC)
{
	float x = m_info.xPos - 110.f;
	float y = m_info.yPos - 110.f;

	if (m_isHit)
	{
		if (GetTickCount() & 1)
		{
			HDC memDC = GetDCByDirection();

			TransparentBlt(hDC, x - CCamera::GetX(), y - CCamera::GetY(), 220, 200, memDC, m_frame.startFrame * 220, m_frame.sceneFrame * 220, 220, 220, RGB(64, 170, 226));
		}
	}
	else
	{
		HDC memDC = GetDCByDirection();

		TransparentBlt(hDC, x - CCamera::GetX(), y - CCamera::GetY(), 220, 200, memDC, m_frame.startFrame * 220, m_frame.sceneFrame * 220, 220, 220, RGB(64, 170, 226));
	}

	HDC UIDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_UI"));
	HDC HPDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_HP"));

	float xPos = (WINCX / 2) - 115.f;
	float yPos = 0;

	float hpPercent = 159.f * (float)m_hp / BOSS_HP;

	TransparentBlt(hDC, xPos, yPos, 229, 46, UIDC, 0, 0, 229, 46, RGB(255,0,255));
	BitBlt(hDC, xPos + 65, yPos + 21, hpPercent, 15, HPDC, 0, 0, SRCCOPY);
}

void CBoss::Release()
{
}

void CBoss::Damaged(int damage)
{
	if (nullptr == target) return;
	INFO targetInfo = target->GetInfo();

	m_hp -= damage;

	CComboManager::IncreaseCombo();

	LookAtTarget();

	CDamageFontManager::CreateDamageFont(m_info.xPos, m_info.yPos, damage);

	CObj* crashEffect = new CEffect(m_info.xPos, m_info.yPos, 127, 127, __T("Crash"), 0, 4, 50, RGB(0, 0, 0), true);
	CObjManager::GetInstance()->AddObject(crashEffect, OBJ::EFFECT);

	CDamageFontManager::CreateDamageFont(m_info.xPos, m_info.yPos, damage);

	CSoundManager::GetInstance()->PlayOverlapSound(__T("Damaged.wav"), CSoundManager::MONSTER);
}

HDC CBoss::GetDCByDirection()
{
	HDC returnDC = nullptr;

	switch (m_direction)
	{
	case EX_DIR::LEFT: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_Left")); break;
	case EX_DIR::LD: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_LD")); break;
	case EX_DIR::DOWN: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_Down")); break;
	case EX_DIR::RD: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_RD")); break;
	case EX_DIR::RIGHT: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_Right")); break;
	case EX_DIR::RU: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_RU")); break;
	case EX_DIR::LU: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_LU")); break;
	case EX_DIR::UP: returnDC = CBitmapManager::GetInstance()->GetDC(__T("Boss_Up")); break;
	}

	return returnDC;
}

void CBoss::ChangePhase()
{
	float percent = 100.f * (float)m_hp / BOSS_HP;

	if (percent > 70.f)
	{
		m_phase = 1;
	}
	else if (percent > 40.f)
	{
		m_phase = 2;
	}
	else
	{
		m_phase = 3;
	}
}

void CBoss::SpawnLightning()
{
	if (m_lightSpawnTime + 3000 < GetTickCount())
	{
		CObj* player = CObjManager::GetInstance()->GetPlayer();
		if (nullptr == player) return;

		INFO playerInfo = player->GetInfo();

		CObj* newEffect = new CEffect(playerInfo.xPos, playerInfo.yPos, 150, 150, __T("Skill_Lightning"), 0, 4, 100, RGB(255, 0, 255), true);
		CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);

		CObj* newCollide = new CCollideRect(playerInfo.xPos, playerInfo.yPos, 100, 100, m_atk);
		CObjManager::GetInstance()->AddObject(newCollide, OBJ::ENEMY_ATTACK_ONE);

		CSoundManager::GetInstance()->PlayOverlapSound(__T("Boss_Thunder.wav"), CSoundManager::MONSTER);

		m_lightSpawnTime = GetTickCount();
	}
}

void CBoss::CalcDistance()
{
	if (nullptr == target) return;
	INFO targetInfo = target->GetInfo();

	float xDist = targetInfo.xPos - m_info.xPos;
	float yDist = targetInfo.yPos - m_info.yPos;
	float dist = sqrtf(xDist * xDist + yDist * yDist);

	if (m_actionTime + m_actionSpeed < GetTickCount())
	{
		LookAtTarget();

		if (!m_isAttack)
		{
			if (dist < 100) Attack();
			else if (dist < 200) WalkToTarget();
			else if (dist < 500) Portal();
		}
	}
}

void CBoss::Attack()
{
	if (!m_isAttack)
	{
		m_isAttack = true;
		ExecuteScene(BOSS_SCENE::ATTACK);
		m_actionSpeed = 1500;
	}
}

void CBoss::Portal()
{
	if (!m_isSkill)
	{
		CSoundManager::GetInstance()->PlayOverlapSound(__T("Boss_Portal.wav"), CSoundManager::MONSTER);

		m_isSkill = true;
		ExecuteScene(BOSS_SCENE::PORTAL);
		m_actionSpeed = 1500;
	}
}

void CBoss::AttackProcess()
{
	bool isEnd = UpdateFrame();

	if (BOSS_FRAME::ATTACK_SPAWNTIME == m_frame.startFrame && !m_isAttackSpawn)
	{
		if (nullptr == target) return;
		INFO targetInfo = target->GetInfo();

		if (1 == m_phase)
		{
			CObj* newCollide = new CCollideRect(targetInfo.xPos, targetInfo.yPos, 50, 50, m_atk);
			CObjManager::GetInstance()->AddObject(newCollide, OBJ::ENEMY_ATTACK_ONE);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Boss_Attack.wav"), CSoundManager::MONSTER);
		}
		else if (2 == m_phase)
		{
			CObj* newSpawner = new CCollideSpawner(targetInfo.xPos, targetInfo.yPos, 100, 100, m_atk, 3, 100, OBJ::ENEMY_ATTACK_ONE, false);
			CObjManager::GetInstance()->AddObject(newSpawner, OBJ::SPAWNER);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Damaged.wav"), CSoundManager::MONSTER);
		}
		else
		{
			CObj* newEffect = new CEffect(targetInfo.xPos, targetInfo.yPos, 512, 512, __T("Skill_Multi"), 0, 41, SKILL_FRAME::SPEED[2], RGB(255, 0, 255), true);
			CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);

			CObj* newSpawner = new CCollideSpawner(targetInfo.xPos, targetInfo.yPos, 100, 100, m_atk * 2, 10, 100, OBJ::ENEMY_ATTACK_MUL, false);
			CObjManager::GetInstance()->AddObject(newSpawner, OBJ::SPAWNER);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Boss_Multi.wav"), CSoundManager::MONSTER);
		}

		m_isAttackSpawn = true;
	}

	if (isEnd && m_isAttack)
	{
		m_isAttack = false;
		m_isAttackSpawn = false;
		m_actionTime = GetTickCount();
		ChangeScene(BOSS_SCENE::IDLE);
	}
}

void CBoss::PortalProcess()
{
	bool isEnd = UpdateFrame();

	if (isEnd && m_isSkill)
	{
		m_isSkill = false;

		m_actionTime = GetTickCount();
		ChangeScene(BOSS_SCENE::IDLE);

		CPlayer* player = dynamic_cast<CPlayer*>(target);
		if (nullptr == player) return;

		EX_DIR::TAG bossDir = player->GetDirection();

		INFO targetInfo = target->GetInfo();

		float xPos = targetInfo.xPos;
		float yPos = targetInfo.yPos;

		float distance = 100.f;
		float shortDistance = distance / sqrtf(2.f);

		switch (bossDir)
		{
		case EX_DIR::LEFT:
			xPos += distance;
			break;
		case EX_DIR::LD:
			xPos += shortDistance;
			yPos -= shortDistance;
			break;
		case EX_DIR::DOWN:
			yPos -= distance;
			break;
		case EX_DIR::RD:
			xPos -= shortDistance;
			yPos -= shortDistance;
			break;
		case EX_DIR::RIGHT: 
			xPos -= distance;
			break;
		case EX_DIR::RU: 
			xPos -= shortDistance;
			yPos += shortDistance;
			break;
		case EX_DIR::LU: 
			xPos += shortDistance;
			yPos += shortDistance;
			break;
		case EX_DIR::UP:
			yPos += distance;
			break;
		}

		m_info.xPos = xPos;
		m_info.yPos = yPos;
		m_direction = bossDir;

		if (1 == m_phase)
		{
			CObj* newCollide = new CCollideRect(targetInfo.xPos, targetInfo.yPos, 50, 50, m_atk);
			CObjManager::GetInstance()->AddObject(newCollide, OBJ::ENEMY_ATTACK_ONE);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Damaged.wav"), CSoundManager::MONSTER);
		}
		else if (2 == m_phase)
		{
			CObj* newEffect = new CEffect(targetInfo.xPos, targetInfo.yPos, 256, 256, __T("Skill_Quake"), 0, 7, 100, RGB(255, 0, 255), true);
			CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);

			CObj* newSpawner = new CCollideSpawner(targetInfo.xPos, targetInfo.yPos, 500, 500, m_atk, 1, 0, OBJ::ENEMY_ATTACK_MUL, false);
			CObjManager::GetInstance()->AddObject(newSpawner, OBJ::SPAWNER);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Boss_Quake.wav"), CSoundManager::MONSTER);
		}
		else
		{
			CObj* newEffect = new CEffect(targetInfo.xPos, targetInfo.yPos, 256, 256, __T("Skill_Quake"), 0, 7, 100, RGB(255, 0, 255), true);
			CObjManager::GetInstance()->AddObject(newEffect, OBJ::EFFECT);

			CObj* newSpawner = new CCollideSpawner(targetInfo.xPos, targetInfo.yPos, 500, 500, m_atk * 2, 4, 200, OBJ::ENEMY_ATTACK_MUL, false);
			CObjManager::GetInstance()->AddObject(newSpawner, OBJ::SPAWNER);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Boss_Quake.wav"), CSoundManager::MONSTER);
		}
	}
}

void CBoss::LookAtTarget()
{
	if (nullptr == target) return;
	INFO targetInfo = target->GetInfo();

	float xDist = targetInfo.xPos - m_info.xPos;
	float yDist = targetInfo.yPos - m_info.yPos;
	float dist = sqrtf(xDist * xDist + yDist * yDist);

	if (0 > xDist && 0 > yDist) m_direction = EX_DIR::LU;
	if (0 <= xDist && 0 > yDist) m_direction = EX_DIR::RU;
	if (0 > xDist && 0 <= yDist) m_direction = EX_DIR::LD;
	if (0 <= xDist && 0 <= yDist) m_direction = EX_DIR::RD;

	float radian = acosf(xDist / dist); // 라디안 값

	if (0.f < yDist) radian *= -1;

	float degree = RADIAN_TO_DEGREE(radian);

	float offset = 15.f;

	if (0.f - offset <= degree && degree <= 0.f + offset) m_direction = EX_DIR::RIGHT;
	else if (90.f - offset <= degree && degree <= 90.f + offset) m_direction = EX_DIR::UP;
	else if (180.f - offset <= degree || degree <= -180.f + offset) m_direction = EX_DIR::LEFT;
	else if (-90.f - offset <= degree && degree <= -90.f + offset) m_direction = EX_DIR::DOWN;
}

void CBoss::WalkToTarget()
{
	if (nullptr == target) return;
	INFO targetInfo = target->GetInfo();

	float xDist = targetInfo.xPos - m_info.xPos;
	float yDist = targetInfo.yPos - m_info.yPos;
	float dist = sqrtf(xDist * xDist + yDist * yDist);

	ChangeScene(BOSS_SCENE::WALK);
	float radian = acosf(xDist / dist); // 라디안 값

	if (0.f < yDist) radian *= -1;

	m_info.xPos += cosf(radian) * m_speed;
	m_info.yPos -= sinf(radian) * m_speed;
}

void CBoss::SpawnDeadTrail()
{
	CObj* deadTrail = new CMotionTrail(m_info.xPos, m_info.yPos, 220, 220, 110, 110, m_frame.endFrame - 1, m_frame.sceneFrame, RGB(64, 170, 226), __T("Boss_Down"));
	CObjManager::GetInstance()->AddObject(deadTrail, OBJ::TRAIL);
}

void CBoss::ChangeAction(EX_DIR::TAG dir, int scene)
{
	if (dir != m_direction) m_direction = dir;
	ChangeScene(scene);
}

void CBoss::ChangeScene(int scene)
{
	if (m_frame.sceneFrame != scene) ExecuteScene(scene);
}

void CBoss::ExecuteScene(int scene)
{
	m_frame.sceneFrame = scene;
	m_frame.startFrame = BOSS_FRAME::START[scene];
	m_frame.endFrame = BOSS_FRAME::END[scene];
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = BOSS_FRAME::SPEED[scene];
}
