#include <stdio.h>
#include <math.h>
#include <glut.h>
#include "maxwell-demon.h"
#include "main.h"


void Draw_Atom(ATOM_MAKE Atom[]){
	for (int i = 0; i < ATOM_NUM; i++){
		Atom[i].x[X] = 0.0;
		Atom[i].x[Y] = 7.0;
		Atom[i].x[Z] = 0.0;
		Atom[i].v[X] = 0.0;
		Atom[i].v[Y] = 7.0;
		Atom[i].v[Z] = 0.0;
		Atom[i].flag[0] = OUT;
		Atom[i].reflect[X] = ON;
		Atom[i].reflect[Y] = ON;
		Atom[i].reflect[Z] = ON;
		for (int num = 0; num < ATOM_NUM; num++){
			Atom[i].collision[num] = ON;
		}
	}
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
	glutWireTeapot(scale);
	glPopMatrix();
}


void Draw_Atom(double atom_scale, ATOM_MAKE atom[], int atom_num, double color[]){
	for (int i = 0; i < atom_num; i++){
		glPushMatrix();
		glColor3d(color[R], color[G], color[B]);
		glTranslated(atom[i].x[X], atom[i].x[Y], atom[i].x[Z]);
		glutWireSphere(atom_scale, 16.0, 16.0);
		glPopMatrix();
	}

}

double Conflict_Function(double V1, double V2, double M1, double M2){
	return V1 + M2 * (EEEEE + 1.0)*(V2 - V1) / (M1 + M2);
}

void Atom_Conflict(ATOM_MAKE atom[], int atom_num){
	for (int i = 0; i < atom_num; i++){
		for (int j = 0; j < atom_num; j++){
			if (i < j){
				if (Spot_distance_3d(atom[i].x, atom[j].x) <= RED_RADIUS + BLUE_RADIUS){
					if (atom[i].colliding[j] == OFF){
						Conflict_Function(atom[i].v[X], atom[j].v[X], RED_M, BLUE_M);
						Conflict_Function(atom[j].v[X], atom[i].v[X], RED_M, BLUE_M);
						Conflict_Function(atom[i].v[Y], atom[j].v[Y], RED_M, BLUE_M);
						Conflict_Function(atom[j].v[Y], atom[i].v[Y], RED_M, BLUE_M);
						Conflict_Function(atom[i].v[Z], atom[j].v[Z], RED_M, BLUE_M);
						Conflict_Function(atom[j].v[Z], atom[i].v[Z], RED_M, BLUE_M);
						atom[i].colliding[j] = ON;
						printf("collision\n");
					}
				}
				else{
					atom[i].colliding[j] = OFF;
				}
			}
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
	printf("iii\n");
	if (distance > L){
		printf("ooo\n");
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