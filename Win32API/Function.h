#pragma once

template<typename T>
void SafeDelete(T& var)
{
	if (nullptr != var) {
		delete var;
		var = nullptr;
	}
}