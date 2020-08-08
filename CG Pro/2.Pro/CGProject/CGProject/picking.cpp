#include "picking.h"

PICKING::PICKING()
{
	objs = NULL;
	obj_count = 0;
}

void PICKING::MousePick(int button, int state, int x, int y, GLfloat width, GLfloat height) {
	
	glSelectBuffer(256, SelectBuffer);
	glGetIntegerv(GL_VIEWPORT, viewport);

	//printf("Mouse Pos: %d %d \n");

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("Left Button Click! %d %d\n", x, y);

		glRenderMode(GL_SELECT);
		glInitNames(); //��ʼ��
		//glPushName(0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((double)x, (double)viewport[3] - y, PICK_TOL, PICK_TOL, viewport); //��������ѡ���ͶӰ����ջ
		GLfloat Ratio = (GLfloat)width / height;
		gluPerspective(45.0f, Ratio, 1.0f, 200.0f);  //����ͶӰ��ʽ

		glMatrixMode(GL_MODELVIEW);
		RenderMode = GL_SELECT;
		DrawPickObject(); //���Ƴ���,ѡ��ģʽ
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glFlush();
		hits = glRenderMode(GL_RENDER);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, Ratio, 1.0f, 200.0f);  //����ͶӰ��ʽ
		glMatrixMode(GL_MODELVIEW);

		printf("Hits: %d\n", hits);

		int modelselect = 0; //���۾��������������֣�ID��

		if (hits > 0) {
			int n = 0;  double minz = SelectBuffer[1]; double maxz = SelectBuffer[2];
			for (int i = 1; i < hits; i++) {
				if (SelectBuffer[1 + i * 4] < minz) {
					n = i; minz = SelectBuffer[1 + i * 4];
				}
			}
			modelselect = SelectBuffer[3];
			printf("minz: %f maxz: %f Picking Object Name: %d\n", minz, maxz, modelselect);
		}
		RenderMode = GL_RENDER;
		DrawPickObject();
		glutPostRedisplay();

	}
}

void PICKING::DrawPickObject() {
	if (RenderMode == GL_SELECT) {

		glPushName(1);
		glPushMatrix();
		glTranslatef(4.5, 1, 1.5);
		glScalef(3.0, 3.0, 3.0);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopName();

		glPushName(2);
		glPushMatrix();
		glTranslatef(-4.5, 1, 1.5);
		glScalef(3.0, 3.0, 3.0);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopName();

	}
	else {

		glPushMatrix();
		glTranslatef(4.5, 1, 1.5);
		glScalef(3.0, 3.0, 3.0);
		glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-4.5, 1, 1.5);
		glScalef(3.0, 3.0, 3.0);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}
