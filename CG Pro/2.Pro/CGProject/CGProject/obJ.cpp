#include "obj.h"
int OBJ::Count = 0;

void displayProgress(int N, int count)
{
	int progress, j;
	if (N > 100 && count % (N / 100) == 0)
	{
		progress = (int)((double)count * 100 / N);
		printf("Reading Graph: %d%% [", progress);
		for (j = 0; j < progress / 5; j++)
			printf("*");
		for (j = 0; j < 20 - progress / 5; j++)
			printf(" ");
		printf("] ");
		switch ((progress / 2) % 4)
		{
		case 0: printf("-"); break;
		case 1: printf("/"); break;
		case 2: printf("|"); break;
		case 3: printf("\\"); break;
		}
		printf("\r");
		fflush(stdout);
	}
	if (N == count)
	{
		printf("Reading Graph: 100%% [********************]   \n");
		printf("Reading Graph: Complete!\n");
	}
}

void OBJ::setFileAddr(const string& obj, const string& tex)
{
	sFileName = obj;
	sTextureName = tex;
}

void OBJ::setContent(const string& obj, const string& tex, int data)
{
	sFileName = obj;
	sTextureName = tex;
	if (data)
	{
		ReadCompact(sFileName);
		readTexture();
	}
	else
	{
		readObj();
		readTexture();
	}
}

void OBJ::readObj()
{
	getLineNum();
	if (!readFile())
	{
		cout << "[" << Num << "][OBJ] - Read obj file: " << sFileName << " succeed!" << endl;
		cout << "[" << Num << "][OBJ] - v_num: " << v_num << ", vn_num: " << vn_num << ", vt_num: " << vt_num << ", f_num: " << f_num << endl;
	}
	EmptyOBJ = false;
}

void OBJ::readTexture()
{
	glGenTextures(1, &tex);
	texload();
	EmptyTEXT = false;
}

void OBJ::readTexture2(const string& filename)
{
	sTextureName2 = filename;
	LoadBitmapFile2();
	cout << "Successfully read Texture2!" << endl;
}

OBJ::OBJ() : sFileName("NULL"), sTextureName("NULL")
{
	v_num = vn_num = vt_num = f_num = 0;
	vArr = vnArr = vtArr = nullptr;
	fvArr = fnArr = ftArr = nullptr;
	Num = Count;
	Count++;
	bitmapInfoHeader = { 0,0,0,0,0,0,0,0,0,0,0 };
	cout << ">>> OBJ (Blank) Generated, NO." << Num << endl;
}

OBJ::OBJ(const string& fileName) : sFileName(fileName)
{
	v_num = vn_num = f_num = 0;
	vArr = vnArr = nullptr;
	fvArr = fnArr = nullptr;
	Num = Count;
	Count++;
	readObj();
	cout << ">>> OBJ (Without Texture) Generated, NO." << Num << endl;
}


OBJ::OBJ(const string& fileName, const string& textureName) : sFileName(fileName), sTextureName(textureName)
{
	v_num = vn_num = vt_num = f_num = 0;
	vArr = vnArr = vtArr = nullptr;
	fvArr = fnArr = ftArr = nullptr;
	Num = Count;
	Count++;
	bitmapInfoHeader = { 0,0,0,0,0,0,0,0,0,0,0 };
	readObj();
	readTexture();
	cout << ">>> OBJ Generated, NO." << Num << endl;
}

OBJ::OBJ(const OBJ& o) : sFileName(o.sFileName), sTextureName(o.sTextureName)
{
	Num = Count;
	Count++;
	EmptyOBJ = o.EmptyOBJ;
	EmptyTEXT = o.EmptyTEXT;
	if (!EmptyOBJ)
		readObj();
	if (!EmptyTEXT)
		readTexture();
	cout << ">>> OBJ (Copy) Generated, NO." << Num << endl;
}

