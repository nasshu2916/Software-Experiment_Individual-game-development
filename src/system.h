/*
 * system.h
 *
 *  Created on: 2015/06/26
 *      Author: c501406006
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define SC_INPUT_KEY_DOWN 'e'
#define SC_INPUT_KEY_UP 'f'

#include "map.h"
#include <SDL/SDL.h>
#include "character.h"
#include "mob.h"

enum {
    true    = 1,
    false   = 0,
};


//キーの状態
int key_on;

int WindowWidth;
int WindowHeight;

int flame_count;


int key_up, key_down, key_left, key_right;

MapType   gMaps[MAP_Y][ MAP_Width ][ MAP_Height ];
int canmove_data[MAP_Width][MAP_Height];
int layer;

SDL_Event event;//	イベント情報が格納される構造体を宣言
SDL_Joystick *joystick;	// ジョイスティックを特定・利用するための構造体


//---------- テクスチャID ---------------//
GLuint texture[10];
GLuint zombie_pigmankin, zombie, zombie_villagers;
GLuint haikei, sora;
GLuint sword, taidenn;
GLuint HP, start_gamenn, game_setumei, game_over,clear;

int joystick_stta[10];
int joystick_anarogu_stta[5];

int InitSystem(void);
int initTexture(void);
void Input(char event, int key, int x, int y);
int SetKeyState(int *KeyBuf, int key, int onoff, int *Keys, int size);
void key_event();
pos random_pos();
void spon(int array);
void player_canhit();
mobInfo knock_back(pos attacker_pos, mobInfo receiver);
mobInfo Knock_back_anim(mobInfo mobdata);
int GetRandom(int min,int max);
void player_knock_back(pos attacker_pos);
void player_knock_back_anim();

/* runaway.c */
extern int  PrintError( const char *str );

#define TEX_HEIGHT 16
#define TEX_WIDTH 16

#endif /* SYSTEM_H_ */
