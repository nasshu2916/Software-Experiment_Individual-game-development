/*
 * system.c
 *
 *  Created on: 2015/06/26
 *      Author: c501406006
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <GL/glew.h>

#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <time.h>
#include "system.h"
#include "map.h"

MapType   gMaps[MAP_Y][ MAP_Width ][ MAP_Height ];

#define KNOCK_BACK_VALUE	0.5
#define KNOCK_BACK_TIME	7

char gMapDataFile[] = "map.data";
char MapData[MAP_Y][15] = {
		"map/map.prn", "map/map1.prn", "map/map2.prn", "map/map3.prn", "map/map4.prn",
		"map/map5.prn", "map/map6.prn", "map/map7.prn", "map/map8.prn"
};

//, "map/map6.prn", "map/map7.prn", "map/map8.prn"};

static int Keys[] = {	//チェックするキーの登録
		'a',	//1
		'd',	//2
		'w',	//4
		's',	//8
		' '		//16
};

int InitSystem(void){
	FILE* fp;
	int a,i,j;
	/* マップ読み込み */

	for (a = 0; a < MAP_Y; a++) {
		fp = fopen( MapData[a], "r" );
		if( fp == NULL ){
			printf("no map date\n");
		}
		for(j=0; j<MAP_Height; j++){

			for(i=0; i<MAP_Width; i++){
				if(fscanf(fp, "%d", &gMaps[a][i][j]) != 1){
					fclose( fp );
				}
				//				printf("%d",gMaps[a][i][j]);
			}
			//printf("\n");
		}
		fclose( fp );
	}




	for(j=0; j<MAP_Height; j++){
		for(i=0; i<MAP_Width; i++){
			if(gMaps[1][i][j] < 0){
				canmove_data[i][j] = true;
			}else{
				canmove_data[i][j] = false;
			}
		}
	}

	//ジョイスティックのデータの初期化
	for (i = 0; i < 10; ++i) {
		joystick_stta[i] = 0;
	}
	for (i = 0; i < 4; ++i) {
		joystick_anarogu_stta[i] = 0;
	}

	//キー
	key_on=0;


	return 0;
}

int GetRandom(int min,int max)
{
	static int flag;

	if (flag == 0) {
		srand((unsigned int)time(NULL));
		flag = 1;
	}

	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}


void Input(char event, int key, int x, int y){
	switch(event){
	case SC_INPUT_KEY_DOWN:	//キーが押されたとき
		SetKeyState(&key_on, key, true, Keys, sizeof(Keys) );
		break;
	case SC_INPUT_KEY_UP:	//キーが離されたとき
		SetKeyState(&key_on, key, false, Keys, sizeof(Keys) );
		break;
	}
}

int SetKeyState(int *KeyBuf, int key, int onoff, int *Keys, int size)
{
	//サイズを要素数に変換
	size /= sizeof(int);

	//サイズが大き過ぎないかチェック
	if( size >= sizeof(int)*8 )
		return false;

	//サイズ分処理
	int i;
	for(i=0; i<size; i++){
		if( key == Keys[i] ){
			int k=1;
			if( onoff )
				*KeyBuf |= k << i;	//ビットON
			else
				*KeyBuf &= ~( k << i );	//ビットOFF
		}
	}

	return true;
}

void key_event(){
	// イベントを取得したなら
	if(SDL_PollEvent(&event)){
		// イベントの種類ごとに処理
		switch (event.type) {
		// ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
		case SDL_JOYAXISMOTION:
			joystick_anarogu_stta[event.jaxis.axis] = event.jaxis.value;
			break;
			// ジョイスティックのボタンが押された時
		case SDL_JOYBUTTONDOWN:
			joystick_stta[event.jbutton.button] = true;
			break;
			// ボタンが離された時
		case SDL_JOYBUTTONUP:
			joystick_stta[event.jbutton.button] = false;
			break;
		}
	}

}


void spon(int array){
	mob_data[array].stts = live;
	mob_data[array].pos = random_pos();
	mob_data[array].hp = mob_status[array].hp;
	mob_data[array].power = mob_status[array].power;
	mob_data[array].speed = mob_status[array].speed;
}

pos random_pos(){
	int x,y,z;

	pos spown_pos;

	x = GetRandom(2,49);
	y = 0.5;
	z = GetRandom(2,49);

	if(canmove_data[x][z] != true){
		spown_pos = random_pos();
	}else{
		spown_pos.x = x;
		spown_pos.y = y;
		spown_pos.z = z;
	}

	return spown_pos;
}

