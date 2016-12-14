/*
 * mob.c
 *
 *  Created on: 2015/07/14
 *      Author: vmware
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include "mob.h"
#include "system.h"

#define IDORYOU 0.15 / 1.25

char OBJFile[30][30] = {"mob_anime/mob_000001.obj","mob_anime/mob_000002.obj", "mob_anime/mob_000003.obj", "mob_anime/mob_000004.obj", "mob_anime/mob_000005.obj",
		"mob_anime/mob_000006.obj", "mob_anime/mob_000007.obj", "mob_anime/mob_000008.obj", "mob_anime/mob_000009.obj", "mob_anime/mob_000010.obj",
		"mob_anime/mob_000011.obj", "mob_anime/mob_000012.obj", "mob_anime/mob_000013.obj", "mob_anime/mob_000014.obj", "mob_anime/mob_000015.obj",
		"mob_anime/mob_000016.obj", "mob_anime/mob_000017.obj", "mob_anime/mob_000018.obj", "mob_anime/mob_000019.obj", "mob_anime/mob_000020.obj",
		"mob_anime/mob_000021.obj", "mob_anime/mob_000022.obj", "mob_anime/mob_000023.obj", "mob_anime/mob_000024.obj", "mob_anime/mob_000025.obj",
		"mob_anime/mob_000026.obj", "mob_anime/mob_000027.obj", "mob_anime/mob_000028.obj", "mob_anime/mob_000029.obj", "mob_anime/mob_000030.obj",
};
char SWORD_OBJ[] = "obj_file/Sword.obj";
char taiden_OBJ[] = "obj_file/taiden.obj";

int mob_face_count[30];
int mob_vertex_count;
int mob_uv_count;
int mob_vertexIndex[30][120][4];
int mob_uvIndex[120][4];

int atakk_anim_count = 0;
int atakk_angel = 30;

vec3d mob_vertex[30][80];
vec2d mob_uv[30][80];


int sword_face_count;
int sword_vertex_count;
int sword_uv_count;
int sword_vertexIndex[260][4];
int sword_uvIndex[260][4];

vec3d sword_vertex[260];
vec2d sword_uv[260];

int taiden_face_count;
int taiden_vertex_count;
int taiden_uv_count;
int taiden_vertexIndex[260][4];
int taiden_uvIndex[260][4];

vec3d taiden_vertex[260];
vec2d taiden_uv[260];

int Initmob(){

	mob_status[Zombie].hp = 3;
	mob_status[Zombie].power = 5;
	mob_status[Zombie].speed = 1;

	mob_status[Zombie_villagers].hp = 3;
	mob_status[Zombie_villagers].power = 7;
	mob_status[Zombie_villagers].speed = 1;

	mob_status[Zombi_pigman].hp    = 35;
	mob_status[Zombi_pigman].power = 5;
	mob_status[Zombi_pigman].speed = 1;

//	mob_status[Big_zombie].hp    = 7;
//	mob_status[Big_zombie].power = 9;
//	mob_status[Big_zombie].speed = 0.8;

	mob_status[Big_zombie_pigman].hp    = 7;
	mob_status[Big_zombie_pigman].power = 10;
	mob_status[Big_zombie_pigman].speed = 0.8;

	spon(Zombi_pigman);

	printf("%d\n",mob_data[Zombi_pigman].hp);

	return 0;
}
int i, j;

void readOBJ(){
	FILE* fp;

	/* 読み込み */
	for (i = 0; i < 30; ++i) {
		mob_face_count[i] = 0;
	}



	for (i = 0; i < 30; i++) {
		fp = fopen( OBJFile[i], "r" );
		if( fp == NULL ){
			printf("ファイルを開けません!n");
		}

		mob_vertex_count = 1;
		mob_uv_count = 1;

		while( 1 ){

			char lineHeader[100];
			// 行の最初の文字列を読み込みます。
			int res = fscanf(fp, "%s", lineHeader);

			if (res == EOF){
				break; // EOF = End Of File. ループを終了します。
			}

			if ( strcmp( lineHeader, "v" ) == 0 ){
				fscanf(fp, "%f %f %fn", &mob_vertex[i][mob_vertex_count].x, &mob_vertex[i][mob_vertex_count].y, &mob_vertex[i][mob_vertex_count].z );
				mob_vertex_count++;
			}else if ( strcmp( lineHeader, "vt" ) == 0 ){
				fscanf(fp, "%f %fn", &mob_uv[i][mob_uv_count].x, &mob_uv[i][mob_uv_count].y );
				mob_uv[i][mob_uv_count].y = 1 - mob_uv[i][mob_uv_count].y;
				mob_uv_count++;
				/*
			}else if ( strcmp( lineHeader, "f" ) == 0 ){
				fscanf(fp, "%d/%d %d/%d %d/%d %d/%dn",
						&mob_vertexIndex[mob_face_count][0], &mob_uvIndex[mob_face_count][0],
						&mob_vertexIndex[mob_face_count][1], &mob_uvIndex[mob_face_count][1],
						&mob_vertexIndex[mob_face_count][2], &mob_uvIndex[mob_face_count][2],
						&mob_vertexIndex[mob_face_count][3], &mob_uvIndex[mob_face_count][3]);
				mob_face_count++;
			}
				 */

			}else if ( strcmp( lineHeader, "f" ) == 0 ){
				fscanf(fp, "%d/%d %d/%d %d/%d %d/%dn",
						&mob_vertexIndex[i][mob_face_count[i]][0], &mob_uvIndex[mob_face_count[i]][0],
						&mob_vertexIndex[i][mob_face_count[i]][1], &mob_uvIndex[mob_face_count[i]][1],
						&mob_vertexIndex[i][mob_face_count[i]][2], &mob_uvIndex[mob_face_count[i]][2],
						&mob_vertexIndex[i][mob_face_count[i]][3], &mob_uvIndex[mob_face_count[i]][3]);


				mob_face_count[i]++;
			}
		}
	}

	fclose( fp );
}



