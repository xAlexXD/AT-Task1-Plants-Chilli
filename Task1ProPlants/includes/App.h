#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
	App();
	~App();
	int Go();
private:
	void DoFrame();
private:
	Window _wnd;
	Timer _timer;

	std::vector<std::unique_ptr<class Cube>> _cubes;
};

