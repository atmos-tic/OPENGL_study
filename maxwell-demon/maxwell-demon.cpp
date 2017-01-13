#include <stdio.h>
#include <math.h>
#include <glut.h>
#include "maxwell-demon.h"
#include "main.h"

void Draw_Atom(void){
	for (int i = 0; i < ATOM_NUM; i++){
		/*	ƒ‰ƒ“ƒ_ƒ€‚É”­¶‚³‚¹‚½‚Æ‚«
		A[i].Vx = (double)(rand() % 20 - 10) / 10.0*CUBE_SCALE;
		A[i].Vy = (double)(rand() % 20 - 10) / 10.0*CUBE_SCALE;
		A[i].Vz = (double)(rand() % 20 - 10) / 10.0*CUBE_SCALE;
		A[i].x = (double)(rand() % 200 - 100) / 100.0*CUBE_SCALE;
		A[i].y = (double)(rand() % 200 - 100) / 100.0*CUBE_SCALE;
		A[i].z = (double)(rand() % 200 - 100) / 100.0*CUBE_SCALE;*/
		A[i].Vx = 3.0;
		A[i].Vy = 0.0;
		A[i].Vz = 2.0;
		A[i].x = 0.0;
		A[i].y = 7.0;
		A[i].z = 0.0;
		A[i].assumption_x = 0.0;
		A[i].assumption_y = 7.0;
		A[i].assumption_z = 0.0;
		A[i].assumption_Vy = 0.0;
		A[i].flag[0] = OUT;
		A[i].reflect[0] = ON;
		A[i].reflect[1] = ON;
		A[i].reflect[2] = ON;
		for (int num = 0; num < ATOM_NUM; num++){
			A[i].collision[num] = ON;
		}
	}
}
void Draw_TeaPot(int scale, int spot[], int angle[], int color[]){
	glPushMatrix();
	glColor3d(color[R], color[G], color[B]);
	glTranslated(spot[X], spot[Y], spot[Z]);
	glRotated(angle[ANGLE], angle[X], angle[Y], angle[Z]);
	glutWireTeapot(scale);
	glPopMatrix();
}

void Draw_Cube(int scale, int spot[], int angle[], int color[]){
	glPushMatrix();
	glColor3d(color[R], color[G], color[B]);
	glTranslated(spot[X], spot[Y], spot[Z]);
	glRotated(angle[ANGLE], angle[X], angle[Y], angle[Z]);
	glutWireTeapot(scale);
	glPopMatrix();
}

void Draw_Atom(int atom_scale, ATOM_MAKE atom[], int atom_num, int color[]){
	for (int i = 0; i < atom_num; i++){
		glPushMatrix();
		glColor3d(color[R], color[G], color[B]);
		glTranslated(atom[i].x, atom[i].y, atom[i].z);
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
				if (sqrt(pow(A[i].x - A[j].x, 2) + pow(A[i].y - A[j].y, 2) + pow(A[i].z - A[j].z, 2)) <= RED_RADIUS + BLUE_RADIUS){
					if (A[i].colliding[j] == OFF){
						Conflict_Function(A[i].Vx, A[j].Vx, RED_M, BLUE_M);
						Conflict_Function(A[j].Vx, A[i].Vx, RED_M, BLUE_M);
						Conflict_Function(A[i].Vy, A[j].Vy, RED_M, BLUE_M);
						Conflict_Function(A[j].Vy, A[i].Vy, RED_M, BLUE_M);
						Conflict_Function(A[i].Vz, A[j].Vz, RED_M, BLUE_M);
						Conflict_Function(A[j].Vz, A[i].Vz, RED_M, BLUE_M);
						A[i].colliding[j] = ON;
						printf("collision\n");
					}
				}
				else{
					A[i].colliding[j] = OFF;
				}
			}
		}
	}

}