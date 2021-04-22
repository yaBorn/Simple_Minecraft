#include "objectList.h"

//存档 读档
void objectList::load_place()
{
	place_Diamond_half.clear();
	place_Diamond_grass.clear();
	place_Diamond_plank.clear();
	place_Tree.clear();

	if (_access("save/", 0) == -1) {
		cout << "未找到存档" << endl;
	}
	string s;
	coordinate p;
	ifstream file;

	file.open("save/diamondGrass.txt");
	if (!file.is_open()) {
		cout << "存档 缺失" << endl;
		return;
	}
	while (getline(file, s)) {
		p.x = atoi(s.c_str());
		getline(file, s);
		p.y = stoi(s.c_str());
		getline(file, s);
		p.z = stoi(s.c_str());
		place_Diamond_grass.insert(p);
	}
	file.close();

	file.open("save/diamondPlank.txt");
	if (!file.is_open()) {
		cout << "存档 缺失" << endl;
		return;
	}
	while (getline(file, s)) {
		p.x = atoi(s.c_str());
		getline(file, s);
		p.y = stoi(s.c_str());
		getline(file, s);
		p.z = stoi(s.c_str());
		place_Diamond_plank.insert(p);
	}
	file.close();

	file.open("save/tree.txt");
	if (!file.is_open()) {
		cout << "存档 缺失" << endl;
		return;
	}
	while (getline(file, s)) {
		p.x = atoi(s.c_str());
		getline(file, s);
		p.y = stoi(s.c_str());
		getline(file, s);
		p.z = stoi(s.c_str());
		place_Tree.insert(p);
	}
	file.close();

	file.open("save/half.txt");
	if (!file.is_open()) {
		cout << "存档 缺失" << endl;
		return;
	}
	while (getline(file, s)) {
		p.x = atoi(s.c_str());
		getline(file, s);
		p.y = stoi(s.c_str());
		getline(file, s);
		p.z = stoi(s.c_str());
		place_Diamond_half.insert(p);
	}
	file.close();

	cout << "读取完毕" << endl;
}
void objectList::save_place()
{
	if (_access("save/", 0) == -1) {
		_mkdir("save");
	}
	ofstream file;
	file.open("save/diamondGrass.txt", ios::trunc);
	if (!file.is_open()) {
		cout << "草方块储存 失败" << endl;
		return;
	}
	for (it = place_Diamond_grass.begin(); it != place_Diamond_grass.end(); it++) {
		file << (*it).x << endl << (*it).y << endl << (*it).z << endl;
	}
	file.close();

	file.open("save/diamondPlank.txt", ios::trunc);
	if (!file.is_open()) {
		cout << "木方块储存 失败" << endl;
		return;
	}
	for (it = place_Diamond_plank.begin(); it != place_Diamond_plank.end(); it++) {
		file << (*it).x << endl << (*it).y << endl << (*it).z << endl;
	}
	file.close();

	file.open("save/tree.txt", ios::trunc);
	if (!file.is_open()) {
		cout << "分形树储存 失败" << endl;
		return;
	}
	for (it = place_Tree.begin(); it != place_Tree.end(); it++) {
		file << (*it).x << endl << (*it).y << endl << (*it).z << endl;
	}
	file.close();

	file.open("save/half.txt", ios::trunc);
	if (!file.is_open()) {
		cout << "半砖储存 失败" << endl;
		return;
	}
	for (it = place_Diamond_half.begin(); it != place_Diamond_half.end(); it++) {
		file << (*it).x << endl << (*it).y << endl << (*it).z << endl;
	}
	file.close();

	cout << "储存完毕" << endl;
}

//加载纹理
void objectList::load_texture()
{
	set_treeGrammar();
	Diamond_grass = texture_Load_Texture("texture/diamondGrass.bmp");
	Diamond_plank = texture_Load_Texture("texture/diamondPlank.bmp");
	Branch = texture_Load_Texture("texture/treeBranch.bmp");
	Leaf = texture_Load_Texture("texture/treeLeaf.bmp");
	Sky = texture_Load_Texture("texture/sky.bmp");
}

