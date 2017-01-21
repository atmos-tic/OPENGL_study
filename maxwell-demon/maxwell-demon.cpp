#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <process.h>
#include <glut.h>
#include "maxwell-demon.h"
#include "main.h"
#include "math_make.h"
#define PLUS 0
#define MINUS 1


void Atom_Apear(ATOM_MAKE atom[]){
	for (int i = 0; i < ATOM_NUM; i++){
		atom[i].x[X] = 1.0;
		atom[i].x[Y] = 3.0;
		atom[i].x[Z] = 0.0;
		atom[i].v[X] = 3.0;
		atom[i].v[Y] = -1.0;
		if (i % 3 == 0){
			atom[i].v[Z] = 1.0;
		}
		else if (i % 3 == 1){
			atom[i].v[Z] = -1.0;
		}
		else{
			atom[i].v[Z] = 0.0;
		}	
		atom[i].flag[0] = OUT;
		atom[i].reflect[X] = ON;
		atom[i].reflect[Y] = ON;
		atom[i].reflect[Z] = ON;
		atom[i].wall_collision[0] = OFF;
		atom[i].wall_collision[1] = OFF;
		atom[i].wall_collision[2] = OFF;
		for (int num = 0; num < ATOM_NUM; num++){
			atom[i].colliding[num] = OFF;
		}
	}
}

void Draw_Atom(double atom_scale, ATOM_MAKE atom[], int atom_num, double color[][3]){
	for (int i = 0; i < atom_num; i++){
		glPushMatrix();
		Atom_thermogram(atom[i].v, color[i]);
		glColor3d(color[i][R], color[i][G], color[i][B]);
		glTranslated(atom[i].x[X], atom[i].x[Y], atom[i].x[Z]);
		glutWireSphere(atom_scale, 16.0, 16.0);
		glPopMatrix();
	}

}

void Atom_thermogram(double v[], double color[]){
	color[R] = abs(v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]) / 1000;
	color[B] = 1.0 - abs(v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]) / 1000;
	//printf("R%lf", color[R]);
	//printf("B%lf\n", color[B]);
}

void Draw_TeaPot(double scale, double spot[], double angle[], double color[]){
	glPushMatrix();
	glColor3d(color[R], color[G], color[B]);
	glTranslated(spot[X], spot[Y], spot[Z]);
	glRotated(angle[ANGLE], angle[X], angle[Y], angle[Z]);
	glutWireTeapot(scale);
	glPopMatrix();
}

void Draw_Cube(double scale, double spot[], double angle[], double color[]){
	glPushMatrix();
	glColor3d(color[R], color[G], color[B]);
	glTranslated(spot[X], spot[Y], spot[Z]);
	glRotated(angle[ANGLE], angle[X], angle[Y], angle[Z]);
	glutWireCube(scale*2);
	glPopMatrix();
}


double Conflict_Function(double V1, double V2, double M1, double M2){
	return V1 + M2 * (ATOM_E + 1.0)*(V2 - V1) / (M1 + M2);
}

void Atom_Conflict(ATOM_MAKE atom[], int atom_num){
	for (int i = 0; i < atom_num; i++){
		for (int j = 0; j < atom_num; j++){
			if (i < j){
				if (Spot_distance_3d(atom[i].x, atom[j].x) <= ATOM_SCALE*2){
					if (atom[i].v[X] == atom[j].v[X] || atom[i].v[Y] == atom[j].v[Y] || atom[i].v[Z] == atom[j].v[Z]){
						atom[i].colliding[j] = OFF;
						if (atom[j].wall_collision[0] == OFF){
							Correct_Spot(atom[i].x, atom[j].x, ATOM_SCALE * 2);
						}
						else{
							Correct_Spot2(atom[i].x, atom[j].x, ATOM_SCALE * 2);
						}
					}
					if (atom[i].colliding[j] == OFF){
						Conflict_Function(atom[i].v[X], atom[j].v[X], ATOM_M, ATOM_M);
						Conflict_Function(atom[j].v[X], atom[i].v[X], ATOM_M, ATOM_M);
						Conflict_Function(atom[i].v[Y], atom[j].v[Y], ATOM_M, ATOM_M);
						Conflict_Function(atom[j].v[Y], atom[i].v[Y], ATOM_M, ATOM_M);
						Conflict_Function(atom[i].v[Z], atom[j].v[Z], ATOM_M, ATOM_M);
						Conflict_Function(atom[j].v[Z], atom[i].v[Z], ATOM_M, ATOM_M);
						atom[i].colliding[j] = ON;
					}
				}
				else{
					atom[i].colliding[j] = OFF;
				}
			}
		}
	}

}

