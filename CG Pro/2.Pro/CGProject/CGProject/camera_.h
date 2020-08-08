#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
/************************************************************************
* ʹ��˵����
*
* 1��
* ������ȫ�ֱ�������
* Camera m_Camera;
* ����һ���������������
*
* 2��
* ��init()�����г�ʼ�����������
* m_Camera.setCamera(0.0f, 10.0f, 60.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f);
*
* 3��
* ��redraw()���ƺ����з��������������һϵ�й������������Ӧ������
* m_Camera.MotionKeyboard();	// ����asdw�ƶ��ļ��̶���
* m_Camera.makeJump();		    // ������Ծ
* m_Camera.setLook();           // �������
* m_Camera.SpotSet();           // �����ֵ�Ͳ
*
*************************************************************************/

#define ANGLE_Y 1000.0f     // Y���ϵ����������
#define ANGLE_Z 2000.0f     // Z���ϵ����������
#define MSPEED 20.0f        // ������˶��ٶ�
#define JUMPDURATION 30     // ��Ծ����
#define ZOOM_SIZE 20        // �����̶�
#define SHOOTINGBOUNCE 50   // ǹ֧��̧����
#define BounceHeight 0.1    // ǹ֧��̧�߶�

#include <windows.h>
#include <iostream>
#include <glut.h>           
#include <stdio.h>
#include "Vector.h"                 /** ����������ͷ�ļ� */
#include "UI.h"     
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) // �ж�ĳ�����Ƿ��ڰ���״̬
/** ������� */
class Camera
{
public:
    /** ���캯������������ */
    Camera();
    ~Camera();

    /* ��������״̬���� */
    Vector3 getPosition() { return m_Position; }        // ����������ǰλ��
    Vector3 getView() { return m_View; }                // ����������ǰ����
    Vector3 getUpVector() { return m_UpVector; }        // ����������ǰ��������
    float   getSpeed();                                 // ����������ǰ�˶��ٶ�

    /* ����������ٶ� */
    void setSpeed(float speed){ m_Speed = speed; }

    /* �����������λ��, �۲����������� */
    void setCamera(float positionX, float positionY, float positionZ,       // ��ʼ�������
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);
    void SetPosition(Vector3 vPosition) { m_Position = vPosition; }         // �����������ǰλ��
    void SetView(Vector3 vView) { m_View = vView; }                         // �����������ǰ����
    void SetUSetViewpVector(Vector3 vUpVector) { m_UpVector = vUpVector; }  // �����������ǰ��������

    /* �ֵ�Ͳ���� */
	void SpotSet();

    /* ��ת��������� */
    void rotateView(float angle, float X, float Y, float Z);

    /* �����ת������۲췽�� */
    void setViewByMouse();                          // ������궯���������������
    void SetMouseMove(bool state);                  // ���õ�ǰ���״̬
    bool GetMouseMoveState();                       // ��õ�ǰ���״̬

    /** ����������ƶ� */
    void yawCamera(float speed);

    /** ǰ���ƶ������ */
    void moveCamera(float speed);

    /* ��Ծʵ�� */
    void makeJump();

	void jumpCamera();

    /* ǹ֧��̧�������� */
    void shootingBounce();

    /* ���ʱ������Ч */
    void Shot() { shooting = SHOOTINGBOUNCE; 
	PlayMusic((char*)"music/shoot_boom.wav");
	}

    /* ��������ʵ�� */
    void downCamera();      // ����
    void upCamera();        // ����

    /** ��������� **/
    void setLook();

    /* ���̻ص� */
    void CameraKeyboard(int key, int x, int y);     // �ƹ⣬��Ծ������ȶ����ļ��̻ص�����
    void MotionKeyboard();                          // �����ǰ�������˶����ٶȿ�����ؼ��̻ص� ��ʵ��˫����Ӧ�ƶ�

    /* ����FPSֵ */
    void setFPS(float fps);

    /* ��ײ������ */
    Vector3        m_Box;         /* ��ײ������ */
    void SetBoxesData(int count, GLfloat** data);   // ������ײ������
    void InitializeCD(float x, float y, float z);   // ��ʼ�����������ײ��
    bool CollisionDetect();                         // ��ײ�ж�
	long long int  count;

    /* ������� */
    void SetZoom() {                 // �ı���ƿ����ı���zoom
        zoom = !zoom;
    }
private:
    /** ��������� */
    Vector3        m_Position;      /* λ�� */
    Vector3        m_View;          /* ���� */
    Vector3        m_UpVector;      /* �������� */
    float          m_Speed;         /* �˶��ٶ� */
    bool           SeeMouseMove;    /* ����Ƿ��ƶ� */
    bool           upordown;        /* �Ƿ���� */
    bool           spot_on = false; /* �ֵ�Ͳ�Ƿ���*/
    int            Boxes_Count;     /* ��ײ������ */
    GLfloat**      Boxes_Coords;    /* ��ײ������ */
    float          FPS;             /* ��ǰ֡�� */
    bool           zoom = false;    /* �Ƿ񿪾� */
    int            shooting = 0;    /* ǹ֧��̧������ */
};

#endif //__CAMERA_H__
