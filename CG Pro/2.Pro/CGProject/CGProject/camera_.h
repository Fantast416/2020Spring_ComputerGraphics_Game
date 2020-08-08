#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
/************************************************************************
* 使用说明：
*
* 1）
* 建议在全局变量区：
* Camera m_Camera;
* 创建一个摄像机用于漫游
*
* 2）
* 在init()函数中初始化相机参数：
* m_Camera.setCamera(0.0f, 10.0f, 60.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f);
*
* 3）
* 在redraw()绘制函数中放置摄像机，处理一系列关于摄像机的响应动作：
* m_Camera.MotionKeyboard();	// 处理asdw移动的键盘动作
* m_Camera.makeJump();		    // 处理跳跃
* m_Camera.setLook();           // 放置相机
* m_Camera.SpotSet();           // 设置手电筒
*
*************************************************************************/

#define ANGLE_Y 1000.0f     // Y轴上的鼠标灵敏度
#define ANGLE_Z 2000.0f     // Z轴上的鼠标灵敏度
#define MSPEED 20.0f        // 摄像机运动速度
#define JUMPDURATION 30     // 跳跃距离
#define ZOOM_SIZE 20        // 开镜程度
#define SHOOTINGBOUNCE 50   // 枪支上抬次数
#define BounceHeight 0.1    // 枪支上抬高度

#include <windows.h>
#include <iostream>
#include <glut.h>           
#include <stdio.h>
#include "Vector.h"                 /** 包含向量类头文件 */
#include "UI.h"     
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) // 判断某个键是否处于按下状态
/** 摄像机类 */
class Camera
{
public:
    /** 构造函数和析构函数 */
    Camera();
    ~Camera();

    /* 获得摄像机状态方法 */
    Vector3 getPosition() { return m_Position; }        // 获得摄像机当前位置
    Vector3 getView() { return m_View; }                // 获得摄像机当前朝向
    Vector3 getUpVector() { return m_UpVector; }        // 获得摄像机当前向上向量
    float   getSpeed();                                 // 获得摄像机当前运动速度

    /* 设置摄像机速度 */
    void setSpeed(float speed){ m_Speed = speed; }

    /* 设置摄像机的位置, 观察点和向上向量 */
    void setCamera(float positionX, float positionY, float positionZ,       // 初始化摄像机
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);
    void SetPosition(Vector3 vPosition) { m_Position = vPosition; }         // 设置摄像机当前位置
    void SetView(Vector3 vView) { m_View = vView; }                         // 设置摄像机当前朝向
    void SetUSetViewpVector(Vector3 vUpVector) { m_UpVector = vUpVector; }  // 设置摄像机当前向上向量

    /* 手电筒设置 */
	void SpotSet();

    /* 旋转摄像机方向 */
    void rotateView(float angle, float X, float Y, float Z);

    /* 鼠标旋转摄像机观察方向 */
    void setViewByMouse();                          // 根据鼠标动作设置摄像机朝向
    void SetMouseMove(bool state);                  // 设置当前鼠标状态
    bool GetMouseMoveState();                       // 获得当前鼠标状态

    /** 左右摄像机移动 */
    void yawCamera(float speed);

    /** 前后移动摄像机 */
    void moveCamera(float speed);

    /* 跳跃实现 */
    void makeJump();

	void jumpCamera();

    /* 枪支上抬次数减少 */
    void shootingBounce();

    /* 射击时播放音效 */
    void Shot() { shooting = SHOOTINGBOUNCE; 
	PlayMusic((char*)"music/shoot_boom.wav");
	}

    /* 蹲下起立实现 */
    void downCamera();      // 蹲下
    void upCamera();        // 起立

    /** 放置摄像机 **/
    void setLook();

    /* 键盘回调 */
    void CameraKeyboard(int key, int x, int y);     // 灯光，跳跃，蹲起等动作的键盘回调函数
    void MotionKeyboard();                          // 摄像机前后左右运动及速度控制相关键盘回调 可实现双键响应移动

    /* 设置FPS值 */
    void setFPS(float fps);

    /* 碰撞检测相关 */
    Vector3        m_Box;         /* 碰撞盒坐标 */
    void SetBoxesData(int count, GLfloat** data);   // 设置碰撞盒坐标
    void InitializeCD(float x, float y, float z);   // 初始化玩家自身碰撞盒
    bool CollisionDetect();                         // 碰撞判断
	long long int  count;

    /* 开镜相关 */
    void SetZoom() {                 // 改变控制开镜的变量zoom
        zoom = !zoom;
    }
private:
    /** 摄像机属性 */
    Vector3        m_Position;      /* 位置 */
    Vector3        m_View;          /* 朝向 */
    Vector3        m_UpVector;      /* 向上向量 */
    float          m_Speed;         /* 运动速度 */
    bool           SeeMouseMove;    /* 鼠标是否移动 */
    bool           upordown;        /* 是否蹲下 */
    bool           spot_on = false; /* 手电筒是否开启*/
    int            Boxes_Count;     /* 碰撞盒数量 */
    GLfloat**      Boxes_Coords;    /* 碰撞盒坐标 */
    float          FPS;             /* 当前帧率 */
    bool           zoom = false;    /* 是否开镜 */
    int            shooting = 0;    /* 枪支上抬计数器 */
};

#endif //__CAMERA_H__
