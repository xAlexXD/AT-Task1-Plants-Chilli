#include <Windows.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(1337);
			break;
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			break;
		case WM_CHAR:
			//Returns Character pressed with modifers (shift + d == D)
			break;
		case WM_LBUTTONDOWN:
			//Left mouse button down
			//POINTS pt = MAKEPOINTS(lParam); // Gets x and y of mouse
			break;
		case WM_RBUTTONDOWN:
			//Right mouse button down
			//POINTS pt = MAKEPOINTS(lParam); // Gets x and y of mouse
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const auto windowClassName = L"AT Proc Plants";
	WNDCLASSEX wc = { 0 };
	//Register Window Class
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hIconSm = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = windowClassName;
	RegisterClassEx(&wc);

	//Create window instance
	HWND _windowHandle = CreateWindowEx(0, windowClassName, L"AT Proc Plants", WS_CAPTION | WS_OVERLAPPEDWINDOW, 200, 200, 800, 600, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(_windowHandle, SW_SHOW);

	//Message loop
	MSG msg;
	BOOL gResult;
	while ( (gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;
}