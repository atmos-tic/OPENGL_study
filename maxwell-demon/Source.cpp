#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <glut.h>

#define RED_RADIUS (0.5)
#define RED_M (0.5)
#define BLUE_RADIUS (0.5)
#define BLUE_M (0.5)
#define dT (0.001)
#define EEEEE (1.00)
#define CUBE_SCALE (4.0)
static double spinAngle = 0.0;
int MouseX = 640, MouseY = 250;
int MouseX_M = 1.0, MouseY_M = 1.0;
int WinW = 1, WinH = 1;
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
}ATOM_MAKE;

#define ATOM_NUM 20
ATOM_MAKE A[ATOM_NUM];

void atom_make(void){
	for (int i = 0; i < ATOM_NUM; i++){
		A[i].Vx = (double)(rand() % 20 - 10) / 10.0*CUBE_SCALE;
		A[i].Vy = (double)(rand() % 20 - 10) / 10.0*CUBE_SCALE;
		A[i].Vz = (double)(rand() % 20 - 10) / 10.0*CUBE_SCALE;
		A[i].x = (double)(rand() % 200 - 100) / 100.0*CUBE_SCALE;
		A[i].y = (double)(rand() % 200 - 100) / 100.0*CUBE_SCALE;
		A[i].z = (double)(rand() % 200 - 100) / 100.0*CUBE_SCALE;
	}
}
void ModelDarw(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//設定した背景の色ので描写バッファをクリア
	glColor3d(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslated(0.0,0.0,0.0);
	glutWireCube(CUBE_SCALE*2);
	glPopMatrix();
	for (int i = 0; i < ATOM_NUM; i++){
		glPushMatrix();
		glTranslated(A[i].x, A[i].y, A[i].z);
		glColor3d(1.0, 1.0, 1.0);
		glutWireSphere(RED_RADIUS, 16.0, 16.0);
		glPopMatrix();
	}
	glutSwapBuffers();
}
void ModelMove(void){
	for (int i = 0; i < ATOM_NUM; i++){
		for (int j = 0; j < ATOM_NUM; j++){
			if (i <= j){
				if (sqrt(pow(A[i].x - A[j].x, 2) + pow(A[i].y - A[j].y, 2) + pow(A[i].z - A[j].z, 2)) <= RED_RADIUS + BLUE_RADIUS){
					A[i].Vx = A[i].Vx + BLUE_M*(EEEEE + 1.0)*(A[j].Vx - A[j].Vx) / (RED_M + BLUE_M);
					A[j].Vx = A[j].Vx + RED_M*(EEEEE + 1.0)*(A[i].Vx - A[j].Vx) / (RED_M + BLUE_M);
					A[i].Vy = A[i].Vy + BLUE_M*(EEEEE + 1.0)*(A[j].Vy - A[i].Vy) / (RED_M + BLUE_M);
					A[j].Vy = A[j].Vy + RED_M*(EEEEE + 1.0)*(A[i].Vy - A[j].Vy) / (RED_M + BLUE_M);
					A[i].Vz = A[i].Vz + BLUE_M*(EEEEE + 1.0)*(A[j].Vz - A[i].Vz) / (RED_M + BLUE_M);
					A[j].Vz = A[j].Vz + RED_M*(EEEEE + 1.0)*(A[i].Vz - A[i].Vz) / (RED_M + BLUE_M);
				}
			}			
		}
	}
	
	for (int i = 0; i < ATOM_NUM; i++){
		if (abs(A[i].x - CUBE_SCALE) <= RED_RADIUS || abs(A[i].x + CUBE_SCALE) <= RED_RADIUS){
			A[i].Vx *= -EEEEE;
		}
		if (abs(A[i].y - CUBE_SCALE) <= RED_RADIUS || abs(A[i].y + CUBE_SCALE) <= RED_RADIUS){
			A[i].Vy *= -EEEEE;
		}
		if (abs(A[i].z - CUBE_SCALE) <= RED_RADIUS || abs(A[i].z + CUBE_SCALE) <= RED_RADIUS){
			A[i].Vz *= -EEEEE;
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
	gluPerspective(40.0, (float)w / (float)h, 0.1, 20.0); //投資法射影の視体積を設定
	glMatrixMode(GL_MODELVIEW);			//モデル変換モードに設定	
	glLoadIdentity();
	gluLookAt(((float)MouseX_M - ((float)WinW / 2.0)) / 1280.0*20.0, ((float)MouseY_M - ((float)WinH / 2.0)) / 1024.0*20.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //視体積の位置を設定
}

void keyboard(unsigned char c, int x, int y){
	if (c == 27)exit(0);
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
/*	A[0].x = 2.0;
	A[0].y = -1.0;
	A[0].z = 0.5;
	A[0].Vx = 0;
	A[0].Vy = -0.04;
	A[0].Vz = 0.04;
	A[1].x = 1.0;
	A[1].y = 2.0;
	A[1].z = 2.0;
	A[1].Vx = 0.03;
	A[1].Vy = -0.05;
	A[1].Vz = -0.05;
	A[2].x = 1.0;
	A[2].y = 2.0;
	A[2].z = 0;
	A[2].Vx = 0.3;
	A[2].Vy = 0.1;
	A[2].Vz = 0;*/
	atom_make();
	glutDisplayFunc(ModelDarw);			//ウィンドウズないに描画する関数のポインタを引数に指定
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	//	initailize();
	glutIdleFunc(ModelMove);
	glutMainLoop();						//無限ループ。ひたすらユーザーからのイベントを待つ
	return 0;
}
