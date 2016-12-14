/*
 * window.c
 *
 *  Created on: 2015/06/26
 *      Author: c501406006
 */
#include <GL/glut.h>
#include <math.h>
#include <GL/freeglut.h>
#include "window.h"
#include "character.h"
#include "map.h"
#include "system.h"

#define IDORYOU 0.15	//移動時の移動量.
#define CAMRA 10	//回転量(単位は度)

#define KEY_A 1		// = 10000000 …
#define KEY_D 2		// = 01000000 …
#define KEY_W 4		// = 00100000 …
#define KEY_S 8
#define KEY_SPACE 16


int InitWindows(void){

	//	//ライトの位置セット
	//	light0pos[0] = 1.0;
	//	light0pos[1] = 10.0;
	//	light0pos[2] = 5.0;
	//	light0pos[3] = 1.0;


	player_vec.x=0.0; player_vec.y=0.0; player_vec.z=1.0;	//前方向ベクトル


	return 0;
}


void UpdateDisplay(){

}

/* メインウインドウに対するイベント処理
 *
 * 返値
 *   継続: 1
 *   終了: 0
 */
int WindowEvent(void){

	return 1;
}

void Fog(){
	//フォグの作成
	float fogColor[4]= {0.9, 1.0, 1.0, 1.0}; //フォグの色

	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor); //色
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_START, 10 );
	glFogi(GL_FOG_END, 60 );
}

void move(){

	player.force.x = 0.0;
	player.force.y = 0.0;
	player.force.z = 0.0;

	if( key_on & KEY_W){
		player.force.x += player_vec.x * IDORYOU;
		player.force.y += player_vec.y * IDORYOU;
		player.force.z += player_vec.z * IDORYOU;
	}else if(joystick_anarogu_stta[1] <= -50){
		player.force.x += player_vec.x * IDORYOU * joystick_anarogu_stta[1] / -32768;
		player.force.y += player_vec.y * IDORYOU * joystick_anarogu_stta[1] / -32768;
		player.force.z += player_vec.z * IDORYOU * joystick_anarogu_stta[1] / -32768;
	}

	if( key_on & KEY_S){
		player.force.x -= player_vec.x *IDORYOU / 1.20 ;
		player.force.y -= player_vec.y *IDORYOU / 1.20;
		player.force.z -= player_vec.z *IDORYOU / 1.20;
	}else if( joystick_anarogu_stta[1] >= 50){
		player.force.x -= player_vec.x *IDORYOU / 1.20 * joystick_anarogu_stta[1] / 32768;
		player.force.y -= player_vec.y *IDORYOU / 1.20 * joystick_anarogu_stta[1] / 32768;
		player.force.z -= player_vec.z *IDORYOU / 1.20 * joystick_anarogu_stta[1] / 32768;
	}

	if( key_left == true){	//左回転
		player.angle.y += CAMRA;	//回転
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}else if(joystick_anarogu_stta[3] <= -50){	//左回転
		player.angle.y += CAMRA / 2 * joystick_anarogu_stta[3] / -32768;	//回転
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}

	if(  key_right == true){	//右回転
		player.angle.y -= CAMRA;	//回転
		player_vec.x = sin(player.angle.y/180 * M_PI);
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}else if( joystick_anarogu_stta[3] >= 50){	//右回転
		player.angle.y -= CAMRA / 2 * joystick_anarogu_stta[3] / 32768;	//回転
		player_vec.x = sin(player.angle.y/180 * M_PI);
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}

	if( key_on & KEY_A){	//右回転
		player.angle.y += 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
		player.force.x += player_vec.x * IDORYOU / 1.3;
		player.force.y += player_vec.y * IDORYOU / 1.3;
		player.force.z += player_vec.z * IDORYOU / 1.3;
		player.angle.y -= 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}else if(joystick_anarogu_stta[0] <= -50){	//右回転
		player.angle.y += 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
		player.force.x += player_vec.x * IDORYOU / 1.3 * joystick_anarogu_stta[0] / -32768;
		player.force.y += player_vec.y * IDORYOU / 1.3 * joystick_anarogu_stta[0] / -32768;
		player.force.z += player_vec.z * IDORYOU / 1.3 * joystick_anarogu_stta[0] / -32768;
		player.angle.y -= 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}

	if( key_on & KEY_D){	//右回転
		player.angle.y -= 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
		player.force.x += player_vec.x * IDORYOU / 1.3;
		player.force.y += player_vec.y * IDORYOU / 1.3;
		player.force.z += player_vec.z * IDORYOU / 1.3;
		player.angle.y += 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}else if( joystick_anarogu_stta[0] >= 50){	//右回転
		player.angle.y -= 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
		player.force.x += player_vec.x * IDORYOU / 1.3 * joystick_anarogu_stta[0] / 32768;
		player.force.y += player_vec.y * IDORYOU / 1.3 * joystick_anarogu_stta[0] / 32768;
		player.force.z += player_vec.z * IDORYOU / 1.3 * joystick_anarogu_stta[0] / 32768;
		player.angle.y += 90;
		player_vec.x = sin(player.angle.y/180 * M_PI);	//前方向ベクトルを修正「/180*M_PI」で度→radへ変換
		player_vec.z = cos(player.angle.y/180 * M_PI);
	}
	if( key_up == true){	//
		if(player.angle.x < 90)
			player.angle.x += CAMRA / 2;	//回転
	}else if( joystick_anarogu_stta[4] <= -50){	//
		if(player.angle.x < 90)
			player.angle.x += CAMRA / 2 * joystick_anarogu_stta[4] / -32768;	//回転
	}

	if( key_down == true){	//
		if(player.angle.x > -90)
			player.angle.x -= CAMRA / 2;	//回転
	}else if( joystick_anarogu_stta[4] >= 50){	//
		if(player.angle.x > -90)
			player.angle.x -= CAMRA / 2 * joystick_anarogu_stta[4] / 32768;	//回転
	}

	int x,z;

	x = roundf(player.pos.x);
	z = roundf(player.pos.z);
	canmove_data[x][z] = true;

	if(player.knock_back_count > 0){
		player_knock_back_anim();
	}{
		if (player.force.x > 0) {
			x = roundf(player.pos.x + player.force.x + 0.5);
			if (canmove_data[x][z] == true) {
				player.pos.x += player.force.x;
			}else{
				player.pos.x = roundf(player.pos.x);
			}
		}else{
			x = roundf(player.pos.x + player.force.x - 0.5);
			if (canmove_data[x][z] == true) {
				player.pos.x += player.force.x;
			}else{
				player.pos.x = roundf(player.pos.x);
			}
		}

		x = roundf(player.pos.x);
		if (player.force.z > 0) {
			z = roundf(player.pos.z + player.force.z + 0.5);
			if (canmove_data[x][z] == true) {
				player.pos.z += player.force.z;
			}else{
				player.pos.z = roundf(player.pos.z);
			}
		}else{
			z = roundf(player.pos.z + player.force.z - 0.5);
			if (canmove_data[x][z] == true) {
				player.pos.z += player.force.z;
			}else{
				player.pos.z = roundf(player.pos.z);
			}
		}
	}
	player.pos.y += player.force.y;

	x = roundf(player.pos.x);
	z = roundf(player.pos.z);
	canmove_data[x][z] = 2;

}

