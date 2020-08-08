

/*
* DEBUG JOURNAL:
* 2020/6/12 2:24
* - ADD JUMP FUNCTION
* - FIX TORCH PROBLEM
* - ADD FASTER PATH TO STORE AND READ FILE (COMPACT FORMAT)
* - HINT: TO GET BETTER LIGHT EFFECT, ADD MORE TRIANGLES TO THE OBJECTS
* - PROGRAMMER: LUNIUM LUK
*/
/*
* DEBUG JOURNAL:
* 2020/6/15 18:24
* - ADD MUSIC
* - ADD WINDOWS_BROKEN
* - ADD PIKA
* - ADD HP AND MK14
* - PROGRAMMER: LUNIUM LUK
*/
//#define OBJTEST
#define NORMAL
#define INCLUDE
#define TEXTUREFOLDER texture

#ifdef INCLUDE

#include <stdlib.h>
#include "glut.h"
#include "obj.h"
#include <iostream>
#include "camera_.h"
#include "UI.h"
#include <math.h>
#include <time.h>
#include "npc.h"
#include "picking.h"
#include "Door.h"
#include"geo.h"
#pragma warning(disable:4996)

#endif // INCLUDE

#ifdef OBJTEST

int main()
{
	/*
	OBJ mk14;
	//mk14.setFileAddr("obj/mk14.obj", "image/gun.bmp");
	clock_t Start = clock();
	//mk14.readObj();
	mk14.ReadCompact("mk14.cpt");
	clock_t End = clock();
	//mk14.WriteToFile("mk14.cpt");
	cout << ((double)End - Start) / CLOCKS_PER_SEC << " sec" << endl;
	*/
	OBJ CollisionBoxes;
	clock_t Start = clock();
	CollisionBoxes.ReadBoxes("obj/CollisionBoxes.obj");
	clock_t End = clock();
	cout << ((double)End - Start) / CLOCKS_PER_SEC << " sec" << endl;
}

#endif // OBJTEST

#ifdef NORMAL

bool SeeMouseMove = true;	// 是否监测鼠标
bool light_on = false;		// 是否开灯
float fps;
void DrawStablizer();
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //必要的，我是背下来的 

int win_width, win_height;
Camera m_Camera;

