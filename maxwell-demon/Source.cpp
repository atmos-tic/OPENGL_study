#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <process.h>
#include <glut.h>
#include "maxwell-demon.h"
#include "main.h"
#include "math_make.h"


static double spinAngle = 0.0;
int MouseX = 640, MouseY = 250;
int MouseX_M = 1.0, MouseY_M = 1.0;
int WinW = 1, WinH = 1;
int now_atom_num = 0; 
double view_vertical = 1.0, view_horizon = 1.0, zoom = 15.0;
float size = 1.0;
double x_R = 0, x_B = 2.0;
double V_R = 0, V_B = -1.0, V_save = 0;
double new_V_B = 0, new_V_R = 0;

double cube_spot[3] = { 0.0, 0.0, 0.0 };
double wall_spot[2][3] = {{ CUBE_SCALE, CUBE_SCALE, CUBE_SCALE }, {-CUBE_SCALE, -CUBE_SCALE, -CUBE_SCALE }};
double cube_angle[4] = { 0.0, 0.0, 1.0, 0.0};
double cube_color[3] = { 1.0, 1.0, 1.0 };

double teapot_spot[3] = { -0.5, 3.5, 0.0 };
double teapot_angle[4] = { 0.0, 0.0, -1.0, 0.0 };
double teapot_color[3] = { 1.0, 1.0, 1.0 };

double atom_color[ATOM_NUM][3] = { {1.0} };

ATOM_MAKE Atom[ATOM_NUM];

int move_task = 0;

unsigned int __stdcall thread_func(void *dmy)
{
	Atom_Conflict(Atom, now_atom_num);
	/*Wall_Conflict2(Atom, now_atom_num, wall_spot);
  	for (int i = 0; i < now_atom_num; i++){
		if (Atom[i].x[Y] > wall_spot[1][Y] + ATOM_SCALE){
			Atom[i].v[Y] -= GGGGG*dT;
		}
		Atom[i].x[X] += Atom[i].v[X] * dT;
		Atom[i].x[Y] += Atom[i].v[Y] * dT;
		Atom[i].x[Z] += Atom[i].v[Z] * dT;
	}*/
	//Wall_Conflict3(Atom, now_atom_num, wall_spot, cube_spot, cube_angle);
	//printf("aaaa\n");
	printf("0\n");
	return 0;
}

unsigned int __stdcall thread_func1(void *dmy)
{
	for (int i = 0; i < now_atom_num; i++){
		//printf("IN%d\n",i);
		if (Atom[i].flag[0] == IN){
			/*�Ǖt�߂Ō��q�̔��a���߂�������*/
			if (((wall_spot[0][X] - ATOM_SCALE < Atom[i].virtual_x[X]) && (wall_spot[0][X] + ATOM_SCALE > Atom[i].virtual_x[X])) ||
				((wall_spot[1][X] + ATOM_SCALE > Atom[i].virtual_x[X]) && (wall_spot[1][X] - ATOM_SCALE < Atom[i].virtual_x[X]))){
				Atom[i].wall_collision[0] = ON;
				if (Atom[i].virtual_x[X] * Atom[i].virtual_v[X] > 0){//�ǂɌ������Ă�����ł���Ƃ��͏Փ�
					Atom[i].reflect[X] = ON;
				}
				if (Atom[i].reflect[X] == ON){
					Atom[i].virtual_v[X] *= -WALL_E;
					Atom[i].reflect[X] = OFF;
				}
			}
			else{
				Atom[i].wall_collision[X] = OFF;
			}
		}
	}
	printf("1\n");
	return 0;
}

unsigned int __stdcall thread_func2(void *dmy)
{
	for (int i = 0; i < now_atom_num; i++){
		//printf("IN%d\n",i);
		if (Atom[i].flag[0] == IN){
			/*�Ǖt�߂Ō��q�̔��a���߂�������*/
			if (((wall_spot[0][Y] - ATOM_SCALE < Atom[i].virtual_x[Y]) && (wall_spot[0][Y] + ATOM_SCALE > Atom[i].virtual_x[Y])) ||
				((wall_spot[1][Y] + ATOM_SCALE > Atom[i].virtual_x[Y]) && (wall_spot[1][Y] - ATOM_SCALE < Atom[i].virtual_x[Y]))){
				Atom[i].wall_collision[Y] = ON;
				if (Atom[i].virtual_x[Y] * Atom[i].virtual_v[Y] > 0){//�ǂɌ������Ă�����ł���Ƃ��͏Փ�
					Atom[i].reflect[Y] = ON;
				}
				if (Atom[i].reflect[Y] == ON){
					Atom[i].virtual_v[Y] *= -WALL_E;
					Atom[i].reflect[Y] = OFF;
				}
			}
			else{
				Atom[i].wall_collision[Y] = OFF;
			}
		}
	}
	printf("2\n");
	return 0;
}

