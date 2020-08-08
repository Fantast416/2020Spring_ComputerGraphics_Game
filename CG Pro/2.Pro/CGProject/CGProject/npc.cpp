#include "npc.h"
#include "UI.h"
extern int score;
NPC::NPC()
{
	this->name = "UNNAMED";
	Speed = N_SPEED;
}

NPC::NPC(const string& name)
{
	this->name = name;
	Speed = N_SPEED;
}

void NPC::setPosition(float x, float y, float z)
{
	v_position.x = x;
	v_position.y = y;
	v_position.z = z;
}

void NPC::setScale(float x, float y, float z)
{
	v_Scale.x = x;
	v_Scale.y = y;
	v_Scale.z = z;
}

void NPC::setRandSeed()
{
	unsigned seed = time(0);
	srand(seed);
}

void NPC::Static()
{
	glPushMatrix();
	glTranslatef(v_position.x, v_position.y, v_position.z);
	glScalef(v_Scale.x, v_Scale.y, v_Scale.z);
	int i;
	for (i = 0; i < obj_num; i++)
		obj[i].draw();
	glPopMatrix();
}

void NPC::CircularMove(float r, float speed)
{
	float dx = r * cos(Para1 * 3.1415926 / 180.0);
	float dz = r * sin(Para1 * 3.1415926 / 180.0);
	glPushMatrix();
	glTranslatef(v_position.x + dx, v_position.y, v_position.z + dz);
	glScalef(v_Scale.x, v_Scale.y, v_Scale.z);
	int i;
	for (i = 0; i < obj_num; i++)
		obj[i].draw();
	glPopMatrix();
	Para1 += speed;
	if (Para1 > 360) Para1 -= 360;
}

void NPC::RandMove(float x1, float x2, float y, float z1, float z2, float speed)
{
	if (HP)
	{
		int Pace = 400;
		int R = rand();
		Para3 += 1;
		//Para4 += 1;
		if (Para3 > Pace) Para3 = 0;
		switch (R % 4)
		{
		case 0:
			Para1++;
			break;
		case 1:
			Para2++;
			break;
		case 2:
			Para1--;
			break;
		case 3:
			Para2--;
			break;
		default:
			break;
		}
		if (Para1 > 100 || Para1 < -100) Para1 = 0;
		if (Para2 > 100 || Para2 < -100) Para2 = 0;
		//cout << Para1 << ' ' << Para2 << endl;
		float Speed = getSpeed();
		float Norm = pow(Para1 * Para1 + Para2 * Para2, 0.5) / speed;
		if (v_rand.x + Para1 / Norm * Speed - v_position.x > x1&& v_rand.x + Para1 / Norm * Speed - v_position.x < x2 &&
			v_rand.z + Para2 / Norm * Speed - v_position.z > z1 && v_rand.z + Para2 / Norm * Speed - v_position.z < z2)
		{
			v_rand.x += Para1 / Norm * Speed;
			v_rand.z += Para2 / Norm * Speed;
		}
		else
		{
			Para1 = -Para1;
			Para2 = -Para2;
		}
		switch ((int)Para3 / (Pace / 4))
		{
		case 0:
			v_rand.y += 0.005;
			Para4 -= 0.4;
			break;
		case 1:
			Para4 += 0.4;
			break;
		case 2:
			v_rand.y -= 0.005;
			Para4 += 0.4;
			break;
		case 3:
			Para4 -= 0.4;
			break;
		default:
			break;
		}
		float Angle = atan((float)Para1 / Para2) * 180 / 3.1415926;
		if (Para2 < 0) Angle += 180;
		Angle = (int)Angle / 10 * 10;
		glPushMatrix();
		glTranslatef(v_rand.x, v_rand.y, v_rand.z);
		glScalef(v_Scale.x, v_Scale.y, v_Scale.z);
		glRotatef(Angle, 0.0, 1.0, 0.0);
		int i;
		for (i = 0; i < 3; i++)
			obj[i].draw();
		glRotatef(Para4, 0.0, 1.0, 0.0);
		for (i = 3; i < 5; i++)
			obj[i].draw();
		glPopMatrix();
	}
}

void NPC::RandMoveDraw()
{
	float Angle = atan((float)Para1 / Para2) * 180 / 3.1415926;
	if (Para2 < 0) Angle += 180;
	Angle = (int)Angle / 10 * 10;
	if (RenderMode == GL_SELECT)
		glPushName(PickNum);
	glPushMatrix();
	glTranslatef(v_rand.x, v_rand.y, v_rand.z);
	glScalef(v_Scale.x, v_Scale.y, v_Scale.z);
	glRotatef(Angle, 0.0, 1.0, 0.0);
	int i;
	for (i = 0; i < 3; i++)
		obj[i].draw();
	glRotatef(Para4, 0.0, 1.0, 0.0);
	for (i = 3; i < 5; i++)
		obj[i].draw();
	glPopMatrix();
	if (RenderMode == GL_SELECT)
		glPopName();
}

