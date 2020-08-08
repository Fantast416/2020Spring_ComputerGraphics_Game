#include"Door.h"

void DOOR::FlashCallBack() {
	if (IsDoorOpen == false && IsDoorOpening == false && IsDoorClosing == false) {
		draw(0);
	}
	else if (IsDoorOpen == false && IsDoorOpening == true) {
		AddAngle();
		draw(angle);
	}
	else if (IsDoorOpen == true && IsDoorClosing == true) {
		SubAngle();
		draw(angle);
	}
	else if (IsDoorOpen == true && IsDoorOpening == false && IsDoorClosing == false) {
		draw(-90);
	}
}
void DOOR::PressECallBack() {
	if (IsDoorOpen == false) {
		ShowText((char*)"Press E to Open the Door", 160, 240);
		if (KEY_DOWN(69)) {
			PlayMusic((char*)"music/open_door.wav");
			IsDoorOpening = true;        //调整门开关状态
		} 
	}
	else {
		ShowText((char*)"Press E to Close the Door", 160, 240);
		if (KEY_DOWN(69)) {
			PlayMusic((char*)"music/open_door.wav");
			IsDoorClosing = true;
		}
	}

}
void DOOR::AddAngle() { 
	if (angle >= -90) {
		angle--;
	}
	else {
		IsDoorOpen = true;       //结束开门
		IsDoorOpening = false;
	}
}
void DOOR::SubAngle() {
	if (angle <= 0)
		angle++;
	else {
		IsDoorOpen = false; 
		IsDoorClosing = false;     //结束关门
	}
}
void DOOR::SetDoorInfo(const string& RealOBJName, const string& RealBMPName, const string& BoxesfileName, double x, double z) {
	RealDoor.setContent(RealOBJName, RealBMPName);
	Boxes.ReadBoxes(BoxesfileName);
	SetBoxesData(Boxes.getBoxesCount(), Boxes.getBoxesCoords());
	IsDoorOpen = false;
	IsDoorClosing = false;
	IsDoorOpening = false;
	posx = x;
	posz = z;
	angle = 0;

}
void DOOR::draw(int angle) {
	glPushMatrix();
	glTranslatef(posx, 0.0f, posz);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(-posx, 0.0f, -posz);
	RealDoor.draw();
	glPopMatrix();
}

bool DOOR::CollisionDetect(Vector3 m_Position, Vector3 m_Box) {
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
			return true;
		}

	}
	return false;
}
void DOOR::SetBoxesData(int count, GLfloat** data)
{
	Boxes_Count = count;
	Boxes_Coords = new GLfloat *[Boxes_Count];
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