#pragma once
#include <windows.h>

//创建下拉菜单
void menu_Creat(HWND hWnd);

//相应下拉菜单
void menu_Command(HWND hWnd, WPARAM wParam);
