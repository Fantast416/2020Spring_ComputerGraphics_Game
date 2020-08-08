#include "Camera_.h"                    /**< ���������ͷ�ļ� */

int Jump = 0;
int SecondJump = 0;
float JumpSpeed = 1.0;
float DashSpeed = 1.0;
extern bool light_on;
extern bool lightcolor;
void Camera::makeJump()
{
    JumpSpeed = 1.0;
    if (Jump)
    {
        JumpSpeed = 2.0;
        if (SecondJump)
        {
            if (SecondJump > JUMPDURATION)
            {
                m_Position.y += 5.0 / JUMPDURATION;
                m_View.y += 5.0 / JUMPDURATION;
            }
            else
            {
                m_Position.y -= 5.0 / JUMPDURATION;
                m_View.y -= 5.0 / JUMPDURATION;
            }
            SecondJump--;
        }
        else {
            if (Jump > JUMPDURATION)
            {
                m_Position.y += 5.0 / JUMPDURATION;
                m_View.y += 5.0 / JUMPDURATION;
            }
            else
            {
                m_Position.y -= 5.0 / JUMPDURATION;
                m_View.y -= 5.0 / JUMPDURATION;
            }
            Jump--;
        }
        //printf("[%d]", Jump);
    }
}

/** ���캯�� */
Camera::Camera()
{

    /** ��ʼ������ֵ */
    Vector3 zero = Vector3(0.0, 0.0, 0.0);
    Vector3 view = Vector3(0.0, 1.0, 0.5);
    Vector3 up = Vector3(0.0, 0.0, 1.0);

    /** ��ʼ������� */
    m_Position = zero;
    m_View = view;
    m_UpVector = up;
    m_Speed = MSPEED;
	SeeMouseMove = true;
	count = 0;
}

Camera::~Camera()
{
}

/** �����������λ��,������������� */
void Camera::setCamera(float positionX, float positionY, float positionZ,
    float viewX, float viewY, float viewZ,
    float upVectorX, float upVectorY, float upVectorZ)
{
    /** �������� */
    Vector3 Position = Vector3(positionX, positionY, positionZ);
    Vector3 View = Vector3(viewX, viewY, viewZ);
    Vector3 UpVector = Vector3(upVectorX, upVectorY, upVectorZ);

    /** ��������� */
    m_Position = Position;
    m_View = View;
    m_UpVector = UpVector;
}

void Camera::SpotSet() {

    float x = m_Position.x;
    float y = m_Position.y;
    float z = m_Position.z;
    float viewx = m_View.x;
    float viewy = m_View.y;
    float viewz = m_View.z;
    float dd = 10;
    double Distance = pow(pow(viewx - x, 2) + pow(viewy - y, 2) + pow(viewz - z, 2), 0.5);
    double ddx = dd * ((double)viewx - x) / Distance;
    double ddy = dd * ((double)viewy - y) / Distance;
    double ddz = dd * ((double)viewz - z) / Distance;

    GLfloat spot_pos[] = { 1.0,1.0,1.0,1.0 };
    spot_pos[0] = x;
    spot_pos[1] = y;
    spot_pos[2] = z;
    
    GLfloat spot_direction[3];
    spot_direction[0] = ddx;
    spot_direction[1] = ddy;
    spot_direction[2] = ddz;
    //printf("%lf,%lf,%lf\n", spot_pos[0], spot_pos[1], spot_pos[2]);
    //printf("%lf,%lf,%lf,%lf\n", vector.x, vector.y, vector.z, atan(vector.x / vector.z) * 180 / 3.14159);
	GLfloat light_color[] = { 1.0,1.0,1.0,1.0 };
	if (lightcolor == true) {
		light_color[0] = 1.0;
		light_color[1] = 0.0;
		light_color[2] = 0.0;
		light_color[3] = 1.0;
	}
	GLfloat white[] = { 1.0,1.0,1.0,1.0 };

	GLfloat Angle = 15.0f;

    //glPushMatrix();
    //glTranslatef(ddx, ddy, ddz);
    //glutSolidCube(1.0);
    

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white); //���þ����ɷ�
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white); //���������ɷ�

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction); //��Դ����
    glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, Angle); //�ü��Ƕ�
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0); //�ۼ���
	/*if (spot_on)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);*/
    //glPopMatrix();
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