void draw_mob(){
	//	int x,z;
	int n;

	for (n = 0; n < 5; ++n) {
		if (mob_data[n].stts == live) {

			glTranslated(mob_data[n].pos.x,0.5,mob_data[n].pos.z);
			glRotatef(mob_data[n].angle,0,1,0);
			glBindTexture(GL_TEXTURE_2D, zombie_pigmankin);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBegin(GL_QUADS);

			int k = flame_count % 30;

			//敵の描画
			for (j = 0; j < mob_face_count[k]; j++) {
				for (i = 0; i < 4; i++) {
					glTexCoord2f(mob_uv[k][mob_uvIndex[j][i]].x, mob_uv[k][mob_uvIndex[j][i]].y);
					glVertex3f(mob_vertex[k][mob_vertexIndex[k][j][i]].x, mob_vertex[k][mob_vertexIndex[k][j][i]].y, mob_vertex[k][mob_vertexIndex[k][j][i]].z);
				}
			}
			glEnd();



			glRotatef(-1 * mob_data[n].angle,0,1,0);
			glTranslated(-1 * mob_data[n].pos.x,-0.5, -1 * mob_data[n].pos.z);

		}else if(mob_data[n].stts == die){
			glTranslated(mob_data[n].pos.x,0.5,mob_data[n].pos.z);
			glRotatef(mob_data[n].angle,0,1,0);

			if (mob_data[n].die_count < 15) {
				glRotatef(mob_data[n].die_count * 5,0,0,1);
			}else if(mob_data[n].die_count <= 40){
				glRotatef(80,0,0,1);
			}else{
				glRotatef(80,0,0,1);
				mob_data[n].stts = died;
			}

			glBindTexture(GL_TEXTURE_2D, zombie_pigmankin);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glBegin(GL_QUADS);
			//敵の描画
			for (j = 0; j < mob_face_count[mob_data[n].die_time]; j++) {
				for (i = 0; i < 4; i++) {
					glTexCoord2f(mob_uv[mob_data[n].die_time][mob_uvIndex[j][i]].x, mob_uv[mob_data[n].die_time][mob_uvIndex[j][i]].y);
					glVertex3f(mob_vertex[mob_data[n].die_time][mob_vertexIndex[mob_data[n].die_time][j][i]].x, mob_vertex[mob_data[n].die_time][mob_vertexIndex[mob_data[n].die_time][j][i]].y, mob_vertex[mob_data[n].die_time][mob_vertexIndex[mob_data[n].die_time][j][i]].z);
				}
			}
			glEnd();

			if (mob_data[n].die_count < 15) {
				glRotatef(mob_data[n].die_count * 5 ,0 ,0,-1);
			}else{
				glRotatef(80 ,0,0,-1);
			}
			glRotatef(-1 * mob_data[n].angle,0,1,0);
			glTranslated(-1 * mob_data[n].pos.x,-0.5, -1 * mob_data[n].pos.z);
			mob_data[n].die_count++;
		}
	}

}

