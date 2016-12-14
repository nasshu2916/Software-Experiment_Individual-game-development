/*
 * map.h
 *
 *  Created on: 2015/07/02
 *      Author: vmware
 */

#ifndef MAP_H_
#define MAP_H_


void ground();
void drawPlate();
//void DRAW_SQU();



enum {
    MAP_Width    = 51,
    MAP_Height   = 51,
    MAP_Center_X = 25,
    MAP_Center_Y = 25,
    MAP_ChipSize = 48,
    MAP_Y = 9
};
/* マップの種類
   (負数のものはキャラクターが移動できない場所) */
typedef enum {
	air = -1,
	grass = 0,
	stone = 1,
	stone2 = 2,
	sand = 3,
	glass = 4,
	wood_plank = 5,
	wood = 6,
	leaves = 7
} MapType;

float vertices[6][4][3];




#endif /* MAP_H_ */