//初始化位置参数
void objectList::set_place()
{
	place_Diamond_half.clear();
	place_Diamond_grass.clear();
	place_Diamond_plank.clear();
	place_Tree.clear();
	diamondChoosed = 1;//初始化选择1编号方块
	coordinate setPlace;
	for (setPlace.z = 0; setPlace.z <= 0; setPlace.z++) {
		for (setPlace.y = 0; setPlace.y <= 100; setPlace.y++) {
			for (setPlace.x = 0; setPlace.x <= 100; setPlace.x++) {
				place_Diamond_grass.insert(setPlace);
			}
		}
	}
}

//判断坐标所在的单位块空间是否绘制方块
int objectList::judge_isDiamond(float x, float y, float z)
{
	coordinate place;
	place.x = (int)x / 50;
	place.y = (int)y / 50;
	place.z = (int)z / 50;
	if (place_Diamond_grass.find(place) != place_Diamond_grass.end()) {
		//有方块
		return 1;
	}
	if (place_Diamond_plank.find(place) != place_Diamond_plank.end()) {
		return 2;
	}
	if (place_Tree.find(place) != place_Tree.end()) {
		return 3;
	}
	if (place_Diamond_half.find(place) != place_Diamond_half.end()) {
		return 4;
	}
	return 0;
}

//添加，删除方块
void objectList::add_place(float x, float y, float z, float per_x, float per_y, float per_z) 
{
	//x,y,z->px,py,pz 引射线 取由近到远 距离根50立方 n个点 判断isDiamond
	//取isDiamond的上一个方块
	float vectorLong = sqrt(pow(per_x - x, 2) + pow(per_y - y, 2) + pow(per_z - z, 2));
	float disX = (per_x - x) * sqrt(pow(50, 2)*3) / vectorLong;
	float disY = (per_y - y) * sqrt(pow(50, 2)*3) / vectorLong;
	float disZ = (per_z - z) * sqrt(pow(50, 2)*3) / vectorLong;
	float exX, exY, exZ;

	for (int i = 1; i <= 50; i++) {
		exX = x + (i*disX/10);
		exY = y + (i*disY/10);
		exZ = z + (i*disZ/10);
		if (judge_isDiamond(exX, exY, exZ)!=0 && i != 1) {//该点绘制方块
			coordinate place;
			place.x = (int)(exX - disX/10) / 50;
			place.y = (int)(exY - disY/10) / 50;
			place.z = (int)(exZ - disZ/10) / 50;
			switch (diamondChoosed)
			{
			case 1:			
				place_Diamond_grass.insert(place);//向容器中添加place
				glDeleteLists(1, 1);
				glNewList(1, GL_COMPILE);//重载显示列表
				for (it = place_Diamond_grass.begin(); it != place_Diamond_grass.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					draw_Diamond_grass();
					glPopMatrix();
				}
				glEndList();
				break;
			case 2:
				place_Diamond_plank.insert(place);
				glDeleteLists(2, 1);
				glNewList(2, GL_COMPILE);//重载显示列表
				for (it = place_Diamond_plank.begin(); it != place_Diamond_plank.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					draw_Diamond_plank();
					glPopMatrix();
				}
				glEndList();
				break;
			case 3:
				place_Tree.insert(place);//向容器中添加place
				glDeleteLists(3, 1);
				glNewList(3, GL_COMPILE);//重载显示列表
				for (it = place_Tree.begin(); it != place_Tree.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					glTranslatef(25, 25, 0);
					draw_Tree(tree_grammar);
					glPopMatrix();
				}
				glEndList();
				break;
			case 4:
				place_Diamond_half.insert(place);//向容器中添加place
				glDeleteLists(4, 1);
				glNewList(4, GL_COMPILE);//重载显示列表
				for (it = place_Diamond_half.begin(); it != place_Diamond_half.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					draw_Diamind_half();
					glPopMatrix();
				}
				glEndList();
				break;
			}
			glutPostRedisplay();
			return;
		}
	}
}
void objectList::delete_place(float x, float y, float z, float per_x, float per_y, float per_z)
{
	//x,y,z->px,py,pz 引射线 取由近到远 距离根50立方 n个点 判断isDiamond
	//取isDiamond的上一个方块
	float vectorLong = sqrt(pow(per_x - x, 2) + pow(per_y - y, 2) + pow(per_z - z, 2));
	float disX = (per_x - x) * sqrt(pow(50, 2) * 3) / vectorLong;
	float disY = (per_y - y) * sqrt(pow(50, 2) * 3) / vectorLong;
	float disZ = (per_z - z) * sqrt(pow(50, 2) * 3) / vectorLong;
	float exX, exY, exZ;//取得离散点
	for (int i = 1; i <= 50; i++) {
		exX = x + (i*disX / 10);
		exY = y + (i*disY / 10);
		exZ = z + (i*disZ / 10);
		int diamond = judge_isDiamond(exX, exY, exZ);
		if (diamond != 0 && i != 1) {//该点在方块内，！=1即不可设置过近的点
			coordinate place;
			place.x = (int)exX / 50;
			place.y = (int)exY / 50;
			place.z = (int)exZ / 50;
			switch (diamond)
			{
			case 1:
				place_Diamond_grass.erase(place);//向容器中添加place
				glDeleteLists(1, 1);
				glNewList(1, GL_COMPILE);//重载显示列表
				for (it = place_Diamond_grass.begin(); it != place_Diamond_grass.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					draw_Diamond_grass();
					glPopMatrix();
				}
				glEndList();
				break;
			case 2:
				place_Diamond_plank.erase(place);
				glDeleteLists(2, 1);
				glNewList(2, GL_COMPILE);//重载显示列表
				for (it = place_Diamond_plank.begin(); it != place_Diamond_plank.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					draw_Diamond_plank();
					glPopMatrix();
				}
				glEndList();
				break;
			case 3:
				place_Tree.erase(place);//向容器中添加place
				glDeleteLists(3, 1);
				glNewList(3, GL_COMPILE);//重载显示列表
				for (it = place_Tree.begin(); it != place_Tree.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					glTranslatef(25, 25, 0);
					draw_Tree(tree_grammar);
					glPopMatrix();
				}
				glEndList();
				break;
			case 4:
				place_Diamond_half.erase(place);
				glDeleteLists(4, 1);
				glNewList(4, GL_COMPILE);
				for (it = place_Diamond_half.begin(); it != place_Diamond_half.end(); it++) {
					glPushMatrix();
					glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
					draw_Diamind_half();
					glPopMatrix();
				}
				glEndList();
				break;
			}
			glutPostRedisplay();
			return;
		}
	}
}

