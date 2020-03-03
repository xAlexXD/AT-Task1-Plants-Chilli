#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "ObjExporter.h"

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
	Camera _cam;
	PointLight _light;
	ObjExporter _exporter;

	//Scene Objects
	std::unique_ptr<class Flower> _flower;
};

