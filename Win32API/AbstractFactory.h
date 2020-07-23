#pragma once

class CObj;

template<typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* newObj = new T;
		return newObj;
	}

	static CObj* Create(float x, float y)
	{
		CObj* newObj = new T(x, y);
		return newObj;
	}
};