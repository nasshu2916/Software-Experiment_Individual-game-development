/*
 * map.c
 *
 *  Created on: 2015/07/02
 *      Author: vmware
 */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_opengl.h>
#include "map.h"
#include "system.h"
#include "voxel.h"

float vertices[6][4][3] = {
		// 立方体の頂点データ
		// 正面
		{{-0.5f, 0.5f, -0.5f},
				{-0.5f, -0.5f, -0.5f},
				{0.5f, -0.5f, -0.5f},
				{0.5f, 0.5f, -0.5f}},

				// 右側面
				{{0.5f, 0.5f, -0.5f},
						{0.5f, -0.5f, -0.5f},
						{0.5f, -0.5f, 0.5f},
						{0.5f, 0.5f, 0.5f}},

						// 裏面
						{{0.5f, 0.5f, 0.5f},
								{0.5f, -0.5f, 0.5f},
								{-0.5f, -0.5f, 0.5f},
								{-0.5f, 0.5f, 0.5f}},

								// 左側面
								{{-0.5f, 0.5f, 0.5f},
										{-0.5f, -0.5f, 0.5f},
										{-0.5f, -0.5f, -0.5f},
										{-0.5f, 0.5f, -0.5f}},

										// 上面
										{{-0.5f, 0.5f, 0.5f},
												{-0.5f, 0.5f, -0.5f},
												{0.5f, 0.5f, -0.5f},
												{0.5f, 0.5f, 0.5f}},

												// 底面
												{{-0.5f, -0.5f, -0.5f},
														{-0.5f, -0.5f, 0.5f},
														{0.5f, -0.5f, 0.5f},
														{0.5f, -0.5f, -0.5f}}
};

int u = 12;
int v = 12;
int i,j;

void ground(){

//	glNormal3d(0.0, 1.0, 0.0);//色指定

	for ( j = 0; j < MAP_Height; j++) {
		for ( i = 0; i < MAP_Width; i++) {
			//			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
//			if ((i + j) % 2 == 0) {
//				glColor3d(0.2, 0.2, 0.2);
//			} else {
//				glColor3d(1.0, 1.0, 1.0); // 各面の頂点に色を設定
//			}
//			glTexCoord2f(0.0, 0.0);
//			glVertex3f((GLfloat)i*(u/12.0), 0.0, (GLfloat)j*(v/12.0));
//			glTexCoord2f(0.0, 1.0);
//			glVertex3f((GLfloat)i*(u/12.0), 0.0, (GLfloat)(j + 1)*(v/12.0));
//			glTexCoord2f(1.0, 1.0);
//			glVertex3f((GLfloat)(i + 1)*(u/12.0), 0.0, (GLfloat)(j + 1)*(v/12.0));
//			glTexCoord2f(1.0, 0.0);
//			glVertex3f((GLfloat)(i + 1)*(u/12.0), 0.0, (GLfloat)j*(v/12.0));
			DRAW_CUBE(i, 0, j, grass);
		}
	}
//	DRAW_CUBE(20, 0, 23, 1);


}

void drawPlate(){
	glClearColor(1.0, 0.0, 1.0, 1.0);	// 塗り潰し色を白色に設定
	//	glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし

	glColor3d(1.0, 1.0, 1.0); // 各面の頂点に色を設定

	glBegin(GL_QUADS);	// 四角形の開始
	// 以下の四角形に（管理番号が指す）テクスチャを描画する

	glEnd(); // 図形終了
}

