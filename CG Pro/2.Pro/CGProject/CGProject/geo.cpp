#include"geo.h"

void GEO::drawPrism(GLfloat h, GLfloat r, int n)
{
	
	if (vArr)
		delete[] vArr;
	vArr = new GLfloat *[2 + 2 * n];
	fvArr = new int *[3 * n];
	vnArr = new GLfloat *[2 + n];
	fnArr = new int *[3 * n];
	v_num = 2 + 2 * n;
	vn_num = 2 + n;
	f_num = 3 * n;
	int i;
	double Angle = 0;
	for (i = 0; i < 2 + 2 * n; i++)
		vArr[i] = new GLfloat[3];
	for (i = 0; i < 3 * n; i++)
	{
		fvArr[i] = new int[5];
		fnArr[i] = new int[5];
	}
	for (i = 0; i < n + 2; i++)
		vnArr[i] = new GLfloat[3];



	vArr[0][0] = vArr[0][1] = vArr[0][2] = vArr[n + 1][0] = vArr[n + 1][2] = 0;
	vArr[n + 1][1] = h;
	for (i = 0; i < n; i++)
	{
		vArr[i + 1][0] = vArr[i + n + 2][0] = r * sin(Angle * 3.1415926 / 180.0);
		vArr[i + 1][1] = 0;
		vArr[i + n + 2][1] = h;
		vArr[i + 1][2] = vArr[i + n + 2][2] = r * cos(Angle * 3.1415926 / 180.0);
		Angle += 360.0 / n;

		fvArr[i][0] = fvArr[i + n][0] = 3;
		fvArr[i][1] = 1;
		fvArr[i + n][1] = n + 2;
		fvArr[i][2] = i + 2;
		fvArr[i][3] = (i + 1) % n + 2;
		fvArr[i + n][2] = i + n + 3;
		fvArr[i + n][3] = (i + 1) % n + n + 3;

		fvArr[i + 2 * n][0] = 4;
		fvArr[i + 2 * n][1] = i + 2;
		fvArr[i + 2 * n][2] = (i + 1) % n + 2;
		fvArr[i + 2 * n][3] = (i + 1) % n + n + 3;
		fvArr[i + 2 * n][4] = i + n + 3;

		fnArr[i][0] = fnArr[i + n][0] = 3;
		fnArr[i][1] = fnArr[i][2] = fnArr[i][3] = 1;
		fnArr[i + n][1] = fnArr[i + n][2] = fnArr[i + n][3] = 2;
		fnArr[i + 2 * n][1] = fnArr[i + 2 * n][2] = fnArr[i + 2 * n][3] = fnArr[i + 2 * n][4] = i + 3;
	}
	vnArr[0][0] = vnArr[0][2] = vnArr[1][0] = vnArr[1][2] = 0;
	vnArr[0][1] = -1.0;
	vnArr[1][1] = 1.0;
	for (i = 2; i < n + 2; i++)
	{
		Angle = (i - 2.0) * 360.0 / n + 180.0 / n;
		vnArr[i][0] = sin(Angle * 3.1415926 / 180.0);
		vnArr[i][1] = 0.0;
		vnArr[i][2] = cos(Angle * 3.1415926 / 180.0);
	}
	for (int i = 0; i < 3 * n; i++)
	{
		if (fvArr[i][0] == 3)
		{
			glBegin(GL_TRIANGLES);

			glNormal3f(vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1],
				vnArr[fnArr[i][1] - 1][2]);
			glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1],
				vArr[fvArr[i][1] - 1][2]);

			glNormal3f(vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1],
				vnArr[fnArr[i][2] - 1][2]);
			glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1],
				vArr[fvArr[i][2] - 1][2]);

			glNormal3f(vnArr[fnArr[i][3] - 1][0], vnArr[fnArr[i][3] - 1][1],
				vnArr[fnArr[i][3] - 1][2]);
			glVertex3f(vArr[fvArr[i][3] - 1][0], vArr[fvArr[i][3] - 1][1],
				vArr[fvArr[i][3] - 1][2]);

			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);

			glNormal3f(vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1],
				vnArr[fnArr[i][1] - 1][2]);
			glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1],
				vArr[fvArr[i][1] - 1][2]);

			glNormal3f(vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1],
				vnArr[fnArr[i][2] - 1][2]);
			glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1],
				vArr[fvArr[i][2] - 1][2]);

			glNormal3f(vnArr[fnArr[i][3] - 1][0], vnArr[fnArr[i][3] - 1][1],
				vnArr[fnArr[i][3] - 1][2]);
			glVertex3f(vArr[fvArr[i][3] - 1][0], vArr[fvArr[i][3] - 1][1],
				vArr[fvArr[i][3] - 1][2]);

			glNormal3f(vnArr[fnArr[i][4] - 1][0], vnArr[fnArr[i][4] - 1][1],
				vnArr[fnArr[i][4] - 1][2]);
			glVertex3f(vArr[fvArr[i][4] - 1][0], vArr[fvArr[i][4] - 1][1],
				vArr[fvArr[i][4] - 1][2]);
			//cout << fvArr[i][1] << ", " << fvArr[i][2] << ", " << fvArr[i][3] << ", " <<  fvArr[i][4] <<  endl;
			glEnd();
		}
	}
}

