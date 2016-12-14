/*--------------------------------------------------------------------------
起動時の方向を基準とし，5000回，ヘッドマウントディスプレイVR920の方向の値を出力します．

出力フォーマット）
yaw, roll, pitch

コンパイル：gcc vr920.c vr920lib.a -lm -lusb
--------------------------------------------------------------------------*/

#include <stdio.h>
#include "vr920.h"

int	ret;

int Init_hmd(){
	vr = VR920Open();
	if(vr == NULL){
		printf("no_hmd.\n");
		return -1;
	}

	/* 起動時の方向を基準とする */
	VR920SetZero(vr);
	return 0;
}

int set_hmd(){
	ret = VR920GetData(vr, &yaw, &roll, &pitch);
	if(ret != 0)
		return -1;
	printf("%6.1f,%6.1f,%6.1f\n",yaw, roll, pitch);
	return 0;
}

