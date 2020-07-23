#pragma once
class CCamera
{
public:
	static void Init();

	static void AddX(int x) { m_x += x; }
	static void AddY(int y) { m_y += y; }
	static void SetX(int x) { m_x = x; }
	static void SetY(int y) { m_y = y; }
	static void SetCenterX(int x);
	static void SetCenterY(int y);
	static void SetMaxX(int x) { m_maxX = x; }
	static void SetMaxY(int y) { m_maxY = y; }

	static int GetX() { return m_x; }
	static int GetY() { return m_y; }
	static int GetMaxX() { return m_maxX; }
	static int GetMaxY() { return m_maxY; }

private:
	static int m_x;
	static int m_y;
	static int m_maxX;
	static int m_maxY;
};