void player_canhit(){
	int x,z;


	x = roundf(player.pos.x) + roundf(sin(player.angle.y / 180 * M_PI));
	z = roundf(player.pos.z) + roundf(cos(player.angle.y / 180 * M_PI));

	if (canmove_data[x][z] >= 3) {
		if (mob_data[canmove_data[x][z] - 3].knock_back_count == 0) {
			printf("hit mob%d\n", canmove_data[x][z] - 3);
			mob_data[canmove_data[x][z] - 3].hp--;
			if(mob_data[canmove_data[x][z] - 3].hp == 0){
				mob_data[canmove_data[x][z] - 3].stts = die;
				mob_data[canmove_data[x][z] - 3].die_time = flame_count % 30;
				mob_data[canmove_data[x][z] - 3].die_count = 0;
				canmove_data[x][z] = true;
				layer = 21;
			}
			mob_data[canmove_data[x][z] - 3] = knock_back(player.pos ,mob_data[canmove_data[x][z] - 3]);
		}
	}

}

mobInfo knock_back(pos attacker_pos, mobInfo receiver){
	float ver_x,ver_z;
	float angle;

	ver_x = attacker_pos.x - receiver.pos.x;
	ver_z = attacker_pos.z - receiver.pos.z;
	//	x = roundf(receiver_pos.x);
	//	z = roundf(receiver_pos.z);
	//	canmove_data[x][z] = true;

	angle = atan2f( ver_x , ver_z) / M_PI * 180;

	receiver.knock_back.x = -1 * sin(angle / 180 * M_PI) * KNOCK_BACK_VALUE;
	receiver.knock_back.z = -1 * cos(angle / 180 * M_PI) * KNOCK_BACK_VALUE;
	receiver.knock_back_count = KNOCK_BACK_TIME;

	return receiver;
}

mobInfo Knock_back_anim(mobInfo mobdata){
	int x,z;

	x = roundf(mobdata.pos.x);
	z = roundf(mobdata.pos.z);

	if (mobdata.knock_back.x > 0) {
		x = roundf(mobdata.pos.x + mobdata.knock_back.x + 0.5);
		if (canmove_data[x][z] == true) {
			mobdata.pos.x += mobdata.knock_back.x;
		}else{
			mobdata.pos.x = roundf(mobdata.pos.x);
		}
	}else{
		x = roundf(mobdata.pos.x + mobdata.knock_back.x - 0.5);
		if (canmove_data[x][z] == true) {
			mobdata.pos.x += mobdata.knock_back.x;
		}else{
			mobdata.pos.x = roundf(mobdata.pos.x);
		}
	}
	x = roundf(mobdata.pos.x);
	if (mobdata.knock_back.z > 0) {
		z = roundf(mobdata.pos.z + mobdata.knock_back.z + 0.5);
		if (canmove_data[x][z] == true) {
			mobdata.pos.z += mobdata.knock_back.z;
		}else{
			mobdata.pos.z = roundf(mobdata.pos.z);
		}
	}else{
		z = roundf(mobdata.pos.z + mobdata.knock_back.z - 0.5);
		if (canmove_data[x][z] == true) {
			mobdata.pos.z += mobdata.knock_back.z;
		}else{
			mobdata.pos.z = roundf(mobdata.pos.z);
		}
	}
	mobdata.knock_back_count--;

	return mobdata;
}

void player_knock_back(pos attacker_pos){
	float ver_x,ver_z;
	float angle;

	ver_x = attacker_pos.x - player.pos.x;
	ver_z = attacker_pos.z - player.pos.z;

	angle = atan2f( ver_x , ver_z) / M_PI * 180;

	player.knock_back.x = -1 * sin(angle / 180 * M_PI) * KNOCK_BACK_VALUE;
	player.knock_back.z = -1 * cos(angle / 180 * M_PI) * KNOCK_BACK_VALUE;
	player.knock_back_count = KNOCK_BACK_TIME;

	if(player.stts == block){
		player.knock_back.x *= 0.75;
		player.knock_back.z *= 0.75;
	}
}


void player_knock_back_anim(){
	int x,z;

	x = roundf(player.pos.x);
	z = roundf(player.pos.z);

	if (player.knock_back.x > 0) {
		x = roundf(player.pos.x + player.knock_back.x + 0.5);
		if (canmove_data[x][z] == true) {
			player.pos.x += player.knock_back.x;
		}else{
			player.pos.x = roundf(player.pos.x);
		}
	}else{
		x = roundf(player.pos.x + player.knock_back.x - 0.5);
		if (canmove_data[x][z] == true) {
			player.pos.x += player.knock_back.x;
		}else{
			player.pos.x = roundf(player.pos.x);
		}
	}

	x = roundf(player.pos.x);
	if (player.knock_back.z > 0) {
		z = roundf(player.pos.z + player.knock_back.z + 0.5);
		if (canmove_data[x][z] == true) {
			player.pos.z += player.knock_back.z;
		}else{
			player.pos.z = roundf(player.pos.z);
		}
	}else{
		z = roundf(player.pos.z + player.knock_back.z - 0.5);
		if (canmove_data[x][z] == true) {
			player.pos.z += player.knock_back.z;
		}else{
			player.pos.z = roundf(player.pos.z);
		}
	}

	player.knock_back_count--;

}
