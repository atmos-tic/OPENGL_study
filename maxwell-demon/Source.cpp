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
double cube_angle[4] = { 0.0
, 0.0, 0.0, 0.0};
double cube_color[3] = { 1.0, 1.0, 1.0 };

double teapot_spot[3] = { -0.5, 3.5, 0.0 };
double teapot_angle[4] = { 0.0, 0.0, -1.0, 45.0 };
double teapot_color[3] = { 1.0, 1.0, 1.0 };

double atom_color[ATOM_NUM][3] = { {1.0} };

ATOM_MAKE Atom[ATOM_NUM];


void ModelDarw(void){
	static int time_counter = 0;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//設定した背景の色ので描写バッファをクリア
	Draw_Cube(CUBE_SCALE, cube_spot, cube_angle, cube_color);
	Draw_TeaPot(TEAPOT_SCALE, teapot_spot, teapot_angle, teapot_color);
	Draw_Atom(ATOM_SCALE, Atom, now_atom_num, atom_color);
	time_counter++;
	/*原子の生成*/
	if (time_counter > APEAR_INTERVAL){
		now_atom_num = (now_atom_num + 1 < ATOM_NUM) ? now_atom_num + 1 : ATOM_NUM;
		time_counter = 0;
		printf("now_atom_num%d\n", now_atom_num);
	}	
	glutSwapBuffers();
}
void ModelMove(void){
	Atom_Conflict(Atom, now_atom_num); 
	Wall_Conflict(Atom, now_atom_num);
	for (int i = 0; i < now_atom_num; i++){
		if (Atom[i].x[Y] > -CUBE_SCALE + ATOM_SCALE){
			Atom[i].v[Y] -= GGGGG*dT;
		}
		Atom[i].x[X] += Atom[i].v[X]*dT;
		Atom[i].x[Y] += Atom[i].v[Y]*dT;
		Atom[i].x[Z] += Atom[i].v[Z]*dT;
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
	Atom_Apear(Atom);
	glutDisplayFunc(ModelDarw);			//ウィンドウズないに描画する関数のポインタを引数に指定
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(ModelMove);
	glutMainLoop();						//無限ループ。ひたすらユーザーからのイベントを待つ
	return 0;
}