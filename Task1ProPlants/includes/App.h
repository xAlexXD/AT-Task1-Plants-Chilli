#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "ObjExporter.h"
#include "PlantBase.h"

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
	char _outDirArray[128] = "";
	char _outNameArray[128] = "";

	enum GuiOrder
	{
		FLOWER = 0,
		BUSH = 1,
		GRASS = 2
	};

	int _currentSelection = 0;

	//Scene Objects
	std::vector<std::unique_ptr<PlantBase>> _plants;
};

