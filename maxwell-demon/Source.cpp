#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <glut.h>
#include "maxwell-demon.h"
#include "main.h"



#define RED_RADIUS (0.5)
#define RED_M (0.5)
#define BLUE_RADIUS (0.5)
#define BLUE_M (0.5)
#define dT (0.001)
#define EEEEE (0.90)
#define GGGGG (980)
#define CUBE_SCALE (4.0)
#define TEAPOT_SCALE (2.0)
static double spinAngle = 0.0;
int MouseX = 640, MouseY = 250;
int MouseX_M = 1.0, MouseY_M = 1.0;
int WinW = 1, WinH = 1;
int now_atom_num = 1; 
float size = 1.0;
double x_R = 0, x_B = 2.0;
double V_R = 0, V_B = -1.0, V_save = 0;
double new_V_B = 0, new_V_R = 0;


typedef struct atom{
	double x;
	double y;
	double z;
	double Vx;
	double Vy;
	double Vz;
	double assumption_x;
	double assumption_y;
	double assumption_z;
	double assumption_Vy;
	double flag[1];
	double reflect[3];//何重にも壁に反射するのを防ぐ
	double collision[ATOM_NUM]; 
	bool colliding[ATOM_NUM];
}ATOM_MAKE;

ATOM_MAKE A[ATOM_NUM];

void atom_make(void){
	for (int i = 0; i < ATOM_NUM; i++){
	/*	ランダムに発生させたとき
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
		A[i].assumption_x  = 0.0;
		A[i].assumption_y  = 7.0;
		A[i].assumption_z  = 0.0;
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

void ModelDarw(void){
	static int time_counter = 0;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//設定した背景の色ので描写バッファをクリア
	glColor3d(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslated(0.0,0.0,0.0);
	glutWireCube(CUBE_SCALE*2);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.0, 7.0, 0.0);
	glRotated(45, 0.0, 0.0, -1.0);
	glutWireTeapot(TEAPOT_SCALE);
	glPopMatrix();
	for (int i = 0; i < now_atom_num; i++){
		glPushMatrix();
		glTranslated(A[i].x, A[i].y, A[i].z);
		glColor3d(1.0, 1.0, 1.0);
		glutWireSphere(RED_RADIUS, 16.0, 16.0);
		glPopMatrix();
	}
	time_counter++;
	/*原子の生成*/
	if (time_counter > 300){
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
		for (int j = 0; j < now_atom_num; j++){
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
						printf("collision\n");
					}
				}else{
					A[i].collision[j] = ON;
				}
			}			
		}
		if (A[i].flag[0] == IN){
			//printf("IN%d\n",i);
			/*壁付近で原子の半径より近いか判定*/
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
			/*箱の内部にあるか判定*/
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
	glViewport(0, 0, w, h);				//表示可能領域をウィンドウの物理的大きさに設定
	WinW = w;
	WinH = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (float)w / (float)h, 1.0, 50.0); //投資法射影の視体積を設定
	glMatrixMode(GL_MODELVIEW);			//モデル変換モードに設定	
	glLoadIdentity();
	gluLookAt(((float)MouseX_M - ((float)WinW / 2.0)) / 1280.0*50.0, ((float)MouseY_M - ((float)WinH / 2.0)) / 1024.0*50.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //視体積の位置を設定
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

int main(int argc, char** argv)			//argcは実行時の引数の数、argvはその実態配列
{
	glutInit(&argc, argv);				//OpenGLとGLUTの初期化
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);		//RGBで色を指定するモードに設定
	glutInitWindowPosition(200, 200);	//描画ウィンドウ左隅の座標を指定
	glutInitWindowSize(500, 500);		//描画ウィンドウのサイズを指定
	glutCreateWindow("b1561297 MaterialEngineering Ishigaki Taiki");//描画用のウィンドウを開く。引数はタイトルバーの文字列 
	glEnable(GL_DEPTH_TEST);           //奥行の比較検査
	atom_make();
	glutDisplayFunc(ModelDarw);			//ウィンドウズないに描画する関数のポインタを引数に指定
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(ModelMove);
	glutMainLoop();						//無限ループ。ひたすらユーザーからのイベントを待つ
	return 0;
}
