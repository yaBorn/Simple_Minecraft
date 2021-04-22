#pragma once
#include <GL/freeglut.h>
#include <math.h>
const float pi = 3.1415926f;
#define CRAMERA_DISTANCE 66666 //相机最大视距

//世界坐标系设置为 竖z轴，水平面xoy

//点结构体 (用以一个函数即可输出xyz)
struct pointXYZ {
	int x, y, z;
};

//摄像机类
class camera {
private:
	//设置投影空间的参数
	float win_long, win_wide;
	float space_long = 1000000;
	float space_wide = 1000000;
	float space_high = 1000000;
	
	//相机的视角向量(x-per_x ,y-per_y ,z-per_z)
	float x, y, z;
	float per_x, per_y, per_z;
	
	void camera_Vector_Unitization();//单位化相机向量

public:
	camera();//构造

	float out_x();//向外输出相机向量
	float out_y();
	float out_z();
	float out_per_x();
	float out_per_y();
	float out_per_z();

	pointXYZ point_Front();//碰撞测试输出相机前后左右点
	pointXYZ point_Front_XOY();
	pointXYZ point_Rear();
	pointXYZ point_Left();
	pointXYZ point_Right();

	void set_light();//玩家光源
	void draw_Sky(GLuint sky,bool day);//绘制天空
	void draw_Sight();//绘制准星

	void get_window_Size(float winlong, float winwide);//获取窗口大小
	void set_projectionSpace(bool projectWay);//设置投影空间(之前要获取窗口大小)
	void set_lookAt();//绘制设定设定相机

	void move_Front_to_Back(float distance);//移动
	void move_Front_to_Back_XOY(float distance);
	void move_Right_to_Left(float distance);
	void move_Up_to_Down(float distance);

	void turn_Right_to_Left(float angle);//转向
	void turn_Up_to_Down(float angle);
};