void Wall_Conflict(ATOM_MAKE atom[], int atom_num){
	for (int i = 0; i < atom_num; i++){
		if (atom[i].flag[0] == IN){
			//printf("IN%d\n",i);
			/*壁付近で原子の半径より近いか判定*/
			if (((CUBE_SCALE - ATOM_SCALE < atom[i].x[X]) && (CUBE_SCALE + ATOM_SCALE > atom[i].x[X])) || 
				((CUBE_SCALE - ATOM_SCALE < -atom[i].x[X]) && (CUBE_SCALE + ATOM_SCALE > -atom[i].x[X]))){
				if (atom[i].x[X] * atom[i].v[X] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[X] = ON;
				}
				if (atom[i].reflect[X] == ON){
					atom[i].v[X] *= -WALL_E;
					atom[i].reflect[X] = OFF;
				}
			}
			if (((CUBE_SCALE - ATOM_SCALE < atom[i].x[Y]) && (CUBE_SCALE + ATOM_SCALE > atom[i].x[Y])) || 
				((CUBE_SCALE - ATOM_SCALE < -atom[i].x[Y])) && (CUBE_SCALE + ATOM_SCALE > -atom[i].x[Y])){
				if (atom[i].x[Y] * atom[i].v[Y] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[Y] = ON;
				}
				if (atom[i].reflect[Y] == ON){
					atom[i].v[Y] *= -WALL_E;
					atom[i].reflect[Y] = OFF;
				}
			}
			if (((CUBE_SCALE - ATOM_SCALE < atom[i].x[Z]) && (CUBE_SCALE + ATOM_SCALE > atom[i].x[Z])) || 
				((CUBE_SCALE - ATOM_SCALE < -atom[i].x[Z]) && (CUBE_SCALE + ATOM_SCALE > -atom[i].x[Z]))){
				if (atom[i].x[Z] * atom[i].v[Z] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[Z] = ON;
				}
				if (atom[i].reflect[Z] == ON){
					atom[i].v[Z] *= -WALL_E;
					atom[i].reflect[Z] = OFF;
				}
			}
			Correct_Spot_Wall(atom[i].x);
		}
		else if (atom[i].flag[0] == OUT){
			//printf("OUT%d\n",i);
			/*箱の内部にあるか判定*/
			if (((CUBE_SCALE - ATOM_SCALE> atom[i].x[X]) && (-CUBE_SCALE + ATOM_SCALE < atom[i].x[X])) &&
				((CUBE_SCALE - ATOM_SCALE> atom[i].x[Y]) && (-CUBE_SCALE + ATOM_SCALE < atom[i].x[Y])) &&
				((CUBE_SCALE - ATOM_SCALE> atom[i].x[Z]) && (-CUBE_SCALE + ATOM_SCALE < atom[i].x[Z]))){
				atom[i].flag[0] = IN;
				//printf("IN_TO\n");
			}
		}	
	}	
}

