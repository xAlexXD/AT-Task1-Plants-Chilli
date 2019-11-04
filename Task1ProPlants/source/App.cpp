#include "..\includes\App.h"
#include <memory>
#include "imgui.h"
#include "MathClass.h"

#include "Leaf.h"
#include "Leaves.h"
#include "Stem.h"
#include "StructDefs.h"

App::App() : _wnd(1280, 720, "AT Task1 Proc Plants"), _light(_wnd.Gfx())
{
	DirectX::XMFLOAT3 zero = {};
	//_leaf = std::make_unique<Leaf>(_wnd.Gfx(), "leaf.tga", zero, zero, zero, zero, zero, zero);
	_bunches.push_back(std::make_unique<Leaves>(_wnd.Gfx(), 4, "leaf.tga", "Leaf"));
	_bunches.push_back(std::make_unique<Leaves>(_wnd.Gfx(), 4, "pinkPetal.tga", "Petal"));
	_stem = std::make_unique<Stem>(_wnd.Gfx(), DirectX::XMFLOAT3(0.0f, 0.0f, 1.75f), zero, zero, zero, zero, zero);
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

	//Update and draw leaves
	for (auto& bunch : _bunches)
	{
		bunch->Update(dt);
		bunch->DrawLeaves(_wnd.Gfx());
	}

	_stem->Update(dt);
	_stem->Draw(_wnd.Gfx());

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

	//_stem->SpawnImGuiWindow(_wnd.Gfx());

	//_leaf->SpawnImGuiWindow(_wnd.Gfx());

	SpawnExporterWindow();

	//present
	_wnd.Gfx().EndFrame();
}

void App::GatherModelDataAndExport() noexcept
{
	//Create a vector of vert vectors, ind vectors and texture string vectors
	std::vector<std::vector<TexturedVertex>> vecOfVertVecs(3, std::vector<TexturedVertex>());
	std::vector<std::vector<int>> vecOfIndVecs(3, std::vector<int>());
	std::vector<std::string> vecOfTexNames;

	//Reserve 3 in each for now as they will store the stem and the two bunches
	vecOfVertVecs.reserve(3);
	vecOfIndVecs.reserve(3);
	vecOfTexNames.reserve(3);

	int currentOffsetVert = 0;

	//Stem
	_stem->UpdateLocalVertsAndInds(_wnd.Gfx());
	vecOfTexNames.push_back(_stem->_texName);

	for (size_t i = 0; i < _stem->_vertOut.size(); i++)
	{
		vecOfVertVecs[0].push_back(_stem->_vertOut[i]);
	}

	for (size_t i = 0; i < _stem->_indexOut.size(); i++)
	{
		vecOfIndVecs[0].push_back(_stem->_indexOut[i] + currentOffsetVert);
	}

	currentOffsetVert = vecOfVertVecs[0].size() + vecOfVertVecs[1].size() + vecOfVertVecs[2].size();

	//Leaves and petals
	for (int i = 0; i < _bunches.size(); i++)
	{
		//for each bunch, add the verts and inds to the grouped vectors
		_bunches[i]->UpdateLocalVertAndInd(_wnd.Gfx());
		vecOfTexNames.push_back(_bunches[i]->_texName);

		for (size_t j = 0; j < _bunches[i]->_leafVerts.size(); j++)
		{
			vecOfVertVecs[i+1].push_back(_bunches[i]->_leafVerts[j]);
		}

		for (size_t j = 0; j < _bunches[i]->_leafIndices.size(); j++)
		{
			vecOfIndVecs[i+1].push_back(_bunches[i]->_leafIndices[j] + currentOffsetVert);
		}

		currentOffsetVert = vecOfVertVecs[0].size() + vecOfVertVecs[1].size() + vecOfVertVecs[2].size();
	}

	//Push them into the obj exporter
	_exporter.ExportToObj("PinkFlower", std::move(vecOfVertVecs), std::move(vecOfIndVecs), std::move(vecOfTexNames));
}

void App::SpawnExporterWindow() noexcept
{
	if (ImGui::Begin("Exporter Window"))
	{
		if (ImGui::Button("Export to .obj"))
		{
			GatherModelDataAndExport();
		}
	}
	ImGui::End();
}
