#include "stdafx.h"
#include "Jelly.h"
#include "CollideRect.h"
#include "MotionTrail.h"
#include "Effect.h"
#include "DamageFont.h"

CJelly::CJelly(float x, float y)
	: m_isAttack(false), m_isHit(false), m_isDeadAnimation(false),
	m_actionTime(GetTickCount()), m_actionSpeed(2000)
{
	m_info.xPos = x;
	m_info.yPos = y;

	Init();
}

CJelly::~CJelly()
{
	Release();
}

void CJelly::Init()
{
	m_info.xSize = 40;
	m_info.ySize = 50;

	m_hp = 50;
	m_atk = 5;

	m_speed = 2.f;

	UpdateRect();

	m_direction = DIR::LD;
	ExecuteScene(JELLY_SCENE::IDLE);

	target = CObjManager::GetInstance()->GetPlayer();
}

int CJelly::Update()
{
	if (m_isDeadAnimation)
	{
		bool isEnd = UpdateFrame();

		if (isEnd)
		{
			m_isDead = true;

			SpawnMotionTrail();
		}
	}

	else
	{
		if (m_isHit)
		{
			bool isEnd = UpdateFrame();

			if (isEnd)
			{
				m_isHit = false;
				ChangeScene(JELLY_SCENE::IDLE);
				m_actionTime = GetTickCount();
				m_actionSpeed = 800;
			}
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

void CJelly::LateUpdate()
{
	if (m_hp <= 0)
	{
		m_isDeadAnimation = true;
		ChangeScene(JELLY_SCENE::HIT);
	}
	UpdateRect();
}

void CJelly::Render(HDC hDC)
{
	HDC memDC = nullptr;

	float xPos = m_info.xPos - (256 / 2.f);
	float yPos = m_info.yPos - (256 / 2.f);

	switch (m_direction)
	{
	case DIR::LD: memDC = CBitmapManager::GetInstance()->GetDC(__T("Jelly_LD")); break;
	case DIR::RD: memDC = CBitmapManager::GetInstance()->GetDC(__T("Jelly_RD")); break;
	case DIR::RU: memDC = CBitmapManager::GetInstance()->GetDC(__T("Jelly_RU")); break;
	case DIR::LU: memDC = CBitmapManager::GetInstance()->GetDC(__T("Jelly_LU")); break;
	}

	TransparentBlt(hDC, xPos - CCamera::GetX(), yPos - CCamera::GetY(), 256, 256, memDC, m_frame.startFrame * 256, m_frame.sceneFrame * 256, 256, 256, RGB(255, 0, 255));

	HDC UIDC = CBitmapManager::GetInstance()->GetDC(__T("Small_UI"));
	HDC HPDC = CBitmapManager::GetInstance()->GetDC(__T("Small_Hp"));

	xPos = m_info.xPos - 26.f;
	yPos = m_info.yPos + 25.f;

	float length = 50 * m_hp / 50.f;

	BitBlt(hDC, xPos++ - CCamera::GetX(), yPos++ - CCamera::GetY(), 52, 5, UIDC, 0, 0, SRCCOPY);
	BitBlt(hDC, xPos - CCamera::GetX(), yPos - CCamera::GetY(), length, 3, HPDC, 0, 0, SRCCOPY);
}

void CJelly::Release()
{
}

void CJelly::CalcDistance()
{
	if (nullptr == target) return;
	INFO targetInfo = target->GetInfo();

	float xDist = targetInfo.xPos - m_info.xPos;
	float yDist = targetInfo.yPos - m_info.yPos;
	float dist = sqrtf(xDist * xDist + yDist * yDist);

	if (0 > xDist && 0 > yDist) m_direction = DIR::LU;
	if (0 <= xDist && 0 > yDist) m_direction = DIR::RU;
	if (0 > xDist && 0 <= yDist) m_direction = DIR::LD;
	if (0 <= xDist && 0 <= yDist) m_direction = DIR::RD;

	if (m_actionTime + m_actionSpeed < GetTickCount())
	{
		if (!m_isAttack)
		{
			if (dist < 50) Attack();
			else if (dist < 300)
			{
				ChangeScene(JELLY_SCENE::WALK);

				float radian = acosf(xDist / dist); // 라디안 값

				if (0.f < yDist) radian *= -1;

				m_info.xPos += cosf(radian) * m_speed;
				m_info.yPos -= sinf(radian) * m_speed;
			}
		}
	}
}

void CJelly::Attack()
{
	if (!m_isAttack)
	{
		m_isAttack = true;
		ExecuteScene(JELLY_SCENE::ATTACK);
		m_actionSpeed = 2000;
	}
}

void CJelly::AttackProcess()
{
	bool isEnd = UpdateFrame();

	if (isEnd && m_isAttack)
	{
		m_isAttack = false;
		m_actionTime = GetTickCount();
		ChangeScene(JELLY_SCENE::IDLE);

		if (nullptr == target) return;
		INFO targetInfo = target->GetInfo();

		CObj* newCollide = new CCollideRect(targetInfo.xPos, targetInfo.yPos, 50, 50, m_atk);
		CObjManager::GetInstance()->AddObject(newCollide, OBJ::ENEMY_ATTACK_ONE);
	}
}

void CJelly::SpawnMotionTrail()
{
	CObj* deadTrail = nullptr;
	switch (m_direction)
	{
	case DIR::LD : deadTrail = new CMotionTrail(m_info.xPos, m_info.yPos, 256, 256, 128, 128, m_frame.endFrame - 1, m_frame.sceneFrame, RGB(255, 0, 255), __T("Jelly_LD")); break;
	case DIR::RD : deadTrail = new CMotionTrail(m_info.xPos, m_info.yPos, 256, 256, 128, 128, m_frame.endFrame - 1, m_frame.sceneFrame, RGB(255, 0, 255), __T("Jelly_RD")); break;
	case DIR::RU : deadTrail = new CMotionTrail(m_info.xPos, m_info.yPos, 256, 256, 128, 128, m_frame.endFrame - 1, m_frame.sceneFrame, RGB(255, 0, 255), __T("Jelly_RU")); break;
	case DIR::LU : deadTrail = new CMotionTrail(m_info.xPos, m_info.yPos, 256, 256, 128, 128, m_frame.endFrame - 1, m_frame.sceneFrame, RGB(255, 0, 255), __T("Jelly_LU")); break;
	}

	CObjManager::GetInstance()->AddObject(deadTrail, OBJ::TRAIL);
}

void CJelly::Damaged(int damage)
{
	if (!m_isHit)
	{
		if (nullptr == target) nullptr;

		//m_hp -= damage;

		CComboManager::IncreaseCombo();

		m_isHit = true;
		m_isAttack = false;

		INFO targetInfo = target->GetInfo();

		float xDist = targetInfo.xPos - m_info.xPos;
		float yDist = targetInfo.yPos - m_info.yPos;

		if (0 > xDist && 0 > yDist) ChangeAction(DIR::LU, JELLY_SCENE::HIT);
		if (0 <= xDist && 0 > yDist) ChangeAction(DIR::RU, JELLY_SCENE::HIT);
		if (0 > xDist && 0 <= yDist) ChangeAction(DIR::LD, JELLY_SCENE::HIT);
		if (0 <= xDist && 0 <= yDist) ChangeAction(DIR::RD, JELLY_SCENE::HIT);

		CObj* crashEffect = new CEffect(m_info.xPos, m_info.yPos, 127, 127, __T("Crash"), 0, 4, 50, RGB(0, 0, 0), true);
		CObjManager::GetInstance()->AddObject(crashEffect, OBJ::EFFECT);

		CDamageFontManager::CreateDamageFont(m_info.xPos, m_info.yPos, damage);
	}
}

void CJelly::ChangeAction(DIR::TAG dir, int scene)
{
	if (dir != m_direction) m_direction = dir;
	ChangeScene(scene);
}

void CJelly::ChangeScene(int scene)
{
	if (scene != m_frame.sceneFrame)
	{
		ExecuteScene(scene);
	}
}

void CJelly::ExecuteScene(int scene)
{
	m_frame.sceneFrame = scene;
	m_frame.startFrame = JELLY_FRAME::START[scene];
	m_frame.endFrame = JELLY_FRAME::END[scene];
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = JELLY_FRAME::SPEED[scene];
}
