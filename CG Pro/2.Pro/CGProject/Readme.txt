1、如何编译项目？
打开CGProject项目工程文件后，请使用Realease x86编译或Debug x86模式编译
2、文件夹阐述：
1）data/obj文件夹内存放的是对象数据文件，部分较大的obj文件已经被手动转换至data文件，
为在文件中加快读取速度。
2）include文件夹 和 glut文件夹内，存放包含头文件信息，本程序额外仅采用glut32
3）lib文件夹内存放动态链接资源
4）SnapScreen中存放游戏截图，在游戏内进行的截图会保存在此处
5）texture/texture2 中存放着两套贴图，第一套贴图为血腥版，第二套为正常版，可在程序内进行修改。