//创建显示列表
void objectList::creat_Display_Lists()
{
	glDeleteLists(1, diamondNum + 2);
	if (!glIsList((GLuint)1)) {//草方块
		glNewList(1, GL_COMPILE);
		for (it = place_Diamond_grass.begin(); it != place_Diamond_grass.end(); it++) {
			glPushMatrix();
			glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
			draw_Diamond_grass();
			glPopMatrix();
		}
		glEndList();
	}
	if (!glIsList((GLuint)2)) {//木板方块
		glNewList(2, GL_COMPILE);
		for (it = place_Diamond_plank.begin(); it != place_Diamond_plank.end(); it++) {
			glPushMatrix();
			glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
			draw_Diamond_plank();
			glPopMatrix();
		}
		glEndList();
	}
	if (!glIsList((GLuint)3)) {//树
		glNewList(3, GL_COMPILE);
		for (it = place_Tree.begin(); it != place_Tree.end(); it++) {
			glPushMatrix();
			glTranslatef(50 * (*it).x,50 * (*it).y, 50 * (*it).z);
			glTranslatef(25, 25, 0);
			draw_Tree(tree_grammar);
			glPopMatrix();
		}
		glEndList();
	}
	if (!glIsList((GLuint)4)) {//半砖
		glNewList(4, GL_COMPILE);
		for (it = place_Diamond_half.begin(); it != place_Diamond_half.end(); it++) {
			glPushMatrix();
			glTranslatef(50 * (*it).x, 50 * (*it).y, 50 * (*it).z);
			draw_Diamind_half();
			glPopMatrix();
		}
		glEndList();
	}
	if (!glIsList((GLuint)5)) {//网格
		glNewList(5, GL_COMPILE);
		draw_Grid();
		glEndList();
	}
	if (!glIsList((GLuint)6)) {//平铺纹理
		glNewList(6, GL_COMPILE);
		draw_Texture();
		glEndList();
	}
}

