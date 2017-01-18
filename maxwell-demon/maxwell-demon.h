#ifndef MAXWELL_DEMON_H
#define MAXWELL_DEMON_H

#define ATOM_NUM 100

#define ATOM_SCALE (0.1)
#define ATOM_M (0.5)
#define dT (0.0005)
#define APEAR_INTERVAL (20)
#define WALL_E (0.700)
#define ATOM_E (0.400)
#define GGGGG (980)
#define CUBE_SCALE (2.0)
#define TEAPOT_SCALE (1.0)

#define ANGLE (3)
#define X (0)
#define Y (1)
#define Z (2)
#define R (0)
#define G (1)
#define B (2)

typedef struct atom{
	double x[3];
	double v[3];
	double flag[1];
	double reflect[3];//âΩèdÇ…Ç‡ï«Ç…îΩéÀÇ∑ÇÈÇÃÇñhÇÆ
	double wall_collision[3];
	bool colliding[ATOM_NUM];
}ATOM_MAKE;

void Atom_Apear(ATOM_MAKE atom[]);
void Draw_Atom(double atom_scale, ATOM_MAKE atom[], int atom_num, double color[][3]);
void Atom_thermogram(double v[], double color[]);
void Draw_TeaPot(double scale, double spot[], double angle[], double color[]);
void Draw_Cube(double scale, double spot[], double angle[], double color[]);
void Correct_Spot(double Spot1[], double Spot2[], double distance);
void Atom_Conflict(ATOM_MAKE atom[], int atom_num);
void Wall_Conflict(ATOM_MAKE atom[], int atom_num);
void Wall_Conflict2(ATOM_MAKE atom[], int atom_num, double cube_spot[][3]);
double Spot_distance_3d(double spot1[], double spot2[]);
void Correct_Spot(double Spot1[], double Spot2[], double distance);
void Correct_Spot2(double Spot1[], double Spot2[], double distance);
void Correct_Spot_Wall(double Spot[]);
void Correct_Spot_Wall2(double Spot[], double cube_spot[][3]);
#endif