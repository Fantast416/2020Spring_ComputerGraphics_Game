/*
* This file 'geo.h' is the header of 'geo.cpp'
* is to handle geometry models' drawing and output as an obj file
* Please note that this is a part of a joint effort of
* the group work for the Final Project of Computer Graphics
* Course 2020 Spring-Summer Semester Session
* Copyright C LuniumLuk
* https://github.com/LuniumLuk
* http://lunium.cn/
*/

#pragma once
#include <iostream>
#include <math.h>
#include <string.h>
#include <fstream>
#include "glut.h"
using namespace std;
#define GEO_PRISM 0

class GEO {
private:
	GLfloat** 	vArr;		// 储存点的坐标
	GLfloat** 	vnArr;		// 储存法线限向量
	int** 		fvArr;		// 储存面-点关系
	int** 		fnArr;		// 储存面-法线向量关系
	int 		v_num;		// 点的数量
	int			vn_num;		// 法线向量的数量
	int 		f_num;		// 面的数量
	int 		r1, r2, r3;	// 储存一些参数
public:

	/* 绘制一个多棱柱，其中高为h，底面外接圆半径为r，侧棱数为n */
	void drawPrism(GLfloat h, GLfloat r, int n);

	/* 将绘制的几何体导出为obj格式文件 */
	void outputObj(const string& filename);

	/* 绘制一个球体 */
	void drawBall(GLUquadricObj *uquadric, GLfloat r, GLfloat cradix, GLfloat hradix);
	
	/* 绘制一个多棱台，高为h，下底面外接圆半径为r1，上底面外接圆半径为r2，侧棱数为n */
	void drawPyramid(GLfloat h, GLfloat r1, GLfloat r2, int n);
};