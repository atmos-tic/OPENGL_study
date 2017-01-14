#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <glut.h>
#include "maxwell-demon.h"
#include "main.h"


static double spinAngle = 0.0;
int MouseX = 640, MouseY = 250;
int MouseX_M = 1.0, MouseY_M = 1.0;
int WinW = 1, WinH = 1;
int now_atom_num = 1; 
float size = 1.0;
double x_R = 0, x_B = 2.0;
double V_R = 0, V_B = -1.0, V_save = 0;
double new_V_B = 0, new_V_R = 0;

double cube_spot[3] = { 0.0, 0.0, 0.0 };
double cube_angle[3] = { 0.0, 0.0, 0.0 };
double cube_color[3] = { 1.0, 1.0, 1.0 };

double teapot_spot[3] = { 0.0, 7.0, 0.0 };
double teapot_angle[3] = { 0.0, 0.0, 0.0 };
double teapot_color[3] = { 1.0, 1.0, 1.0 };

ATOM_MAKE Atom[ATOM_NUM];


void ModelDarw(void){
	static int time_counter = 0;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�ݒ肵���w�i�̐F�̂ŕ`�ʃo�b�t�@���N���A
	Draw_Cube(CUBE_SCALE, cube_spot, cube_angle, cube_color);
	Draw_TeaPot()
	for (int i = 0; i < now_atom_num; i++){
		glPushMatrix();
		glTranslated(A[i].x, A[i].y, A[i].z);
		glColor3d(1.0, 1.0, 1.0);
		glutWireSphere(RED_RADIUS, 16.0, 16.0);
		glPopMatrix();
	}
	time_counter++;
	/*���q�̐���*/
	if (time_counter > 50){
		now_atom_num = (now_atom_num + 1 < ATOM_NUM) ? now_atom_num + 1 : ATOM_NUM;
		time_counter = 0;
		printf("now_atom_num%d\n", now_atom_num);
	}
	
	glutSwapBuffers();
}
void ModelMove(void){
	for (int i = 0; i < now_atom_num; i++){
		if (A[i].assumption_y == CUBE_SCALE - RED_RADIUS){
			A[i].assumption_Vy = 0;
		}else{
			A[i].assumption_Vy = A[i].Vy - GGGGG*dT;
		}		
		A[i].assumption_x = A[i].x + A[i].Vx*dT;
		A[i].assumption_y = A[i].y + A[i].Vy*dT;
		A[i].assumption_z = A[i].z + A[i].Vz*dT;
	}
	for (int i = 0; i < now_atom_num; i++){
/*		for (int j = 0; j < now_atom_num; j++){
			if (i < j){
				if (sqrt(pow(A[i].assumption_x - A[j].assumption_x, 2) + pow(A[i].assumption_y - A[j].assumption_y, 2) + pow(A[i].assumption_z - A[j].assumption_z, 2)) <= RED_RADIUS + BLUE_RADIUS){
					if (A[i].collision[j] == ON){
						A[i].Vx = A[i].Vx + BLUE_M*(EEEEE + 1.0)*(A[j].Vx - A[i].Vx) / (RED_M + BLUE_M);
						A[j].Vx = A[j].Vx + RED_M*(EEEEE + 1.0)*(A[i].Vx - A[j].Vx) / (RED_M + BLUE_M);
						A[i].Vy = A[i].Vy + BLUE_M*(EEEEE + 1.0)*(A[j].Vy - A[i].Vy) / (RED_M + BLUE_M);
						A[j].Vy = A[j].Vy + RED_M*(EEEEE + 1.0)*(A[i].Vy - A[j].Vy) / (RED_M + BLUE_M);
						A[i].Vz = A[i].Vz + BLUE_M*(EEEEE + 1.0)*(A[j].Vz - A[i].Vz) / (RED_M + BLUE_M);
						A[j].Vz = A[j].Vz + RED_M*(EEEEE + 1.0)*(A[i].Vz - A[j].Vz) / (RED_M + BLUE_M);
						A[i].collision[j] = OFF;
						printf("collision%d%d\n", i, j);
					}
				}else{
					A[i].collision[j] = ON;
				}
			}			
		}*/
		for (int j = 0; j < now_atom_num; j++){
			if (i < j){
				if (sqrt(pow(A[i].x - A[j].x, 2) + pow(A[i].y - A[j].y, 2) + pow(A[i].z - A[j].z, 2)) <= RED_RADIUS + BLUE_RADIUS){
					A[i].xyz[0] = A[i].x;
					A[i].xyz[1] = A[i].y;
					A[i].xyz[2] = A[i].z;
					A[j].xyz[0] = A[j].x;
					A[j].xyz[1] = A[j].y;
					A[j].xyz[2] = A[j].z;
					Correct_Spot(A[i].xyz, A[j].xyz, RED_M + BLUE_M);
					A[i].x = A[i].xyz[0];
					A[i].y = A[i].xyz[1];
					A[i].z = A[i].xyz[2];
					A[j].x = A[j].xyz[0];
					A[j].y = A[j].xyz[1];
					A[j].z = A[j].xyz[2];
					if (A[i].collision[j] == ON){
						A[i].Vx = A[i].Vx + BLUE_M*(EEEEE + 1.0)*(A[j].Vx - A[i].Vx) / (RED_M + BLUE_M);
						A[j].Vx = A[j].Vx + RED_M*(EEEEE + 1.0)*(A[i].Vx - A[j].Vx) / (RED_M + BLUE_M);
						A[i].Vy = A[i].Vy + BLUE_M*(EEEEE + 1.0)*(A[j].Vy - A[i].Vy) / (RED_M + BLUE_M);
						A[j].Vy = A[j].Vy + RED_M*(EEEEE + 1.0)*(A[i].Vy - A[j].Vy) / (RED_M + BLUE_M);
						A[i].Vz = A[i].Vz + BLUE_M*(EEEEE + 1.0)*(A[j].Vz - A[i].Vz) / (RED_M + BLUE_M);
						A[j].Vz = A[j].Vz + RED_M*(EEEEE + 1.0)*(A[i].Vz - A[j].Vz) / (RED_M + BLUE_M);
						A[i].collision[j] = OFF;
						printf("collision%d%d\n", i, j);
					}
				}
				else{
					A[i].collision[j] = ON;
				}				
			}
		}
		if (A[i].flag[0] == IN){
			//printf("IN%d\n",i);
			/*�Ǖt�߂Ō��q�̔��a���߂�������*/
			if((CUBE_SCALE - RED_RADIUS <= A[i].assumption_x && CUBE_SCALE + RED_RADIUS >= A[i].assumption_x)||
				(CUBE_SCALE - RED_RADIUS <= -A[i].assumption_x && CUBE_SCALE + RED_RADIUS >= -A[i].assumption_x)){
				if (A[i].reflect[0] == ON){
					A[i].Vx *= -EEEEE;
					A[i].reflect[0] = OFF;
				}			
			}else{
				A[i].reflect[0] = ON;
			}
			if((CUBE_SCALE - RED_RADIUS <= A[i].assumption_y && CUBE_SCALE + RED_RADIUS >= A[i].assumption_y)||
				(CUBE_SCALE - RED_RADIUS <= -A[i].assumption_y && CUBE_SCALE + RED_RADIUS >= -A[i].assumption_y)){
				if (A[i].reflect[1] == ON){
					A[i].Vy *= -EEEEE;
					A[i].reflect[1] = OFF;
				}
			}else{
				A[i].reflect[1] = ON;
			}
			if((CUBE_SCALE - RED_RADIUS <= A[i].assumption_z && CUBE_SCALE + RED_RADIUS >= A[i].assumption_z)||
				(CUBE_SCALE - RED_RADIUS <= -A[i].assumption_z && CUBE_SCALE + RED_RADIUS >= -A[i].assumption_z)){
				if (A[i].reflect[2] == ON){
					A[i].Vz *= -EEEEE;
					A[i].reflect[2] = OFF;
				}
			}else{
				A[i].reflect[2] = ON;
			}
		}else if (A[i].flag[0] == OUT){
			//printf("OUT%d\n",i);
			/*���̓����ɂ��邩����*/
			if (((CUBE_SCALE - RED_RADIUS> A[i].assumption_x) && (-CUBE_SCALE + RED_RADIUS < A[i].assumption_x)) &&
				((CUBE_SCALE - RED_RADIUS> A[i].assumption_y) && (-CUBE_SCALE + RED_RADIUS < A[i].assumption_y)) &&
				((CUBE_SCALE - RED_RADIUS> A[i].assumption_z) && (-CUBE_SCALE + RED_RADIUS < A[i].assumption_z))){
				A[i].flag[0] = IN;
				//printf("IN_TO\n");
			}
		}
		if (A[i].y <= -CUBE_SCALE + RED_RADIUS){
			A[i].Vy = 0;
		}else{
			A[i].Vy -= GGGGG*dT;
		}
		A[i].x += A[i].Vx*dT;
		A[i].y += A[i].Vy*dT;
		A[i].z += A[i].Vz*dT;
	}
	ModelDarw();
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
	gluLookAt(((float)MouseX_M - ((float)WinW / 2.0)) / 1280.0*50.0, ((float)MouseY_M - ((float)WinH / 2.0)) / 1024.0*50.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //���̐ς̈ʒu��ݒ�
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

int main(int argc, char** argv)			//argc�͎��s���̈����̐��Aargv�͂��̎��Ԕz��
{
	glutInit(&argc, argv);				//OpenGL��GLUT�̏�����
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);		//RGB�ŐF���w�肷�郂�[�h�ɐݒ�
	glutInitWindowPosition(200, 200);	//�`��E�B���h�E�����̍��W���w��
	glutInitWindowSize(500, 500);		//�`��E�B���h�E�̃T�C�Y���w��
	glutCreateWindow("b1561297 MaterialEngineering Ishigaki Taiki");//�`��p�̃E�B���h�E���J���B�����̓^�C�g���o�[�̕����� 
	glEnable(GL_DEPTH_TEST);           //���s�̔�r����
	atom_make();
	glutDisplayFunc(ModelDarw);			//�E�B���h�E�Y�Ȃ��ɕ`�悷��֐��̃|�C���^�������Ɏw��
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(ModelMove);
	glutMainLoop();						//�������[�v�B�Ђ����烆�[�U�[����̃C�x���g��҂�
	return 0;
}
