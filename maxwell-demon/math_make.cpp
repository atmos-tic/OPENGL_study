#include <stdio.h>
#include <math.h>


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

void Spot_Rotate(double x[], double spot[], double angle[]){
	x[0] = cos(angle[1])*cos(angle[2])*(x[0] - spot[0]) + (cos(angle[0]) * sin(angle[1])* cos(angle[2]) - cos(angle[0])*sin(angle[2]))*(x[1] - spot[1]) + (sin(angle[0])*sin(angle[2]) + cos(angle[0])*sin(angle[1])*cos(angle[2]))*(x[2] - spot[2]) + spot[0];
	x[1] = 

}