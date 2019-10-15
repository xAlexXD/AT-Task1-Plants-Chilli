#include "..\includes\App.h"
#include "Cube.h"
#include <memory>
#include "imgui.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> worldRot(0.0f, 3.1415f * 2.0f); //Chosing a random dist between 0 and 2PI aka full radius for radians
	std::uniform_real_distribution<float> localRotDelta(0.0f, 3.1415f * 2.0f); //Chosing a random dist between 0 and 2PI aka full radius for radians
	std::uniform_real_distribution<float> worldRotDelta(0.0f, 3.1415f * 0.3f); //Chosing a random dist between 0 and 2PI aka full radius for radians
	std::uniform_real_distribution<float> rDist(6.0f, 20.0f); 

	for (size_t i = 0; i < 50; i++)
	{
		_cubes.push_back(std::make_unique<Cube>(_wnd.Gfx(), rng, rDist, localRotDelta, worldRotDelta, worldRot));
	}
	
	_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 720.0f / 1280.0f, 0.5f, 40.0f));
	_wnd.Gfx().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
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

	//updates the game objects in the scene
	for (auto& cube : _cubes)
	{
		cube->Update(_wnd._keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		cube->Draw(_wnd.Gfx());
	}

	//Simple box to adjust speed of simulation
	if (ImGui::Begin("Simulation Speed"))
	{
		//Small text box using c style print fs to pass in parameters
		ImGui::Text("Application Average: %.3f ms || Framerate: %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//Slider to adjust the factors value
		ImGui::SliderFloat("Speed Modifer", &_dtSpeedFactor, 0.0f, 4.0f);
	}

	//After processing all windows for imgui always end
	ImGui::End();

	//present
	_wnd.Gfx().EndFrame();
}