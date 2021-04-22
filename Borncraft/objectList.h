#pragma once
#include <GL/freeglut.h>
#include "texture.h"
#include <set>
#include <fstream>
#include <sstream>
#include <io.h>
#include <direct.h>
using namespace std;
#define DISTANCE 9999 //空间距离

//物品表类 （因为位置参数容器需要跨文件,用类来传递值）
class objectList {
private:
	//物体坐标
	struct coordinate {
		int x;
		int y;
		int z;
		//重载 < ,定义红黑树中的排序
		friend bool operator < (const coordinate &n1, const coordinate &n2) {
			if (n1.x != n2.x) {
				return n1.x < n2.x;
			}
			else {
				if (n1.y != n2.y) {
					return n1.y < n2.y;
				}
				else {
					return n1.z < n2.z;
				}
			}
		}
	};

	set<coordinate>::iterator it;//容器迭代器
	set<coordinate> place_Diamond_grass;//方块坐标容器
	set<coordinate> place_Diamond_plank;
	set<coordinate> place_Tree;
	set<coordinate> place_Diamond_half;

public:
	//纹理编号
	GLuint Diamond_grass;
	GLuint Diamond_plank;
	GLuint Branch;
	GLuint Leaf;
	GLuint Sky;
	int diamondNum = 4;//方块种类
	int diamondChoosed;//当前选择的方块

	//读取本地存档
	void load_place();
	void save_place();

	//加载纹理
	void load_texture();
	//初始化位置参数
	void set_place();

	//判断坐标x,y,z所在的单位块空间是否绘制方块
	//返回方块的纹理编号，返回0为不绘制
	int judge_isDiamond(float x, float y, float z);

	//添加,删除方块
	void add_place(float x, float y, float z, float per_x, float per_y, float per_z);
	void delete_place(float x, float y, float z, float per_x, float per_y, float per_z);

	//创建显示列表
	void creat_Display_Lists();

private:
	//绘制平铺纹理
	void draw_Texture();
	//绘制网线
	void draw_Grid();
	//绘制方块
	void draw_Diamind_half();
	void draw_Diamond_grass();
	void draw_Diamond_plank();
	void draw_Diamond_colour();
	//绘制树
	string tree_grammar;
	void tree_Branch(float baseRadius, float topRadius, float height);
	void tree_Leaf();
	void draw_Tree(string grammar);
	void set_treeGrammar();

};
