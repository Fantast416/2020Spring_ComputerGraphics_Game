/*
* This file 'obj.h' is the header of 'obj.cpp'
* is to handle reading .obj file and import model into
* an opengl program
* Please note that this is a part of a joint effort of
* the group work for the Final Project of Computer Graphics
* Course 2020 Spring-Summer Semester Session
* Copyright C LuniumLuk
* https://github.com/LuniumLuk
* http://lunium.cn/
*/
#ifndef __OBJ__H__
#define __OBJ__H__

/************************************************************************
* 使用说明：
*
* 1）
* 建议在全局变量区：
* OBJ obj_name;
* 建立空类
*
* 2）
* 在init()函数或者main()函数中设置内容：
* obj_name.setContent("XXX.obj","XXX.bmp");
*
* 3）
* 在绘制函数中使用：
* obj_name.draw(); // 绘制图形
*
* Debug记录：
* 现在全部默认为读取含有四边面（最大支持为四边面）的obj文件
* 支持UV读取，应用于贴图
* 2020/6/4
* 将贴图功能内置到类中，使用大大简化
*************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include "glut.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <stdio.h>
using namespace std;
#define BITMAP_ID 0x4D42
#define MAXTEXT 10

/************************************************************************
* 小笔记：OBJ文件的结构
* OBJ文件每一行的格式如下：
* 前缀 参数1 参数2 参数3
*
* v（顶点）X Y Z（坐标值）
* vt（纹理坐标） U Y（值）*
* vn（法线向量） X Y Z（坐标值）
* f（表面）具体如下：
* f 1 2 3 表示以第1、2、3号顶点组成一个三角形面
* f 1/3 2/5 3/4 表示同上，同时其顶点的纹理坐标的索引值分别为3、5、4
* f 1/3/4 2/5/6 3/4/2 表示同上，同时其顶点的法线索引值分别为4、6、2
* f 1//4 2//6 3//2 则表示忽略纹理坐标
* 注意：纹理坐标从1开始记录，而不是从0开始
*
* 注意：maya的文件请预先处理为三角形面，不要有四边面
*************************************************************************/

class OBJ {
private:
	/* 类相关变量 */
	static int 	Count;				// 记录类的数量（类静态）
	int 		Num;				// 记录类的编号
	string 		sFileName;			// 读取的OBJ的文件名
	string 		sTextureName;		// 读取的纹理贴图的文件名
	string		sTextureName2;		// 读取的纹理贴图的文件名，支持读入两张贴图
	bool 		EmptyOBJ = true;	// OBJ文件是否未空（未读入/未设置文件名）
	bool 		EmptyTEXT = true;	// 纹理文件是否为空（未读入/未设置文件名）

	/* OBJ相关变量 */
	int 		v_num;				// 记录点的数量
	int 		vn_num;				// 记录法线的数量
	int 		vt_num;				// 记录UV的数量
	int 		f_num;				// 记录面的数量
	GLfloat** 	vArr;				// 存放点的二维数组
	GLfloat** 	vnArr;				// 存放法线的二维数组
	GLfloat** 	vtArr;				// 存放UV的二维数组
	int** 		fvArr;				// 存放面顶点的二维数组
	int** 		fnArr;				// 存放面法线的二维数组
	int** 		ftArr;				// 存放面UV的二维数组

	/* 纹理贴图相关变量 */
	GLuint 				tex = 0;								// 用于gl生成纹理
	unsigned char* 		bitmapImage = NULL;						// bitmap图像数据
	unsigned char* 		bitmapImage2 = NULL;					// bitmap图像数据2
	unsigned char* 		bitmapImageMix = NULL;					// 混合模式下的bitmap图像数据
	BITMAPINFOHEADER 	bitmapInfoHeader;						// 图像信息头
	BITMAPINFOHEADER 	bitmapInfoHeader2;						// 图像信息头2
	GLfloat 			AMBI_COLOR[4] = { 1.0, 1.0, 1.0, 1.0 };	// 默认全局反射颜色（白色）
	GLfloat 			SPEC_COLOR[4] = { 1.0, 1.0, 1.0, 1.0 }; // 默认镜面反射颜色（白色）
	GLfloat 			SHININESS = 0.0;						// 默认镜面系数（0.0）

