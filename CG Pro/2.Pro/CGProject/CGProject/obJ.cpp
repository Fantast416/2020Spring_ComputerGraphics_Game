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
		glBindTexture(GL_TEXTURE_2D, tex); //绑定纹理
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, AMBI_COLOR); //散射和环境反射
	glMaterialfv(GL_FRONT, GL_SPECULAR, SPEC_COLOR); //镜面颜色
	glMaterialfv(GL_FRONT, GL_SHININESS, &SHININESS); //镜面指数
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// 使用glNormal3f设置法线方向，glVertex3f绘制三角面
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

// 读文件并且获取点、法线、面的数量
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
	{// 从指定文件逐行读取
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

int OBJ::readFile() //将文件内容读到数组中去
{
	if (EmptyOBJ)
	{
		//cout << "Empty Class" << endl;
		return -1;
	}
	string s1, s2;
	GLfloat f1, f2, f3;
	int i;
	// vArr[i][0]储存该面的边数
	// 初始化储存点的数组为 v_num * 3 大小的二维数组
	vArr = new GLfloat *[v_num];
	for (i = 0; i < v_num; i++)
	{
		vArr[i] = new GLfloat[3];
		vArr[i][0] = vArr[i][1] = vArr[i][2] = 0;
	}
	// 初始化储存法线的数组为 vn_num * 3 大小的二维数组
	vnArr = new GLfloat *[vn_num];
	for (i = 0; i < vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
		vnArr[i][0] = vnArr[i][1] = vnArr[i][2] = 0;
	}
	// 初始化储存UV的数组为 vn_num * 2 大小的二维数组
	vtArr = new GLfloat *[vt_num];
	for (i = 0; i < vt_num; i++)
	{
		vtArr[i] = new GLfloat[2];
		vtArr[i][0] = vtArr[i][1] = 0;
	}
	// 初始化储存面顶点和法线和UV的数组为 f_num * 5 大小的二维数组
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
	// 打开文件
	ifstream infile(sFileName.c_str());
	string sline;//每一行
	// ii法线编号 jj顶点编号 vvUV编号 kk面编号
	int ii = 0, jj = 0, vv = 0, kk = 0;
	while (getline(infile, sline))
	{
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')
			{ // 处理法向量的读入
				istringstream sin(sline);
				sin >> s1 >> f1 >> f2 >> f3;
				vnArr[ii][0] = f1;
				vnArr[ii][1] = f2;
				vnArr[ii][2] = f3;
				ii++;
			}
			else if (sline[1] == ' ')
			{ // 处理顶点的读入
				istringstream sin(sline);
				sin >> s1 >> f1 >> f2 >> f3;
				vArr[jj][0] = f1;
				vArr[jj][1] = f2;
				vArr[jj][2] = f3;
				jj++;
			}
			else if (sline[1] == 't')
			{ // 处理UV的读入
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
		{ // 处理面的读入
			istringstream in(sline);
			GLfloat a;
			in >> s1; // 去掉前缀f
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
				// 取得顶点索引和法线索引
				a = 0;
				// 获取顶点的坐标索引，注意索引从1开始编号
				for (k = 0; s1[k] != '/'; k++)
				{
					a = a * 10 + (s1[k] - 48);
				}
				fvArr[kk][i] = a;
				a = 0;
				// 跳过纹理UV部分
				for (k = k + 1; s1[k] != '/'; k++)
				{
					a = a * 10 + (s1[k] - 48);
				}
				ftArr[kk][i] = a;
				a = 0;
				// 获取顶点的法线索引，注意索引从1开始编号
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
	FILE* filePtr;						// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	int	imageIdx = 0;					// 图像位置索引
	unsigned char	tempRGB;			// 交换变量
	//unsigned char* bitmapImageR;

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(sTextureName.c_str(), "rb");
	if (filePtr == NULL)
	{
		std::cout << "Such file does not exist!" << std::endl;
		EmptyTEXT = true;
		return;
	}

	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		std::cout << stderr << ", Error in LoadBitmapFile: the file is not a bitmap file" << std::endl;
		EmptyTEXT = true;
		return;
	}

	// 读入bitmap信息头
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biHeight * bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount / 8;
	bitmapImage = new unsigned char[bitmapInfoHeader.biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		std::cout << stderr << ", Error in LoadBitmapFile: memory error" << std::endl;
		EmptyTEXT = true;
		return;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader.biSizeImage, filePtr);
	// 确认读入成功
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
		// 验证内存是否创建成功
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
		// 关闭bitmap图像文件
		fclose(filePtr);
		free(bitmapImage);
		bitmapImage = bitmapImageReal;
	}
	else if (bitmapInfoHeader.biBitCount == 24)
	{
		//std::cout << "Reading 24 bitCount Image ..." << std::endl;
		//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3) {
			tempRGB = bitmapImage[imageIdx];
			bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
			bitmapImage[imageIdx + 2] = tempRGB;
		}
		// 关闭bitmap图像文件
		fclose(filePtr);
	}
	return;
}

void OBJ::LoadBitmapFile2()
{
	FILE* filePtr;						// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	int	imageIdx = 0;					// 图像位置索引
	unsigned char	tempRGB;			// 交换变量
	//unsigned char* bitmapImageR;

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(sTextureName2.c_str(), "rb");
	if (filePtr == NULL)
	{
		std::cout << "Such file does not exist!" << std::endl;
		return;
	}

	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		std::cout << stderr << ", Error in LoadBitmapFile2: the file is not a bitmap file" << std::endl;
		return;
	}

	// 读入bitmap信息头
	fread(&bitmapInfoHeader2, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapInfoHeader2.biSizeImage = bitmapInfoHeader2.biHeight * bitmapInfoHeader2.biWidth * bitmapInfoHeader2.biBitCount / 8;
	bitmapImage2 = new unsigned char[bitmapInfoHeader2.biSizeImage];
	if (bitmapInfoHeader2.biHeight != bitmapInfoHeader.biHeight || bitmapInfoHeader2.biWidth != bitmapInfoHeader.biWidth)
	{
		std::cout << "Error in LoadBitmapFile2: Different Size!" << std::endl;
		return;
	}
	// 验证内存是否创建成功
	if (!bitmapImage2) {
		std::cout << stderr << ", Error in LoadBitmapFile2: memory error" << std::endl;
		return;
	}

	// 读入bitmap图像数据
	fread(bitmapImage2, 1, bitmapInfoHeader2.biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage2 == NULL) {
		std::cout << stderr << ", Error in LoadBitmapFile2: memory error" << std::endl;
		return;
	}
	if (bitmapInfoHeader2.biBitCount == 32)
	{
		//std::cout << "Reading 32 bitCount Image ..." << std::endl;
		unsigned char* bitmapImageReal;
		bitmapImageReal = new unsigned char[(bitmapInfoHeader2.biSizeImage * 3 / 4)];
		// 验证内存是否创建成功
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
		// 关闭bitmap图像文件
		fclose(filePtr);
		free(bitmapImage2);
		bitmapImage2 = bitmapImageReal;
	}
	else if (bitmapInfoHeader2.biBitCount == 24)
	{
		//std::cout << "Reading 24 bitCount Image ..." << std::endl;
		//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader2.biSizeImage; imageIdx += 3) {
			tempRGB = bitmapImage2[imageIdx];
			bitmapImage2[imageIdx] = bitmapImage2[imageIdx + 2];
			bitmapImage2[imageIdx + 2] = tempRGB;
		}
		// 关闭bitmap图像文件
		fclose(filePtr);
	}
	return;
}