void Wall_Conflict2(ATOM_MAKE atom[], int atom_num, double wall_spot[][3]){
	for (int i = 0; i < atom_num; i++){
		if (atom[i].flag[0] == IN){
			//printf("IN%d\n",i);
			/*壁付近で原子の半径より近いか判定*/
			if (((wall_spot[PLUS][X] - ATOM_SCALE < atom[i].x[X]) && (wall_spot[PLUS][X] + ATOM_SCALE > atom[i].x[X])) ||
				((wall_spot[MINUS][X] + ATOM_SCALE > atom[i].x[X]) && (wall_spot[MINUS][X] - ATOM_SCALE < atom[i].x[X]))){
				atom[i].wall_collision[0] = ON;
				if (atom[i].x[X] * atom[i].v[X] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[X] = ON;
				}
				if (atom[i].reflect[X] == ON){
					atom[i].v[X] *= -WALL_E;
					atom[i].reflect[X] = OFF;
				}
			}
			else{
				atom[i].wall_collision[0] = OFF;
			}
			if (((wall_spot[PLUS][Y] - ATOM_SCALE < atom[i].x[Y]) && (wall_spot[PLUS][Y] + ATOM_SCALE > atom[i].x[Y])) ||
				((wall_spot[MINUS][Y] + ATOM_SCALE > atom[i].x[Y])) && (wall_spot[MINUS][Y] - ATOM_SCALE < atom[i].x[Y])){
				atom[i].wall_collision[1] = ON;
				if (atom[i].x[Y] * atom[i].v[Y] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[Y] = ON;
				}
				if (atom[i].reflect[Y] == ON){
					atom[i].v[Y] *= -WALL_E;
					atom[i].reflect[Y] = OFF;
				}
			}
			else{
				atom[i].wall_collision[1] = OFF;
			}
			if (((wall_spot[PLUS][Z] - ATOM_SCALE < atom[i].x[Z]) && (wall_spot[PLUS][Z] + ATOM_SCALE > atom[i].x[Z])) ||
				((wall_spot[MINUS][Z] + ATOM_SCALE > atom[i].x[Z]) && (wall_spot[MINUS][Z] - ATOM_SCALE < atom[i].x[Z]))){
				atom[i].wall_collision[2] = ON;
				if (atom[i].x[Z] * atom[i].v[Z] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[Z] = ON;
				}
				if (atom[i].reflect[Z] == ON){
					atom[i].v[Z] *= -WALL_E;
					atom[i].reflect[Z] = OFF;
				}
			}
			else{
				atom[i].wall_collision[2] = OFF;
			}
			Correct_Spot_Wall2(atom[i].x, wall_spot);
		}
		else if (atom[i].flag[0] == OUT){
			//printf("OUT%d\n",i);
			/*箱の内部にあるか判定*/
			if (((wall_spot[PLUS][X] - ATOM_SCALE> atom[i].x[X]) && (wall_spot[MINUS][X] + ATOM_SCALE < atom[i].x[X])) &&
				((wall_spot[PLUS][Y] - ATOM_SCALE> atom[i].x[Y]) && (wall_spot[MINUS][Y] + ATOM_SCALE < atom[i].x[Y])) &&
				((wall_spot[PLUS][Z] - ATOM_SCALE> atom[i].x[Z]) && (wall_spot[MINUS][Z] + ATOM_SCALE < atom[i].x[Z]))){
				atom[i].flag[0] = IN;
				//printf("IN_TO\n");
			}
		}
	}
}

