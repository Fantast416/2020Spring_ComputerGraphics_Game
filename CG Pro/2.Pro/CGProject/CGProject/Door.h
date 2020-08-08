/*
* This file 'Door.h' is the header of 'Door.cpp'
* is to handle the Door Obj, and the animation of the door opening and closing
* Also with a Boxes to detect whether there is a people is near
* an opengl program
* Please note that this is a part of a joint effort of
* the group work for the Final Project of Computer Graphics
* Course 2020 Spring-Summer Semester Session
* Copyright C Fantast
* https://github.com/Fantast416
*/

#ifndef _DOOR_H_
#define _DOOR_H_
/************************************************************************
* 使用说明：
*
* 1）
* 建议在全局变量区：
* Door door1;
* 建立空类
*
* 2）
* 在init()函数或者main()函数中设置内容：
* door_name.SetDoorInfo(…………)
* door_name.SetBoxesData(…………);
*
* 3）
* 在绘制函数中使用：
*	if (door_name.CollisionDetect(m_Camera.getPosition(), m_Camera.m_Box)) {
*		door_name.PressECallBack();
*	}
*   来进行检测 人是否到达该该扇门周围
* 使用下列函数
*   door_name.FlahsCallBack()
*   通过判断当前门的状态，来进行门的绘制
* Debug记录：
* 2020/6/4
* 
*************************************************************************/
#define PI_ 3.14159265358979323846 
#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include <iostream>
#include <ctime>
#include <time.h>
#include "obJ.h"
#include "Vector.h" 
#include "UI.h"
#pragma warning(disable:4996)
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //必要的
class DOOR {
private:
	OBJ RealDoor;
	OBJ Boxes;
	double posx;     //门要绕的旋转的轴的x z坐标
	double posz;
	int            Boxes_Count;
	GLfloat**      Boxes_Coords;
	int angle;
	bool IsDoorOpen;		//判断门的当前状态
	bool IsDoorOpening;     
	bool IsDoorClosing;
	long long int  count;	//用于计数
	void AddAngle();
	void SubAngle();
	void draw(int angle);
public:
	void FlashCallBack();	//通过获取当前门的状态
	void PressECallBack();  //通过碰撞检测，来判断是否显示E
	void SetDoorInfo(const string& RealOBJName, const string& RealBMPName, const string& BoxesfileName, double x, double z);  //设置门的参数信息
	bool CollisionDetect(Vector3 m_Position, Vector3 m_Box);   //碰撞检测
	void SetBoxesData(int count, GLfloat** data);  //设置Box数据
};

#endif