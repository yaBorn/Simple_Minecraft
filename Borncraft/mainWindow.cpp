#include <gl/GLUT.H>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include "camera.h"
#include "objectList.h"
#pragma comment(lib, "Winmm.lib")
HWND hWnd;//渲染窗口句柄
int win_id;//渲染窗口id
int win_id_instruct;//说明子窗口id
GLuint instruction;//说明纹理

//视窗长宽
float win_Long_instruct = 600;
float win_Wide_instruct = 400;
float win_Long = 1980*2 / 3;
float win_Wide = 1080 / 2 + 200;

camera playerCamera;
objectList objectlist;
bool isDay = true;

//渲染窗口初始化
void Initial(void)
{
	glutSetWindow(win_id);//多窗口，声明焦点窗口

	glEnable(GL_CULL_FACE);//背面剔除
	glEnable(GL_COLOR_MATERIAL);//光照下模型颜色起作用
	glEnable(GL_DEPTH_TEST);//启动深度测试
	glDepthFunc(GL_LESS);//现实生活中的前景

	objectlist.set_place();//初始化物体坐标容器
	objectlist.load_texture();//加载纹理
	objectlist.creat_Display_Lists();  //创建显示列表 与 纹理

	glClearColor(1.0, 0.5, 0.5, 0.1f);//背景

	//设置投影空间
	playerCamera.get_window_Size(win_Long, win_Wide);
	playerCamera.set_projectionSpace(TRUE);
	cout << endl << "右键弹出菜单" << endl;
}
//渲染窗口绘制
void myDisplay(void)
{
	glutSetWindow(win_id);//多窗口，声明焦点窗口
	glViewport(0, 0, win_Long, win_Wide);//视口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//颜色和深度缓存清空

	//设定机位
	playerCamera.set_lookAt();
	
	//天空
	playerCamera.draw_Sky(objectlist.Sky, isDay);
	
	//绘制准星
	playerCamera.draw_Sight();
	
	//光源
	if (!isDay) {
		playerCamera.set_light();//玩家光源
	}

	//显示列表
	for (int i = 1; i <= objectlist.diamondNum + 2; i++) {
		glCallList(i);
	}

	glutSwapBuffers();   //双缓冲刷新
	glDisable(GL_LIGHTING);//关闭灯源
}

