#include <stdio.h>
#include <math.h>

int numofbits(int bits)
{
	bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
	bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
	bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
	bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}
void Translate(double x[], double dx[]){
	x[0] += dx[0];
	x[1] += dx[1];
	x[2] += dx[2];
}
void Rotate_X(double x[], double angle){
	x[0] = x[0];
	x[1] = cos(angle)*x[1] - sin(angle)*x[2];
	x[2] = sin(angle)*x[1] + cos(angle)*x[2];
}

void Rotate_Y(double x[], double angle){
	x[0] = cos(angle)*x[0] + sin(angle)*x[2];
	x[1] = x[1];
	x[2] = -sin(angle)*x[0] + cos(angle)*x[2];
}


void Rotate_Z(double x[], double angle){
	x[0] = cos(angle)*x[0] - sin(angle)*x[1];
	x[1] = sin(angle)*x[0] + cos(angle)*x[1];
	x[2] = x[2];
}

void Spot_Rotate(double x1[], double x2[], double spot[], double angle[]){
	x1[0] = cos(angle[1])*cos(angle[2])*(x2[0] - spot[0]) + (cos(angle[0]) * sin(angle[1])* cos(angle[2]) - cos(angle[0])*sin(angle[2]))*(x2[1] - spot[1]) + (sin(angle[0])*sin(angle[2]) + cos(angle[0])*sin(angle[1])*cos(angle[2]))*(x2[2] - spot[2]) + spot[0];
	x1[1] = cos(angle[1])*sin(angle[2])*(x2[0] - spot[0]) + (cos(angle[0]) * cos(angle[2]) + sin(angle[0])*sin(angle[1])*sin(angle[2])) * (x2[1] - spot[1]) + (cos(angle[0])*sin(angle[1])*sin(angle[2]) - sin(angle[0])*cos(angle[2]))*(x2[2] - spot[2]) + spot[1];
	x1[2] = -sin(angle[1]) * (x2[0] - spot[0]) + sin(angle[0])*cos(angle[1])*(x2[1] - spot[1]) + cos(angle[0])*cos(angle[1])*(x2[2] - spot[2]) + spot[2];
}