void OBJ::texload()
{
	LoadBitmapFile();
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapImage);	//bitmap数据指针  
	EmptyTEXT = false;
	cout << "[" << Num << "][TEXT] - Reading Texture: " << sTextureName << "succeed! [" << bitmapInfoHeader.biWidth << " * " << bitmapInfoHeader.biHeight << "]" << endl;
}
void OBJ::texload2()
{
	glBindTexture(GL_TEXTURE_2D, tex);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader2.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader2.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapImage2);	//bitmap数据指针  
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
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader2.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader2.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapImageMix);	//bitmap数据指针  
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
	// vArr[i][0]储存该面的边数
	// 初始化储存点的数组为 v_num * 3 大小的二维数组
	vArr = new GLfloat * [v_num];
	for (i = 0; i < v_num; i++)
	{
		vArr[i] = new GLfloat[3];
	}
	// 初始化储存法线的数组为 vn_num * 3 大小的二维数组
	vnArr = new GLfloat * [vn_num];
	for (i = 0; i < vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
	}
	// 初始化储存UV的数组为 vn_num * 2 大小的二维数组
	vtArr = new GLfloat * [vt_num];
	for (i = 0; i < vt_num; i++)
	{
		vtArr[i] = new GLfloat[2];
	}
	// 初始化储存面顶点和法线和UV的数组为 f_num * 5 大小的二维数组
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