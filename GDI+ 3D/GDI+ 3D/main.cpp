#define _USE_MATH_DEFINES
#include <windows.h>
#include <gdiplus.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <vector>

#pragma comment (lib, "Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#include "definitions.h"

//Initialisation
void init_program()
{
	//Objects
	if (!meshTestObject.Load()){MessageBox(NULL, "Failed to load 'meshTestObject'", "Error", MB_ICONEXCLAMATION);}
}

//Graphics handling
void render(HWND hwnd, HDC hdc)
{
	//Pre-draw
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	WIDTH = rcClient.right - rcClient.left;
	HEIGHT = rcClient.bottom - rcClient.top;

	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
	SelectObject(hdcMem, hBitmap);

	Graphics graphics(hdcMem);    

	Pen whitePen(Color(255, 255, 255, 255), 1);
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	Font debugfont(L"Ms Shell Dlg", 10);

	graphics.FillRectangle(&blackBrush, rcClient.left, rcClient.top, WIDTH, HEIGHT);

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	//Drawing
	vector <Vec3> main_pixel_buffer;
	mainViewport.Update(0, 0, WIDTH, HEIGHT, 0, 1);
	viewMatrix = Mat4x4LookAtLH(&camPos, &Vec3(0, 0, 0), &Vec3(0, 1, 0));
	projMatrix = Mat4x4PerspectiveFovLH(DegToRad(60), float(WIDTH)/float(HEIGHT), 1, 10);

	//Test object
		worldMatrix = Mat4x4RotationX(objRotationX)*Mat4x4RotationY(objRotationY);
		main_pixel_buffer.clear();
		for (unsigned i = 0; i < meshTestObject.vertexbuffer.size(); i++)
		{
			main_pixel_buffer.push_back(Vec3Project(&meshTestObject.vertexbuffer[i], &mainViewport, &worldMatrix, &viewMatrix, &projMatrix));
		}
		//Wireframe
		if (drawMode == 0)
		{
			for (unsigned i = 0; i < meshTestObject.indexbuffer.size(); i++)
			{
				PointF points[3] = {PointF(main_pixel_buffer[unsigned(meshTestObject.indexbuffer[i].x)].x, main_pixel_buffer[unsigned(meshTestObject.indexbuffer[i].x)].y),
									PointF(main_pixel_buffer[unsigned(meshTestObject.indexbuffer[i].y)].x, main_pixel_buffer[unsigned(meshTestObject.indexbuffer[i].y)].y),
									PointF(main_pixel_buffer[unsigned(meshTestObject.indexbuffer[i].z)].x, main_pixel_buffer[unsigned(meshTestObject.indexbuffer[i].z)].y)};
				graphics.DrawPolygon(&whitePen, points, 3);
			}
		}
		//Particle
		else if (drawMode == 1)
		{
			for (unsigned i = 0; i < main_pixel_buffer.size(); i++)
			{
				graphics.FillEllipse(&whiteBrush, int(main_pixel_buffer[i].x - 1), int(main_pixel_buffer[i].y - 1), 2, 2);
			}
		}

	//Post-draw
	RECT rcClip;
	GetClipBox(hdc, &rcClip);
	BitBlt(hdc, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, hdcMem, rcClip.left, rcClip.top, SRCCOPY);

	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
}

//Windows functions
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
    PAINTSTRUCT ps;
	
	switch(msg)
	{
		case WM_CREATE:
			init_program();
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_RIGHT:
					objRotationY += 0.02f;
					break;

				case VK_LEFT:
					objRotationY -= 0.02f;
					break;

				case VK_UP:
					objRotationX += 0.02f;
					break;

				case VK_DOWN:
					objRotationX -= 0.02f;
					break;

				case VK_RETURN:
					objRotationX = 0;
					objRotationY = 0;
					break;

				case VK_SPACE:
					drawMode = drawMode == 1 ? 0 : drawMode + 1;
					break;
			}
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
			break;

		case WM_SIZE:
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
			break;

		case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
			render(hwnd, hdc);
            EndPaint(hwnd, &ps);
			break;

        case WM_DESTROY:
            PostQuitMessage(0);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    //Step 1: Registering the Window Class
	WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "WindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
	if(!RegisterClassEx(&wc))
	{MessageBox(NULL, "The window could not be registered!", "Error", MB_ICONEXCLAMATION | MB_OK); return 0;}

    //Step 2: Creating the Window
	HWND hwnd = CreateWindow("WindowClass", "GDI+ 3D", WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);
	if(hwnd == NULL){MessageBox(NULL, "Window creation failed!", "Error", MB_ICONEXCLAMATION | MB_OK); return 0;}

    ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

    //Step 3: The Message Loop
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
    {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
    }
	
	GdiplusShutdown(gdiplusToken);
	return Msg.wParam;
}
