#include "Window.h"
#include <sstream>

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

const char* Window::WindowClass::GetName() noexcept
{
	return _windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return _windowClass._hInstance;
}

//Window Related Functions

Window::Window(int width, int height, const char* name)
{
	//Calc window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	if (FAILED(AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false)))
	{
		throw WND_LAST_EXCEPT();
	}

	_width = wr.right - wr.left;
	_height = wr.bottom - wr.top;

	//Create window and store hWnd
	_hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, _width, _height,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	if (_hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

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

//Custom window exception functions
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	: ExceptionHandler(line, file), hr(hr)
{
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[ERROR CODE] " << GetErrorCode() << std::endl
		<< "[DESCRIPTION] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* msgBuf = nullptr;
	DWORD msgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&msgBuf),
		0,
		nullptr
	);
	if (msgLen == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = msgBuf;
	LocalFree(msgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
