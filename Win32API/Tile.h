#pragma once
#include "Obj.h"
class CTile :
	public CObj
{
public:
	explicit CTile(float x, float y, int state);
	virtual ~CTile();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	int GetState() { return m_state; }
	void SetState(int state) { m_state = state; }

private:
	int m_state;
};

