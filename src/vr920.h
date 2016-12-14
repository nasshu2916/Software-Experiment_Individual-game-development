#ifndef __VR920_HEADER__
#define __VR920_HEADER__

typedef void	VR920;

extern VR920 *VR920Open();
extern void VR920Close(VR920 *vr);
extern int VR920SetZero(VR920 *vr);
extern int VR920GetData(VR920 *vr, double *yaw, double *roll, double *pitch);

int Init_hmd();
int set_hmd();

double yaw,roll,pitch;
VR920 *vr;

#endif