/**  ��ת���������  */
void Camera::rotateView(float angle, float x, float y, float z)
{
    Vector3 newView;

    /** ���㷽������ */
    Vector3 view = m_View - m_Position;

    /** ���� sin ��cosֵ */
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    /** ������ת������xֵ */
    newView.x = (cosTheta + (1 - cosTheta) * x * x) * view.x;
    newView.x += ((1 - cosTheta) * x * y - z * sinTheta) * view.y;
    newView.x += ((1 - cosTheta) * x * z + y * sinTheta) * view.z;

    /** ������ת������yֵ */
    newView.y = ((1 - cosTheta) * x * y + z * sinTheta) * view.x;
    newView.y += (cosTheta + (1 - cosTheta) * y * y) * view.y;
    newView.y += ((1 - cosTheta) * y * z - x * sinTheta) * view.z;

    /** ������ת������zֵ */
    newView.z = ((1 - cosTheta) * x * z - y * sinTheta) * view.x;
    newView.z += ((1 - cosTheta) * y * z + x * sinTheta) * view.y;
    newView.z += (cosTheta + (1 - cosTheta) * z * z) * view.z;

    /** ����������ķ��� */
    m_View = m_Position + newView;
}
bool Camera::GetMouseMoveState() {
	return this->SeeMouseMove;
}

/** ��������ת */
void Camera::setViewByMouse()
{

    POINT mousePos;                                   /**< ���浱ǰ���λ�� */
    int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< �õ���Ļ��ȵ�һ�� */
    int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< �õ���Ļ�߶ȵ�һ�� */
    float angleY = 0.0f;                              /**< �����������ת�Ƕ� */
    float angleZ = 0.0f;                              /**< �����������ת�Ƕ� */
    static float currentRotX = 0.0f;

    /** �õ���ǰ���λ�� */
    GetCursorPos(&mousePos);
	ShowCursor(false);
	this->count--;
    /** ������û���ƶ�,���ø��� */
    if ((mousePos.x == middleX) && (mousePos.y == middleY))
        return;

    /** �������λ������Ļ���� */
    SetCursorPos(middleX, middleY);

    /**< �õ�����ƶ����� */
    angleY = (float)((middleX - mousePos.x)) / ANGLE_Y;
    angleZ = (float)((middleY - mousePos.y)) / ANGLE_Z;

    static float lastRotX = 0.0f;      /**< ���ڱ�����ת�Ƕ� */
    lastRotX = currentRotX;

    /** ���������������ת�Ƕ� */
    currentRotX += angleZ;

        /** �ҵ�����ת����ֱ���� */
        Vector3 vAxis = m_View - m_Position;
        vAxis = vAxis.crossProduct(m_UpVector);
        vAxis = vAxis.normalize();

        ///��ת
        rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);

    /** ����������ת����� */
    rotateView(angleY, 0, 1, 0);
}

/** �����ƶ������ */
void Camera::yawCamera(float speed)
{
    Vector3 Cross = m_UpVector.crossProduct(m_View - m_Position);
    Cross = Cross.normalize();

    m_Position.x += Cross.x * speed * JumpSpeed * DashSpeed;
    m_Position.z += Cross.z * speed * JumpSpeed * DashSpeed;
    m_View.x += Cross.x * speed * JumpSpeed * DashSpeed;
    m_View.z += Cross.z * speed * JumpSpeed * DashSpeed;
    if (CollisionDetect())
    {
        /* �ع� */
        m_Position.x -= Cross.x * speed * JumpSpeed * DashSpeed;
        m_Position.z -= Cross.z * speed * JumpSpeed * DashSpeed;
        m_View.x -= Cross.x * speed * JumpSpeed * DashSpeed;
        m_View.z -= Cross.z * speed * JumpSpeed * DashSpeed;
    }
}

