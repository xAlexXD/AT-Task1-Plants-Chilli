#include "..\includes\App.h"
#include <memory>
#include "imgui.h"
#include "MathClass.h"

#include "Flower.h"
#include "Bushes.h"
#include "Grass.h"
#include "StructDefs.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants"), _light(_wnd.Gfx())
{
	_plants.reserve(3);
	_plants.emplace_back(std::make_unique<Flower>(_wnd.Gfx(), "leaf.tga", "pinkPetal.tga"));
	_plants.emplace_back(std::make_unique<Bushes>(_wnd.Gfx(), "bushTexture.tga"));
	_plants.emplace_back(std::make_unique<Grass>(_wnd.Gfx(), "grassTexture.tga"));
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
	_wnd.Gfx().SetExportCamera(_cam.GetExportMatrix());

	//Bind the light in the pipeline
	_light.Bind(_wnd.Gfx(), _wnd.Gfx().GetCamera());

	//MAKE SURE MODELS ARE IN BETWEEN THESE LIGHTS OR IT ALL GOES WRONG FAM
	_plants[_currentSelection]->Update(dt);
	_plants[_currentSelection]->Draw(_wnd.Gfx());

	//Draw the light as it has a model representing it
	_light.Draw(_wnd.Gfx());

	//ImGui::ShowDemoWindow();

	//Setup for the plant editor segment of the UI
	ImGuiWindowFlags guiFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(1000, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(280, 720), ImGuiCond_Always);
	if(ImGui::Begin("Plant Editor", NULL, guiFlags))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None | ImGuiTabBarFlags_NoTooltip;
		if (ImGui::BeginTabBar("Plant Options##Tab", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Flower##Tab"))
			{
				_currentSelection = GuiOrder::FLOWER;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Bushes##Tab"))
			{
				_currentSelection = GuiOrder::BUSH;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Grass##Tab"))
			{
				_currentSelection = GuiOrder::GRASS;
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		_plants[_currentSelection]->SpawnImgui(_wnd.Gfx());
	}
	ImGui::End();

	//Set up for the scene elements, camera, light etc.
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(250, 720), ImGuiCond_Always);
	guiFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	if (ImGui::Begin("Scene Parameters", NULL, guiFlags))
	{
		ImGui::Text("Camera Controls:");
		_cam.SpawnImguiControlWindow();
		ImGui::Text("Light Controls:");
		_light.SpawnControlWindow();
		ImGui::Text("Model Exporter:");
		ImGui::Indent();
		ImGui::Text("File Name:");
		ImGui::InputTextWithHint("##OutnameText", "Just standard letters and numbers!", _outNameArray, IM_ARRAYSIZE(_outNameArray));
		ImGui::Text("Out Directory:");
		ImGui::InputTextWithHint("##OutdirText", "Outdir must be with /!", _outDirArray, IM_ARRAYSIZE(_outDirArray));
		if (ImGui::SmallButton("Export Model"))
		{
			std::string dir(_outDirArray);
			std::string name(_outNameArray);
			_plants[_currentSelection]->Export(_wnd.Gfx(), _exporter, dir.c_str(), name.c_str());
		}
		ImGui::Unindent();
	}
	ImGui::End();

	//present
	_wnd.Gfx().EndFrame();
}
