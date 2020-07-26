#pragma once

namespace EX_DIR {
	enum TAG{LEFT, LD, DOWN, RD, RIGHT, RU, UP, LU, END};
}

namespace DIR {
	enum TAG{LD, RD, RU, LU, END};
}

namespace OBJ {
	enum TAG{MAP, BUTTON, PLAYER_ATTACK, PLAYER_SKILL, ENEMY_ATTACK_ONE, ENEMY_ATTACK_MUL, TRAIL, EFFECT, MONSTER, PLAYER, SPAWNER, COMBO, SHOP, UI, MOUSE, END};
}

namespace MOUSE {
	enum STATE { IDLE, ATTACK, PORTAL, FAIL };
}

namespace EDIT {
	enum STATE {READY, TOWN, SHOP, FIELD, BOSS};
}

namespace STATE {
	enum ID { None, Walk, Run, Attack, Hit, Ready, Skill };
}

namespace ITEM
{
	enum ID
	{
		HP_POTION, MP_POTION, SP_POTION, HIGH_ARMOR, LOW_ARMOR, HIGH_SWORD, LOW_SWORD, HIGH_BOOT, LOW_BOOT,
		HIGH_RING, LOW_RING, HIGH_HAT, LOW_HAT, STRANGE_SOMETHING
	};

	constexpr int COST[14] =
	{
		10, 10, 10,
		1000, 500, 1000, 500,
		1000, 500, 1000, 500,
		1000, 500, 1000000
	};
}