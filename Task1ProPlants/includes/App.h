#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"

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

	float _dtSpeedFactor = 1.0f;
	//std::unique_ptr<class Stem> _stem;
	//std::vector<std::unique_ptr<class Leaves>> _bunches;
	std::unique_ptr<class Leaf> _leaf;
	Camera _cam;
	PointLight _light;
};