void mob_move(){

	int x, z;
	float ver_x,ver_y;
	pos mob_vec;

	for (i = 0; i < 5; ++i) {
		if (mob_data[i].stts == live) {

			ver_x = player.pos.x - mob_data[i].pos.x;
			ver_y = player.pos.z - mob_data[i].pos.z;
			mob_data[i].angle = atan2f( ver_x , ver_y) / M_PI * 180;

			mob_vec.x = sin(mob_data[i].angle/180 * M_PI);
			mob_vec.z = cos(mob_data[i].angle/180 * M_PI);

			mob_data[i].force.x = 0;
			mob_data[i].force.z = 0;

			mob_data[i].force.x += mob_vec.x * IDORYOU;
			mob_data[i].force.z += mob_vec.z * IDORYOU;

			x = roundf(mob_data[i].pos.x);
			z = roundf(mob_data[i].pos.z);
			canmove_data[x][z] = true;

			if(mob_data[i].knock_back_count > 0){
				mob_data[i] = Knock_back_anim(mob_data[i]);
			}else{
				if (mob_data[i].force.x > 0) {
					x = roundf(mob_data[i].pos.x + mob_data[i].force.x + 0.5);
					if (canmove_data[x][z] == true) {
						mob_data[i].pos.x += mob_data[i].force.x;
					}else if(canmove_data[x][z] == 2){
						mob_data[i].pos.x = roundf(mob_data[i].pos.x);
						if(player.stts == block){
							player.hp -= mob_data[i].power / 2;
						}else{
							player.hp -= mob_data[i].power;
						}

						if(player.hp <= 0){
							player.hp = 0;
							layer = 20;
							return;
						}
						printf("player Hit %d\n",player.hp);
						player_knock_back(mob_data[i].pos);
					}else{
						mob_data[i].pos.x = roundf(mob_data[i].pos.x);
					}
				}else{
					x = roundf(mob_data[i].pos.x + mob_data[i].force.x - 0.5);
					if (canmove_data[x][z] == true) {
						mob_data[i].pos.x += mob_data[i].force.x;
					}else if(canmove_data[x][z] == 2){
						mob_data[i].pos.x = roundf(mob_data[i].pos.x);
						if(player.stts == block){
							player.hp -= mob_data[i].power / 2;
						}else{
							player.hp -= mob_data[i].power;
						}
						if(player.hp <= 0){
							player.hp = 0;
							layer = 20;
							return;
						}
						printf("player Hit %d\n",player.hp);
						player_knock_back(mob_data[i].pos);
					}else{
						mob_data[i].pos.x = roundf(mob_data[i].pos.x);
					}
				}

				x = roundf(mob_data[i].pos.x);
				if (mob_data[i].force.z > 0) {
					z = roundf(mob_data[i].pos.z + mob_data[i].force.z + 0.5);
					if (canmove_data[x][z] == true) {
						mob_data[i].pos.z += mob_data[i].force.z;
					}else if(canmove_data[x][z] == 2){
						mob_data[i].pos.z = roundf(mob_data[i].pos.z);
						if(player.stts == block){
							player.hp -= mob_data[i].power / 2;
						}else{
							player.hp -= mob_data[i].power;
						}
						if(player.hp <= 0){
							player.hp = 0;
							layer = 20;
							return;
						}
						printf("player Hit %d\n",player.hp);
						player_knock_back(mob_data[i].pos);
					}else{
						mob_data[i].pos.z = roundf(mob_data[i].pos.z);
					}
				}else{
					z = roundf(mob_data[i].pos.z + mob_data[i].force.z - 0.5);
					if (canmove_data[x][z] == true) {
						mob_data[i].pos.z += mob_data[i].force.z;
					}else if(canmove_data[x][z] == 2){
						mob_data[i].pos.z = roundf(mob_data[i].pos.z);
						if(player.stts == block){
							player.hp -= mob_data[i].power / 2;
						}else{
							player.hp -= mob_data[i].power;
						}
						if(player.hp <= 0){
							player.hp = 0;
							layer = 20;
							return;
						}
						printf("player Hit %d\n",player.hp);
						player_knock_back(mob_data[i].pos);
					}else{
						mob_data[i].pos.z = roundf(mob_data[i].pos.z);
					}
				}
			}


//			printf("%d",canmove_data[x][z]);
			x = roundf(mob_data[i].pos.x);
			z = roundf(mob_data[i].pos.z);
			canmove_data[x][z] = 3 + i;
		}
	}
}


