#pragma once

namespace PLAYER_SCENE
{
	constexpr int ATTACK = 0;
	constexpr int SIT = 1;
	constexpr int HIT = 2;
	constexpr int SKILL = 3;
	constexpr int RUN = 4;
	constexpr int READY = 5;
	constexpr int IDLE = 6;
	constexpr int WALK = 7;
}

namespace PLAYER_FRAME
{
	constexpr int ATTACK_SPAWNTIME = 2;
	constexpr int SKILL_SPANWTIME = 6;

	constexpr int START[8] = { 0, 0, 1, 0, 0, 0, 0, 0 };
	constexpr int END[8] = { 6, 1, 2, 9, 10, 1, 5, 10 };
	constexpr int SCENE[8] = { 0, 1, 1, 2, 3, 4, 5, 6 };
	constexpr DWORD SPEED[8] = { 100, 500, 300, 100, 100, 500, 200, 100 };
}

namespace BOSS_SCENE
{
	constexpr int IDLE = 0;
	constexpr int WALK = 1;
	constexpr int ATTACK = 2;
	constexpr int PORTAL = 3;
	constexpr int DIE = 4;
}

namespace BOSS_FRAME
{
	constexpr int ATTACK_SPAWNTIME = 5;

	constexpr int START[5] = { 0, 0, 0, 0, 0 };
	constexpr int END[5] = { 4, 10, 7, 6, 3 };
	constexpr int SCENE[5] = { 0, 1, 2, 3, 4 };
	constexpr DWORD SPEED[5] = { 200, 100, 100, 100, 100 };
}

namespace JELLY_SCENE
{
	constexpr int ATTACK = 0;
	constexpr int HIT = 1;
	constexpr int IDLE = 2;
	constexpr int WALK = 3;
}

namespace JELLY_FRAME
{
	constexpr int START[4] = { 0,0,0,0 };
	constexpr int END[4] = { 12, 4, 5, 8 };
	constexpr DWORD SPEED[4] = { 100, 50, 200, 100 };
}

namespace SKILL
{
	constexpr int Moon = 0;
	constexpr int Boom = 1;
}

namespace SKILL_FRAME
{
	constexpr int START[2] = { 0, 0 };
	constexpr int WIDTH[2] = { 242, 128 };
	constexpr int HEIGHT[2] = { 309, 128 };
	constexpr int END[2] = { 13, 11 };
	constexpr DWORD SPEED[2] = { 100, 100 };
	constexpr DWORD COLOR[2] = { RGB(0,0,0), RGB(255,0,255) };
}