#include "texture.h"

//判断n是否为2的整数次幂
int texture_Power_of_Two(int n)
	{
		if (n <= 0) {
			return 0;
		}
		return (n & (n - 1)) == 0;
	}

//读取bmp文件加载纹理并返回纹理编号
GLuint texture_Load_Texture(const char * file_name)
	{
		GLint width, height, total_bytes;
		GLubyte* pixels = 0;
		GLuint last_texture_ID = 0, texture_ID = 0;

		// 打开文件，如果失败，返回	
		FILE* pFile;
		if (fopen_s(&pFile, file_name, "rb")) {
			std::cout << "文件 " << file_name << " 加载失败 无法打开" << std::endl;
			return 0;
		}

		// 读取文件中图象的宽度和高度	
		fseek(pFile, 0x0012, SEEK_SET);
		fread(&width, 4, 1, pFile);
		fread(&height, 4, 1, pFile);
		fseek(pFile, BMP_Header_Length, SEEK_SET);

		// 计算每行像素所占字节数，并根据此数据计算总像素字节数	
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0) {
			++line_bytes;
		}
		total_bytes = line_bytes * height;

		// 根据总像素字节数分配内存
		pixels = (GLubyte*)malloc(total_bytes);
		if (0 == pixels) {
			fclose(pFile);
			std::cout << "文件 " << file_name << " 加载失败 像素为0" << std::endl;
			return 0;
		}

		// 读取像素数据	
		if (fread(pixels, total_bytes, 1, pFile) <= 0) {
			free(pixels);
			fclose(pFile);
			std::cout << "文件 " << file_name << " 加载失败 数据缺失" << std::endl;
			return 0;
		}

		// 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
		// 若图像宽高超过了OpenGL规定的最大值，也缩放	
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		//std::cout << (int)max;
		if (!texture_Power_of_Two(width)
			|| !texture_Power_of_Two(height)
			|| width > max
			|| height > max)
		{
			GLint new_width;
			GLint new_height;
			if (file_name == "sky.bmp"&&max > 4096) {//天空盒用最大像素
				new_width = 4096;
				new_height = 4096;
			}
			else {
				new_width = 512;
				new_height = 512; // 规定缩放后新的大小为边长的正方形		
			}
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数	
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0) {
				++new_line_bytes;
			}
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存		
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (0 == new_pixels) {
				free(pixels);
				fclose(pFile);
				std::cout << "文件 " << file_name << " 加载失败 内存分配失败" << std::endl;
				return 0;
			}

			// 进行像素缩放		
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height		
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}

		//设置白色为透明的0,之前读取的RGB，现在需要RGBA
		unsigned char* texture = 0;
		texture = (unsigned char*)malloc(width * height * 4);
		for (int m = 0; m < width; m++) {
			for (int j = 0; j < height; j++) {
				//把颜色值写入   	
				texture[m*width * 4 + j * 4] = pixels[m*width * 3 + j * 3];
				texture[m*width * 4 + j * 4 + 1] = pixels[m*width * 3 + j * 3 + 1];
				texture[m*width * 4 + j * 4 + 2] = pixels[m*width * 3 + j * 3 + 2];

				//设置alpha值,假设白色为透明色   
				if (texture[m*width * 4 + j * 4] >= 240
					&& texture[m*width * 4 + j * 4 + 1] >= 240
					&& texture[m*width * 4 + j * 4 + 2] >= 240) {
					texture[m*width * 4 + j * 4 + 3] = 0;//透明，alpha=0  
					//cout << (int)texture[m*width * 4 + j * 4 + 3];
				}
				else {
					texture[m*width * 4 + j * 4 + 3] = 255;//不透明，alpha=255  
					//cout << (int)texture[m*width * 4 + j * 4 + 3];
				}
			}
		}

		// 分配一个新的纹理编号
		glGenTextures(1, &texture_ID);
		if (0 == texture_ID) {
			free(pixels);
			free(texture);
			fclose(pFile);
			std::cout << "文件 " << file_name << " 加载失败 纹理分配失败" << std::endl;
			return 0;
		}

		// 绑定新的纹理，载入纹理并设置纹理参数
		// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复	
		GLint lastTextureID = last_texture_ID;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
		free(pixels);
		free(texture);
		std::cout << "文件 " << file_name << " 加载成功" << std::endl;
		return texture_ID;
	}