OBJ::~OBJ()
{
	int i;
	for (i = 0; i < v_num; i++)
		delete[] vArr[i];
	delete[] vArr;
	for (i = 0; i < vn_num; i++)
		delete[] vnArr[i];
	delete[] vnArr;
	for (i = 0; i < f_num; i++)
	{
		delete[] fvArr[i];
		delete[] fnArr[i];
	}
	delete[] fvArr;
	delete[] fnArr;
	delete[] bitmapImage;
	cout << ">>> OBJ Destroyed, NO." << Num << endl;
}

void OBJ::draw()
{
	if (EmptyOBJ)
		return;
	if (!EmptyTEXT)
	{
		glBindTexture(GL_TEXTURE_2D, tex); //������
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, AMBI_COLOR); //ɢ��ͻ�������
	glMaterialfv(GL_FRONT, GL_SPECULAR, SPEC_COLOR); //������ɫ
	glMaterialfv(GL_FRONT, GL_SHININESS, &SHININESS); //����ָ��
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// ʹ��glNormal3f���÷��߷���glVertex3f����������
	for (int i = 0; i < f_num; i++)
	{
		if (fnArr[i][0] == 3)
		{
			glBegin(GL_TRIANGLES);

			glTexCoord2f(vtArr[ftArr[i][1] - 1][0], vtArr[ftArr[i][1] - 1][1]);
			glNormal3f(vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1],
				vnArr[fnArr[i][1] - 1][2]);
			glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1],
				vArr[fvArr[i][1] - 1][2]);

			glTexCoord2f(vtArr[ftArr[i][2] - 1][0], vtArr[ftArr[i][2] - 1][1]);
			glNormal3f(vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1],
				vnArr[fnArr[i][2] - 1][2]);
			glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1],
				vArr[fvArr[i][2] - 1][2]);

			glTexCoord2f(vtArr[ftArr[i][3] - 1][0], vtArr[ftArr[i][3] - 1][1]);
			glNormal3f(vnArr[fnArr[i][3] - 1][0], vnArr[fnArr[i][3] - 1][1],
				vnArr[fnArr[i][3] - 1][2]);
			glVertex3f(vArr[fvArr[i][3] - 1][0], vArr[fvArr[i][3] - 1][1],
				vArr[fvArr[i][3] - 1][2]);

			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);

			glTexCoord2f(vtArr[ftArr[i][1] - 1][0], vtArr[ftArr[i][1] - 1][1]);
			glNormal3f(vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1],
				vnArr[fnArr[i][1] - 1][2]);
			glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1],
				vArr[fvArr[i][1] - 1][2]);

			glTexCoord2f(vtArr[ftArr[i][2] - 1][0], vtArr[ftArr[i][2] - 1][1]);
			glNormal3f(vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1],
				vnArr[fnArr[i][2] - 1][2]);
			glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1],
				vArr[fvArr[i][2] - 1][2]);

			glTexCoord2f(vtArr[ftArr[i][3] - 1][0], vtArr[ftArr[i][3] - 1][1]);
			glNormal3f(vnArr[fnArr[i][3] - 1][0], vnArr[fnArr[i][3] - 1][1],
				vnArr[fnArr[i][3] - 1][2]);
			glVertex3f(vArr[fvArr[i][3] - 1][0], vArr[fvArr[i][3] - 1][1],
				vArr[fvArr[i][3] - 1][2]);

			glTexCoord2f(vtArr[ftArr[i][4] - 1][0], vtArr[ftArr[i][4] - 1][1]);
			glNormal3f(vnArr[fnArr[i][4] - 1][0], vnArr[fnArr[i][4] - 1][1],
				vnArr[fnArr[i][4] - 1][2]);
			glVertex3f(vArr[fvArr[i][4] - 1][0], vArr[fvArr[i][4] - 1][1],
				vArr[fvArr[i][4] - 1][2]);

			glEnd();
		}
	}
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

