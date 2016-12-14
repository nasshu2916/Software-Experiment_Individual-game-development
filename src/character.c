/*
 * character.c
 *
 *  Created on: 2015/07/02
 *      Author: vmware
 */
#include "character.h"
#include "map.h"


int InitCharactar(){
	player.pos.x = MAP_Center_X;
	player.pos.z = 1;
	player.pos.z = MAP_Center_Y;

	player.hp = 100;
	return 0;

}
