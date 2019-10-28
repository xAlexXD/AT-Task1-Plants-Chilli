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

	DirectX::XMFLOAT3 _worldOrigin = { 0.0f, 1.5f, 0.0f };

	float _dtSpeedFactor = 1.0f;
	std::unique_ptr<class DynamicSolid> _dynamicCube;
	std::unique_ptr<class Leaf> _leaf;
	Camera _cam;
	PointLight _light;
};