// ���ļ����һ�ȡ�㡢���ߡ��������
void OBJ::getLineNum()
{
	ifstream infile(sFileName.c_str());
	if (!infile)
	{
		cout << "Such File does not exist!" << endl;
		EmptyOBJ = true;
		return;
	}
	EmptyOBJ = false;
	string sline;
	v_num = vn_num = vt_num = f_num = 0;
	getline(infile, sline);
	while (!infile.eof())
	{// ��ָ���ļ����ж�ȡ
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')
				vn_num++;
			else if (sline[1] == ' ')
				v_num++;
			else if (sline[1] == 't')
				vt_num++;
		}
		else if (sline[0] == 'f')
			f_num++;
		getline(infile, sline);
		//cout << sline << endl;
	}
	infile.close();
	//cout << v_num << ' ' << f_num << endl;
}

int OBJ::readFile() //���ļ����ݶ���������ȥ
{
	if (EmptyOBJ)
	{
		//cout << "Empty Class" << endl;
		return -1;
	}
	string s1, s2;
	GLfloat f1, f2, f3;
	int i;
	// vArr[i][0]�������ı���
	// ��ʼ������������Ϊ v_num * 3 ��С�Ķ�ά����
	vArr = new GLfloat *[v_num];
	for (i = 0; i < v_num; i++)
	{
		vArr[i] = new GLfloat[3];
		vArr[i][0] = vArr[i][1] = vArr[i][2] = 0;
	}
	// ��ʼ�����淨�ߵ�����Ϊ vn_num * 3 ��С�Ķ�ά����
	vnArr = new GLfloat *[vn_num];
	for (i = 0; i < vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
		vnArr[i][0] = vnArr[i][1] = vnArr[i][2] = 0;
	}
	// ��ʼ������UV������Ϊ vn_num * 2 ��С�Ķ�ά����
	vtArr = new GLfloat *[vt_num];
	for (i = 0; i < vt_num; i++)
	{
		vtArr[i] = new GLfloat[2];
		vtArr[i][0] = vtArr[i][1] = 0;
	}
	// ��ʼ�������涥��ͷ��ߺ�UV������Ϊ f_num * 5 ��С�Ķ�ά����
	fvArr = new int*[f_num];
	fnArr = new int*[f_num];
	ftArr = new int*[f_num];
	for (i = 0; i < f_num; i++)
	{
		fvArr[i] = new int[5];
		fnArr[i] = new int[5];
		ftArr[i] = new int[5];
		for (int j = 0; j < 5; j++)
		{
			fvArr[i][j] = fnArr[i][j] = ftArr[i][j] = 0;
		}
	}
	int N = v_num + vn_num + vt_num + f_num;
	int count = 0;
	// ���ļ�
	ifstream infile(sFileName.c_str());
	string sline;//ÿһ��
	// ii���߱�� jj������ vvUV��� kk����
	int ii = 0, jj = 0, vv = 0, kk = 0;
	while (getline(infile, sline))
	{
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')
			{ // ���������Ķ���
				istringstream sin(sline);
				sin >> s1 >> f1 >> f2 >> f3;
				vnArr[ii][0] = f1;
				vnArr[ii][1] = f2;
				vnArr[ii][2] = f3;
				ii++;
			}
			else if (sline[1] == ' ')
			{ // ������Ķ���
				istringstream sin(sline);
				sin >> s1 >> f1 >> f2 >> f3;
				vArr[jj][0] = f1;
				vArr[jj][1] = f2;
				vArr[jj][2] = f3;
				jj++;
			}
			else if (sline[1] == 't')
			{ // ����UV�Ķ���
				istringstream sin(sline);
				sin >> s1 >> f1 >> f2;
				vtArr[vv][0] = f1;
				vtArr[vv][1] = f2;
				vv++;
			}
			count++;
			displayProgress(N, count);
		}
		if (sline[0] == 'f')
		{ // ������Ķ���
			istringstream in(sline);
			GLfloat a;
			in >> s1; // ȥ��ǰ׺f
			int i, k;
			for (i = 1; i < 5; i++)
			{
				if (i == 4)
				{
					if (in.eof())
					{
						fvArr[kk][0] = 3;
						fnArr[kk][0] = 3;
						ftArr[kk][0] = 3;
						break;
					}
					else
					{
						fvArr[kk][0] = 4;
						fnArr[kk][0] = 4;
						ftArr[kk][0] = 4;
					}
				}
				in >> s1;
				//cout << s1 << ' ' << endl;
				// ȡ�ö��������ͷ�������
				a = 0;
				// ��ȡ���������������ע��������1��ʼ���
				for (k = 0; s1[k] != '/'; k++)
				{
					a = a * 10 + (s1[k] - 48);
				}
				fvArr[kk][i] = a;
				a = 0;
				// ��������UV����
				for (k = k + 1; s1[k] != '/'; k++)
				{
					a = a * 10 + (s1[k] - 48);
				}
				ftArr[kk][i] = a;
				a = 0;
				// ��ȡ����ķ���������ע��������1��ʼ���
				for (k = k + 1; s1[k]; k++)
				{
					a = a * 10 + (s1[k] - 48);
				}
				fnArr[kk][i] = a;
			}
			kk++;
			count++;
			displayProgress(N, count);
		}
	}
	return 0;
}

