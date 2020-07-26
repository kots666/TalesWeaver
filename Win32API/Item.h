#pragma once
#include "Obj.h"
class CItem :
	public CObj
{
public:
	explicit CItem(int ID, int count = 1);
	virtual ~CItem();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetCount(int count) { m_count = count; }
	void IncreaseCount() { ++m_count; }
	void DecreaseCount() { --m_count; }

	int GetID() const { return m_ID; }
	int GetCount() const { return m_count; }
	const TCHAR* GetKey() const { return m_key; }

private:
	TCHAR* m_key;
	int m_ID;
	int m_count;
};

