#include "..\includes\App.h"
#include <memory>
#include "imgui.h"
#include "MathClass.h"

#include "Flower.h"
#include "StructDefs.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants"), _light(_wnd.Gfx())
{
	_flower = std::make_unique<Flower>(_wnd.Gfx(), "leaf.tga", "pinkPetal.tga");
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
	_wnd.Gfx().BeginFrame(0.7f, 0.73f, 0.78f);
	_wnd.Gfx().SetCamera(_cam.GetMatrix());

	//Bind the light in the pipeline
	_light.Bind(_wnd.Gfx(), _wnd.Gfx().GetCamera());

	//Draw the light as it has a model representing it
	_light.Draw(_wnd.Gfx());

	//ImGui::ShowDemoWindow();
	_flower->Update(dt);
	_flower->Draw(_wnd.Gfx());

	if(ImGui::Begin("Plant Editor"))
	{
		_flower->SpawnImgui(_wnd.Gfx());
	}
	ImGui::End();

	if (ImGui::Begin("Scene Parameters"))
	{
		ImGui::Text("Camera Controls");
		_cam.SpawnImguiControlWindow();
		ImGui::Text("Light Controls");
		_light.SpawnControlWindow();
	}
	ImGui::End();

	//present
	_wnd.Gfx().EndFrame();
}
