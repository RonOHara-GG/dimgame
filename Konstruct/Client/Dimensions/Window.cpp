#include "StdAfx.h"
#include "Dimensions.h"
#include "GameState.h"
#include "PlayerCharacter.h"

extern bool g_bExitGame;
extern GameState* g_pGameState;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


HWND CreateRenderWindow(HINSTANCE hInstance)
{
	// Register the window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_DIMENSIONS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("DIMENSIONS CLASS");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	if( !RegisterClassEx(&wcex) )
		return 0;

	// Create the window
	HWND hWnd = CreateWindow(TEXT("DIMENSIONS CLASS"), TEXT("Dimensions"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if( hWnd )
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}

	return hWnd;
}

void UpdateRenderWindow(HWND hWnd)
{
	MSG msg;
	while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
	{
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CloseRenderWindow(HWND hWnd)
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			g_bExitGame = true;
			break;
		/*case WM_LBUTTONUP:
			if( g_pGameState )
			{
				g_pGameState->MouseUpdate(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_KEYDOWN:
			{
				if( (wParam & VK_SPACE) && g_pGameState )
				{
					g_pGameState->GetPlayer()->UseSkill(0, PlayerClass::eCL_Brawler);
				}
				break;
			}*/
		/*case WM_MOUSEMOVE:
			if( (wParam & MK_LBUTTON) && g_pGameState )
			{
				g_pGameState->MouseUpdate(LOWORD(lParam), HIWORD(lParam));
			}
			break;*/
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
