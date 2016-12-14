/*
 * window.h
 *
 *  Created on: 2015/06/26
 *      Author: c501406006
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "character.h"



enum {
    Display_wide    = 800,
    Display_Height   = 600,
};

pos player_vec;

//ライト
	GLfloat light0pos[4];	//ライトの位置


int InitWindows(void);
void display();
int WindowEvent(void);
void Fog();
void move();
pos GetPlayerPos();
void draw_haikei();
void draw2D(int width, int height);
//void DRAW_STRING(int x, int y, char *string, void *font);
void DrawString(const char *str,void *font,float x,float y,float z);
void set2D();
void set3D();
void moji(int width, int height);
void mozi_hp(int width, int height);




#endif /* WINDOW_H_ */
