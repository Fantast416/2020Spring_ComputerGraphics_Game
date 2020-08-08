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
* ʹ��˵����
*
* 1��
* ������ȫ�ֱ�������
* OBJ obj_name;
* ��������
*
* 2��
* ��init()��������main()�������������ݣ�
* obj_name.setContent("XXX.obj","XXX.bmp");
*
* 3��
* �ڻ��ƺ�����ʹ�ã�
* obj_name.draw(); // ����ͼ��
*
* Debug��¼��
* ����ȫ��Ĭ��Ϊ��ȡ�����ı��棨���֧��Ϊ�ı��棩��obj�ļ�
* ֧��UV��ȡ��Ӧ������ͼ
* 2020/6/4
* ����ͼ�������õ����У�ʹ�ô���
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
* С�ʼǣ�OBJ�ļ��Ľṹ
* OBJ�ļ�ÿһ�еĸ�ʽ���£�
* ǰ׺ ����1 ����2 ����3
*
* v�����㣩X Y Z������ֵ��
* vt���������꣩ U Y��ֵ��*
* vn������������ X Y Z������ֵ��
* f�����棩�������£�
* f 1 2 3 ��ʾ�Ե�1��2��3�Ŷ������һ����������
* f 1/3 2/5 3/4 ��ʾͬ�ϣ�ͬʱ�䶥����������������ֵ�ֱ�Ϊ3��5��4
* f 1/3/4 2/5/6 3/4/2 ��ʾͬ�ϣ�ͬʱ�䶥��ķ�������ֵ�ֱ�Ϊ4��6��2
* f 1//4 2//6 3//2 ���ʾ������������
* ע�⣺���������1��ʼ��¼�������Ǵ�0��ʼ
*
* ע�⣺maya���ļ���Ԥ�ȴ���Ϊ�������棬��Ҫ���ı���
*************************************************************************/

class OBJ {
private:
	/* ����ر��� */
	static int 	Count;				// ��¼����������ྲ̬��
	int 		Num;				// ��¼��ı��
	string 		sFileName;			// ��ȡ��OBJ���ļ���
	string 		sTextureName;		// ��ȡ��������ͼ���ļ���
	string		sTextureName2;		// ��ȡ��������ͼ���ļ�����֧�ֶ���������ͼ
	bool 		EmptyOBJ = true;	// OBJ�ļ��Ƿ�δ�գ�δ����/δ�����ļ�����
	bool 		EmptyTEXT = true;	// �����ļ��Ƿ�Ϊ�գ�δ����/δ�����ļ�����

	/* OBJ��ر��� */
	int 		v_num;				// ��¼�������
	int 		vn_num;				// ��¼���ߵ�����
	int 		vt_num;				// ��¼UV������
	int 		f_num;				// ��¼�������
	GLfloat** 	vArr;				// ��ŵ�Ķ�ά����
	GLfloat** 	vnArr;				// ��ŷ��ߵĶ�ά����
	GLfloat** 	vtArr;				// ���UV�Ķ�ά����
	int** 		fvArr;				// ����涥��Ķ�ά����
	int** 		fnArr;				// ����淨�ߵĶ�ά����
	int** 		ftArr;				// �����UV�Ķ�ά����

	/* ������ͼ��ر��� */
	GLuint 				tex = 0;								// ����gl��������
	unsigned char* 		bitmapImage = NULL;						// bitmapͼ������
	unsigned char* 		bitmapImage2 = NULL;					// bitmapͼ������2
	unsigned char* 		bitmapImageMix = NULL;					// ���ģʽ�µ�bitmapͼ������
	BITMAPINFOHEADER 	bitmapInfoHeader;						// ͼ����Ϣͷ
	BITMAPINFOHEADER 	bitmapInfoHeader2;						// ͼ����Ϣͷ2
	GLfloat 			AMBI_COLOR[4] = { 1.0, 1.0, 1.0, 1.0 };	// Ĭ��ȫ�ַ�����ɫ����ɫ��
	GLfloat 			SPEC_COLOR[4] = { 1.0, 1.0, 1.0, 1.0 }; // Ĭ�Ͼ��淴����ɫ����ɫ��
	GLfloat 			SHININESS = 0.0;						// Ĭ�Ͼ���ϵ����0.0��

	/* ��ײ������ر��� */
	/************************************************************************
	* ��ײ���˵����
	* OBJ��ֻ���������ײ��OBJ�ļ�
	* ����ͨ��getBoxCount()������ȡ��ײ�еĸ���
	* �Լ�getBoxCoords()������ȡ��ײ�еĶԽ�����
	* �����÷���μ����溯����ע��
	*************************************************************************/
	int Boxes_Count = 0;
	GLfloat** Boxes_Coords = NULL;

