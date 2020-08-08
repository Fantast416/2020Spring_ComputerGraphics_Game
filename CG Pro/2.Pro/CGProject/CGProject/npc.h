/*
* This file 'npc.h' is the header of 'npc.cpp'
* is to handle NPC modeling/Moving and Interacting
* Please note that this is a part of a joint effort of
* the group work for the Final Project of Computer Graphics
* Course 2020 Spring-Summer Semester Session
* Copyright C LuniumLuk
* https://github.com/LuniumLuk
* http://lunium.cn/
*/

#pragma once
#include "obJ.h"
#include "vector.h"
#include "glut.h"
#include <math.h>
#include <ctime>

#define N_SPEED 			200.0	// 默认移动速度设置
#define PICK_TOL 			5
#define PICK_BUFFER_SIZE 	256
#define RES_TIME 			2000	// 默认复活时长设置

class NPC {
private:
	Vector3 v_position = Vector3(0.0, 0.0, 0.0);		// NPC初始位置坐标矢量
	Vector3 v_rand = Vector3(0.0, 0.0, 0.0);			// NPC随机移动坐标矢量
	Vector3 v_Scale = Vector3(1.0, 1.0, 1.0);			// NPC缩放矢量
	float 	Para1 = 0, Para2 = 0, Para3 = 0, Para4 = 0;	// NPC移动相关参数
	
	float 	FPS;			// 游戏FPS，用于调整移动速度
	float 	Speed;			// NPC移动速度
	
	int 	HP = 10;
	int 	Respawn = 0;
	
	/*
	**	鼠标点击检测相关变量
	*/
	int 			PickNum = 1;	
	unsigned int 	SelectBuffer[PICK_BUFFER_SIZE];
	int 			hits;
	int 			viewport[4];
	GLenum 			RenderMode = GL_RENDER;
	bool 			Pick = true;
public:
	OBJ 	objHit;
	OBJ* 	obj;
	int 	obj_num = 0;
	string 	name;			// NPC名称

	/*
	**	构造和析构函数
	*/
	NPC();
	NPC(const string& name);

	/*
	**	参数设置类函数
	*/

	/* 设置鼠标点击编号 */
	void setPickNum(int num) { PickNum = num; }

	/* 设置NPC初始位置 */
	void setPosition(float x, float y, float z);

	/* 设置NPC比例矢量 */
	void setScale(float x, float y, float z);

	/* 设置随机数种子（取系统时间） */
	void setRandSeed();

	/* 设置需要读入的OBJ文件的数量，支持读入多个OBJ */
	void setObjNum(int n);

	/* 修改默认速度 */
	void setSpeed(float speed) { Speed = speed; }

	/* 设置FPS，需要在redraw函数中不断更新FPS信息 */
	void setFPS(float fps) { FPS = fps; }

	/*
	**	移动/绘制类函数
	*/

	/*
	**	NPC随机移动函数
	**	x1,x2 	-----------	x坐标移动范围
	**	y		-----------	y坐标（随机移动在y轴上不移动）
	**	z1,z2	-----------	z坐标移动范围
	**	speed	-----------	设置移动速度（在默认速度基础上，1.0即为默认速度，2.0即为两倍默认速度）
	*/
	void RandMove(float x1, float x2, float y, float z1, float z2, float speed);

	/* NPC在初始位置不动 */
	void Static();

	/*
	**	NPC环状运动
	**	r为环绕半径，speed为环绕速度（默认速度基础上）
	*/
	void CircularMove(float r, float speed);

	/*
	**	NPC朝某一坐标走去（带范围限制）
	**	x1,x2 	-----------	x坐标移动范围
	**	y		-----------	y坐标（随机移动在y轴上不移动）
	**	z1,z2	-----------	z坐标移动范围
	**	x,y,z	-----------	目标坐标
	**	speed	-----------	设置移动速度（在默认速度基础上，1.0即为默认速度，2.0即为两倍默认速度）
	*/
	void Chase(float x1, float x2, float z1, float z2, float x, float y, float z, float speed);

	/* NPC复活 */
	void resurrect();

	/* 在NPC头顶显示NPC的血量，需要输入玩家的坐标信息 */
	void DisplayHP( float x, float y, float z);

	void DrawPika(float x1, float x2, float y, float z1, float z2, float speed);

	/*
	**	辅助类函数
	*/

	/*
	**	判断NPC是否能发现玩家
	**	angle	--------------	NPC左/右视野广度
	**	sight	--------------	NPC视野范围（距离）
	**	x,y,z	--------------	玩家坐标
	*/
	bool Found(float angle, float sight, float x, float y, float z);

	/* 获取速度 */
	float getSpeed();

	/* 获取NPC当前HP */
	int getHP() { return HP; }

	/*
	**	鼠标点击检测相关函数
	*/

	void MousePick(int button, int state, int x, int y, GLfloat width, GLfloat height);
	void RandMoveDraw();
	void disablePick();

};

