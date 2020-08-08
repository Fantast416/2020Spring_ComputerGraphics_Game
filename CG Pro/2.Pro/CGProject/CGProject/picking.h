#ifndef PICKING_H_
#define PICKING_H
/************************************************************************
* ʹ��˵����
*
* ʰȡ����ʵ��
*
*************************************************************************/

#include "obj.h"
#include "glut.h"
#include <stdio.h>
#define PICK_TOL 5				/* ʰȡ���ڴ�С */
#define PICK_BUFFER_SIZE 256	/* ʰȡ���������С */

class PICKING {
private:
	unsigned int SelectBuffer[PICK_BUFFER_SIZE];  // ʰȡ��������
	int hits;							// ʰȡ��������
	int viewport[4];					// �ӿ�
	GLenum RenderMode = GL_RENDER;		//��Ⱦģʽ
public:
	PICKING();			// ��ʼ��
	OBJ* objs;			// ��ʰȡ����
	int obj_count;		// ��ʰȡ��������
	void MousePick(int button, int state, int x, int y, GLfloat width, GLfloat height);		// �����ʰȡʵ��
	void DrawPickObject();	// ���ƴ�ʰ
};



#endif // !PICKING_H_

