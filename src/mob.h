/*
 * mob.h
 *
 *  Created on: 2015/07/14
 *      Author: vmware
 */

#ifndef MOB_H_
#define MOB_H_

#include <SDL/SDL.h>
#include "character.h"

int Initmob();
void readOBJ();
void draw_mob();
void mob_move();
void read_sword_obj();
void draw_sword();
void read_taiden_obj();
void draw_taiden();

typedef struct {
	float x;
	float y;
	float z;
}vec3d;

//UV座標
typedef struct {
	float x;//x値
	float y;//y値
}vec2d;

enum {
	Zombie = 0,
	Zombie_villagers = 1,
	Zombi_pigman = 2,
	Big_zombie_pigman = 3
};

enum {
	//	false = 0,
	live = 1,
	die = 2,
	died = 3
};


typedef struct{
	pos pos;//位置
	pos force;
	float angle;
	int stts;
	int die_count;
	int die_time;
	pos knock_back;
	int knock_back_count;
	int hp;
	int power;
	int speed;
}mobInfo;

typedef struct{
	int hp;
	int power;
	int speed;
}mob_stts;

pos sword_pos;

mobInfo mob_data[5];

mob_stts mob_status[9];

int atakk_anim_count;

#endif /* MOB_H_ */
