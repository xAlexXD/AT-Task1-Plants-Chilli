#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"

class App
{
public:
	App();
	~App();
	int Go();
private:
	void DoFrame();
private:
	ImguiManager _imgui;
	Window _wnd;
	Timer _timer;

	bool _showDemoWindow = true;
	std::vector<std::unique_ptr<class Cube>> _cubes;
};

