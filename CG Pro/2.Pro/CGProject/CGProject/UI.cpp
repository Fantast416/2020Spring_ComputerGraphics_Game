#include "UI.h"
#pragma warning(disable:4996)
float getFPS(int posx,int posy)   //默认在左下角显示fps，可以进行调整          
{  
	glDisable(GL_LIGHTING);
	static int frame = 0, time, timebase = 0;
	static char buffer[256];
	char mode[64];
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer, "FPS:%4.2f",
			frame * 1000.0 / ((float)time - timebase));
		timebase = time;		
		frame = 0;
	}
	char *c;
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0,400,0,400,-1,1);    // 位置正投影   0~400 0~400 -1~1  xmin xmax ymin ymax zmin zmax
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(posx,posy);     //在视口的x,y处显示字体
	for (c=buffer; *c != '\0'; c++) {		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_LIGHTING);
	return frame * 1000.0 / (time - timebase);
}

bool SnapScreen(int width, int height)
{
	FILE *fp;            //文件指针
	unsigned char *imgdata;
	BITMAPINFOHEADER bi;
	BITMAPFILEHEADER bf;
	bi.biBitCount = 24;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	bi.biCompression = 0;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biSize = 40;
	bi.biSizeImage = 0;
	bi.biWidth = width;
	bi.biXPelsPerMeter = 10000;
	bi.biYPelsPerMeter = 10000;
	bf.bfOffBits = 54;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	bf.bfSize = width * height * 3 + 54;
	bf.bfType = 0x4d42;
	imgdata = (unsigned char *)malloc(sizeof(unsigned char)*bf.bfSize);
	if (imgdata == NULL)
	{
		free(imgdata);
		printf("Exception: No enough space!\n");
		return false;
	}
	//像素格式设置4字节对齐
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//接收出像素的数据
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgdata);
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	int second = p->tm_sec;
	int year = p->tm_year +1900;
	int mon = p->tm_mon+1;
	int day = p->tm_mday;
	int hour = p->tm_hour + 8;
	int minute = p->tm_min;
	char cyear[30] = "";
	char cmonth[30] = "";
	char cday[30] = "";
	char chour[30] = "";
	char cminute[30] = "";
	char csecond[30] = "";
	char filename[50] = "Snapscreen/";
	itoa(year, cyear, 10);
	itoa(mon, cmonth, 10);
	itoa(day, cday, 10);
	itoa(hour, chour, 10);
	itoa(minute, cminute, 10);
	strcat(filename, cyear);
	strcat(filename, cmonth);
	strcat(filename, cday);
	strcat(filename, chour);
	strcat(filename, cminute);
	strcat(filename, "_SnapScreen.bmp");
	puts(filename);
	fp = fopen(filename, "wb+");
	if (fp == NULL)
	{
		printf("Exception: Fail to open file!\n");
		return false;
	}
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(imgdata, bf.bfSize, 1, fp);
	free(imgdata);
	fclose(fp);
	return true;
}


void DrawCircle2(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= num_segments; i++)
	{
		glVertex2f(cx + r * cos((2 * PI_*i) / num_segments),
			cy + r * sin((2 * PI_*i) / num_segments));
	}
	glEnd();
}

//posx posy 中心坐标 
//r 相对于标准的放缩比例
//circlewidth 外圈圆厚度
//srecwidth 内部小长方条宽度
//brecwidth 外部小长方条宽度
//ccirdle 中心圆半径
//scircle 内圈圆半径
//bcircle 外圈圆半径
//brecheight 外部长方条到达的最外偏移量
void DrawSightBead(GLuint swidth, GLuint sheight,int posx,int posy,float r,int circlewidth,float srecwidth,float brecwidth,
	float ccircle,float scircle, float bcircle,float brecheight) {
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);

	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, swidth, 0, sheight, -1, 1);  // 位置正投影    
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	
	glBegin(GL_QUADS);
	glVertex2f(posx + srecwidth * r, posy + ccircle * r);
	glVertex2f(posx + srecwidth * r, posy + scircle * r);
	glVertex2f(posx - srecwidth * r, posy + scircle * r);
	glVertex2f(posx - srecwidth * r, posy + ccircle * r);
	glEnd();

	DrawCircle2(posx, posy, ccircle * r, 360);
	DrawCircle2(posx, posy, scircle * r, 360);
	glBegin(GL_QUADS);
	glVertex3f(posx + brecwidth * r, posy + scircle * r, 0);
	glVertex3f(posx + brecwidth * r, posy + brecheight * r, 0);
	glVertex3f(posx - brecwidth * r, posy + brecheight * r, 0);
	glVertex3f(posx - brecwidth * r, posy + scircle * r, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(posx + srecwidth * r, posy - ccircle * r, 0);
	glVertex3f(posx + srecwidth * r, posy - scircle * r, 0);
	glVertex3f(posx - srecwidth * r, posy - scircle * r, 0);
	glVertex3f(posx - srecwidth * r, posy - ccircle * r, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(posx + brecwidth * r, posy - scircle * r, 0);
	glVertex3f(posx + brecwidth * r, posy - brecheight * r, 0);
	glVertex3f(posx - brecwidth * r, posy - brecheight * r, 0);
	glVertex3f(posx - brecwidth * r, posy - scircle * r, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(posx + ccircle * r, posy + srecwidth * r, 0);
	glVertex3f(posx + scircle * r, posy + srecwidth * r, 0);
	glVertex3f(posx + scircle * r, posy - srecwidth * r, 0);
	glVertex3f(posx + ccircle * r, posy - srecwidth * r, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(posx + scircle * r, posy + brecwidth * r, 0);
	glVertex3f(posx + brecheight * r, posy + brecwidth * r, 0);
	glVertex3f(posx + brecheight * r, posy - brecwidth * r, 0);
	glVertex3f(posx + scircle * r, posy - brecwidth * r, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(posx - ccircle * r, posy + srecwidth * r, 0);
	glVertex3f(posx - scircle * r, posy + srecwidth * r, 0);
	glVertex3f(posx - scircle * r, posy - srecwidth * r, 0);
	glVertex3f(posx - ccircle * r, posy - srecwidth * r, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(posx - scircle * r, posy + brecwidth * r, 0);
	glVertex3f(posx - brecheight * r, posy + brecwidth * r, 0);
	glVertex3f(posx - brecheight * r, posy - brecwidth * r, 0);
	glVertex3f(posx - scircle * r, posy - brecwidth * r, 0);
	glEnd();


	for (int j = 0; j < circlewidth; j++) {
		DrawCircle2(posx, posy, bcircle*r + j*0.33, 360);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_LIGHTING);
}
void ShowText(char* text, int posx, int posy)  //显示的文字内容，视口位置x，视口位置y,整个视口坐标为400*400,(0,0)在左下角
{
	char *c;
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 400, 0, 400, -1, 1);  // 位置正投影   0~400 0~400 -1~1  xmin xmax ymin ymax zmin zmax
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(posx, posy);           //在视口的左下角显示字体
	for (c = text; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_LIGHTING);
}

void PlayMusic(char *filename) {
	FILE *fp = fopen(filename, "rb+");
	if (fp == NULL) {
		printf("Music Not Found!");
	}
	else {
		PlaySound(TEXT(filename), NULL, SND_FILENAME | SND_ASYNC );
	}
}