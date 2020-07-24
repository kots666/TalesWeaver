#pragma once
#include "Obj.h"
class CNPC :
	public CObj
{
public:
	CNPC(float x, float y, int cx, int cy, int ID = 0);
	virtual ~CNPC();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void ClickCheck();
	void OpenShopUI();

private:
	int m_ID;
};

