#pragma once

// 게임 사이즈 관련
constexpr int WINCX = 800;
constexpr int WINCY = 600;

// 타일 관련
constexpr int TILECX = 50;
constexpr int TILECY = 50;
constexpr int TILEX = 80;
constexpr int TILEY = 40;

// 타일 상태 관련
constexpr int TILE_NORMAL = 0;
constexpr int TILE_FAIL = 1;
constexpr int TILE_TOWN = 2;
constexpr int TILE_SHOP = 3;
constexpr int TILE_FIELD = 4;
constexpr int TILE_DUN1 = 5;
constexpr int TILE_DUN2 = 6;
constexpr int TILE_BOSS = 7;

// 씬 전환 관련
constexpr int TITLE_TO_TOWN = 0;
constexpr int SHOP_TO_TOWN = 1;
constexpr int FIELD_TO_TOWN = 2;

// Obj 기본 스펙 관련
constexpr int PLAYER_CX = 30;
constexpr int PLAYER_CY = 30;
constexpr int PLAYER_MAX_ATT = 3;
constexpr float PLAYER_RUN_SPEED = 5.f;
constexpr float PLAYER_WALK_SPEED = 3.f;
constexpr DWORD PLAYER_FRAME_SPEED = 250;
constexpr float MONSTER_SPEED = 5.f;

// UPDATE RETURN
constexpr int NO_EVENT = 0;
constexpr int DEAD_EVENT = 1;

// FRAME UPDATE RETURN
constexpr int FRAME_NO_EVENT = 0;
constexpr int FRAME_END = 1;

// 키보드 관련
constexpr DWORD KEY_LBUTTON = 0x00000001;
constexpr DWORD KEY_RBUTTON = 0x00000002;
constexpr DWORD KEY_1 = 0x00000004;
constexpr DWORD KEY_2 = 0x00000008;
constexpr DWORD KEY_3 = 0x00000010;
constexpr DWORD KEY_4 = 0x00000020;
constexpr DWORD KEY_5 = 0x00000040;
constexpr DWORD KEY_6 = 0x00000080;
constexpr DWORD KEY_UP = 0x00000100;
constexpr DWORD KEY_LEFT = 0x00000200;
constexpr DWORD KEY_DOWN = 0x00000400;
constexpr DWORD KEY_RIGHT = 0x00000800;
constexpr DWORD KEY_BACK = 0x00001000;
constexpr DWORD KEY_R = 0x00002000;
constexpr DWORD KEY_SPACE = 0x00004000;
constexpr DWORD KEY_Z = 0x00008000;
constexpr DWORD KEY_X = 0x00010000;

constexpr int JELLY_HP = 1000;
constexpr int BOSS_HP = 50000;

// etc
constexpr float PI = 3.141592f;

#define DEGREE_TO_RADIAN(N) N * PI / 180.f
#define RADIAN_TO_DEGREE(N) N * 180.f / PI