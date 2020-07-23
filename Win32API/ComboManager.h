#pragma once
class CComboManager abstract
{
public:
	static int GetCombo() { return m_combo; }
	static void IncreaseCombo();
	static void ResetCombo() { m_combo = 0; }

private:
	static int m_combo;
};