//平铺纹理
void objectList::draw_Texture()
{
	GLfloat earth_mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的环境光颜色，骗蓝色	
	GLfloat earth_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的漫反射光颜色，偏蓝色	
	GLfloat earth_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //定义材质的镜面反射光颜色，红色	
	GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射光颜色，为0	
	GLfloat earth_mat_shininess = 30.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

	glEnable(GL_ALPHA_TEST);//开启ALPHA
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_TEXTURE_2D);//开启纹理功能
	glBindTexture(GL_TEXTURE_2D, Diamond_grass);//绑定纹理
	glBegin(GL_QUADS);//绘制多边形
	glColor3f(0.5, 0.5, 0.5);
	glNormal3f(0, 0, 1);//指定法向量
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 100);//纹理坐标与绘制坐标对应
	glTexCoord2f(1.0, 0.0); glVertex3f(99, 0, 100);
	glTexCoord2f(1.0, 1.0); glVertex3f(99, 132, 100);
	glTexCoord2f(0.0, 1.0); glVertex3f(0, 132, 100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}
//网格
void objectList::draw_Grid()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);//红色x轴
	glVertex3f(-DISTANCE, 0, 0); glVertex3f(DISTANCE, 0, 0);
	glColor3f(0, 1, 0);//绿色y轴
	glVertex3f(0, -DISTANCE, 0); glVertex3f(0, DISTANCE, 0);
	glColor3f(0, 0.7, 1);//蓝色z轴
	glVertex3f(0, 0, -DISTANCE); glVertex3f(0, 0, DISTANCE);
	glColor3f(0.5, 0.5, 0.5);
	for (int i = 0; i * 50 <= DISTANCE; i++) {
		glVertex2f(i * 50, -DISTANCE); glVertex2f(i * 50, DISTANCE);
		glVertex2f(-i * 50, -DISTANCE); glVertex2f(-i * 50, DISTANCE);
	}
	for (int i = 0; i * 50 <= DISTANCE; i++) {
		glVertex2f(-DISTANCE, i * 50); glVertex2f(DISTANCE, i * 50);
		glVertex2f(-DISTANCE, -i * 50); glVertex2f(DISTANCE, -i * 50);
	}
	glEnd();
}
//半方块 
void objectList::draw_Diamind_half()
{
	glEnable(GL_ALPHA_TEST);//开启ALPHA
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_TEXTURE_2D);//开启纹理功能
	glBindTexture(GL_TEXTURE_2D, Diamond_plank);//绑定纹理

	glBegin(GL_QUADS);
	//底面	
	glNormal3f(0, 0, -1);//法线
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.25, 0.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.25, 0.25); glVertex3f(0, 50, 0);
	glTexCoord2f(0.5, 0.25); glVertex3f(50, 50, 0);
	glTexCoord2f(0.5, 0.0); glVertex3f(50, 0, 0);
	//侧面A		
	glNormal3f(0, -1, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.25, 1.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.5, 1.0); glVertex3f(50, 0, 0);
	glColor3ub(132.5, 140, 67.5);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 25);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 25);
	//侧面B	
	glNormal3f(1, 0, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.75, 0.75); glVertex3f(50, 0, 0);
	glTexCoord2f(0.75, 0.5); glVertex3f(50, 50, 0);
	glColor3ub(132.55, 140, 67.5);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 25);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 25);
	//侧面C	
	glNormal3f(0, 1, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.5, 0.25); glVertex3f(50, 50, 0);
	glTexCoord2f(0.25, 0.25); glVertex3f(0, 50, 0);
	glColor3ub(132.5, 140, 67.5);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 25);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 25);
	//侧面D	
	glNormal3f(-1, 0, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.0, 0.5); glVertex3f(0, 50, 0);
	glTexCoord2f(0.0, 0.75); glVertex3f(0, 0, 0);
	glColor3ub(132.5, 140, 67.5);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 25);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 25);
	//顶面	
	glNormal3f(0, 0, 1);
	glColor3ub(90, 180, 60);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 25);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 25);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 25);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 25);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}
