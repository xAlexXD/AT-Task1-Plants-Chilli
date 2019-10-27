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
	std::vector<std::unique_ptr<class TexturedCube>> _cubes;
	std::vector<std::unique_ptr<class Cylinder>> _cylinders;
	std::vector<std::unique_ptr<class Pyramid>> _pyramids;
	std::unique_ptr<class DynamicSolid> _dynamicCube;
	Camera _cam;
	PointLight _light;
};