void Wall_Conflict3(ATOM_MAKE atom[], int atom_num, double wall_spot[][3], double cube_spot[], double cube_angle[]){
	double angle[3];
	angle[X] = -cube_angle[3] * cube_angle[X] * M_PI / 180;
	angle[Y] = -cube_angle[3] * cube_angle[Y] * M_PI / 180;
	angle[Z] = -cube_angle[3] * cube_angle[Z] * M_PI / 180;
	for (int i = 0; i < atom_num; i++){
		Spot_Rotate(atom[i].virtual_x, atom[i].x, cube_spot, angle);
		Spot_Rotate(atom[i].virtual_v, atom[i].v, cube_spot, angle);
	}
	for (int i = 0; i < atom_num; i++){
		if (atom[i].flag[0] == IN){
			//printf("IN%d\n",i);
			/*壁付近で原子の半径より近いか判定*/
			if (((wall_spot[PLUS][X] - ATOM_SCALE < atom[i].virtual_x[X]) && (wall_spot[PLUS][X] + ATOM_SCALE > atom[i].virtual_x[X])) ||
				((wall_spot[MINUS][X] + ATOM_SCALE > atom[i].virtual_x[X]) && (wall_spot[MINUS][X] - ATOM_SCALE < atom[i].virtual_x[X]))){
				atom[i].wall_collision[0] = ON;
				if (atom[i].virtual_x[X] * atom[i].virtual_v[X] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[X] = ON;
				}
				if (atom[i].reflect[X] == ON){
					atom[i].virtual_v[X] *= -WALL_E;
					atom[i].reflect[X] = OFF;
				}
			}
			else{
				atom[i].wall_collision[0] = OFF;
			}
			if (((wall_spot[PLUS][Y] - ATOM_SCALE < atom[i].virtual_x[Y]) && (wall_spot[PLUS][Y] + ATOM_SCALE > atom[i].virtual_x[Y])) ||
				((wall_spot[MINUS][Y] + ATOM_SCALE > atom[i].virtual_x[Y])) && (wall_spot[MINUS][Y] - ATOM_SCALE < atom[i].virtual_x[Y])){
				atom[i].wall_collision[1] = ON;
				if (atom[i].virtual_x[Y] * atom[i].virtual_v[Y] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[Y] = ON;
				}
				if (atom[i].reflect[Y] == ON){
					atom[i].virtual_v[Y] *= -WALL_E;
					atom[i].reflect[Y] = OFF;
				}
			}
			else{
				atom[i].wall_collision[1] = OFF;
			}
			if (((wall_spot[PLUS][Z] - ATOM_SCALE < atom[i].virtual_x[Z]) && (wall_spot[PLUS][Z] + ATOM_SCALE > atom[i].virtual_x[Z])) ||
				((wall_spot[MINUS][Z] + ATOM_SCALE > atom[i].virtual_x[Z]) && (wall_spot[MINUS][Z] - ATOM_SCALE < atom[i].virtual_x[Z]))){
				atom[i].wall_collision[2] = ON;
				if (atom[i].x[Z] * atom[i].virtual_v[Z] > 0){//壁に向かってすすんでいるときは衝突
					atom[i].reflect[Z] = ON;
				}
				if (atom[i].reflect[Z] == ON){
					atom[i].virtual_v[Z] *= -WALL_E;
					atom[i].reflect[Z] = OFF;
				}
			}
			else{
				atom[i].wall_collision[2] = OFF;
			}
			Correct_Spot_Wall2(atom[i].virtual_x, wall_spot);
		}
		else if (atom[i].flag[0] == OUT){
			//printf("OUT%d\n",i);
			/*箱の内部にあるか判定*/
			if (((wall_spot[PLUS][X] - ATOM_SCALE> atom[i].x[X]) && (wall_spot[MINUS][X] + ATOM_SCALE < atom[i].x[X])) &&
				((wall_spot[PLUS][Y] - ATOM_SCALE> atom[i].x[Y]) && (wall_spot[MINUS][Y] + ATOM_SCALE < atom[i].x[Y])) &&
				((wall_spot[PLUS][Z] - ATOM_SCALE> atom[i].x[Z]) && (wall_spot[MINUS][Z] + ATOM_SCALE < atom[i].x[Z]))){
				atom[i].flag[0] = IN;
				//printf("IN_TO\n");
			}
		}

		angle[X] = cube_angle[3] * cube_angle[X] * M_PI / 180;
		angle[Y] = cube_angle[3] * cube_angle[Y] * M_PI / 180;
		angle[Z] = cube_angle[3] * cube_angle[Z] * M_PI / 180;
		for (int i = 0; i < atom_num; i++){
			Spot_Rotate(atom[i].x, atom[i].virtual_x, cube_spot, angle);
			Spot_Rotate(atom[i].v, atom[i].virtual_v, cube_spot, angle);
		}
	}
}

void Wall_Conflict4(ATOM_MAKE atom[], int atom_num, double wall_spot[][3], double cube_spot[], double cube_angle[]){
	double angle[3];
	angle[X] = -cube_angle[3] * cube_angle[X] * M_PI / 180;
	angle[Y] = -cube_angle[3] * cube_angle[Y] * M_PI / 180;
	angle[Z] = -cube_angle[3] * cube_angle[Z] * M_PI / 180;
	for (int i = 0; i < atom_num; i++){
		Spot_Rotate(atom[i].virtual_x, atom[i].x, cube_spot, angle);
		Spot_Rotate(atom[i].virtual_v, atom[i].v, cube_spot, angle);
	}
	for (int i = 0; i < atom_num; i++){
		if (atom[i].flag[0] == IN){
			_beginthreadex(NULL, 0, &thread_func1, NULL, 0, NULL);
			_beginthreadex(NULL, 0, &thread_func2, NULL, 0, NULL);
			_beginthreadex(NULL, 0, &thread_func3, NULL, 0, NULL);
		}
		else if (atom[i].flag[0] == OUT){
			//printf("OUT%d\n",i);
			/*箱の内部にあるか判定*/
			if (((wall_spot[PLUS][X] - ATOM_SCALE> atom[i].x[X]) && (wall_spot[MINUS][X] + ATOM_SCALE < atom[i].x[X])) &&
				((wall_spot[PLUS][Y] - ATOM_SCALE> atom[i].x[Y]) && (wall_spot[MINUS][Y] + ATOM_SCALE < atom[i].x[Y])) &&
				((wall_spot[PLUS][Z] - ATOM_SCALE> atom[i].x[Z]) && (wall_spot[MINUS][Z] + ATOM_SCALE < atom[i].x[Z]))){
				atom[i].flag[0] = IN;
				//printf("IN_TO\n");
			}
		}

		angle[X] = cube_angle[3] * cube_angle[X] * M_PI / 180;
		angle[Y] = cube_angle[3] * cube_angle[Y] * M_PI / 180;
		angle[Z] = cube_angle[3] * cube_angle[Z] * M_PI / 180;
		for (int i = 0; i < atom_num; i++){
			Spot_Rotate(atom[i].x, atom[i].virtual_x, cube_spot, angle);
			Spot_Rotate(atom[i].v, atom[i].virtual_v, cube_spot, angle);
		}
	}
}