//草方块(注意每个面绘制顺序，背面剔除)
void objectList::draw_Diamond_grass()
{
	glEnable(GL_ALPHA_TEST);//开启ALPHA
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_TEXTURE_2D);//开启纹理功能
	glBindTexture(GL_TEXTURE_2D, Diamond_grass);//绑定纹理

	glBegin(GL_QUADS);
	//底面	
	glNormal3f(0, 0, -1);//法线
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.25, 0.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.25, 0.25); glVertex3f(0, 50, 0);
	glTexCoord2f(0.5, 0.25); glVertex3f(50, 50, 0);
	glTexCoord2f(0.5, 0.0); glVertex3f(50, 0, 0);
	//侧面A		
	glNormal3f(0, -1, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.25, 1.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.5, 1.0); glVertex3f(50, 0, 0);
	glColor3ub(132.5, 140, 67.5);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 50);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 50);
	//侧面B	
	glNormal3f(1, 0, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.75, 0.75); glVertex3f(50, 0, 0);
	glTexCoord2f(0.75, 0.5); glVertex3f(50, 50, 0);
	glColor3ub(132.55, 140, 67.5);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 50);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 50);
	//侧面C	
	glNormal3f(0, 1, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.5, 0.25); glVertex3f(50, 50, 0);
	glTexCoord2f(0.25, 0.25); glVertex3f(0, 50, 0);
	glColor3ub(132.5, 140, 67.5);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 50);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 50);
	//侧面D	
	glNormal3f(-1, 0, 0);
	glColor3ub(175, 100, 75);
	glTexCoord2f(0.0, 0.5); glVertex3f(0, 50, 0);
	glTexCoord2f(0.0, 0.75); glVertex3f(0, 0, 0);
	glColor3ub(132.5, 140, 67.5);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 50);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 50);
	//顶面	
	glNormal3f(0, 0, 1);
	glColor3ub(90, 180, 60);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 50);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 50);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 50);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 50);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}
//木板方块
void objectList::draw_Diamond_plank()
{
	glEnable(GL_ALPHA_TEST);//开启ALPHA
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_TEXTURE_2D);//开启纹理功能
	glBindTexture(GL_TEXTURE_2D, Diamond_plank);//绑定纹理

	glBegin(GL_QUADS);
	//底面	
	glNormal3f(0, 0, -1);
	glColor3ub(250, 193, 107);
	glTexCoord2f(0.25, 0.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.25, 0.25); glVertex3f(0, 50, 0);
	glTexCoord2f(0.5, 0.25); glVertex3f(50, 50, 0);
	glTexCoord2f(0.5, 0.0); glVertex3f(50, 0, 0);
	//侧面A	
	glNormal3f(0, -1, 0);
	glTexCoord2f(0.25, 1.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.5, 1.0); glVertex3f(50, 0, 0);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 50);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 50);
	//侧面B	
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.75, 0.75); glVertex3f(50, 0, 0);
	glTexCoord2f(0.75, 0.5); glVertex3f(50, 50, 0);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 50);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 50);
	//侧面C	
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.5, 0.25); glVertex3f(50, 50, 0);
	glTexCoord2f(0.25, 0.25); glVertex3f(0, 50, 0);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 50);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 50);
	//侧面D	
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0, 0.5); glVertex3f(0, 50, 0);
	glTexCoord2f(0.0, 0.75); glVertex3f(0, 0, 0);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 50);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 50);
	//顶面	
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.25, 0.75); glVertex3f(0, 0, 50);
	glTexCoord2f(0.5, 0.75); glVertex3f(50, 0, 50);
	glTexCoord2f(0.5, 0.5); glVertex3f(50, 50, 50);
	glTexCoord2f(0.25, 0.5); glVertex3f(0, 50, 50);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}
