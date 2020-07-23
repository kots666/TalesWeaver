#pragma once

namespace EX_DIR {
	enum TAG{LEFT, LD, DOWN, RD, RIGHT, RU, UP, LU, END};
}

namespace DIR {
	enum TAG{LD, RD, RU, LU, END};
}

namespace OBJ {
	enum TAG{MAP, PLAYER_ATTACK, PLAYER_SKILL, ENEMY_ATTACK_ONE, ENEMY_ATTACK_MUL, TRAIL, EFFECT, MONSTER, PLAYER, COMBO, UI, MOUSE, END};
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