#include "ChainGunItem.h"
#include "../server_src/Weapons/ChainGun.h"

ChainGunItem::ChainGunItem(float _x, float _y, TextureID& _texture,
							int _value) : Item(_x, _y, _texture, _value) {}

void ChainGunItem::equipTo(PlayerActions& action) {
	if(!action.hasWeapon(this->value)) throw "Can't equip weapon";

	action.equip(new ChainGun());
}

ChainGunItem::~ChainGunItem() {}