void read_sword_obj(){

	sword_face_count = 0;

	FILE* fp;
	fp = fopen( SWORD_OBJ, "r" );
	if( fp == NULL ){
		printf("ファイルを開けません!n");
	}

	sword_uv_count = 1;
	sword_vertex_count = 1;

	while( 1 ){
		char lineHeader[100];
		// 行の最初の文字列を読み込みます。
		int res = fscanf(fp, "%s", lineHeader);

		if (res == EOF){
			break; // EOF = End Of File. ループを終了します。
		}

		if ( strcmp( lineHeader, "v" ) == 0 ){
			fscanf(fp, "%f %f %fn", &sword_vertex[sword_vertex_count].x, &sword_vertex[sword_vertex_count].y, &sword_vertex[sword_vertex_count].z );
			sword_vertex_count++;
			//			printf("%f\n",sword_vertex[sword_vertex_count].x);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			fscanf(fp, "%f %fn", &sword_uv[sword_uv_count].x, &sword_uv[sword_uv_count].y );
			sword_uv[sword_uv_count].y = 1 - sword_uv[sword_uv_count].y;
			sword_uv_count++;
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			fscanf(fp, "%d/%d %d/%d %d/%d %d/%dn",
					&sword_vertexIndex[sword_face_count][0], &sword_uvIndex[sword_face_count][0],
					&sword_vertexIndex[sword_face_count][1], &sword_uvIndex[sword_face_count][1],
					&sword_vertexIndex[sword_face_count][2], &sword_uvIndex[sword_face_count][2],
					&sword_vertexIndex[sword_face_count][3], &sword_uvIndex[sword_face_count][3]);
			sword_face_count++;
		}
	}
	fclose( fp );
}

void draw_sword(){

	sword_pos.x = player.pos.x + sin(player.angle.y/180 * M_PI) * 0.6;
	sword_pos.z = player.pos.z + cos(player.angle.y/180 * M_PI) * 0.6;

	glTranslated(sword_pos.x,2.5,sword_pos.z);
	glRotatef(player.angle.y, 0, 1, 0);
	glRotatef(-45, 0, 1, 0);

	if(joystick_stta[3] == true){//攻撃のアニメーション
		player.stts = attack;
	}

	if (player.stts == attack) {
		if(atakk_anim_count <= 6){
			atakk_angel = (6 - atakk_anim_count) * 5;
			atakk_anim_count ++;
		}else if(atakk_anim_count < 12){
			atakk_angel = (atakk_anim_count - 6) * 5;
			atakk_anim_count ++;
		}else{
			atakk_anim_count = 0;
			player.stts = nasi;
			atakk_angel = 30;
		}
	}

	if (joystick_stta[2] == true && player.stts != attack) {
		player.stts = block;
		glRotatef(45, 0, 1, 0);
		glTranslated( 0.2,-0.1,-0.25);
		glRotatef(45, 0, 0, -1);
	}else{

	}
	glRotatef(atakk_angel, 0, -0.5, 1);
	glBindTexture(GL_TEXTURE_2D, sword);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_QUADS);

	for (j = 0; j < sword_face_count; j++) {
		for (i = 0; i < 4; i++) {
			glTexCoord2f(sword_uv[sword_uvIndex[j][i]].x, sword_uv[sword_uvIndex[j][i]].y);
			glVertex3f(sword_vertex[sword_vertexIndex[j][i]].x * 0.2 - 0.5, sword_vertex[sword_vertexIndex[j][i]].y * 0.2, sword_vertex[sword_vertexIndex[j][i]].z * 0.2);
		}
	}
	glEnd();

	glRotatef( -1 * atakk_angel, 0, -0.5, 1);



	//	if (atakk_anim == true) {
	//		glRotatef(-2 * atakk_angel, 0, -0.5, 1);
	//	}else{
	//		glRotatef(-30, 0, 0, 1);
	//	}
	if (player.stts == block) {
		glRotatef(-45, 0, 0, -1);
		glTranslated( -0.2,0.1,0.25);
		glRotatef(-45, 0, 1, 0);
	}else{

	}
	glRotatef(45, 0, 1, 0);
	glRotatef(-1 * player.angle.y, 0, 1, 0);
	glTranslated(-1 * sword_pos.x,-2.5,-1 * sword_pos.z);

}