	/* 碰撞检测盒相关变量 */
	/************************************************************************
	* 碰撞检测说明：
	* OBJ类只负责读入碰撞盒OBJ文件
	* 并且通过getBoxCount()函数获取碰撞盒的个数
	* 以及getBoxCoords()函数获取碰撞盒的对角坐标
	* 具体用法请参见下面函数的注释
	*************************************************************************/
	int Boxes_Count = 0;
	GLfloat** Boxes_Coords = NULL;

	/*-----------------------------------------------------------------------------------
	**	私有类型成员函数
	*/

	/* 
	** 	统计.obj文件中的顶点、法线点、UV点以及面的数量
	**	存入v_num, vt_num, vn_num, f_num中
	*/
	void getLineNum();

	/* 
	** 	读取.obj文件中的信息
	**	存入vArr, vtArr, vnArr, fvArr, ftArr, fnArr中
	*/
	int readFile();

	/*
	** 	读取BMP文件并且返回Bitmap，用于opengl绑定贴图
	*/
	void LoadBitmapFile();

	/*
	** 	读取第二张BMP文件并且返回Bitmap，用于opengl绑定贴图
	**	绑定时使用texload2()
	*/
	void LoadBitmapFile2();

	/*
	**	绑定贴图（第一张读入的贴图）
	*/
	void texload();

public:

	/*-----------------------------------------------------------------------------------
	**	构造以及析构函数
	*/
	OBJ();

	/*
	**	以下未较少用的构造函数，不建议使用
	*/
	OBJ(const string& fileName);
	OBJ(const string& fileName, const string& textureName);
	OBJ(const OBJ& o);
	~OBJ();

	/*-----------------------------------------------------------------------------------
	**	初始化/读取文件相关函数
	*/

	/*
	**	仅初始化文件名，不读取文件
	*/
	void setFileAddr(const string& obj, const string& tex);

	/*
	**	初始化文件名并且读取obj和bmp纹理贴图文件，若data==1，则读取data类型的模型文件
	**	data类型模型文件请参见下面相关注释
	*/
	void setContent(const string& obj, const string& tex, int data = 0);

	/*
	**	读取data类型模型文件
	*/
	void ReadCompact(const string& filename);

	/*
	**	读取已初始化文件名的obj文件
	*/
	void readObj();
	
	/*
	**	读取已初始化文件名的纹理贴图文件
	*/
	void readTexture();
	
	/*
	**	读入第二张纹理贴图文件
	*/
	void readTexture2(const string& filename);

	/*-----------------------------------------------------------------------------------
	**	模型/贴图参数设置相关函数
	*/

	/*
	**	设置镜面系数
	*/
	void setShininess(GLfloat S);

	/*
	**	设置环境光成分
	*/
	void setAmbiColor(GLfloat C[]);

	/*
	**	设置镜面光成分
	*/
	void setSpecColor(GLfloat C[]);

	/*
	** 	加载第二张贴图，需要先读入第二张贴图
	*/
	void texload2();

	/*
	**	混合两张贴图，其中第一张贴图的权重为 alpha，第二张为 1-alpha
	**	读入两张贴图后调用
	*/
	void texMix(float alpha);

	/*-----------------------------------------------------------------------------------
	**	输出相关函数
	*/

	/*
	**	在opengl中绘制该obj图形
	*/
	void draw();

	/*
	**	将已读入的obj文件输出为data文件
	*/
	/************************************************************************
	* data文件说明：
	* 因为读入obj文件时，需要先遍历一遍文件以获得相关点、面数量
	* 以初始化内存，因此耗时较长
	* 因此，提供将obj文件输出为新类型文件，建议.data
	* 使用前述的方式读入模型可以节约大概一半的时间
	*************************************************************************/
	void WriteToFile(const string& filename);
	
	/*-----------------------------------------------------------------------------------
	**	碰撞检测盒相关函数
	*/

	/*
	**	读入碰撞盒obj文件，该模型应该只包含若干长方体，否则会产生错误
	*/
	void ReadBoxes(const string& filename);

	/*
	**	获取读入的碰撞盒文件中长方体的数量
	*/
	int getBoxesCount();

	/*
	**	获取读入的碰撞盒文件中的各个长方体的对角坐标，用于Camera中的碰撞检测
	*/
	GLfloat** getBoxesCoords();
};

/*
**	在console显示加载进度条，显示进度为 N/count
*/
void displayProgress(int N, int count);

#endif