//操作说明窗口初始化
void Inital_instruct() {
	glutSetWindow(win_id_instruct);
	glClearColor(0.8, 0.8, 0.8, 0.0f);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, win_Long_instruct, 0.0, win_Wide_instruct);
	instruction = texture_Load_Texture("texture/instruction.bmp");
}
//操作说明窗口绘制
void instructDisplay(void) {
	glutSetWindow(win_id_instruct);
	glViewport(0, 0, win_Long_instruct, win_Wide_instruct);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_ALPHA_TEST);//开启ALPHA
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_TEXTURE_2D);//开启纹理功能
	glBindTexture(GL_TEXTURE_2D, instruction);//绑定纹理
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.8, 0.8);
	glTexCoord2f(1.0, 1.0); glVertex3f(win_Long_instruct, win_Wide_instruct, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(win_Long_instruct, 0, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0, win_Wide_instruct, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glutSwapBuffers();
}

bool button_MIDDLE_Down = false;
int rx , ry ;//记录上次回调鼠标位置
float window_x, window_y;//屏幕坐标
//按键回调
void OnMouse(int button, int state, int x, int y)
{
	glutSetWindow(win_id);//多窗口，声明焦点窗口

	rx = x;
	ry = y;
	//按下中键，移动视角开关
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		button_MIDDLE_Down = !button_MIDDLE_Down;
		if (button_MIDDLE_Down) {//菜单关联
			glutDetachMenu(GLUT_RIGHT_BUTTON);
		}
		else {
			glutAttachMenu(GLUT_RIGHT_BUTTON);
		}
	}
	//按下左键，添加方块
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		objectlist.add_place(
			playerCamera.out_x(),
			playerCamera.out_y(),
			playerCamera.out_z(),
			playerCamera.out_per_x(),
			playerCamera.out_per_y(),
			playerCamera.out_per_z());
	}
	//按下右键，删除方块
	if (button_MIDDLE_Down && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		objectlist.delete_place(
			playerCamera.out_x(),
			playerCamera.out_y(),
			playerCamera.out_z(),
			playerCamera.out_per_x(),
			playerCamera.out_per_y(),
			playerCamera.out_per_z());
	}
	return;
}
//移动回调 控制视角
void OnMotion(int x, int y)
{
	glutSetWindow(win_id);//多窗口，声明焦点窗口

	if (button_MIDDLE_Down) {
		SetCursor(LoadCursor(NULL, "cursor1.cur"));//设置鼠标透明
		float x_move = (x - rx) / 4;//绕轴旋转度数
		float y_move = (y - ry) / 4;
		if (x_move < 10 && y_move < 10 && -10 < x_move && -10 < y_move) {
			playerCamera.turn_Right_to_Left(x_move);
			playerCamera.turn_Up_to_Down(y_move);
		}
		if (x + win_Long_instruct >= win_Long ||
			y + win_Wide_instruct >= win_Wide ||
			x - win_Long_instruct <= 0 ||
			y - win_Wide_instruct <= 0) {//鼠标到达窗口边界附近时
			window_x = glutGet(GLUT_WINDOW_X);
			window_y = glutGet(GLUT_WINDOW_Y);
			SetCursorPos(window_x + (win_Long / 2), window_y + (win_Wide / 2));
			rx = win_Long / 2;
			ry = win_Wide / 2;
			glutPostRedisplay();
			return;
		}
	}
	rx = x;
	ry = y;
	glutPostRedisplay();
	return;
}
//滚轮回调 控制道具种类
void onMouseWheel(int button, int dir, int x, int y)
{
	glutSetWindow(win_id);//多窗口，声明焦点窗口

	if (dir > 0) {
		objectlist.diamondChoosed -= 1;
		if (0 == objectlist.diamondChoosed) {
			objectlist.diamondChoosed = objectlist.diamondNum;
		}
	}
	if (dir < 0) {
		objectlist.diamondChoosed += 1;
		if (objectlist.diamondNum+1 == objectlist.diamondChoosed) {
			objectlist.diamondChoosed = 1;
		}
	}
}

//键盘回调
void keyBoard(unsigned char key, int x, int y) {
	glutSetWindow(win_id);//多窗口，声明焦点窗口

	float distance = 10;
	switch (key) {
		//判断相机六方向的点处是否有方块（碰撞测试）
	case 'q':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.point_Front().x,
			playerCamera.point_Front().y,
			playerCamera.point_Front().z) &&
			0 == objectlist.judge_isDiamond(
				playerCamera.point_Front().x,
				playerCamera.point_Front().y,
				playerCamera.point_Front().z - 50)) {
			playerCamera.move_Front_to_Back(distance);;
		}break;
	case 'w':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.point_Front_XOY().x,
			playerCamera.point_Front_XOY().y,
			playerCamera.point_Front_XOY().z) &&
			0 == objectlist.judge_isDiamond(
				playerCamera.point_Front_XOY().x,
				playerCamera.point_Front_XOY().y,
				playerCamera.point_Front_XOY().z - 50)) {
			playerCamera.move_Front_to_Back_XOY(distance);
		}break;
	case 's':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.point_Rear().x,
			playerCamera.point_Rear().y,
			playerCamera.point_Rear().z) &&
			0 == objectlist.judge_isDiamond(
				playerCamera.point_Rear().x,
				playerCamera.point_Rear().y,
				playerCamera.point_Rear().z - 50)) {
			playerCamera.move_Front_to_Back_XOY(-distance);
		}break;

	case 'a':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.point_Left().x,
			playerCamera.point_Left().y,
			playerCamera.point_Left().z) &&
			0 == objectlist.judge_isDiamond(
				playerCamera.point_Left().x,
				playerCamera.point_Left().y,
				playerCamera.point_Left().z - 50)) {
			playerCamera.move_Right_to_Left(distance);
		}break;
	case 'd':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.point_Right().x,
			playerCamera.point_Right().y,
			playerCamera.point_Right().z) &&
			0 == objectlist.judge_isDiamond(
				playerCamera.point_Right().x,
				playerCamera.point_Right().y,
				playerCamera.point_Right().z)) {
			playerCamera.move_Right_to_Left(-distance);
		}break;

	case 'e':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.out_x(),
			playerCamera.out_y(),
			playerCamera.out_z() + 25)) {
			playerCamera.move_Up_to_Down(distance);
		}break;
	case 'c':
		if (0 == objectlist.judge_isDiamond(
			playerCamera.out_x(),
			playerCamera.out_y(),
			playerCamera.out_z() - 75)) {
			playerCamera.move_Up_to_Down(-distance);
		}break;
	}
	glutPostRedisplay();
	return;
}

