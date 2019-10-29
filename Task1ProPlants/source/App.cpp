#include "..\includes\App.h"
#include <memory>
#include "imgui.h"
#include "MathClass.h"

#include "TexturedCube.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "DynamicSolid.h"
#include "Leaf.h"
#include "Leaves.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants"), _light(_wnd.Gfx())
{
	_leaves = std::make_unique<Leaves>(_wnd.Gfx(), 0.0f, 4);
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
	_leaves->Update(dt);
	_leaves->DrawLeaves(_wnd.Gfx());

	//Draw the light as it has a model representing it
	_light.Draw(_wnd.Gfx());

	//ImGui::ShowDemoWindow();

	_cam.SpawnImguiControlWindow();
	_light.SpawnControlWindow();
	_leaves->SpawnImGuiWindow();

	//present
	_wnd.Gfx().EndFrame();
}