#pragma once
#include "Obj.h"
class CMouse :
	public CObj
{
public:
	explicit CMouse();
	virtual ~CMouse();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void CheckIdleClick();
	void ChangeAction(MOUSE::STATE state);

private:
	MOUSE::STATE m_state;
	bool m_isClicked;
};