void OBJ::setShininess(GLfloat S)
{
	SHININESS = S;
}

void OBJ::setAmbiColor(GLfloat C[])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		AMBI_COLOR[i] = C[i];
	}
}
void OBJ::setSpecColor(GLfloat C[])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		SPEC_COLOR[i] = C[i];
	}
}

//
// reading a BMP file and return its Bitmap
//
void OBJ::LoadBitmapFile()
{
	FILE* filePtr;						// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	int	imageIdx = 0;					// ͼ��λ������
	unsigned char	tempRGB;			// ��������
	//unsigned char* bitmapImageR;

	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(sTextureName.c_str(), "rb");
	if (filePtr == NULL)
	{
		std::cout << "Such file does not exist!" << std::endl;
		EmptyTEXT = true;
		return;
	}

	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		std::cout << stderr << ", Error in LoadBitmapFile: the file is not a bitmap file" << std::endl;
		EmptyTEXT = true;
		return;
	}

	// ����bitmap��Ϣͷ
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biHeight * bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount / 8;
	bitmapImage = new unsigned char[bitmapInfoHeader.biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		std::cout << stderr << ", Error in LoadBitmapFile: memory error" << std::endl;
		EmptyTEXT = true;
		return;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader.biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		std::cout << stderr << ", Error in LoadBitmapFile: memory error" << std::endl;
		EmptyTEXT = true;
		return;
	}
	if (bitmapInfoHeader.biBitCount == 32)
	{
		//std::cout << "Reading 32 bitCount Image ..." << std::endl;
		unsigned char* bitmapImageReal;
		bitmapImageReal = new unsigned char[(bitmapInfoHeader.biSizeImage * 3 / 4)];
		// ��֤�ڴ��Ƿ񴴽��ɹ�
		if (!bitmapImageReal) {
			std::cout << stderr << ", Error in LoadBitmapFile: memory error" << std::endl;
			EmptyTEXT = true;
			return;
		}
		int imageIdxReal = 0;
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 4) {
			bitmapImageReal[imageIdxReal] = bitmapImage[imageIdx + 2];
			bitmapImageReal[imageIdxReal + 1] = bitmapImage[imageIdx + 1];
			bitmapImageReal[imageIdxReal + 2] = bitmapImage[imageIdx];
			imageIdxReal += 3;
		}
		// �ر�bitmapͼ���ļ�
		fclose(filePtr);
		free(bitmapImage);
		bitmapImage = bitmapImageReal;
	}
	else if (bitmapInfoHeader.biBitCount == 24)
	{
		//std::cout << "Reading 24 bitCount Image ..." << std::endl;
		//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3) {
			tempRGB = bitmapImage[imageIdx];
			bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
			bitmapImage[imageIdx + 2] = tempRGB;
		}
		// �ر�bitmapͼ���ļ�
		fclose(filePtr);
	}
	return;
}