unsigned int __stdcall thread_func3(void *dmy)
{
	for (int i = 0; i < now_atom_num; i++){
		//printf("IN%d\n",i);
		if (Atom[i].flag[0] == IN){
			/*�Ǖt�߂Ō��q�̔��a���߂�������*/
			if (((wall_spot[0][Z] - ATOM_SCALE < Atom[i].virtual_x[Z]) && (wall_spot[0][Z] + ATOM_SCALE > Atom[i].virtual_x[Z])) ||
				((wall_spot[1][Z] + ATOM_SCALE > Atom[i].virtual_x[Z]) && (wall_spot[1][Z] - ATOM_SCALE < Atom[i].virtual_x[Z]))){
				Atom[i].wall_collision[Z] = ON;
				if (Atom[i].virtual_x[Z] * Atom[i].virtual_v[Z] > 0){//�ǂɌ������Ă�����ł���Ƃ��͏Փ�
					Atom[i].reflect[Z] = ON;
				}
				if (Atom[i].reflect[Z] == ON){
					Atom[i].virtual_v[Z] *= -WALL_E;
					Atom[i].reflect[Z] = OFF;
				}
			}
			else{
				Atom[i].wall_collision[Z] = OFF;
			}
		}
	}
	printf("3\n");
	return 0;
}
void ModelDarw(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�ݒ肵���w�i�̐F�̂ŕ`�ʃo�b�t�@���N���A
	Draw_Cube(CUBE_SCALE, cube_spot, cube_angle, cube_color);
	if (move_task < 4){
		Draw_TeaPot(TEAPOT_SCALE, teapot_spot, teapot_angle, teapot_color);
	}
	Draw_Atom(ATOM_SCALE, Atom, now_atom_num, atom_color);
	glutSwapBuffers();
}

void Object_Move(double x, double y, double z){
	static double dx[3];
	dx[0] = x;
	dx[1] = y;
	dx[2] = z;
	Translate(wall_spot[0], dx);
	Translate(wall_spot[1], dx);
	Translate(cube_spot, dx);
	Translate(teapot_spot, dx);

}

void ModelMove(void){
	static int time_counter = 0;
	 int thread = 1;
	//printf("move%d\n", move_task);
	//_beginthreadex(NULL, 0, &thread_func, NULL, 0, NULL);
	//printf("thread\n");
	switch (move_task){
	case 0:
		teapot_angle[3] += 0.1;
		if (teapot_angle[3] > 30.0){
			move_task++;
		}
		break;
	case 1:
		teapot_angle[3] += 0.1;
		time_counter++;
		if (time_counter > APEAR_INTERVAL){
			now_atom_num = (now_atom_num + 1 < ATOM_NUM) ? now_atom_num + 1 : ATOM_NUM;
			time_counter = 0;
			//printf("now_atom_num%d\n", now_atom_num);
		}
		if (teapot_angle[3] > 45.0){
			move_task++;
		}
	case 2:
		time_counter++;
		if (time_counter > APEAR_INTERVAL){
			now_atom_num = (now_atom_num + 1 < ATOM_NUM) ? now_atom_num + 1 : ATOM_NUM;
			time_counter = 0;
			//printf("now_atom_num%d\n", now_atom_num);
		}
		if (now_atom_num >= ATOM_NUM){
			move_task++;
		}
		break;
	case 3:
		teapot_angle[3] -= 0.1;
		if (teapot_angle[3] <= 0){
			move_task++;
		}
		break;
	case 4:
		cube_angle[3] += 0.1;
		//Object_Move(0.05, -0.01, 0.0);
		if (cube_angle[3] >= 90){
			move_task++;
		}
		break;
	case 5:
		/*Object_Move(0.05, +0.01, 0.0);
		if (cube_spot[0] >= 4.0){
			move_task++;
		}*/
		break;
	case 6:
		Object_Move(0.0, -0.01, -0.05);
		if (cube_spot[2] <= 0.0){
			move_task++;
		}
		break;
	case 7:
		Object_Move(0.0, +0.01, -0.05);
		if (cube_spot[2] <= -4.0){
			move_task++;
		}
		break;
	case 8:
		Object_Move(-0.05, -0.01, 0.0);
		if (cube_spot[0] <= 0.0){
			move_task++;
		}
		break;
	case 9:
		Object_Move(-0.05, 0.01, 0.0);
		if (cube_spot[0] <= -4.0){
			move_task++;
		}
		break;
	case 10:
		Object_Move(0.0, -0.01, 0.05);
		if (cube_spot[2] >= 0.0){
			move_task++;
		}
		break;
	case 11:
		Object_Move(0.0, 0.01, 0.05);
		if (cube_spot[2] >= 4.0){
			move_task=4;
		}
		break;
	}
	
	//printf("bbbbb\n");
	Wall_Conflict4(Atom, now_atom_num, wall_spot, cube_spot, cube_angle);
	Atom_Conflict(Atom, now_atom_num);
//	Wall_Conflict2(Atom, now_atom_num, wall_spot);
	//_endthread();
	//printf("th%d\n", thread);
	for (int i = 0; i < now_atom_num; i++){
		if (Atom[i].x[Y] > wall_spot[1][Y] + ATOM_SCALE){
			Atom[i].v[Y] -= GGGGG*dT;
		}
		Atom[i].x[X] += Atom[i].v[X] * dT;
		Atom[i].x[Y] += Atom[i].v[Y] * dT;
		Atom[i].x[Z] += Atom[i].v[Z] * dT;
	}
	ModelDarw();
}
void timer(int value){
	ModelMove();
	glutTimerFunc(3, timer, 1);
}
void reshape(int w, int h){
	glViewport(0, 0, w, h);				//�\���\�̈���E�B���h�E�̕����I�傫���ɐݒ�
	WinW = w;
	WinH = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (float)w / (float)h, 1.0, 50.0); //�����@�ˉe�̎��̐ς�ݒ�
	glMatrixMode(GL_MODELVIEW);			//���f���ϊ����[�h�ɐݒ�	
	glLoadIdentity();
	//gluLookAt(((float)MouseX_M - ((float)WinW / 2.0)) / 1280.0*50.0, ((float)MouseY_M - ((float)WinH / 2.0)) / 1024.0*50.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //���̐ς̈ʒu��ݒ�
	gluLookAt(view_horizon, view_vertical, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //���̐ς̈ʒu��ݒ�
}

void mouse(int button, int state, int x, int y){
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP){
			MouseX = x;
			MouseY = y;
			size = (float)MouseX / (float)WinW;
			glutPostRedisplay();
		}
		if (state == GLUT_DOWN)break;
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_UP){
			MouseX_M = x;
			MouseY_M = y;
			reshape(WinW, WinH);
			glutPostRedisplay();
		}
		if (state == GLUT_DOWN)break;
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		break;
	}
}

void keyboad(unsigned char c, int x, int y){
	if (c == '2'){
		printf("2\n");
		view_vertical += 1.0;
		reshape(WinW, WinH);
		glutPostRedisplay();
	}
	if (c == '8'){
		printf("8\n");
		view_vertical -= 1.0;
		reshape(WinW, WinH);
		glutPostRedisplay();
	}
	if (c == '6'){
		printf("6\n");
		view_horizon += 1.0;
		reshape(WinW, WinH);
		glutPostRedisplay();
	}
	if (c == '4'){
		printf("4\n");
		view_horizon -= 1.0;
		reshape(WinW, WinH);
		glutPostRedisplay();
	}
	if (c == '1'){
		zoom += 1.0;
		reshape(WinW, WinH);
		glutPostRedisplay();
	}
	if (c == '3'){
		zoom -= 1.0;
		reshape(WinW, WinH);
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)			//argc�͎��s���̈����̐��Aargv�͂��̎��Ԕz��
{
	glutInit(&argc, argv);				//OpenGL��GLUT�̏�����
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);		//RGB�ŐF���w�肷�郂�[�h�ɐݒ�
	glutInitWindowPosition(200, 200);	//�`��E�B���h�E�����̍��W���w��
	glutInitWindowSize(500, 500);		//�`��E�B���h�E�̃T�C�Y���w��
	glutCreateWindow("Maxwell-Demon");//�`��p�̃E�B���h�E���J���B�����̓^�C�g���o�[�̕����� 
	glEnable(GL_DEPTH_TEST);           //���s�̔�r����
	Atom_Apear(Atom);
	glutDisplayFunc(ModelDarw);			//�E�B���h�E�Y�Ȃ��ɕ`�悷��֐��̃|�C���^�������Ɏw��
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboad);
//	glutIdleFunc(ModelMove);
	glutTimerFunc(10, timer, 1);
	glutMainLoop();						//�������[�v�B�Ђ����烆�[�U�[����̃C�x���g��҂�
	return 0;
}