#pragma once
#include "Obj.h"
class CMotionTrail :
	public CObj
{
public:
	CMotionTrail(float x, float y, int width, int height, float gapX, float gapY, int frame, int scene,  DWORD transColor, const TCHAR* key);
	virtual ~CMotionTrail();

	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	BLENDFUNCTION m_bf;
	DWORD m_transColor;
	const TCHAR* m_key;
	int m_alpha;
	float m_gapX;
	float m_gapY;
	float m_flyY;
};