void OBJ::LoadBitmapFile2()
{
	FILE* filePtr;						// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	int	imageIdx = 0;					// ͼ��λ������
	unsigned char	tempRGB;			// ��������
	//unsigned char* bitmapImageR;

	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(sTextureName2.c_str(), "rb");
	if (filePtr == NULL)
	{
		std::cout << "Such file does not exist!" << std::endl;
		return;
	}

	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		std::cout << stderr << ", Error in LoadBitmapFile2: the file is not a bitmap file" << std::endl;
		return;
	}

	// ����bitmap��Ϣͷ
	fread(&bitmapInfoHeader2, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapInfoHeader2.biSizeImage = bitmapInfoHeader2.biHeight * bitmapInfoHeader2.biWidth * bitmapInfoHeader2.biBitCount / 8;
	bitmapImage2 = new unsigned char[bitmapInfoHeader2.biSizeImage];
	if (bitmapInfoHeader2.biHeight != bitmapInfoHeader.biHeight || bitmapInfoHeader2.biWidth != bitmapInfoHeader.biWidth)
	{
		std::cout << "Error in LoadBitmapFile2: Different Size!" << std::endl;
		return;
	}
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage2) {
		std::cout << stderr << ", Error in LoadBitmapFile2: memory error" << std::endl;
		return;
	}

	// ����bitmapͼ������
	fread(bitmapImage2, 1, bitmapInfoHeader2.biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage2 == NULL) {
		std::cout << stderr << ", Error in LoadBitmapFile2: memory error" << std::endl;
		return;
	}
	if (bitmapInfoHeader2.biBitCount == 32)
	{
		//std::cout << "Reading 32 bitCount Image ..." << std::endl;
		unsigned char* bitmapImageReal;
		bitmapImageReal = new unsigned char[(bitmapInfoHeader2.biSizeImage * 3 / 4)];
		// ��֤�ڴ��Ƿ񴴽��ɹ�
		if (!bitmapImageReal) {
			std::cout << stderr << ", Error in LoadBitmapFile2: memory error" << std::endl;
			return;
		}
		int imageIdxReal = 0;
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader2.biSizeImage; imageIdx += 4) {
			bitmapImageReal[imageIdxReal] = bitmapImage2[imageIdx + 2];
			bitmapImageReal[imageIdxReal + 1] = bitmapImage2[imageIdx + 1];
			bitmapImageReal[imageIdxReal + 2] = bitmapImage2[imageIdx];
			imageIdxReal += 3;
		}
		// �ر�bitmapͼ���ļ�
		fclose(filePtr);
		free(bitmapImage2);
		bitmapImage2 = bitmapImageReal;
	}
	else if (bitmapInfoHeader2.biBitCount == 24)
	{
		//std::cout << "Reading 24 bitCount Image ..." << std::endl;
		//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader2.biSizeImage; imageIdx += 3) {
			tempRGB = bitmapImage2[imageIdx];
			bitmapImage2[imageIdx] = bitmapImage2[imageIdx + 2];
			bitmapImage2[imageIdx + 2] = tempRGB;
		}
		// �ر�bitmapͼ���ļ�
		fclose(filePtr);
	}
	return;
}

void OBJ::texload()
{
	LoadBitmapFile();
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapImage);	//bitmap����ָ��  
	EmptyTEXT = false;
	cout << "[" << Num << "][TEXT] - Reading Texture: " << sTextureName << "succeed! [" << bitmapInfoHeader.biWidth << " * " << bitmapInfoHeader.biHeight << "]" << endl;
}
void OBJ::texload2()
{
	glBindTexture(GL_TEXTURE_2D, tex);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader2.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader2.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapImage2);	//bitmap����ָ��  
	cout << "[" << Num << "][TEXT2] - Reading Texture: " << sTextureName2 << "succeed! [" << bitmapInfoHeader2.biWidth << " * " << bitmapInfoHeader2.biHeight << "]" << endl;
}

void OBJ::texMix(float alpha)
{
	if (bitmapImageMix == NULL)
		bitmapImageMix = new unsigned char[bitmapInfoHeader2.biWidth * bitmapInfoHeader2.biHeight * 3];
	int i;
	for (i = 0; i < bitmapInfoHeader2.biWidth * bitmapInfoHeader2.biHeight * 3; i++)
	{
		bitmapImageMix[i] = bitmapImage[i] * alpha + bitmapImage2[i] * (1.0 - alpha);
	}
	glBindTexture(GL_TEXTURE_2D, tex);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader2.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader2.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapImageMix);	//bitmap����ָ��  
	cout << "[" << Num << "][TEXTMIX]" << bitmapInfoHeader2.biWidth << " * " << bitmapInfoHeader2.biHeight << "]" << endl;
}

