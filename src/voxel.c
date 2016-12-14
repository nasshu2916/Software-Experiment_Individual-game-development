/*
 * voxel.c
 *
 *  Created on: 2015/07/13
 *      Author: vmware
 */
#include <GL/glut.h>
#include "../glpng.h"
#include "system.h"
#include "voxel.h"

float cube[6][4][3] = {
		// 立方体の頂点データ
		// 正面
		{{-0.51f, 0.5f, -0.51f},
				{-0.51f, -0.51f, -0.51f},
				{0.5f, -0.51f, -0.51f},
				{0.5f, 0.5f, -0.51f}},

				// 右側面
				{{0.5f, 0.5f, -0.51f},
						{0.5f, -0.51f, -0.51f},
						{0.5f, -0.51f, 0.5f},
						{0.5f, 0.5f, 0.5f}},

						// 裏面
						{{0.5f, 0.5f, 0.5f},
								{0.5f, -0.51f, 0.5f},
								{-0.51f, -0.51f, 0.5f},
								{-0.51f, 0.5f, 0.5f}},

								// 左側面
								{{-0.51f, 0.5f, 0.5f},
										{-0.51f, -0.51f, 0.5f},
										{-0.51f, -0.51f, -0.51f},
										{-0.51f, 0.5f, -0.51f}},

										// 上面
										{{-0.51f, 0.5f, 0.5f},
												{-0.51f, 0.5f, -0.51f},
												{0.5f, 0.5f, -0.51f},
												{0.5f, 0.5f, 0.5f}},

												// 底面
												{{-0.51f, -0.51f, -0.51f},
														{-0.51f, -0.51f, 0.5f},
														{0.5f, -0.51f, 0.5f},
														{0.5f, -0.51f, -0.51f}}
};

GLdouble normal[][3] = {
		{ 0.0, 0.0,-1.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{-1.0, 0.0, 0.0 },
		{ 0.0,-1.0, 0.0 },
		{ 0.0, 1.0, 0.0 }
};

int i,j;

void DRAW_SQU()
{
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1,1);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1,-1);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(1,-1);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(1,1);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void DRAW_SQU3(int x, int y, int z){
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4d(0.5,0.5,0.5,0.5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5 + x, 0.5 + y, 0.5 + z);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5 + x, -0.5 + y, 0.5 + z);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5 + x, -0.5 + y, 0.5 + z);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5 + x, 0.5 + y, 0.5 + z);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void DRAW_CUBE3(int x, int y, int z){
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor4d(0.5,0.5,0.5,0.5);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++){
			switch (j) {
			case 0:
				glTexCoord2f(0.0, 0.0);
				break;
			case 1:
				glTexCoord2f(0.0, 1.0);
				break;
			case 2:
				glTexCoord2f(1.0, 1.0);
				break;
			case 3:
				glTexCoord2f(1.0, 0.0);
				break;

			}
			glVertex3f(cube[i][j][0] + x, cube[i][j][1] + y, cube[i][j][2] + z);
		}
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void DRAW_CUBE(int x, int y, int z ,MapType id){
	glBindTexture(GL_TEXTURE_2D, texture[id]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBegin(GL_QUADS);
	for (i = 0; i < 6; i++) {
		//		glNormal3dv(normal[j]);
		for (j = 0; j < 4; j++){
			switch (j) {
			case 0:
				glTexCoord2f(0.0, 0.0);
				break;
			case 1:
				glTexCoord2f(0.0, 1.0);
				break;
			case 2:
				glTexCoord2f(1.0, 1.0);
				break;
			case 3:
				glTexCoord2f(1.0, 0.0);
				break;
			}
			glVertex3f(cube[i][j][0] + x, cube[i][j][1] + y, cube[i][j][2] + z);
		}
	}

	glEnd();

}
