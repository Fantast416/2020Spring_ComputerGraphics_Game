#ifndef PICKING_H_
#define PICKING_H
/************************************************************************
* 使用说明：
*
* 拾取机制实现
*
*************************************************************************/

#include "obj.h"
#include "glut.h"
#include <stdio.h>
#define PICK_TOL 5				/* 拾取窗口大小 */
#define PICK_BUFFER_SIZE 256	/* 拾取缓存数组大小 */

class PICKING {
private:
	unsigned int SelectBuffer[PICK_BUFFER_SIZE];  // 拾取缓存数组
	int hits;							// 拾取数量返回
	int viewport[4];					// 视口
	GLenum RenderMode = GL_RENDER;		//渲染模式
public:
	PICKING();			// 初始化
	OBJ* objs;			// 被拾取物体
	int obj_count;		// 被拾取物体数量
	void MousePick(int button, int state, int x, int y, GLfloat width, GLfloat height);		// 鼠标点击拾取实现
	void DrawPickObject();	// 绘制待拾
};



#endif // !PICKING_H_