void read_taiden_obj(){

	taiden_face_count = 0;

	FILE* fp;
	fp = fopen( taiden_OBJ, "r" );
	if( fp == NULL ){
		printf("ファイルを開けません!n");
	}

	taiden_uv_count = 1;
	taiden_vertex_count = 1;

	while( 1 ){
		char lineHeader[100];
		// 行の最初の文字列を読み込みます。
		int res = fscanf(fp, "%s", lineHeader);

		if (res == EOF){
			break; // EOF = End Of File. ループを終了します。
		}

		if ( strcmp( lineHeader, "v" ) == 0 ){
			fscanf(fp, "%f %f %fn", &taiden_vertex[taiden_vertex_count].x, &taiden_vertex[taiden_vertex_count].y, &taiden_vertex[taiden_vertex_count].z );
			taiden_vertex_count++;
			//			printf("%f\n",taiden_vertex[taiden_vertex_count].x);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			fscanf(fp, "%f %fn", &taiden_uv[taiden_uv_count].x, &taiden_uv[taiden_uv_count].y );
			taiden_uv[taiden_uv_count].y = 1 - taiden_uv[taiden_uv_count].y;
			taiden_uv_count++;
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			fscanf(fp, "%d/%d %d/%d %d/%d %d/%dn",
					&taiden_vertexIndex[taiden_face_count][0], &taiden_uvIndex[taiden_face_count][0],
					&taiden_vertexIndex[taiden_face_count][1], &taiden_uvIndex[taiden_face_count][1],
					&taiden_vertexIndex[taiden_face_count][2], &taiden_uvIndex[taiden_face_count][2],
					&taiden_vertexIndex[taiden_face_count][3], &taiden_uvIndex[taiden_face_count][3]);
			taiden_face_count++;
		}
	}
	fclose( fp );
}

float anime_x = 0;

void draw_taiden(){

	glTranslated(mob_data[Zombi_pigman].pos.x,0.5,mob_data[Zombi_pigman].pos.z);
	glRotatef(mob_data[Zombi_pigman].angle,0,1,0);
	glBindTexture(GL_TEXTURE_2D, taidenn);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_QUADS);

	anime_x = anime_x + 0.002;
	if(anime_x >= 0.5)
		anime_x = 0;
	for (j = 0; j < taiden_face_count; j++) {
		for (i = 0; i < 4; i++) {
			glTexCoord2f(taiden_uv[taiden_uvIndex[j][i]].x / 2 + anime_x , taiden_uv[taiden_uvIndex[j][i]].y / 2 + anime_x);
			glVertex3f(taiden_vertex[taiden_vertexIndex[j][i]].x, taiden_vertex[taiden_vertexIndex[j][i]].y, taiden_vertex[taiden_vertexIndex[j][i]].z);
		}
	}
	glEnd();

	glRotatef(-1 * mob_data[Zombi_pigman].angle,0,1,0);
	glTranslated(-1 * mob_data[Zombi_pigman].pos.x,-0.5, -1 * mob_data[Zombi_pigman].pos.z);


}
