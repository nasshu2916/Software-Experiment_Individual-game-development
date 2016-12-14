/*
 * character.h
 *
 *  Created on: 2015/07/02
 *      Author: vmware
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <SDL/SDL.h>

enum{
	Player = 0,
	mob = 1
};

typedef struct{
	float x;
	float y;
	float z;
}pos;

typedef struct{
	pos pos;
	int target;
	SDLKey dir;
	pos force;
	pos angle;
	int stts;
	pos knock_back;
	int knock_back_count;
	int hp;
}CharaInfo;

CharaInfo player;


enum{
	nasi = 0,
	attack = 1,
	block = 2
};

int InitCharactar(void);




#endif /* CHARACTER_H_ */