void GEO::outputObj(const string& filename)
{
	ofstream file(filename, ios::out | ios::trunc);
	if (!file.is_open())
	{
		cout << "File Create Failed!" << endl;
		return;
	}
	file << "# This obj file is generated by geo.h" << endl;
	int i, j;
	//vArr = new GLfloat * [2 + 2 * n];
	//fvArr = new int* [3 * n];
	//vnArr = new GLfloat * [3 * n];
	//fnArr = new int* [3 * n];
	for (i = 0; i < v_num; i++)
		file << "v " << vArr[i][0] << ' ' << vArr[i][1] << ' ' << vArr[i][2] << endl;
	for (i = 0; i < vn_num; i++)
		file << "vn " << vnArr[i][0] << ' ' << vnArr[i][1] << ' ' << vnArr[i][2] << endl;
	file << "g prism" << endl;
	for (i = 0; i < f_num; i++)
	{
		file << "f ";
		for (j = 0; j < fvArr[i][0]; j++)
		{
			file << fvArr[i][j + 1] << "//" << fnArr[i][j + 1] << ' ';
		}
		file << endl;
	}
	file.close();
}

void GEO::drawBall(GLUquadricObj *uquadric,GLfloat r, GLfloat cradix, GLfloat hradix) {
	uquadric = gluNewQuadric();
	glPushMatrix();
	gluSphere(uquadric, r, cradix, hradix);
	glPopMatrix();
}

void GEO::drawPyramid(GLfloat h, GLfloat r1, GLfloat r2, int n)
{
	int i;
	double Angle = 0;
	for (i = 0; i < n; i++)
	{
		glBegin(GL_TRIANGLES);

		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);

		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(r1 * sin(Angle * 3.1415926 / 180.0), 0.0,
			r1 * cos(Angle * 3.1415926 / 180.0));

		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(r1 * sin((Angle + 360.0 / n) * 3.1415926 / 180.0), 0.0,
			r1 * cos((Angle + 360.0 / n) * 3.1415926 / 180.0));

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, h, 0.0);

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(r2 * sin(Angle * 3.1415926 / 180.0), h,
			r2 * cos(Angle * 3.1415926 / 180.0));

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(r2 * sin((Angle + 360.0 / n) * 3.1415926 / 180.0), h,
			r2 * cos((Angle + 360.0 / n) * 3.1415926 / 180.0));

		glEnd();

		glBegin(GL_QUADS);

		glNormal3f(r1 * sin((Angle + 180.0 / n) * 3.1415926 / 180.0), r1 * (r2 - r1) / h,
			r1 * cos((Angle + 180.0 / n) * 3.1415926 / 180.0));
		glVertex3f(r1 * sin(Angle * 3.1415926 / 180.0), 0.0,
			r1 * cos(Angle * 3.1415926 / 180.0));

		glNormal3f(r1 * sin((Angle + 180.0 / n) * 3.1415926 / 180.0), r1 * (r2 - r1) / h,
			r1 * cos((Angle + 180.0 / n) * 3.1415926 / 180.0));
		glVertex3f(r1 * sin((Angle + 360.0 / n) * 3.1415926 / 180.0), 0.0,
			r1 * cos((Angle + 360.0 / n) * 3.1415926 / 180.0));

		glNormal3f(r1 * sin((Angle + 180.0 / n) * 3.1415926 / 180.0), r1 * (r2 - r1) / h,
			r1 * cos((Angle + 180.0 / n) * 3.1415926 / 180.0));
		glVertex3f(r2 * sin((Angle + 360.0 / n) * 3.1415926 / 180.0), h,
			r2 * cos((Angle + 360.0 / n) * 3.1415926 / 180.0));

		glNormal3f(r1 * sin((Angle + 180.0 / n) * 3.1415926 / 180.0), r1 * (r2 - r1) / h,
			r1 * cos((Angle + 180.0 / n) * 3.1415926 / 180.0));
		glVertex3f(r2 * sin(Angle * 3.1415926 / 180.0), h,
			r2 * cos(Angle * 3.1415926 / 180.0));

		glEnd();

		Angle += 360.0 / n;
	}
}