//彩方块
void objectList::draw_Diamond_colour()
{
	glBegin(GL_QUADS);
	//底面	
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);
	glColor3f(0, 0, 1);
	glVertex3f(50, 50, 0);
	glVertex3f(50, 0, 0);
	//侧面A		
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(50, 0, 50);
	glVertex3f(0, 0, 50);
	//侧面B	
	glColor3f(0, 1, 0);
	glVertex3f(50, 0, 0);
	glVertex3f(50, 50, 0);
	glColor3f(0, 1, 1);
	glVertex3f(50, 50, 50);
	glVertex3f(50, 0, 50);
	//侧面C		
	glColor3f(1, 1, 0);
	glVertex3f(50, 50, 0);
	glVertex3f(0, 50, 0);
	glColor3f(1, 0, 1);
	glVertex3f(0, 50, 50);
	glVertex3f(50, 50, 50);
	//侧面D	
	glColor3f(1, 0, 1);
	glVertex3f(0, 50, 0);
	glVertex3f(0, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 50);
	glVertex3f(0, 50, 50);
	//顶面		
	glColor3f(1, 1, 0);
	glVertex3f(0, 0, 50);
	glVertex3f(50, 0, 50);
	glColor3f(0, 0, 1);
	glVertex3f(50, 50, 50);
	glVertex3f(0, 50, 50);
	glEnd();
}
//树
void objectList::tree_Branch(float baseRadius, float topRadius, float height)
{
	GLUquadricObj *tree_obj;
	tree_obj = gluNewQuadric();//创建二次曲面对象
	glBindTexture(GL_TEXTURE_2D, Branch);//绑定纹理
	gluQuadricTexture(tree_obj, GL_TRUE);
	//glColor3ub(155, 81, 33);
	glColor3ub(125, 125, 125);
	gluCylinder(tree_obj, baseRadius, topRadius, height, 30, 30);//绘制
	glTranslatef(0.0f, 0.0f, height);
	gluSphere(tree_obj, topRadius, 30, 30);
	gluDeleteQuadric(tree_obj);
}
void objectList::tree_Leaf()
{
	glBindTexture(GL_TEXTURE_2D, Leaf);//绑定纹理
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3ub(125, 125, 125);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, 10, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0, 20, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-10, 10, 0);
	glEnd();	
}
void objectList::draw_Tree(string grammar)
{
	glDisable(GL_CULL_FACE);//背面剔除
	glEnable(GL_ALPHA_TEST);//开启ALPHA
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_TEXTURE_2D);//开启纹理功能

	float baseRadius = 10, topRadius = 6, height = 200;
	int i = 0;
	while (i < grammar.length()) {
		switch (grammar[i]) {
			//F绘制树干，X绘制叶子，AB更改树干参数，+-/*%&坐标变换，[]进出栈
		case'F':tree_Branch(baseRadius, topRadius, height); break;
		case'X':tree_Leaf(); break;
		case'A':height *= 0.6; baseRadius *= 0.5; topRadius *= 0.5; break;
		case'B':height /= 0.6; baseRadius /= 0.5; topRadius /= 0.5; break;
		case'+':glRotatef(35, 1, 0, 0); break;
		case'-':glRotatef(-35, 1, 0, 0); break;
		case'*':glRotatef(35, 0, 1, 0); break;
		case'/':glRotatef(-35, 0, 1, 0); break;
		case'%':glRotatef(35, 0, 0, 1); break;
		case'&':glRotatef(-35, 0, 0, 1); break;
		case'[':glPushMatrix(); break;
		case']':glPopMatrix(); break;
		}
		i++;
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);//背面剔除
}
void objectList::set_treeGrammar()
{
	ifstream file("texture/treeGrammar.txt");
	if (!file.is_open()) {
		cout << "文件 texture/treeGrammar.txt 打开失败" << endl;
		tree_grammar = "";
		return;
	}
	cout << "文件 texture/treeGrammar.txt 打开成功" << endl;
	stringstream ss;
	ss << file.rdbuf();
	tree_grammar = ss.str();
	file.close();
}
