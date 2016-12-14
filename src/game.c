#include <stdio.h>
#include <SDL/SDL.h>        // SDLを用いるために必要なヘッダファイルをインクルード
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include "../glpng.h"
#include "system.h"
#include "window.h"
#include "character.h"
#include "map.h"
#include "voxel.h"
#include "mob.h"
#include "vr920.h"

//後ろでインクルード

#define BLOK	12
#define TIMER_WAIT 33	//タイマーの待ち時間(画面の書き換え間隔)

//----------変数宣言 --------------------//

int WindowWidth = 1280;
int WindowHeight = 720;



int vr920 = true;

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };

//---------- プロトタイプ宣言 --------------------//
void display();
void reshape(int w, int h);
void timer(int value);
void timer2(int value);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void Close();
int PrintError( const char *str );




//-------- 画像を読み込んでテクスチャに ------------//
int LoadGLTextures()
{
	pngInfo info;
	texture[grass] = pngBind("texture/grass.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	if (texture[0] == 0) //「0」の場合失敗
	{
		return false;
	}
	texture[stone] = pngBind("texture/stone.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	texture[stone2] = pngBind("texture/stone2.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	texture[sand] = pngBind("texture/Sand.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	texture[glass] = pngBind("texture/Glass.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	texture[leaves] = pngBind("texture/Leaves.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	texture[wood] = pngBind("texture/Wood.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	texture[wood_plank] = pngBind("texture/Wood_Plank.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	zombie_pigmankin = pngBind("texture/zombie_pigman.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	zombie = pngBind("texture/zombie.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	zombie_villagers = pngBind("texture/zombie_villagers.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	haikei = pngBind("texture/haikei.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	sora = pngBind("texture/sora.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	sword = pngBind("texture/Iron_Sword.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	taidenn = pngBind("texture/taiden.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	HP = pngBind("moji/HP.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	start_gamenn  = pngBind("texture/start_gamen.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	game_setumei  = pngBind("texture/game_setumei.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	game_over  = pngBind("texture/game_over.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	clear = pngBind("texture/Clear.png", PNG_BUILDMIPMAPS, PNG_ALPHA, &info, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	return true;
}

//---------- OpenGLの初期設定 -------------//
void GLUT_INIT(){
	glutInitWindowPosition(50, 50);//ウィンドウの位置
	glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutCreateWindow("VOXEL ACTION");
	glutCloseFunc(Close);
	//	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);	//キー入力
	glutKeyboardUpFunc(keyboardUp);	//キーが離された
}

void GLUT_CALL_FUNC()
{
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	flame_count = 0;
	glutTimerFunc(TIMER_WAIT,timer,flame_count);
	glutTimerFunc(TIMER_WAIT / 2,timer2,0);
}


void MY_INIT()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	if( LoadGLTextures() == false){  //テクスチャの読み込み
		//                std::cerr << "Can't Load Image\n";
	}

	glEnable(GL_DEPTH_TEST);

	//	glEnable(GL_LIGHT1);
}

void INIT_key(){
	key_up = false;
	key_down = false;
	key_left = false;
	key_right = false;
}


//------------ メイン関数 ---------------//
int main(int argc, char **argv){
	//初期化処理

	// SDL初期化（ビデオとジョイスティック）
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* ゲームシステム */
	if( InitSystem() < 0 ){
		return PrintError( SDL_GetError() );
	}

	/* ゲームシステム */
	if( InitWindows() < 0 ){
		return PrintError( SDL_GetError() );
	}

	/* キャラクタ */
	if( InitCharactar() < 0 ){
		return PrintError( SDL_GetError() );
	}
	if( Initmob() < 0 ){
		return PrintError( SDL_GetError() );
	}

	if( Init_hmd() < 0 ){
		vr920 = false;
	}

	INIT_key();

	yaw = 0;
	roll = 0;
	pitch = 0;

	layer = 0;

	joystick = SDL_JoystickOpen(0);	// ジョイスティックを開いて構造体に割り当てる（有効にする）．ここではジョイスティックは1つとして引数に0を指定
	SDL_JoystickEventState(SDL_ENABLE);	// ジョイスティック関連のイベントを取得可能にする

	readOBJ();
	read_sword_obj();
	read_taiden_obj();

	glutInit(&argc,argv);

	GLUT_INIT();
	GLUT_CALL_FUNC();
	MY_INIT();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glutMainLoop();

	return 0;
}


//----------- ここからコールバック ---------------//
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	//	glLoadIdentity();
	pos camerapos;
	int i, j, a;

	switch (layer) {
	case 0:

		gluLookAt(camerapos.x, 2.75,camerapos.z,
				sin((player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.x, pitch * 0.5 + player.angle.x,cos( (player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.z,
				0.0,1.0,0.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //MODELVIEW行列を保存
		glLoadIdentity();
		set2D();
		glBindTexture(GL_TEXTURE_2D, start_gamenn);
		glColor4f(1, 1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glTexCoord2f(0, 0);
		glVertex2f( 0, 0);
		glTexCoord2f(0, 1);
		glVertex2f( 0, WindowHeight);
		glTexCoord2f(1, 1);
		glVertex2f( WindowWidth, WindowHeight);
		glTexCoord2f(1, 0);
		glVertex2f( WindowWidth, 0);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		set3D();

		if(joystick_stta[3] == true){
			joystick_stta[3] = false;
			layer = 10;
		}

		if(joystick_stta[2] == true){
			joystick_stta[2] = false;
			layer = 1;
		}
		break;

	case 1:
		gluLookAt(camerapos.x, 2.75,camerapos.z,
				sin((player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.x, pitch * 0.5 + player.angle.x,cos( (player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.z,
				0.0,1.0,0.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //MODELVIEW行列を保存
		glLoadIdentity();
		set2D();
		glBindTexture(GL_TEXTURE_2D, game_setumei);
		glColor4f(1, 1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glTexCoord2f(0, 0);
		glVertex2f( 0, 0);
		glTexCoord2f(0, 1);
		glVertex2f( 0, WindowHeight);
		glTexCoord2f(1, 1);
		glVertex2f( WindowWidth, WindowHeight);
		glTexCoord2f(1, 0);
		glVertex2f( WindowWidth, 0);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		set3D();

		if(joystick_stta[2] == true){
			joystick_stta[2] = false;
			layer = 0;
		}

		break;
	case 10:

		move();//プレイヤーの移動処理

		camerapos = GetPlayerPos();
		yaw *= -1;

		gluLookAt(camerapos.x, 2.75,camerapos.z,
				sin((player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.x, pitch * 0.5 + player.angle.x,cos( (player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.z,
				0.0,1.0,0.0);

		glEnable(GL_TEXTURE_2D);


		for (a = 0; a < MAP_Y; ++a) {
			for (j = 0; j < MAP_Height; j++) {
				for ( i = 0; i < MAP_Width; i++) {
					if (gMaps[a][i][j] != -1) {
						DRAW_CUBE(i, a, j, gMaps[a][i][j]);//マップ描画
					}
				}
			}
		}
		if(player.stts == attack && atakk_anim_count <= 6 && player.stts == attack && atakk_anim_count > 3){
			player_canhit();
		}

		//モブの移動
		mob_move();
		draw_mob();//モブの表示

		draw_haikei();


		if (mob_data[Zombi_pigman].hp >= 20) {
			draw_taiden();
		}
		draw_sword();

		glDisable(GL_TEXTURE_2D);
		//2Dへ移行
		//PROJECTION行列を操作に変更

		//MODELVIEW行列を操作
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //MODELVIEW行列を保存
		glLoadIdentity();
		//いろいろ無効にする
		//	glDisable(GL_DEPTH_TEST);
		//	//描画

		set2D();
		moji(WindowWidth,WindowHeight);
		draw2D(WindowWidth,WindowHeight);

		set3D();
		//	//元に戻す（3D用へ戻る）
		//	glEnable(GL_DEPTH_TEST);


		break;
	case 20:

		camerapos = GetPlayerPos();

		gluLookAt(camerapos.x, 2.75,camerapos.z,
				sin((player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.x, pitch * 0.5 + player.angle.x,cos( (player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.z,
				0.0,1.0,0.0);

		glEnable(GL_TEXTURE_2D);

		for (a = 0; a < MAP_Y; ++a) {
			for (j = 0; j < MAP_Height; j++) {
				for ( i = 0; i < MAP_Width; i++) {
					if (gMaps[a][i][j] != -1) {
						DRAW_CUBE(i, a, j, gMaps[a][i][j]);//マップ描画
					}
				}
			}
		}


		//モブの移動
		draw_mob();//モブの表示

		draw_haikei();


		if (mob_data[Zombi_pigman].hp >= 20) {
			draw_taiden();
		}
		draw_sword();

		glDisable(GL_TEXTURE_2D);
		//2Dへ移行
		//PROJECTION行列を操作に変更

		//MODELVIEW行列を操作
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //MODELVIEW行列を保存
		glLoadIdentity();
		//いろいろ無効にする
		//	glDisable(GL_DEPTH_TEST);
		//	//描画

		set2D();

		glBindTexture(GL_TEXTURE_2D, game_over);
		glColor4f(1, 1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glTexCoord2f(0, 0);
		glVertex2f( WindowWidth / 2 - 300, WindowHeight / 2 - 300);
		glTexCoord2f(0, 1);
		glVertex2f( WindowWidth / 2 - 300, WindowHeight / 2 + 300);
		glTexCoord2f(1, 1);
		glVertex2f( WindowWidth / 2 + 300, WindowHeight / 2 + 300);
		glTexCoord2f(1, 0);
		glVertex2f( WindowWidth / 2 + 300, WindowHeight / 2 - 300);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		set3D();
		//	//元に戻す（3D用へ戻る）
		//	glEnable(GL_DEPTH_TEST);

		if(joystick_stta[1] == true){
			joystick_stta[1] = false;
			layer = 23;
		}

		break;
	case 21:
		camerapos = GetPlayerPos();

		gluLookAt(camerapos.x, 2.75,camerapos.z,
				sin((player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.x, pitch * 0.5 + player.angle.x,cos( (player.angle.y + yaw ) / 180 * M_PI) * 35 + camerapos.z,
				0.0,1.0,0.0);

		glEnable(GL_TEXTURE_2D);

		for (a = 0; a < MAP_Y; ++a) {
			for (j = 0; j < MAP_Height; j++) {
				for ( i = 0; i < MAP_Width; i++) {
					if (gMaps[a][i][j] != -1) {
						DRAW_CUBE(i, a, j, gMaps[a][i][j]);//マップ描画
					}
				}
			}
		}


		//モブの移動
		draw_mob();//モブの表示

		draw_haikei();


		if (mob_data[Zombi_pigman].hp >= 20) {
			draw_taiden();
		}
		draw_sword();

		glDisable(GL_TEXTURE_2D);
		//2Dへ移行
		//PROJECTION行列を操作に変更

		//MODELVIEW行列を操作
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //MODELVIEW行列を保存
		glLoadIdentity();
		//いろいろ無効にする
		//	glDisable(GL_DEPTH_TEST);
		//	//描画

		set2D();

		glBindTexture(GL_TEXTURE_2D, clear);
		glColor4f(1, 1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glTexCoord2f(0, 0);
		glVertex2f( WindowWidth / 2 - 300, WindowHeight / 2 - 300);
		glTexCoord2f(0, 1);
		glVertex2f( WindowWidth / 2 - 300, WindowHeight / 2 + 300);
		glTexCoord2f(1, 1);
		glVertex2f( WindowWidth / 2 + 300, WindowHeight / 2 + 300);
		glTexCoord2f(1, 0);
		glVertex2f( WindowWidth / 2 + 300, WindowHeight / 2 - 300);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		set3D();
		//	//元に戻す（3D用へ戻る）
		//	glEnable(GL_DEPTH_TEST);

		if(joystick_stta[1] == true){
			joystick_stta[1] = false;
			layer = 23;
		}
		break;
	case 23:
		InitCharactar();
		Initmob();
		layer = 0;
		break;

	default:
		break;
	}






	glutSwapBuffers();
}



void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (double)w / (double)h, 0.1, 100.0);
	Fog();
	//	glDepthRange(0,10);
	glMatrixMode(GL_MODELVIEW);
}


void timer(int value){
	flame_count = 1 + value;
	glutPostRedisplay();
	glutTimerFunc(TIMER_WAIT,timer,flame_count); //タイマー関数
	//	printf("%d\n",flame_count);
}

void timer2(int value){

	key_event();
	glutTimerFunc(TIMER_WAIT / 10,timer2,0); //タイマー関数
	//	printf("%d\n",SDL_JoystickGetAxis(joystick, 0));
	if(vr920){
		set_hmd();
	}
}


/*-----------------------------------------------------------------------------------*
    入力関係
 *-----------------------------------------------------------------------------------*/


//マウス
void mouse(int button, int state, int x, int y){
	switch (button) {
	}
	switch (state) {
	}
}

//キーボード
void keyboard(unsigned char key, int x, int y){
	Input(SC_INPUT_KEY_DOWN, (int)key, x, y);
	switch (key) {
	case '\033':  /* '\033' は ESC の ASCII コード */
		exit(0);
		break;
	case '8':
		key_up = true;
		break;
	case '2':
		key_down = true;
		break;
	case '4':
		key_left = true;
		break;
	case '6':
		key_right = true;
		break;
	}
}

void keyboardUp(unsigned char key, int x, int y){
	Input(SC_INPUT_KEY_UP, (int)key, x, y);
	switch (key) {
	case '8':
		key_up = false;
		break;
	case '2':
		key_down = false;
		break;
	case '4':
		key_left = false;
		break;
	case '6':
		key_right = false;
		break;
	}
}


/*-----------------------------------------------------------------------------------*
    その他
 *-----------------------------------------------------------------------------------*/

// 終了処理
void Close(){
	printf("finish!\n");
	if(vr920){
		VR920Close(vr);
	}

}

//エラー処理
int PrintError( const char *str ){
	fprintf( stderr, "%s\n", str );
	return -1;
}