void NPC::setObjNum(int n)
{
	obj_num = n;
	obj = new OBJ[n];
}

float NPC::getSpeed()
{
	if (FPS && !isnan(FPS))
		return Speed / FPS;
	else
		return Speed / 100.0;
}

void NPC::MousePick(int button, int state, int x, int y, GLfloat width, GLfloat height) {
	if (HP && Pick)
	{
		glSelectBuffer(256, SelectBuffer);
		glGetIntegerv(GL_VIEWPORT, viewport);

		//printf("Mouse Pos: %d %d \n");

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			printf("Left Button Click! %d %d\n", x, y);

			glRenderMode(GL_SELECT);
			glInitNames(); //初始化
			//glPushName(0);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluPickMatrix((double)x, (double)viewport[3] - y, PICK_TOL, PICK_TOL, viewport); //创建用于选择的投影矩阵栈
			GLfloat Ratio = (GLfloat)width / height;
			gluPerspective(45.0f, Ratio, 1.0f, 200.0f);  //设置投影方式

			glMatrixMode(GL_MODELVIEW);
			//gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);			// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
			RenderMode = GL_SELECT;
			RandMoveDraw();
			//DrawPickObject(); //绘制场景,选择模式
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			glFlush();
			hits = glRenderMode(GL_RENDER);

			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, Ratio, 1.0f, 200.0f);  //设置投影方式
			glMatrixMode(GL_MODELVIEW);
			//gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);			// 场景（0，0，0）的视点中心 (0,


			printf("Hits: %d\n", hits);


			int modelselect = 0; //离眼睛最近的物件的名字（ID）

			if (hits > 0) {
				int n = 0;  double minz = SelectBuffer[1]; double maxz = SelectBuffer[2];
				for (int i = 1; i < hits; i++) {
					if (SelectBuffer[1 + i * 4] < minz) {
						n = i; minz = SelectBuffer[1 + i * 4];
					}
				}
				modelselect = SelectBuffer[3];
				printf("minz: %f maxz: %f Picking Object Name: %d\n", minz, maxz, modelselect);
				if (modelselect == PickNum)
				{
					//Hit = 200;
					if (HP) {
						PlayMusic((char*)"music/men_hurt.wav");
						score++;
						HP--;
					}
					if (!HP)
						Respawn = RES_TIME;
					obj[0].texMix(HP / 10.0);
				}
			}
			RenderMode = GL_RENDER;
			//DrawPickObject();
			RandMoveDraw();
			glutPostRedisplay();

		}
	}
}

bool NPC::Found(float angle, float sight, float x, float y, float z)
{
	float rand_angle = atan((double)Para1 / Para2) * 180.0 / 3.1415926;
	float target_angle = atan(((double)x - v_rand.x) / ((double)z - v_rand.z)) * 180.0 / 3.1415926;
	float angle_diff = rand_angle - target_angle > 0 ? rand_angle - target_angle : target_angle - rand_angle;
	float Dist = pow(pow(x - v_rand.x, 2) + pow(z - v_rand.z, 2), 0.5);
	if (Para1 * (x - v_rand.x) >= 0 && Para2 * (z - v_rand.z) >= 0 && 
		angle_diff <= angle && Dist <= sight)
	{
		return true;
	}
	return false;
}

void NPC::Chase(float x1, float x2, float z1, float z2, float x, float y, float z, float speed)
{
	int Pace = 400;
	int R = rand();
	Para3 += 1;
	//Para4 += 1;
	if (Para3 > Pace) Para3 = 0;
	switch ((int)Para3 / (Pace / 4))
	{
	case 0:
		v_rand.y += 0.005;
		Para4 -= 0.4;
		break;
	case 1:
		Para4 += 0.4;
		break;
	case 2:
		v_rand.y -= 0.005;
		Para4 += 0.4;
		break;
	case 3:
		Para4 -= 0.4;
		break;
	default:
		break;
	}
	Para1 = (int)(x - v_rand.x);
	Para2 = (int)(z - v_rand.z);
	float Speed = getSpeed();
	float Norm = pow(Para1 * Para1 + Para2 * Para2, 0.5) / speed;
	if (v_rand.x + Para1 / Norm * Speed - v_position.x > x1 && v_rand.x + Para1 / Norm * Speed - v_position.x < x2 &&
		v_rand.z + Para2 / Norm * Speed - v_position.z > z1 && v_rand.z + Para2 / Norm * Speed - v_position.z < z2)
	{
		v_rand.x += Para1 / Norm * Speed;
		v_rand.z += Para2 / Norm * Speed;
	}
	float Angle = atan((float)Para1 / Para2) * 180 / 3.1415926;
	if (Para2 < 0) Angle += 180;
	Angle = (int)Angle / 10 * 10;
	glPushMatrix();
	glTranslatef(v_rand.x, v_rand.y, v_rand.z);
	glScalef(v_Scale.x, v_Scale.y, v_Scale.z);
	glRotatef(Angle, 0.0, 1.0, 0.0);
	int i;
	for (i = 0; i < 3; i++)
		obj[i].draw();
	glRotatef(Para4, 0.0, 1.0, 0.0);
	for (i = 3; i < 5; i++)
		obj[i].draw();
	glPopMatrix();
}

