#pragma once
class CScrollManager
{
public:
	CScrollManager();
	~CScrollManager();

public:
	static int GetScrollX() { return m_scrollX; }
	static int GetScrollY() { return m_scrollY; }

	static void SetScrollX(int x) { m_scrollX += x; }
	static void SetScrollY(int y) { m_scrollY += y; }

private:
	static int m_scrollX;
	static int m_scrollY;
};

