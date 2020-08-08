/*
* This file 'UI.h' is the header of 'UI.cpp'
* is to handle the UI， like showing text or draw sightbead ……
* an opengl program
* Please note that this is a part of a joint effort of
* the group work for the Final Project of Computer Graphics
* Course 2020 Spring-Summer Semester Session
* Copyright C Fantast
* https://github.com/Fantast416
*/
#ifndef _UI_H_
#define _UI_H_
#define PI_ 3.14159265358979323846 
/************************************************************************
* 使用说明：
* 见下每个函数前的使用说明
*
*************************************************************************/
#include<stdio.h>
#include <stdlib.h>
#include "glut.h"
#include<Windows.h>
#include<iostream>
#include<ctime>
#include <time.h>
#define BITMAP_ID 0x4D42
#pragma warning(disable:4996)
/*****************************************************************************
******************************************************************************
*在视口的posx,posy处显示FPS以及需要显示的文字，整个视口大小为(0,0)——(400,400)*
******************************************************************************
*****************************************************************************/
float getFPS(int posx,int posy);
void ShowText(char* text,int posx,int posy);
/********************************************************************************
*********************************************************************************
*截图函数，请视窗口大小决定生成图片大小 例：窗口400*400，图片也必须的输入400*400*
*********************************************************************************
********************************************************************************/
bool SnapScreen(int width, int height);
/********************************************************************************
*********************************************************************************
*绘制准星函数参数说明见下*
*********************************************************************************
********************************************************************************/
//posx posy 中心坐标 
//r 相对于标准的放缩比例
//circlewidth 外圈圆厚度
//srecwidth 内部小长方条宽度
//brecwidth 外部小长方条宽度
//ccirdle 中心圆半径
//scircle 内圈圆半径
//bcircle 外圈圆半径
//brecheight 外部长方条到达的最外偏移量
void DrawCircle2(float cx, float cy, float r, int num_segments);
void DrawSightBead(GLuint swidth,GLuint sheight,int posx,int posy,float r,int circlewidth,float srecwidth,float brecwidth,
	float ccircle,float scircle, float bcircle,float brecheight);
/********************************************************************************
*********************************************************************************
*播放音乐函数
*********************************************************************************
********************************************************************************/
void PlayMusic(char* filename);  //播放文件路径需在函数里改。 
#endif