#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

//判断n是否为2的整数次幂
int texture_Power_of_Two(int n);

//读取bmp文件加载纹理并返回纹理编号
GLuint texture_Load_Texture(const char* file_name);
