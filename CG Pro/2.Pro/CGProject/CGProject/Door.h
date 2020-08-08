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
* ʹ��˵����
*
* 1��
* ������ȫ�ֱ�������
* Door door1;
* ��������
*
* 2��
* ��init()��������main()�������������ݣ�
* door_name.SetDoorInfo(��������)
* door_name.SetBoxesData(��������);
*
* 3��
* �ڻ��ƺ�����ʹ�ã�
*	if (door_name.CollisionDetect(m_Camera.getPosition(), m_Camera.m_Box)) {
*		door_name.PressECallBack();
*	}
*   �����м�� ���Ƿ񵽴�ø�������Χ
* ʹ�����к���
*   door_name.FlahsCallBack()
*   ͨ���жϵ�ǰ�ŵ�״̬���������ŵĻ���
* Debug��¼��
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
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //��Ҫ��
class DOOR {
private:
	OBJ RealDoor;
	OBJ Boxes;
	double posx;     //��Ҫ�Ƶ���ת�����x z����
	double posz;
	int            Boxes_Count;
	GLfloat**      Boxes_Coords;
	int angle;
	bool IsDoorOpen;		//�ж��ŵĵ�ǰ״̬
	bool IsDoorOpening;     
	bool IsDoorClosing;
	long long int  count;	//���ڼ���
	void AddAngle();
	void SubAngle();
	void draw(int angle);
public:
	void FlashCallBack();	//ͨ����ȡ��ǰ�ŵ�״̬
	void PressECallBack();  //ͨ����ײ��⣬���ж��Ƿ���ʾE
	void SetDoorInfo(const string& RealOBJName, const string& RealBMPName, const string& BoxesfileName, double x, double z);  //�����ŵĲ�����Ϣ
	bool CollisionDetect(Vector3 m_Position, Vector3 m_Box);   //��ײ���
	void SetBoxesData(int count, GLfloat** data);  //����Box����
};

#endif