double Spot_distance_3d(double spot1[], double spot2[]){
	return sqrt((spot1[0] - spot2[0])*(spot1[0] - spot2[0]) + (spot1[1] - spot2[1])*(spot1[1] - spot2[1]) + (spot1[2] - spot2[2])*(spot1[2] - spot2[2]));
}

void Correct_Spot(double Spot1[], double Spot2[], double distance){
	double L = Spot_distance_3d(Spot1, Spot2);
	double xy_distance = sqrt((Spot1[0] - Spot2[0])*(Spot1[0] - Spot2[0]) + (Spot1[1] - Spot2[1])*(Spot1[1] - Spot2[1]));
	double slope_z, slope_xy;
	if (distance > L){
		slope_z = atan2(Spot1[2] - Spot2[2], xy_distance);
		slope_xy = atan2(Spot1[1] - Spot2[1], Spot1[0] - Spot2[0]);
		Spot1[2] += (distance - L)*sin(slope_z) / 2;
		Spot2[2] -= (distance - L)*sin(slope_z) / 2;
		Spot1[1] += (distance - L)*cos(slope_z) * cos(slope_xy) / 2;
		Spot2[1] -= (distance - L)*cos(slope_z) * cos(slope_xy) / 2;
		Spot1[0] += (distance - L)*cos(slope_z) * sin(slope_xy) / 2;
		Spot2[0] -= (distance - L)*cos(slope_z) * sin(slope_xy) / 2;
	}
}
void Correct_Spot2(double Spot1[], double Spot2[], double distance){
	double L = Spot_distance_3d(Spot1, Spot2);
	double xy_distance = sqrt((Spot1[0] - Spot2[0])*(Spot1[0] - Spot2[0]) + (Spot1[1] - Spot2[1])*(Spot1[1] - Spot2[1]));
	double slope_z, slope_xy;
	if (distance > L){
		slope_z = atan2(Spot1[2] - Spot2[2], xy_distance);
		slope_xy = atan2(Spot1[1] - Spot2[1], Spot1[0] - Spot2[0]);
		Spot1[2] += (distance - L)*sin(slope_z);
		Spot1[1] += (distance - L)*cos(slope_z) * cos(slope_xy);
		Spot1[0] += (distance - L)*cos(slope_z) * sin(slope_xy);
	}
}
void Correct_Spot_Wall(double Spot[]){
	for (int i = 0; i < 3; i++){
		if (Spot[i] > CUBE_SCALE - ATOM_SCALE){
			Spot[i] = CUBE_SCALE - ATOM_SCALE;
		}else if (-Spot[i] > CUBE_SCALE - ATOM_SCALE){
			Spot[i] = -CUBE_SCALE + ATOM_SCALE;
		}
	}
}
void Correct_Spot_Wall2(double Spot[], double cube_spot[][3]){
	for (int i = X; i < Z+1; i++){
		if (Spot[i] > cube_spot[PLUS][i] - ATOM_SCALE){
			Spot[i] = cube_spot[PLUS][i] - ATOM_SCALE;
		}
		else if (Spot[i] < cube_spot[MINUS][i] + ATOM_SCALE){
			Spot[i] = cube_spot[MINUS][i] + ATOM_SCALE;
		}
	}
}