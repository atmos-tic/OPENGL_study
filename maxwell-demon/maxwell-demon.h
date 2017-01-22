#ifndef MAXWELL_DEMON_H
#define MAXWELL_DEMON_H

#define ATOM_NUM 200

#define ATOM_SCALE (0.15)
#define ATOM_M (0.5)
#define dT (0.001)
#define APEAR_INTERVAL (10)
#define WALL_E (0.700)
#define ATOM_E (0.400)
#define GGGGG (500)
#define CUBE_SCALE (2.0)
#define TEAPOT_SCALE (1.0)

#define ANGLE (3)
#define X (0)
#define Y (1)
#define Z (2)
#define R (0)
#define G (1)
#define B (2)

#define LL (0x3)
#define LU (0x7)
#define UL (0xE)
#define UU (0xC)

#define DIGITAL_X(x) (x<<8)
#define DIGITAL_Y(y) (y<<4)
#define DIGITAL_Z(z) (z)

typedef struct atom{
	double x[3];
	double virtual_x[3];
	int digital_x = 0x000; // 3,7,E,C
	double v[3];
	double virtual_v[3];
	double flag[1];
	int atnum = 0;
	double reflect[3];//‰½d‚É‚à•Ç‚É”½ŽË‚·‚é‚Ì‚ð–h‚®
	double wall_collision[3];
	bool colliding[ATOM_NUM];
}ATOM_MAKE, *AD_ATOM_MAKE;

int Digital_Atom_Spot(double x[]);
void Atom_Setting(ATOM_MAKE atom[], int atom_num);
void Atom_Apear(ATOM_MAKE atom[]);
void Draw_Atom(double atom_scale, ATOM_MAKE atom[], int atom_num, double color[][3]);
void Atom_thermogram(double v[], double color[]);
void Draw_TeaPot(double scale, double spot[], double angle[], double color[]);
void Draw_Cube(double scale, double spot[], double angle[], double color[]);
void Correct_Spot(double Spot1[], double Spot2[], double distance);
void Atom_Conflict(ATOM_MAKE atom[], int atom_num);
void Atom_Conflict2(ATOM_MAKE atom[], int atom_num);
void Wall_Conflict(ATOM_MAKE atom[], int atom_num);
void Wall_Conflict2(ATOM_MAKE atom[], int atom_num, double cube_spot[][3]);
void Wall_Conflict3(ATOM_MAKE atom[], int atom_num, double wall_spot[][3], double cube_spot[], double cube_angle[]);
void Wall_Conflict4(ATOM_MAKE atom[], int atom_num, double wall_spot[][3], double cube_spot[], double cube_angle[]);
double Spot_distance_3d(double spot1[], double spot2[]);
void Correct_Spot(double Spot1[], double Spot2[], double distance);
void Correct_Spot2(double Spot1[], double Spot2[], double distance);
void Correct_Spot_Wall(double Spot[]);
void Correct_Spot_Wall2(double Spot[], double wall_spot[][3]);

#endif