GLuint swidth;
GLuint sheight;
OBJ coords, mk14;
OBJ classroom, wall, wall2, wall3, table, chair, blackboard, door, floor1, restroom, washbasin, toilet, mirror, balcony;
OBJ Boxes;
NPC npc[5];
DOOR door1, door2, door3, door4;
OBJ windows[41];
OBJ handgun;
NPC pika[5];
GEO geo;
float lighty = 0.0;
int num = 0;
int score = 0;
bool broken = false;
double MyHp = 500;
bool Status = true;
bool Pause = false;
bool lightcolor = false;
int bs = 24;  //棱柱边数
//PICKING Pick;
void InitWindow() {
	windows[0].setContent("obj/windows1.obj", "texture/glass.bmp");
	windows[1].setContent("obj/windows2.obj", "texture/glass.bmp");
	windows[2].setContent("obj/windows3.obj", "texture/glass.bmp");
	windows[3].setContent("obj/windows4.obj", "texture/glass.bmp");
	windows[4].setContent("obj/windows5.obj", "texture/glass.bmp");
	windows[5].setContent("obj/windows6.obj", "texture/glass.bmp");
	windows[6].setContent("obj/windows7.obj", "texture/glass.bmp");
	windows[7].setContent("obj/windows8.obj", "texture/glass.bmp");
	windows[8].setContent("obj/windows9.obj", "texture/glass.bmp");
	windows[9].setContent("obj/windows10.obj", "texture/glass.bmp");
	windows[10].setContent("obj/windows11.obj", "texture/glass.bmp");
	windows[11].setContent("obj/windows12.obj", "texture/glass.bmp");
	windows[12].setContent("obj/windows13.obj", "texture/glass.bmp");
	windows[13].setContent("obj/windows14.obj", "texture/glass.bmp");
	windows[14].setContent("obj/windows15.obj", "texture/glass.bmp");
	windows[15].setContent("obj/windows16.obj", "texture/glass.bmp");
	windows[16].setContent("obj/windows17.obj", "texture/glass.bmp");
	windows[17].setContent("obj/windows18.obj", "texture/glass.bmp");
	windows[18].setContent("obj/windows19.obj", "texture/glass.bmp");
	windows[19].setContent("obj/windows20.obj", "texture/glass.bmp");
	windows[20].setContent("obj/windows21.obj", "texture/glass.bmp");
	windows[21].setContent("obj/windows22.obj", "texture/glass.bmp");
	windows[22].setContent("obj/windows23.obj", "texture/glass.bmp");
	windows[23].setContent("obj/windows24.obj", "texture/glass.bmp");
	windows[24].setContent("obj/windows25.obj", "texture/glass.bmp");
	windows[25].setContent("obj/windows26.obj", "texture/glass.bmp");
	windows[26].setContent("obj/windows27.obj", "texture/glass.bmp");
	windows[27].setContent("obj/windows28.obj", "texture/glass.bmp");
	windows[28].setContent("obj/windows29.obj", "texture/glass.bmp");
	windows[29].setContent("obj/windows30.obj", "texture/glass.bmp");
	windows[30].setContent("obj/windows31.obj", "texture/glass.bmp");
	windows[31].setContent("obj/windows32.obj", "texture/glass.bmp");
	windows[32].setContent("obj/windows33.obj", "texture/glass.bmp");
	windows[33].setContent("obj/windows34.obj", "texture/glass.bmp");
	windows[34].setContent("obj/windows35.obj", "texture/glass.bmp");
	windows[35].setContent("obj/windows36.obj", "texture/glass.bmp");
	windows[36].setContent("obj/windows37.obj", "texture/glass.bmp");
	windows[37].setContent("obj/windows38.obj", "texture/glass.bmp");
	windows[38].setContent("obj/windows39.obj", "texture/glass.bmp");
	windows[39].setContent("obj/windows40.obj", "texture/glass.bmp");
	windows[40].setContent("obj/windows41.obj", "texture/glass.bmp");
}