pos GetPlayerPos(){
	pos campos;

	campos.x = player.pos.x;
	campos.y = player.pos.y;
	campos.z = player.pos.z;
	return campos;
}


void draw_haikei(){
	glBindTexture(GL_TEXTURE_2D, haikei);

	glBegin(GL_QUADS);

	glTexCoord2f(0 , 1);
	glVertex3f(-20, 5 , -20);
	glTexCoord2f(1 , 1);
	glVertex3f(70, 5 , -20);
	glTexCoord2f(1 , 0);
	glVertex3f(70, 50 , -20);
	glTexCoord2f(0 , 0);
	glVertex3f(-20, 50 , -20);

	glTexCoord2f(0 , 1);
	glVertex3f(-20, 5 , -20);
	glTexCoord2f(1 , 1);
	glVertex3f(-20, 5 , 70);
	glTexCoord2f(1 , 0);
	glVertex3f(-20, 50 , 70);
	glTexCoord2f(0 , 0);
	glVertex3f(-20, 50 , -20);

	glTexCoord2f(0 , 1);
	glVertex3f(70, 5 , 70);
	glTexCoord2f(1 , 1);
	glVertex3f(-20, 5 , 70);
	glTexCoord2f(1 , 0);
	glVertex3f(-20, 50 , 70);
	glTexCoord2f(0 , 0);
	glVertex3f(70, 50 , 70);

	glTexCoord2f(0 , 1);
	glVertex3f(70, 5 , 70);
	glTexCoord2f(1 , 1);
	glVertex3f(70, 5 , -20);
	glTexCoord2f(1 , 0);
	glVertex3f(70, 50 , -20);
	glTexCoord2f(0 , 0);
	glVertex3f(70, 50 , 70);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, sora);
	glBegin(GL_QUADS);
	glTexCoord2f(0 , 1);
	glVertex3f(-20, 50 , -20);
	glTexCoord2f(1 , 1);
	glVertex3f(70, 50 , -20);
	glTexCoord2f(1 , 0);
	glVertex3f(70, 50 , 70);
	glTexCoord2f(0 , 0);
	glVertex3f(-20, 50 , 70);

	glEnd();

}