void OBJ::WriteToFile(const string& filename)
{
	if (EmptyOBJ) return;

	ofstream file(filename, ios::out | ios::trunc);
	if (!file.is_open())
	{
		cout << "File Create Failed!" << endl;
		return;
	}
	file << v_num << ' ' << vn_num << ' ' << vt_num << ' ' << f_num << endl;
	int i;
	for (i = 0; i < v_num; i++)
	{
		file << vArr[i][0] << ' ' << vArr[i][1] << ' ' << vArr[i][2] << ' ';
	}
	for (i = 0; i < vn_num; i++)
	{
		file << vnArr[i][0] << ' ' << vnArr[i][1] << ' ' << vnArr[i][2] << ' ';
	}
	for (i = 0; i < vt_num; i++)
	{
		file << vtArr[i][0] << ' ' << vtArr[i][1] << ' ';
	}
	for (i = 0; i < f_num; i++)
	{
		file << fvArr[i][0] << ' ' << fvArr[i][1] << ' ' << fvArr[i][2] << ' ' << fvArr[i][3] << ' ' << fvArr[i][4] << ' ';
		file << fnArr[i][0] << ' ' << fnArr[i][1] << ' ' << fnArr[i][2] << ' ' << fnArr[i][3] << ' ' << fnArr[i][4] << ' ';
		file << ftArr[i][0] << ' ' << ftArr[i][1] << ' ' << ftArr[i][2] << ' ' << ftArr[i][3] << ' ' << ftArr[i][4] << ' ';
	}
	file.close();
}

void OBJ::ReadCompact(const string& filename)
{
	int count = 0;
	sFileName = filename;
	ifstream file(filename, ios::in);
	if (!file.is_open())
	{
		cout << "**File Open Failed!**[" << filename << ']' << endl;
		return;
	}
	file >> v_num >> vn_num >> vt_num >> f_num;
	int N = v_num + vn_num + vt_num + f_num;
	int i;
	if (!EmptyOBJ)
	{
		for (i = 0; i < v_num; i++)
			delete[] vArr[i];
		delete[] vArr;
		for (i = 0; i < vn_num; i++)
			delete[] vnArr[i];
		delete[] vnArr;
		for (i = 0; i < f_num; i++)
		{
			delete[] fvArr[i];
			delete[] fnArr[i];
		}
		delete[] fvArr;
		delete[] fnArr;
	}
	// vArr[i][0]�������ı���
	// ��ʼ������������Ϊ v_num * 3 ��С�Ķ�ά����
	vArr = new GLfloat * [v_num];
	for (i = 0; i < v_num; i++)
	{
		vArr[i] = new GLfloat[3];
	}
	// ��ʼ�����淨�ߵ�����Ϊ vn_num * 3 ��С�Ķ�ά����
	vnArr = new GLfloat * [vn_num];
	for (i = 0; i < vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
	}
	// ��ʼ������UV������Ϊ vn_num * 2 ��С�Ķ�ά����
	vtArr = new GLfloat * [vt_num];
	for (i = 0; i < vt_num; i++)
	{
		vtArr[i] = new GLfloat[2];
	}
	// ��ʼ�������涥��ͷ��ߺ�UV������Ϊ f_num * 5 ��С�Ķ�ά����
	fvArr = new int* [f_num];
	fnArr = new int* [f_num];
	ftArr = new int* [f_num];
	for (i = 0; i < f_num; i++)
	{
		fvArr[i] = new int[5];
		fnArr[i] = new int[5];
		ftArr[i] = new int[5];
	}
	for (i = 0; i < v_num; i++)
	{
		file >> vArr[i][0] >> vArr[i][1] >> vArr[i][2];
		count++;
		displayProgress(N, count);
	}
	for (i = 0; i < vn_num; i++)
	{
		file >> vnArr[i][0] >> vnArr[i][1] >> vnArr[i][2];
		count++;
		displayProgress(N, count);
	}
	for (i = 0; i < vt_num; i++)
	{
		file >> vtArr[i][0] >> vtArr[i][1];
		count++;
		displayProgress(N, count);
	}
	for (i = 0; i < f_num; i++)
	{
		file >> fvArr[i][0] >> fvArr[i][1] >> fvArr[i][2] >> fvArr[i][3] >> fvArr[i][4];
		file >> fnArr[i][0] >> fnArr[i][1] >> fnArr[i][2] >> fnArr[i][3] >> fnArr[i][4];
		file >> ftArr[i][0] >> ftArr[i][1] >> ftArr[i][2] >> ftArr[i][3] >> ftArr[i][4];
		count++;
		displayProgress(N, count);
	}
	EmptyOBJ = false;
	cout << "[" << Num << "][OBJ] - Read obj file: " << sFileName << " succeed!" << endl;
	cout << "[" << Num << "][OBJ] - v_num: " << v_num << ", vn_num: " << vn_num << ", vt_num: " << vt_num << ", f_num: " << f_num << endl;
}

