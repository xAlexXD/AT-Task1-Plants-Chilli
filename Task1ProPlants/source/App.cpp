#include "..\includes\App.h"
#include <memory>
#include "imgui.h"
#include "MathClass.h"

#include "Leaf.h"
#include "Leaves.h"
#include "Stem.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants"), _light(_wnd.Gfx())
{
	DirectX::XMFLOAT3 zero = {};
	//_leaf = std::make_unique<Leaf>(_wnd.Gfx(), "leaf.tga", zero, zero, zero, zero, zero, zero);
	_bunches.push_back(std::make_unique<Leaves>(_wnd.Gfx(), 4, "leaf.tga", "Leaf"));
	//_bunches.push_back(std::make_unique<Leaves>(_wnd.Gfx(), 4, "pinkPetal.tga", "Petal"));
	//_stem = std::make_unique<Stem>(_wnd.Gfx(), DirectX::XMFLOAT3(0.0f, 0.0f, 1.75f), zero, zero, zero, zero, zero);
	_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 720.0f / 1280.0f, 0.5f, 100.0f));
}

App::~App()
{
}

int App::Go()
{
	while (true)
	{
		//processes messages pending but doesnt block application flow. If ecode has a value after getting fed optional from process messages its time to quit
		if (const auto ecode = Window::ProcessMessages())
		{
			//if the returned optional actually has a value its time to leave
			return *ecode;
		}
		//Else just carry on with normal execution
		DoFrame();
	}
}

void App::DoFrame()
{
	//Creates a mark for this updates delta time and clears back buffer to solid colour
	auto dt = _timer.Mark() * _dtSpeedFactor;

	//Start DirectX frame
	_wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	_wnd.Gfx().SetCamera(_cam.GetMatrix());

	//Bind the light in the pipeline
	_light.Bind(_wnd.Gfx(), _wnd.Gfx().GetCamera());

	//Update and draw leaves
	for (auto& bunch : _bunches)
	{
		bunch->Update(dt);
		bunch->DrawLeaves(_wnd.Gfx());
	}

	//_stem->Update(dt);
	//_stem->Draw(_wnd.Gfx());

	//_leaf->Update(dt);
	//_leaf->Draw(_wnd.Gfx());

	//Draw the light as it has a model representing it
	_light.Draw(_wnd.Gfx());

	//ImGui::ShowDemoWindow();

	_cam.SpawnImguiControlWindow();
	_light.SpawnControlWindow();

	for (auto& bunch : _bunches)
	{
		bunch->SpawnImGuiWindow(_wnd.Gfx());
	}

	//_leaf->SpawnImGuiWindow(_wnd.Gfx());

	//present
	_wnd.Gfx().EndFrame();
}