float Camera::getSpeed()
{
    //std::cout << FPS << ' ' << m_Speed << isnan(FPS) << std::endl;
    if (FPS && !isnan(FPS))
        return m_Speed / FPS;
    else
        return m_Speed / 100.0;
}

void Camera::jumpCamera()
{

	/** ��������� */
	m_Position.y += 3 ;    /**< �����ٶȸ���λ�� */
	m_View.y += 3;
	for (long long int i = 0; i < 10000000;i++) {
		i*i*i*i;
	}
	m_Position.y -= 3;    /**< �����ٶȸ���λ�� */
	m_View.y -= 3;
}


/** ǰ���ƶ������ */
void Camera::moveCamera(float speed)
{
    /** ���㷽������ */
    Vector3 vector = m_View - m_Position;
    vector = vector.normalize();         /**< ��λ�� */

    //printf("x,y,z: %lf, %lf, %lf\n", vector.x, vector.y, vector.z);
    /** ��������� */
    m_Position.x += vector.x * speed * JumpSpeed * DashSpeed;    /**< �����ٶȸ���λ�� */
    m_Position.z += vector.z * speed * JumpSpeed * DashSpeed;
    m_View.x += vector.x * speed * JumpSpeed * DashSpeed;        /**< �����ٶȸ��·��� */
    m_View.z += vector.z * speed * JumpSpeed * DashSpeed;
    if (CollisionDetect())
    {
        /* �ع� */
        m_Position.x -= vector.x * speed * JumpSpeed * DashSpeed;    /**< �����ٶȸ���λ�� */
        m_Position.z -= vector.z * speed * JumpSpeed * DashSpeed;
        m_View.x -= vector.x * speed * JumpSpeed * DashSpeed;        /**< �����ٶȸ��·��� */
        m_View.z -= vector.z * speed * JumpSpeed * DashSpeed;
    }
}


void Camera::SetMouseMove(bool state){
	this->SeeMouseMove = state;
}

void Camera::MotionKeyboard()
{
    //bool bmoved = false;
    //87: w  65:a  83:s  68:d  
    //DashSpeed = 1.0;
    if (KEY_DOWN(87)) {
        moveCamera(getSpeed()); //ǰ��
        //bmoved = true;
    }
    if (KEY_DOWN(83)) {
        moveCamera(-getSpeed()); //����
        //bmoved = true;
    }
    if (KEY_DOWN(65)) {
        yawCamera(getSpeed()); //����
        //bmoved = true;
    }
    if (KEY_DOWN(68)) {
        yawCamera(-getSpeed()); //����
        //bmoved = true;
    }
    if (KEY_DOWN(16))
    {
        DashSpeed = 3.0;
    }
    else
    {
        DashSpeed = 1.0;
    }
}

/** ���̻ص����� */
void Camera::CameraKeyboard(int key, int x, int y){
    
    if (key == 27) { //ESC
        this->SetMouseMove(false);
        count--;
        while (count != 0) {
            ShowCursor(true);
            count++;
        }
        return;
    }

    switch (key)
    {
        case 'q':
        {
            exit(0);
            break; 
        }
        case 'c':
        {
            if (upordown == false) {
                downCamera();
            }
            else {
                upCamera();
            }
            break;
        }
        case 'o':
        {
            spot_on = !spot_on;
            break;
        }
        case ' ':
        {
            if (!Jump)
                Jump = JUMPDURATION * 2;
            else
            {
                if (!SecondJump)
                    SecondJump = JUMPDURATION * 2;
            }
            break;
        }
        case 'l':
        {
            light_on = !light_on;
            break;
        }
		case '1':
		{
			lightcolor = !lightcolor;
		}
		case'2':
		{

		}
		case'3':
		{

		}
    }
}

void Camera::setFPS(float fps)
{
    FPS = fps;
}

/*����*/
void Camera::downCamera() {
		m_Position.y -= 5.0;
		m_View.y -= 5.0;
		upordown = true;
}