void NPC::DisplayHP(float x, float y, float z)
{
	int i;
	glPushMatrix();
	glTranslatef(v_rand.x - 0.0, v_rand.y + 12.0, v_rand.z + 0.0);
	float Rotate = atan((v_rand.x - x) / (v_rand.z - z)) * 180.0 / 3.1415926;
	if (v_rand.z - z > 0)
		Rotate += 180.0;
	glRotatef(Rotate, 0.0, 1.0, 0.0);
	for (i = 0; i < HP; i++)
	{
		glutSolidCube(0.2);
		glTranslatef(0.4, 0.0, 0.0);
	}
	//cout << v_rand.x << ' ' << v_rand.z << ' ' << x << ' ' << z << endl;
	glPopMatrix();
}

void NPC::resurrect()
{
	if (Respawn)
	{
		Respawn--;
		if (!Respawn)
		{
			HP = 10;
			obj[0].texMix(1.0);
		}
	}
}

void NPC::DrawPika(float x1, float x2, float y, float z1, float z2, float speed)
{
	int Pace = 400;
	int R = rand();
	Para3 += 1;
	//Para4 += 1;
	if (Para3 > Pace) Para3 = 0;
	switch (R % 4)
	{
	case 0:
		Para1++;
		break;
	case 1:
		Para2++;
		break;
	case 2:
		Para1--;
		break;
	case 3:
		Para2--;
		break;
	default:
		break;
	}
	if (Para1 > 100 || Para1 < -100) Para1 = 0;
	if (Para2 > 100 || Para2 < -100) Para2 = 0;
	//cout << Para1 << ' ' << Para2 << endl;
	float Speed = getSpeed();
	float Norm = pow(Para1 * Para1 + Para2 * Para2, 0.5) / speed;
	if (v_rand.x + Para1 / Norm * Speed - v_position.x > x1&& v_rand.x + Para1 / Norm * Speed - v_position.x < x2 &&
		v_rand.z + Para2 / Norm * Speed - v_position.z > z1 && v_rand.z + Para2 / Norm * Speed - v_position.z < z2)
	{
		v_rand.x += Para1 / Norm * Speed;
		v_rand.z += Para2 / Norm * Speed;
	}
	else
	{
		Para1 = -Para1;
		Para2 = -Para2;
	}
	switch ((int)Para3 / (Pace / 4))
	{
	case 0:
		v_rand.y += 0.01;
		Para4 -= 0.4;
		break;
	case 1:
		v_rand.y += 0.05;
		Para4 += 0.4;
		break;
	case 2:
		v_rand.y -= 0.05;
		Para4 += 0.4;
		break;
	case 3:
		v_rand.y -= 0.01;
		Para4 -= 0.4;
		break;
	default:
		break;
	}
	float Angle = atan((float)Para1 / Para2) * 180 / 3.1415926;
	if (Para2 < 0) Angle += 180;
	Angle = (int)Angle / 10 * 10;
	glPushMatrix();
	glTranslatef(v_rand.x, v_rand.y + y, v_rand.z);
	glScalef(v_Scale.x, v_Scale.y, v_Scale.z);
	glRotatef(Angle, 0.0, 1.0, 0.0);
	int i;
	for (i = 0; i < 3; i++)
		obj[i].draw();
	switch (((int)Para3/4) % 4)
	{
	case 0:
		obj[3].draw();
		obj[4].draw();
		break;
	case 1:
		obj[5].draw();
		obj[6].draw();
		break;
	case 2:
		obj[7].draw();
		obj[8].draw();
		break;
	case 3:
		obj[5].draw();
		obj[6].draw();
		break;
	default:
		break;
	}
	glPopMatrix();
}

void NPC::disablePick()
{
	Pick = false;
}