void draw2D(int width, int height){

	//	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	//	gluOrtho2D(0, WindowWidth, WindowHeight, 0); //ビューイング領域を設定(正射影で設定)
	//	DrawString("Hello",GLUT_BITMAP_HELVETICA_18,0,0,0);


	if(player.hp < 0){
		player.hp = 0;
		layer = 20;
	}


	if(player.hp < 25){
		glColor4f(1.0, 0.2, 0.2, 1.0);
	}else if (player.hp < 50) {
		glColor4f(1.0, 1.0, 0.2, 0.8);
	}else {
		glColor4f(0.2, 1.0, 0.2, 0.8);
	}

	glBegin(GL_QUADS);
	glVertex2f( width / 2 - 400, height - 150);
	glVertex2f( width / 2 - 400 + player.hp * 8 ,height - 150);
	glVertex2f( width / 2 - 400 + player.hp * 8 , height - 100);
	glVertex2f( width / 2 - 400, height - 100);

	glColor4f(1.0, 1.0, 1.0, 0.8);
	glVertex2f( width / 2 - 400, height - 150);
	glVertex2f( width / 2 + 400, height - 150);
	glVertex2f( width / 2 + 400, height - 100);
	glVertex2f( width / 2 - 400, height - 100);

	glColor4f(0.5, 0.4, 0.2, 0.5);
	glVertex2f( width / 2 - 410, height - 160);
	glVertex2f( width / 2 + 410, height - 160);
	glVertex2f( width / 2 + 410, height - 90);
	glVertex2f( width / 2 - 410, height - 90);
	glEnd();

	if(mob_data[Zombi_pigman].hp < 0){
		mob_data[Zombi_pigman].hp = 0;
		layer = 21;
	}

	glColor4f(1.0, 0.2, 0.2, 1.0);

	glBegin(GL_QUADS);


	if (mob_data[Zombi_pigman].hp >= 20) {
		glVertex2f( width / 2 - 400, 100);
		glVertex2f( width / 2 + 400 ,100);
		glVertex2f( width / 2 + 400 , 150);
		glVertex2f( width / 2 - 400, 150);
	}else{
		glVertex2f( width / 2 - 400, 100);
		glVertex2f( width / 2 - 400 + mob_data[Zombi_pigman].hp * 4 * 8 ,100);
		glVertex2f( width / 2 - 400 + mob_data[Zombi_pigman].hp * 4 * 8 , 150);
		glVertex2f( width / 2 - 400, 150);
	}

	glColor4f(1.0, 1.0, 1.0, 0.8);
	glVertex2f( width / 2 - 400, 150);
	glVertex2f( width / 2 + 400, 150);
	glVertex2f( width / 2 + 400, 100);
	glVertex2f( width / 2 - 400, 100);
//
	glColor4f(0.5, 0.4, 0.2, 0.5);
	glVertex2f( width / 2 - 410, 160);
	glVertex2f( width / 2 + 410, 160);
	glVertex2f( width / 2 + 410, 90);
	glVertex2f( width / 2 - 410, 90);
	glEnd();



}

void moji(int width, int height){
	glBindTexture(GL_TEXTURE_2D, HP);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	mozi_hp(width,height);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void mozi_hp(int width, int height){
	glTexCoord2f(1, 0);
	glVertex2f( width / 2 - 410, height - 150);
	glTexCoord2f(0, 0);
	glVertex2f( width / 2 - 460, height - 150);
	glTexCoord2f(0, 1);
	glVertex2f( width / 2  - 460, height - 100);
	glTexCoord2f(1, 1);
	glVertex2f( width / 2 - 410, height - 100);
}

void DrawString(const char *str,void *font,float x,float y,float z)
{
	//	glRasterPos2f(0,0);
	//	while(*str){
	//		glutBitmapCharacter(font, *str);
	//		++str;
	//	}
	//	glutBitmapCharacter(font, str[0]);
}

void set2D(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); //PROJECTION行列を保存
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0); //ビューイング領域を設定(正射影で設定)
	glMatrixMode(GL_MODELVIEW);
}

void set3D(){
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();// 射影変換行列を復元
	glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
	glPopMatrix();// モデルビュー行列を復元
	glLoadIdentity();// 単位行列を設定
}
