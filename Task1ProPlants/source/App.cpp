#include "..\includes\App.h"
#include <memory>
#include "imgui.h"
#include "MathClass.h"

#include "TexturedCube.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "DynamicSolid.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants"), _light(_wnd.Gfx())
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> position(-10.0f, 10.0f); //Chosing a random dist between 0 and 2PI aka full radius for radians
	std::uniform_real_distribution<float> localRot(0.0f, PI * 2.0f); //Chosing a random dist between 0 and 2PI aka full radius for radians
	std::uniform_real_distribution<float> localRotDelta(0.0f, PI * 2.0f);
	std::uniform_real_distribution<float> matColour(0.0f, 1.0f);
	std::uniform_int_distribution<int> tessalation(3, 30);

	//for (size_t i = 0; i < 20; i++)
	//{
	//	_cubes.push_back(std::make_unique<TexturedCube>(_wnd.Gfx(), 
	//		DirectX::XMFLOAT3(position(rng), position(rng), position(rng)),
	//		DirectX::XMFLOAT3(localRot(rng), localRot(rng), localRot(rng)),
	//		DirectX::XMFLOAT3(0.0f,0.0f,0.0f),
	//		DirectX::XMFLOAT3(localRotDelta(rng), localRotDelta(rng), localRotDelta(rng)),
	//		i % 2 == 0 ? "sheb.tga" : "UWE_Logo.tga"
	//	));
	//}

	//for (size_t i = 0; i < 20; i++)
	//{
	//	_cylinders.push_back(std::make_unique<Cylinder>(_wnd.Gfx(),
	//		DirectX::XMFLOAT3(position(rng), position(rng), position(rng)),
	//		DirectX::XMFLOAT3(localRot(rng), localRot(rng), localRot(rng)),
	//		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	//		DirectX::XMFLOAT3(localRotDelta(rng), localRotDelta(rng), localRotDelta(rng)),
	//		DirectX::XMFLOAT3(matColour(rng), matColour(rng), matColour(rng)),
	//		tessalation(rng)
	//		));
	//}

	//for (size_t i = 0; i < 20; i++)
	//{
	//	_pyramids.push_back(std::make_unique<Pyramid>(_wnd.Gfx(),
	//		DirectX::XMFLOAT3(position(rng), position(rng), position(rng)),
	//		DirectX::XMFLOAT3(localRot(rng), localRot(rng), localRot(rng)),
	//		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	//		DirectX::XMFLOAT3(localRotDelta(rng), localRotDelta(rng), localRotDelta(rng)),
	//		DirectX::XMFLOAT3(matColour(rng), matColour(rng), matColour(rng)),
	//		tessalation(rng)
	//		));
	//}

	//_cubes.push_back(std::make_unique<TexturedCube>(_wnd.Gfx()));
	_dynamicCube = std::make_unique<DynamicSolid>(
			_wnd.Gfx(),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(matColour(rng), matColour(rng), matColour(rng))
		);
	
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

	//updates the game objects in the scene
	//for (auto& cube : _cubes)
	//{
	//	cube->Update(_wnd._keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	//	cube->Draw(_wnd.Gfx());
	//}

	//for (auto& cylinder : _cylinders)
	//{
	//	cylinder->Update(_wnd._keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	//	cylinder->Draw(_wnd.Gfx());
	//}

	//for (auto& pyramid : _pyramids)
	//{
	//	pyramid->Update(_wnd._keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	//	pyramid->Draw(_wnd.Gfx());
	//}

	_dynamicCube->Update(_wnd._keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	_dynamicCube->Draw(_wnd.Gfx());

	//Draw the light as it has a model representing it
	_light.Draw(_wnd.Gfx());

	//Simple box to adjust speed of simulation
	if (ImGui::Begin("Simulation Speed"))
	{
		//Small text box using c style print fs to pass in parameters
		ImGui::Text("Application Average: %.3f ms || Framerate: %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//Slider to adjust the factors value
		ImGui::SliderFloat("Speed Modifer", &_dtSpeedFactor, 0.0f, 4.0f);
	}
	//After processing a window for imgui always end
	ImGui::End();

	//ImGui::ShowDemoWindow();

	_cam.SpawnImguiControlWindow();
	_light.SpawnControlWindow();
	_dynamicCube->SpawnImguiVerticesWindow();

	//present
	_wnd.Gfx().EndFrame();
}