//窗口大小回调
void changeSize(GLsizei w, GLsizei h){
	glutSetWindow(win_id);//多窗口，声明焦点窗口

	win_Long = w;
	win_Wide = h;
	playerCamera.get_window_Size(win_Long, win_Wide);
	playerCamera.set_projectionSpace(TRUE);
	glutPostRedisplay();
	return;
}

bool instructOpen = true;
//菜单回调
void menu_Events(int option) {
	switch (option) {
	case 1:
		objectlist.save_place();
		break;
	case 2:
		objectlist.load_place();
		objectlist.creat_Display_Lists();
		break;
	case 3:
		objectlist.set_place();
		objectlist.creat_Display_Lists();
		cout << "清除方块" << endl;
		break;
	case 4:
		isDay = true;
		break;
	case 5:
		isDay = false;
		break;
	case 6:
		if (!instructOpen) {
			instructOpen = true;
			win_id_instruct = glutCreateSubWindow(win_id, 0, 0, win_Long_instruct, win_Wide_instruct);
			Inital_instruct();
			glutDisplayFunc(&instructDisplay);
		}
		break;
	case 7:
		if (instructOpen) {
			instructOpen = false;
			glutDestroyWindow(win_id_instruct);
		}
		break;
	case 8:
		MessageBox(hWnd, "图形学\nBorncraft\n@杨博文", "关于:", MB_OK);
		break;
	}
}
//设置菜单
void menu_Set() {
	glutCreateMenu(&menu_Events);
	//给菜单增加条目
	glutAddMenuEntry("Save", 1);
	glutAddMenuEntry("Load", 2);
	glutAddMenuEntry("Inital diamond", 3);
	glutAddMenuEntry("--------------", 0);
	glutAddMenuEntry("Day", 4);
	glutAddMenuEntry("Night", 5);
	glutAddMenuEntry("--------------", 0);
	glutAddMenuEntry("Instructions  open", 6);
	glutAddMenuEntry("Instructions  close", 7);
	glutAddMenuEntry("--------------", 0);
	glutAddMenuEntry("About", 8);
	// 把菜单和鼠标右键关联起来。
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char * argv[])
{
	PlaySound("texture\\bgm.wav", GetModuleHandle(NULL), SND_ASYNC | SND_LOOP | SND_FILENAME);

	glutInit(&argc, argv);//glut库初始化
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//RGBA模式，双缓冲

	glutInitWindowPosition(500, 200);//窗口位置
	glutInitWindowSize(win_Long, win_Wide);//窗口大小
	
	win_id = glutCreateWindow("杨博文-Broncraft");//创建窗口，窗口标题
	hWnd = GetActiveWindow();//获取绘制窗口句柄
	Initial();//初始化函数
	glutDisplayFunc(&myDisplay);//myDisplay指定为当前窗口显示内容函数
	glutKeyboardFunc(&keyBoard);//键盘事件回调
	glutMouseFunc(&OnMouse);  //鼠标按键事件回调
	glutPassiveMotionFunc(&OnMotion);//鼠标移动事件回调
	glutMouseWheelFunc(&onMouseWheel);//鼠标滚轮回调函数
	glutReshapeFunc(&changeSize);//窗口大小事件回调
	
	menu_Set();//设置菜单

	//子窗口
	win_id_instruct = glutCreateSubWindow(win_id, 0, 0, win_Long_instruct, win_Wide_instruct);
	Inital_instruct();
	glutDisplayFunc(&instructDisplay);


	glutMainLoop();//窗口框架运行，回调函数工作
	return 0;
}