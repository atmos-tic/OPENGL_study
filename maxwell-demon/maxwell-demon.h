#ifndef MAXWELL_DEMON_H
#define MAXWELL_DEMON_H

#define ATOM_NUM 10

#define RED_RADIUS (0.5)
#define RED_M (0.5)
#define BLUE_RADIUS (0.5)
#define BLUE_M (0.5)
#define dT (0.001)
#define EEEEE (0.800)
#define GGGGG (980)
#define CUBE_SCALE (4.0)
#define TEAPOT_SCALE (2.0)

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
	double assumption_x;
	double assumption_y;
	double assumption_z;
	double assumption_Vy;
	double flag[1];
	double reflect[3];//âΩèdÇ…Ç‡ï«Ç…îΩéÀÇ∑ÇÈÇÃÇñhÇÆ
	double collision[ATOM_NUM];
	bool colliding[ATOM_NUM];
}ATOM_MAKE;

void Draw_Atom(ATOM_MAKE Atom[]);
void Draw_TeaPot(double scale, double spot[], double angle[], double color[]);
void Draw_Cube(double scale, double spot[], double angle[], double color[]);
void Draw_Atom(double atom_scale, ATOM_MAKE atom[], int atom_num, double color[]);
void Correct_Spot(double Spot1[], double Spot2[], double distance);
void Atom_Conflict(ATOM_MAKE atom[], int atom_num);

double Spot_distance_3d(double spot1[], double spot2[]);
void Correct_Spot(double Spot1[], double Spot2[], double distance);
#endif