	/*-----------------------------------------------------------------------------------
	**	˽�����ͳ�Ա����
	*/

	/* 
	** 	ͳ��.obj�ļ��еĶ��㡢���ߵ㡢UV���Լ��������
	**	����v_num, vt_num, vn_num, f_num��
	*/
	void getLineNum();

	/* 
	** 	��ȡ.obj�ļ��е���Ϣ
	**	����vArr, vtArr, vnArr, fvArr, ftArr, fnArr��
	*/
	int readFile();

	/*
	** 	��ȡBMP�ļ����ҷ���Bitmap������opengl����ͼ
	*/
	void LoadBitmapFile();

	/*
	** 	��ȡ�ڶ���BMP�ļ����ҷ���Bitmap������opengl����ͼ
	**	��ʱʹ��texload2()
	*/
	void LoadBitmapFile2();

	/*
	**	����ͼ����һ�Ŷ������ͼ��
	*/
	void texload();

public:

	/*-----------------------------------------------------------------------------------
	**	�����Լ���������
	*/
	OBJ();

	/*
	**	����δ�����õĹ��캯����������ʹ��
	*/
	OBJ(const string& fileName);
	OBJ(const string& fileName, const string& textureName);
	OBJ(const OBJ& o);
	~OBJ();

	/*-----------------------------------------------------------------------------------
	**	��ʼ��/��ȡ�ļ���غ���
	*/

	/*
	**	����ʼ���ļ���������ȡ�ļ�
	*/
	void setFileAddr(const string& obj, const string& tex);

	/*
	**	��ʼ���ļ������Ҷ�ȡobj��bmp������ͼ�ļ�����data==1�����ȡdata���͵�ģ���ļ�
	**	data����ģ���ļ���μ��������ע��
	*/
	void setContent(const string& obj, const string& tex, int data = 0);

	/*
	**	��ȡdata����ģ���ļ�
	*/
	void ReadCompact(const string& filename);

	/*
	**	��ȡ�ѳ�ʼ���ļ�����obj�ļ�
	*/
	void readObj();
	
	/*
	**	��ȡ�ѳ�ʼ���ļ�����������ͼ�ļ�
	*/
	void readTexture();
	
	/*
	**	����ڶ���������ͼ�ļ�
	*/
	void readTexture2(const string& filename);

	/*-----------------------------------------------------------------------------------
	**	ģ��/��ͼ����������غ���
	*/

	/*
	**	���þ���ϵ��
	*/
	void setShininess(GLfloat S);

	/*
	**	���û�����ɷ�
	*/
	void setAmbiColor(GLfloat C[]);

	/*
	**	���þ����ɷ�
	*/
	void setSpecColor(GLfloat C[]);

	/*
	** 	���صڶ�����ͼ����Ҫ�ȶ���ڶ�����ͼ
	*/
	void texload2();

	/*
	**	���������ͼ�����е�һ����ͼ��Ȩ��Ϊ alpha���ڶ���Ϊ 1-alpha
	**	����������ͼ�����
	*/
	void texMix(float alpha);

	/*-----------------------------------------------------------------------------------
	**	�����غ���
	*/

	/*
	**	��opengl�л��Ƹ�objͼ��
	*/
	void draw();

	/*
	**	���Ѷ����obj�ļ����Ϊdata�ļ�
	*/
	/************************************************************************
	* data�ļ�˵����
	* ��Ϊ����obj�ļ�ʱ����Ҫ�ȱ���һ���ļ��Ի����ص㡢������
	* �Գ�ʼ���ڴ棬��˺�ʱ�ϳ�
	* ��ˣ��ṩ��obj�ļ����Ϊ�������ļ�������.data
	* ʹ��ǰ���ķ�ʽ����ģ�Ϳ��Խ�Լ���һ���ʱ��
	*************************************************************************/
	void WriteToFile(const string& filename);
	
	/*-----------------------------------------------------------------------------------
	**	��ײ������غ���
	*/

	/*
	**	������ײ��obj�ļ�����ģ��Ӧ��ֻ�������ɳ����壬������������
	*/
	void ReadBoxes(const string& filename);

	/*
	**	��ȡ�������ײ���ļ��г����������
	*/
	int getBoxesCount();

	/*
	**	��ȡ�������ײ���ļ��еĸ���������ĶԽ����꣬����Camera�е���ײ���
	*/
	GLfloat** getBoxesCoords();
};

/*
**	��console��ʾ���ؽ���������ʾ����Ϊ N/count
*/
void displayProgress(int N, int count);

#endif