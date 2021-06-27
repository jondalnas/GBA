#include "Entity.h"
#include "Level.h"

void Entity::update() {
	_attributeObj->attr0 &= ~0x00ff; //0x00ff = y bitmask
	_attributeObj->attr0 |= (y - 16) & 0x00ff;
	_attributeObj->attr1 &= ~0x01ff; //0x01ff = x bitmask
	_attributeObj->attr1 |= (x - 16) & 0x01ff;
	
	tdx = tdy = 0;
	u8 newTX = x >> 3, newTY = y >> 3;
	if (tx != newTX) {
		if (tx < newTX) {
			tdx = 1;
		} else {
			tdx = -1;
		}
		
		tx = newTX;
	}
	
	if (ty != newTY) {
		if (ty < newTY) {
			tdy = 1;
		} else {
			tdy = -1;
		}
		
		ty = newTY;
	}
}

void Entity::move(short dx, short dy) {
	if (dx != 0 && dy != 0) {
		move(dx, 0);
		move(0, dy);
	}
	
	for (u8 i = 0; i < _level->getNumEntites(); i++) {
		Entity* e = _level->getEntity(i);
		
		if (e == this) continue;
		
		*((vu16*) 0x02000100) = x;
		*((vu16*) 0x02000102) = y;
		*((vu16*) 0x02000104) = e->x;
		*((vu16*) 0x02000106) = e->y;
		
		if (x + dx - _radius < e->x + e->getRadius() && x + dx + _radius > e->x - e->getRadius() &&
			y + dy - _radius < e->y + e->getRadius() && y + dy + _radius > e->y - e->getRadius()) return;
	}

	u8 tx0 = (x + dx - _radius) >> 3;
	u8 ty0 = (y + dy - _radius) >> 3;
	u8 tx1 = (x + dx + _radius) >> 3;
	u8 ty1 = (y + dy + _radius) >> 3;
	
	//If outside screen, then return
	if (tx0 >= _level->_width || tx1 >= _level->_width || ty0 >= _level->_height || ty1 >= _level->_height) return;
	
	//Offset by 4 pixels, because all collision layers are as well
	tx0 = (x + dx - 4 - _radius) >> 3;
	ty0 = (y + dy - 4 - _radius) >> 3;
	tx1 = (x + dx - 4 + _radius) >> 3;
	ty1 = (y + dy - 4 + _radius) >> 3;
	
	for (u8 yy = ty0; yy <= ty1; yy++) {
		for (u8 xx = tx0; xx <= tx1; xx++) {
			if (_level->getTileFlag(xx, yy) & 0b00000001) return;
		}
	}
	
	x += dx;
	y += dy;
}

/*void Entity::init(EntityData_t* entityData) {
	_attributeObj->attr0 = y | (entityData->rotScale << 8) | (entityData->doubleMode << 9) | ((entityData->OBJMode & 3) << 10) | (entityData->mosaic << 12) | (entityData->colorMode << 13) | ((entityData->shape & 3) << 14);
	_attributeObj->attr1 = x | ((entityData->rotScaleParam & 31) << 9) | (entityData->flipH << 12) | (entityData->flipH << 13) | ((entityData->size & 3) << 14);
	_attributeObj->attr2 = entityData->characterName | OBJ_PRIORITY(entityData->priority & 3) | OBJ_PALETTE(entityData->paleteBank & 15);
}*/