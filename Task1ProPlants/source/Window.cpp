#include "Window.h"

////Window class related functions
Window::WindowClass Window::WindowClass::_windowClass;

Window::WindowClass::WindowClass() noexcept : _hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	//Register Window Class
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hIconSm = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(_windowClassName, GetInstance());
}

const LPCWSTR Window::WindowClass::GetName() noexcept
{
	return _windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return _windowClass._hInstance;
}

//Window Related Functions

Window::Window(int width, int height, const LPCWSTR name) noexcept
{
	//Calc window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	_width = wr.right - wr.left;
	_height = wr.bottom - wr.top;

	//Create window and store hWnd
	_hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, _width, _height,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	//Show Window
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(_hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Check if the message is the window being created and highjack it to point to our member function by adding the reference "this" to the create window function
	if (msg == WM_NCCREATE)
	{
		//Extract ptr to window class from creation data
		const CREATESTRUCTW* const createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam); //Recasting lParam to a createstruct so we can access our "this" from the create window
		Window* const wnd = static_cast<Window*>(createStruct->lpCreateParams);

		//Set WinAPI-managed user data to store ptr to window class. Creates a link between the window itself and the class we want to control it
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));

		//Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		//Forward message to the window class handler
		return wnd->HandleMsg(hWnd, msg, wParam, lParam);

		//Messages after this point AKA initialising the window will be handled by HandleMsgThunk
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Since the WINAPI cant call member functions directly we use this as a middle man to HandleMsg

	//retrieve ptr to window class
	Window* const wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//Forward the message to our member function because we changed GWLP_Userdata to point to our window class instance
	return wnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(1337);
			//Add a return to reprevent the default window handler from destroying our window and allow our destructor to handle it instead
			return 0;
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