void OBJ::ReadBoxes(const string& filename)
{
	sFileName = filename;
	readObj();
	Boxes_Count = f_num / 6;
	cout << Boxes_Count << " Boxes in Collision Detect" << endl;
	Boxes_Coords = new GLfloat * [Boxes_Count];
	int i, j, k;
	for (i = 0; i < Boxes_Count; i++)
	{
		Boxes_Coords[i] = new GLfloat[6];
	}
	GLfloat x_max, y_max, z_max, x_min, y_min, z_min;
	for (i = 0; i < Boxes_Count; i++)
	{
		x_max = x_min = vArr[fvArr[i * 6][1] - 1][0];
		y_max = y_min = vArr[fvArr[i * 6][1] - 1][1];
		z_max = z_min = vArr[fvArr[i * 6][1] - 1][2];
		for (j = 0; j < 6; j++)
		{
			for (k = 0; k < 4; k++)
			{
				//cout << i << ' ' << j << ' ' << k << endl;
				//cout << fvArr[i * 6 + j][k + 1] << endl;
				if (x_max < vArr[fvArr[i * 6 + j][k + 1] - 1][0])
				{
					x_max = vArr[fvArr[i * 6 + j][k + 1] - 1][0];
				}
				if (x_min > vArr[fvArr[i * 6 + j][k + 1] - 1][0])
				{
					x_min = vArr[fvArr[i * 6 + j][k + 1] - 1][0];
				}
				if (y_max < vArr[fvArr[i * 6 + j][k + 1] - 1][1])
				{
					y_max = vArr[fvArr[i * 6 + j][k + 1] - 1][1];
				}
				if (y_min > vArr[fvArr[i * 6 + j][k + 1] - 1][1])
				{
					y_min = vArr[fvArr[i * 6 + j][k + 1] - 1][1];
				}
				if (z_max < vArr[fvArr[i * 6 + j][k + 1] - 1][2])
				{
					z_max = vArr[fvArr[i * 6 + j][k + 1] - 1][2];
				}
				if (z_min > vArr[fvArr[i * 6 + j][k + 1] - 1][2])
				{
					z_min = vArr[fvArr[i * 6 + j][k + 1] - 1][2];
				}
			}
		}
		Boxes_Coords[i][0] = x_max;
		Boxes_Coords[i][1] = y_max;
		Boxes_Coords[i][2] = z_max;
		Boxes_Coords[i][3] = x_min;
		Boxes_Coords[i][4] = y_min;
		Boxes_Coords[i][5] = z_min;
		//cout << '(' << x_max << ", " << y_max << ", " << z_max << ") (" << x_min << ", " << y_min << ", " << z_min << ")" << endl;
	}
}

int OBJ::getBoxesCount()
{
	return Boxes_Count;
}

GLfloat** OBJ::getBoxesCoords()
{
	return Boxes_Coords;
}