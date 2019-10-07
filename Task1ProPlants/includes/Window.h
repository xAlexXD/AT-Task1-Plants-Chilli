#pragma once
#include "WindowsCustomInclude.h"

class Window
{
private:
	//Singleton manages register and cleanup of window class
	class WindowClass
	{
	public:
		static const LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCWSTR _windowClassName = L"AT Task 1 Plantu";
		static WindowClass _windowClass;
		HINSTANCE _hInstance;
	};

public:
	Window(int width, int height, const LPCWSTR name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	//Cannot set windows callback to member functions so static is like making it global, we'll then use these to call our member function
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int _width;
	int _height;
	HWND _hWnd;
};

