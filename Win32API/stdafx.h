// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma comment(lib,"msimg32.lib")

#ifdef _DEBUG
 #ifdef UNICODE
  #pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
 #else
  #pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ ���
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Extern.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Frames.h"

#include "CollisionManager.h"
#include "AbstractFactory.h"

#include "ObjManager.h"
#include "KeyManager.h"
#include "ScrollManager.h"
#include "BitmapManager.h"
#include "SceneManager.h"
#include "TileManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "DamageFontManager.h"
#include "ComboManager.h"