/*����*/
void Camera::upCamera() {
	if (upordown == true) {
		m_Position.y += 5.0;
		m_View.y += 5.0;
		upordown = false;
	}
}

void Camera::SetBoxesData(int count, GLfloat** data)
{
    Boxes_Count = count;
    Boxes_Coords = new GLfloat * [Boxes_Count];
    int i, j;
    for (i = 0; i < Boxes_Count; i++)
    {
        Boxes_Coords[i] = new GLfloat[6];
    }
    for (i = 0; i < Boxes_Count; i++)
    {
        for (j = 0; j < 6; j++)
        {
            Boxes_Coords[i][j] = data[i][j];
        }
    }
}

void Camera::InitializeCD(float x, float y, float z)
{
    m_Box = Vector3(x, y, z);
}

bool Camera::CollisionDetect()
{
    int i;
    float x = m_Position.x, y = m_Position.y, z = m_Position.z;
    float x_max = x + m_Box.x, y_max = y + m_Box.y, z_max = z + m_Box.z;
    float x_min = x - m_Box.x, y_min = y - m_Box.y, z_min = z - m_Box.z;
    //printf("[%lf,%lf,%lf]", x, y, z);
    for (i = 0; i < Boxes_Count; i++)
    {
        if (!(x_min >= Boxes_Coords[i][0] || y_min >= Boxes_Coords[i][1] || z_min >= Boxes_Coords[i][2] ||
            x_max <= Boxes_Coords[i][3] || y_max <= Boxes_Coords[i][4] || z_max <= Boxes_Coords[i][5]))
        {
            //printf("[(%lf, %lf, %lf), (%lf, %lf, %lf)] - ", x_max, y_max, z_max, x_min, y_min, z_min);
            //printf("[(%lf, %lf, %lf), (%lf, %lf, %lf)]\n", Boxes_Coords[i][0], Boxes_Coords[i][1], 
            //    Boxes_Coords[i][2], Boxes_Coords[i][3], Boxes_Coords[i][4], Boxes_Coords[i][5]);
            return true;
        }
           
    }
    return false;
}

/** �����ӵ� */
void Camera::setLook()
{
    /** �����ӿ� */
    if (zoom) {
        /** ���㷽������ */
        //printf("zoom start\n");
        Vector3 vector = m_View - m_Position;
        vector = vector.normalize();         /**< ��λ�� */
        Vector3 pos, view;

        //printf("\nVector x,y,z: %lf, %lf, %lf\n", vector.x, vector.y, vector.z);
        /** ��������� */
        pos.x = m_Position.x + vector.x * ZOOM_SIZE;    /**< �����ٶȸ���λ�� */
        pos.z = m_Position.z + vector.z * ZOOM_SIZE;
        pos.y = m_Position.y;
        view.x = m_View.x + vector.x * ZOOM_SIZE;       /**< �����ٶȸ��·��� */
        view.z = m_View.z + vector.z * ZOOM_SIZE;
        view.y = m_View.y;
        //printf("pos x y z:%lf %lf %lf\n", pos.x, pos.y, pos.z);
       // printf("m_pos x y z:%lf %lf %lf\n", m_Position.x, m_Position.y, m_Position.z);
        //printf("view x y z:%lf %lf %lf\n", view.x, view.y, view.z);
        //printf("m_view x y z:%lf %lf %lf\n", m_View.x, m_View.y, m_View.z);
        gluLookAt(pos.x, pos.y, pos.z,
            view.x, view.y + shooting * BounceHeight, view.z,
            m_UpVector.x, m_UpVector.y, m_UpVector.z);
    }
    else {
        //printf("zoom end\n");
        gluLookAt(m_Position.x, m_Position.y, m_Position.z,
            m_View.x, m_View.y + shooting * BounceHeight, m_View.z,
            m_UpVector.x, m_UpVector.y, m_UpVector.z);
    }

}

void Camera::shootingBounce()
{
    if (shooting)
    {
        shooting--;
    }
}