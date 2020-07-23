#pragma once
#include "Obj.h"
class CUI :
	public CObj
{
public:
	explicit CUI(float x, float y, int cx, int cy,
		const TCHAR* key, int* sx = nullptr, bool isTrans = true,
		int* state = nullptr, int maxState = NULL, int offset = NULL,
		DWORD color = RGB(255,0,255));
	virtual ~CUI();

	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	const TCHAR* m_key;

	bool m_isTrans;
	int* m_state;
	int* m_startX;
	int m_maxState;
	int m_offset;
	DWORD m_color;
};