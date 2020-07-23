#pragma once
#include "Scene.h"
class CSceneEdit :
	public CScene
{
public:
	CSceneEdit();
	virtual ~CSceneEdit();

public:
	// CScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void ReadyPick();
	void PickTile();
	void ScrollTile();
	void ReadyRender(HDC hDC);
	void OtherRender(HDC hDC);
	void LastRender(HDC hDC);

private:
	EDIT::STATE m_editState;
	int m_selectTile;
	float m_scrollX;
	float m_scrollY;
	float m_maxX;
	float m_maxY;
	float m_speed;
};

