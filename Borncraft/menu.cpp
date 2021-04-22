#include "menu.h"

//创建菜单
void menu_Creat(HWND hWnd)
{
	HMENU hMenu = CreateMenu();
	HMENU hMenu1 = CreatePopupMenu();
	HMENU hMenu2 = CreatePopupMenu();
	HMENU hSubi = CreatePopupMenu();
	//g_menu = hMenu1;
	AppendMenu(hMenu, MF_POPUP, (UINT)hMenu1, "菜单1");
	AppendMenu(hMenu1, MF_STRING | MF_CHECKED, 1001, "勾选");
	AppendMenu(hMenu1, MF_POPUP, (UINT)hSubi, "子菜单");
	AppendMenu(hSubi, MF_STRING, 1002, "信息2");
	AppendMenu(hSubi, MF_SEPARATOR, 0, "");
	AppendMenu(hSubi, MF_STRING, 1003, "信息3");
	AppendMenu(hMenu, MF_POPUP, (UINT)hMenu2, "菜单2");
	AppendMenu(hMenu2, MF_STRING, 1004, "信息4");
	AppendMenu(hMenu2, MF_SEPARATOR, 0, "");
	AppendMenu(hMenu2, MF_STRING, 1005, "信息5");
	SetMenu(hWnd, hMenu);
}

//响应菜单
void menu_Command(HWND hWnd, WPARAM wParam)
{
	static int g_state = 0;//标志量
	switch (LOWORD(wParam))
	{
	case 1002:
		MessageBox(hWnd, "信息2", "信息", MB_OK);
		break;
	case 1003:
		MessageBox(hWnd, "信息3", "信息", MB_OK);
		break;
	case 1004:
		MessageBox(hWnd, "信息4", "信息", MB_OK);
		break;
	case 1005:
		MessageBox(hWnd, "信息5", "信息", MB_OK);
		break;
	}
}

// 回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	switch (msgID)
	{
	case WM_COMMAND:
		menu_Command(hWnd, wParam);
		break;
	case WM_CREATE:
		menu_Creat(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}