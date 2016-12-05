#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glut.h>

static double spinAngle = 0.0;
int MouseX = 1280, MouseY = 1024;
int MouseX_M = 1.0, MouseY_M = 1.0;
int WinW = 1, WinH = 1;
float size = 1.0;

void draw_tree(int n, double len){
	if (n == 0){
		glBegin(GL_LINES);
			glVertex2d(0.0, 0.0);
			glVertex2d(0.0, len);
		glEnd();
		glTranslated(0.0, -len, 0.0);
	}else{
		glBegin(GL_LINES);
			glVertex2d(0.0, 0.0);
			glVertex2d(0.0, len);
		glEnd();
			glPushMatrix();			
			glTranslated(0.0, len - n / 10, 0.0);
			glRotated(7.5*n, 0.0, 0.0, 1.0);
			draw_tree(n - 1, len - n / 10);
			glPopMatrix();
			glPushMatrix();
			glTranslated(0.0, len - n / 10, 0.0);
			glRotated(-7.5*n, 0.0, 0.0, 1.0);
			draw_tree(n - 1, len - n / 10);
			glPopMatrix();
		
	}
}
void draw_branch2d(int n, double len, int pm){
	if (n > 1){
		/*幹、枝*/
		glColor3d(0.5, 0.2, 0.0);
	}else{
		/*葉*/
		glColor3d(0.0, 0.8, 0.1);
	}
	if (n <= 0){
		glBegin(GL_LINES);
		glVertex2d(0.0, 0.0);
		glVertex2d(0.0, len);
		glEnd();
		glTranslated(0.0, -len, 0.0);
	}
	else{
		glBegin(GL_LINES);
		glVertex2d(0.0, 0.0);
		glVertex2d(0.0, len);
		glEnd();
		
		glPushMatrix();//最初に左側
		if (pm > 0){//外側
			printf("a\n");
			glTranslated(0.0, len, 0.0);
			glRotated(2.0*n, 0.0, 0.0, 1.0);
			draw_branch2d(n - 1, len*0.80, pm);
		}
		else{//内側
			printf("b\n");
			glTranslated(0.0, len, 0.0);
			glRotated(3.0*n, 0.0, 0.0, 1.0);
			draw_branch2d(n - 2, len, pm);//内側は枝の伸びを減らす
		}
		glPopMatrix();
		glPushMatrix();//最初に右側
		if (pm < 0){//外側
			printf("c\n");
			glTranslated(0.0, len, 0.0);
			glRotated(-2.0*n, 0.0, 0.0, 1.0);
			draw_branch2d(n - 1, len*0.80, pm);
		}
		else{//内側
			printf("d\n");
			glTranslated(0.0, len, 0.0);
			glRotated(-3.0*n, 0.0, 0.0, 1.0);
			draw_branch2d(n - 2, len, pm);//内側は枝の伸びを減らす。
		}
		glPopMatrix();

	}
}
void draw_branch(int n, double len, int pm){
	if (n > 2){
		/*幹、枝*/
		glColor3d(0.5, 0.2, 0.0);
	}
	else{
		/*葉*/
		glColor3d(0.0, 0.5, 0.0);
	}
	if (n <= 0){
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, len, 0.0);
		glEnd();
		glTranslated(0.0, -len, 0.0);
	}
	else{
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, len, 0.0);
		glEnd();

		glPushMatrix();//最初に左側
		if (pm == 1){//外側
			printf("a\n");
			glTranslated(0.0, len, 0.0);
			glRotated(2.0*n, 0.0, 1.0, 0.3);
			draw_branch(n - 1, len*0.80, pm);
			pm *= -1;
		}
		else{//内側
			printf("b\n");
			glTranslated(0.0, len, 0.0);
			glRotated(3.0*n, 1.0, 0.0, 0.3);
			draw_branch(n - 2, len, pm);//内側は枝の伸びを減らす
			pm *= -1;
		}
		glPopMatrix();
		glPushMatrix();//最初に右側
		if (pm == 1){//外側
			printf("c\n");
			glTranslated(0.0, len, 0.0);
			glRotated(-2.0*n, 0.0, 1.0, 0.3);
			draw_branch(n - 1, len*0.80, pm);
			pm *= -1;
		}
		else{//内側
			printf("d\n");
			glTranslated(0.0, len, 0.0);
			glRotated(-3.0*n, 1.0, 0.0, -0.3);
			draw_branch(n - 2, len, pm);//内側は枝の伸びを減らす。
			pm *= -1;
		}
		glPopMatrix();

	}
}
void make_tree_2d(int n, double len){
	for (int j = 1; j <= n; j++){
		glColor3d(0.5, 0.2, 0.0);
		glBegin(GL_LINES);
			glVertex2d(0.0, 0);
			glVertex2d(0.0, len*1.5*(1+(n-j)/n));
		glEnd();

		glTranslated(0.0, len*1.5*(1 + (n - j) / n), 0.0);
		glPushMatrix();		
		glRotated(7.5*(n+1-j), 0.0, 1.0, 0.0);
			draw_branch((n + 1 - j), len, 1);
		glPopMatrix();
		glPushMatrix();
		glRotated(-7.5*(n + 1 - j), 0.0, 1.0, 0.0);
			draw_branch((n + 1 - j), len, -1);
		glPopMatrix();
	}
	glBegin(GL_LINES);
	glVertex2d(0.0, 0);
	glVertex2d(0.0, len);
	glEnd();

}
#define radiun_gain 30
void make_tree(int n, double len){
	int start_radiun ;
	for (int j = 1; j <= n; j++){
		start_radiun = 0;
		int x;
		if (j < n - 2){
			x = 3;
		}
		else{
			x = 1;
		}
		for (int l = 0; l<x; l++){
			glColor3d(0.5, 0.2, 0.0);
			if (j>n - 2)glLineWidth(5.0);
			else glLineWidth(10.0);
			glBegin(GL_LINES);
			glVertex2d(0.0, 0);
			glVertex2d(0.0, len*(0.8 + (n - j) / 2 / n));
			glEnd();
			glTranslated(0.0, len*(0.8 + (n - j) / 2 / n), 0.0);
			printf("X =%d\n", start_radiun);
			glLineWidth(2.0);
			for (int k = start_radiun; k <= 360; k += 45){
				glPushMatrix();
				glRotated(k, 0.0, 1.0, 0.0);
				glRotated(7.5*(n + 1 - j), 0.0, 0.0, 1.0);
				draw_branch((n + 1 - j), len, 1);
				glPopMatrix();
			}
			start_radiun += radiun_gain;
		}
	}
	glBegin(GL_LINES);
	glVertex2d(0.0, 0);
	glVertex2d(0.0, len);
	glEnd();

}
#define TABLE (1.5)
void ModelDarw(void){
	glClearColor(0.8, 0.8, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//設定した背景の色ので描写バッファをクリア
	glRotated(spinAngle, 0.0, 1.0, 0.0);
	glPushMatrix();
		glTranslated(0.0, -2.0, 0.0);
		glColor3d(0.0, 0.8, 0.4);
		glBegin(GL_POLYGON);
		glVertex3d(TABLE, 0.0, TABLE);
		glVertex3d(TABLE, 0.0, -TABLE);
		glVertex3d(-TABLE, 0.0, -TABLE);
		glVertex3d(-TABLE, 0.0, TABLE);
		glEnd();
	//	gluCylinder(5,0.5,0.55,0.6,16,16);
		make_tree(8, 0.2);
		glColor3d(0.8, 0.8, 0.0);
		glTranslated(0.0, 0.4, 0.0);
		glutSolidSphere(0.18, 16, 16);
	glPopMatrix();

	glutSwapBuffers();
}
void ModelSpin(void){
	spinAngle += 0.05;
	if (spinAngle > 360.0)spinAngle -= 360.0;
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
//	gluLookAt(((float)MouseX_M - ((float)WinW / 2.0)) / 1280.0*20.0, ((float)MouseY_M - ((float)WinH / 2.0)) / 1024.0*20.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(2.0, 0.5, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //視体積の位置を設定
}

void keyboard(unsigned char c, int x, int y){
	if (c == 27)exit(0);
}

void mouse(int button, int state, int x, int y){
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP){
//			MouseX = x;
//			MouseY = y;
//			size = (float)MouseX / (float)WinW;
//			glutPostRedisplay();
			MouseX_M = x;
			MouseY_M = y;
			reshape(WinW, WinH);
			glutPostRedisplay();
		}
		if (state == GLUT_DOWN)break;
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_UP){
			MouseX_M = x;
			MouseY_M = y;
//			reshape(WinW, WinH);
//			glutPostRedisplay();
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
	glutDisplayFunc(ModelDarw);			//ウィンドウズないに描画する関数のポインタを引数に指定
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	//	initailize();
	glutIdleFunc(ModelSpin);
	glutMainLoop();						//無限ループ。ひたすらユーザーからのイベントを待つ
	return 0;
}