void InitPika() {
	int i;
	for (i = 0; i < 5; i++)
	{
		pika[i].setObjNum(9);
		pika[i].obj[0].setContent("obj/pika/eye.obj", "texture/pika/eye.bmp");
		pika[i].obj[1].setContent("obj/pika/mouth.obj", "texture/pika/mouth.bmp");
		pika[i].obj[2].setContent("obj/pika/red.obj", "texture/pika/red.bmp");
		pika[i].obj[3].setContent("obj/pika/1/body1.obj", "texture/pika/body.bmp");
		pika[i].obj[4].setContent("obj/pika/1/tail1.obj", "texture/pika/tail.bmp");
		pika[i].obj[5].setContent("obj/pika/2/body2.obj", "texture/pika/body.bmp");
		pika[i].obj[6].setContent("obj/pika/2/tail2.obj", "texture/pika/tail.bmp");
		pika[i].obj[7].setContent("obj/pika/3/body3.obj", "texture/pika/body.bmp");
		pika[i].obj[8].setContent("obj/pika/3/tail3.obj", "texture/pika/tail.bmp");
		pika[i].disablePick();
	}
}
void init()
{
	SetCursor(LoadCursor(0, 0));

	m_Camera.setCamera(0.0f, 10.0f, 60.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	coords.setContent("obj/coords.obj", "texture/coords.bmp");
	InitWindow();
	InitPika();
	geo.drawPrism(4.0, 2.0, 24);
	wall.setContent(		"data/wall.data",		"texture2/wall.bmp", 1);
	wall2.setContent(		"data/wall2.data",		"texture2/wall2.bmp", 1);
	wall3.setContent(		"data/wall3.data",		"texture2/wall3.bmp", 1);
	floor1.setContent(		"data/floor1.data",		"texture2/floor.bmp", 1);
	restroom.setContent(	"data/restroom.data",	"texture2/restroom.bmp", 1);
	toilet.setContent(		"data/toilet.data",		"texture2/toilet.bmp", 1);
	mirror.setContent(		"data/mirror.data",		"texture2/mirror.bmp", 1);
	balcony.setContent(		"data/balcony.data",	"texture2/balcony.bmp", 1);
	classroom.setContent(	"data/classroom.data",	"texture2/classroom.bmp", 1);
	table.setContent(		"data/table.data",		"texture2/table.bmp", 1);
	chair.setContent(		"data/chair.data",		"texture2/chair.bmp", 1);
	blackboard.setContent(	"data/blackboard.data",	"texture2/blackboard.bmp", 1);
	door.setContent(		"data/door.data",		"texture2/door.bmp", 1);

	Boxes.ReadBoxes("obj/Cube.obj");
	m_Camera.InitializeCD(1.0, 5.0, 1.0);
	m_Camera.SetBoxesData(Boxes.getBoxesCount(), Boxes.getBoxesCoords());
	
	int i;
	for (i = 0; i < 5; i++)
	{
		npc[i].setObjNum(5);
		npc[i].obj[0].setContent("obj/people_body.obj", "texture/people_body.bmp");
		npc[i].obj[1].setContent("obj/people_deco.obj", "texture/people_body.bmp");
		npc[i].obj[2].setContent("obj/people_feet.obj", "texture/people_feet.bmp");
		npc[i].obj[3].setContent("obj/people_hair.obj", "texture/people_hair.bmp");
		npc[i].obj[4].setContent("obj/people_head.obj", "texture/people_head.bmp");
		npc[i].obj[0].readTexture2("texture/people_body_blood.bmp");
		npc[i].setScale(0.7, 0.7, 0.7);
		npc[i].setSpeed(400.0);
		npc[i].setPickNum(15);
		npc[i].objHit.setContent("obj/hit.obj", "texture/hit.bmp");
	}
	handgun.setContent("obj/mk14.obj", "texture/mk14.bmp");
	door1.SetDoorInfo("obj/door1.obj", "texture/door.bmp", "obj/door1box.obj", 20.73, 38.5);
	door2.SetDoorInfo("obj/door2.obj", "texture/door.bmp", "obj/door2box.obj", 20.73, -20.0);
	door3.SetDoorInfo("obj/door3.obj", "texture/door.bmp", "obj/door3box.obj", -14.1, -20.0);
	door4.SetDoorInfo("obj/door4.obj", "texture/door.bmp", "obj/door4box.obj", -14.1, 38.5);

	glEnable(GL_LIGHTING);   //开灯
}

void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);                                    /**< 重新设置视口 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	glutPostRedisplay();
}

void KeyBoardCallBack(unsigned char k, int x, int y)
{
	if (k == 'm')
	{
		SnapScreen(swidth, sheight);
	}
	else if (k == 'b') {
		broken = true;
		PlayMusic((char*)"music/glass_broken.wav");
	}
	else if (k == 'x') {
		Pause = !Pause;
	}
	else if (k == 'f') {
		MyHp += 500;
	}
	else if (k == '2') {
		bs++;
	}
	else if (k == '3') {
		if (bs > 3){
			bs--;
		}
	}
	else if (k == '4') {
		geo.outputObj("Test.obj");
	}
	else if (k == '5') {
		lighty+=0.02;
		cout << "lighty"<<lighty << endl;
	}
	else if (k == '6') {
		lighty-=0.02;
	}
	m_Camera.CameraKeyboard(k, x, y);
	glutPostRedisplay();
}

void SpecialFunc(int k, int x, int y)
{  
	m_Camera.CameraKeyboard(k, x, y);
	glutPostRedisplay();
}

void mouseMove(int x,int y) {
	if (m_Camera.GetMouseMoveState() == true) {
		m_Camera.setViewByMouse();
		glutPostRedisplay();
	}
}

void mouseClick(int button,int state,int x, int y) {
	//Pick.MousePick(button, state, x, y, swidth, sheight);
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		m_Camera.SetZoom();
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		m_Camera.Shot();
		int i;
		for (i = 0; i < 5; i++)
			npc[i].MousePick(button, state, x, y, swidth, sheight);
		ShowCursor(false);
		m_Camera.count--;
		m_Camera.SetMouseMove(true);
	}
}
 
void redraw()
{
	if (Status) {
		if (!Pause) {
			if (MyHp <= 100)
				MyHp += 0.01;
			float x = m_Camera.getPosition().x;
			float y = m_Camera.getPosition().y;
			float z = m_Camera.getPosition().z;
			m_Camera.shootingBounce();
			m_Camera.MotionKeyboard();	// 处理asdw移动的键盘函数，-> Camera.h
			m_Camera.makeJump();		// 处理跳跃的函数，-> Camera.h
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			fps = getFPS(0, 0);			// 显示FPS在屏幕左下角，-> UI.h
			m_Camera.setFPS(fps);
			int i;
			for (i = 0; i < 5; i++)
				npc[i].setFPS(fps);
			glColor3f(1, 1, 1);
			//ShowText((char*)"Press E to open the door", 200, 240);
			DrawSightBead(swidth, sheight, swidth / 2, sheight / 2, 0.5, 4, 3, 5,
				10, 50, 70, 100);
			m_Camera.setLook();			// 摄像机类函数 -> Camera.h
			m_Camera.SpotSet();
			DrawStablizer();			// 摄像机旁的稳定器 -> main.cpp
			char show[50] = "Score:";
			char sc[5] = "";
			itoa(score, sc, 10);
			strcat(show, sc);
			ShowText(show, 380, 380);
			char show2[50] = "HP:";
			char sc2[10] = "";
			itoa(MyHp, sc2, 10);
			strcat(show2, sc2);
			ShowText(show2, 380, 360);
			for (i = 0; i < 5; i++)
			{
				if (npc[i].getHP())
				{
					if (npc[i].Found(90.0, 10.0, x, y, z)) {
						if (MyHp >= 1)
							MyHp--;
						else {
							Status = false;
						}
					}
					if (npc[i].Found(60.0, 60.0, x, y, z))
						npc[i].Chase(-50.0, 50.0, -80.0, 80.0, x, y, z, 0.03);
					else
						npc[i].RandMove(-50.0, 50.0, 0.0, -80.0, 80.0, 0.01);
					npc[i].DisplayHP(x, y, z);
				}
				else
				{
					npc[i].resurrect();
				}
			}
			for (i = 0; i < 5; i++)
				pika[i].DrawPika(-50.0, 50.0, 2.0 * i, -50.0, 50.0, 0.05);

			glEnable(GL_DEPTH_TEST);
			//Pick.DrawPickObject();	// 绘制需要鼠标点击反馈的物品 -> picking.h

			classroom.draw();			// 绘制场景物品 -> obj.h
			//door.draw();
			table.draw();
			chair.draw();
			blackboard.draw();
			wall.draw();
			wall2.draw();
			wall3.draw();
			restroom.draw();
			washbasin.draw();
			toilet.draw();
			mirror.draw();
			balcony.draw();
			floor1.draw();

			if (door1.CollisionDetect(m_Camera.getPosition(), m_Camera.m_Box)) {
				door1.PressECallBack();
			}
			if (door2.CollisionDetect(m_Camera.getPosition(), m_Camera.m_Box)) {
				door2.PressECallBack();
			}
			if (door3.CollisionDetect(m_Camera.getPosition(), m_Camera.m_Box)) {
				door3.PressECallBack();
			}
			if (door4.CollisionDetect(m_Camera.getPosition(), m_Camera.m_Box)) {
				door4.PressECallBack();
			}

			door1.FlashCallBack();
			door2.FlashCallBack();
			door3.FlashCallBack();
			door4.FlashCallBack();
			//Boxes.draw();				// 绘制碰撞检测盒 -> Camera.h & obj.h

			glTranslatef(24, 4, 26);
			glDisable(GL_TEXTURE_2D);
			geo.drawPrism(2.0, 1.0, bs);
			glEnable(GL_TEXTURE_2D);
			glTranslatef(-24, -4, -26);

			if (broken && num < 41) {
				windows[num++].draw();
			}
			else
				if (num == 0)
					windows[0].draw();
				else if (num >= 41)
					windows[40].draw();

			GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat light_pos[] = { 0.0, lighty, 0.0, 1.0 };
			GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat attrnuation[] = {lighty};
			glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

			glLightfv(GL_LIGHT0, GL_AMBIENT, white);
			//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, attrnuation);
			if (light_on)
				glEnable(GL_LIGHT0);
			else
				glDisable(GL_LIGHT0);

			glutSwapBuffers();
		}
		else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1, 1, 1);
		ShowText((char*)"Pause", 180, 220);
		glutSwapBuffers();
		}
	}
	else {
		PlayMusic((char*)"music/men_die.wav");
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1, 1, 1);
		ShowText((char*)"Game Over", 180, 220);
		glutSwapBuffers();
	}
}

void DrawStablizer() {
	////////////////////////////////////////////////////////
	////////////////摄像机稳定器V1.0.0//////////////////////
	////////////////////////////////////////////////////////
	Vector3 temp = m_Camera.getPosition();
	float x = temp.x;
	float y = temp.y;
	float z = temp.z;
	Vector3 temp2 = m_Camera.getView();
	float viewx = temp2.x;
	float viewy = temp2.y;
	float viewz = temp2.z;

	float dx = 3;
	float dy = -3;
	float dd = 5;

	double Distance = pow(pow(viewx - x, 2) + pow(viewy - y, 2) + pow(viewz - z, 2), 0.5);

	double ddx = dd * ((double)viewx - x) / Distance;
	double ddy = dd * ((double)viewy - y) / Distance;
	double ddz = dd * ((double)viewz - z) / Distance;

	double xzDistance = pow(pow(ddx, 2) + pow(ddz, 2), 0.5);

	double rx = -dx * ddz / xzDistance;
	double rz = dx * ddx / xzDistance;

	double PI = 3.1415926;

	//not rotate with view change
	glPushMatrix();
	glTranslatef(x + ddx + rx, y + ddy + dy, z + ddz + rz);
	coords.draw(); // 方向与坐标轴保持一致，随摄像机移动
	glTranslatef(0, 1, 0);
	int Flag1 = (ddz + rz) < 0 ? 0 : 1;
	int Flag2 = (ddz + rz) < 0 ? -1 : 1;
	glRotatef(atan((ddx + rx) / (ddz + rz)) * 180.0 / PI + Flag1 * 180.0, 0, 1, 0);
	glRotatef(atan(ddy / xzDistance) * 180.0 / PI, 1, 0, 0);
	coords.draw(); // 随摄像机移动和旋转
	glRotatef(-70, 0, 1, 0);
	handgun.draw();
	glPopMatrix();
}


int main(int argc, char *argv[])
{
	LONG style;
	HWND hWnd;
	glutInit(&argc, argv);
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	swidth = glutGet(GLUT_SCREEN_WIDTH) * 0.8;
	sheight = glutGet(GLUT_SCREEN_HEIGHT) * 0.8;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(swidth, sheight);
	glutInitWindowPosition((screenWidth - swidth) / 2, (screenHeight - sheight) / 2);
	int windowHandle = glutCreateWindow("保护皮卡丘大作战");

	//hWnd = FindWindow(NULL, "CGProject");
	//style = GetWindowLong(hWnd, GWL_STYLE);
	//style = style & (~WS_CAPTION) & ~(WS_BORDER) & ~WS_THICKFRAME;
	//SetWindowLong(hWnd, GWL_STYLE, style);   //无边框无标题

	init();
	PlayMusic((char*)"music/bgm_relax.wav");
	glutDisplayFunc(redraw);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(KeyBoardCallBack);
	glutSpecialFunc(SpecialFunc);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

#endif // NORMAL