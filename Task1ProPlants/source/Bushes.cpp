#include "..\includes\Bushes.h"
#include "ObjExporter.h"
#include "imgui.h"

Bushes::Bushes(Graphics& gfx, const char* planeTexture) : _basePlaneTexture(planeTexture)
{
	DirectX::XMFLOAT3 zero = {};
	_bushVector.emplace_back(std::make_unique<Bush>(gfx, 4u, planeTexture, "Bush 0"));
}

Bushes::~Bushes()
{
}

void Bushes::Update(float dt)
{
	for (auto& bush : _bushVector)
	{
		bush->Update(dt);
	}
}

void Bushes::Draw(Graphics& gfx)
{
	for (auto& bush : _bushVector)
	{
		bush->Draw(gfx);
	}
}

void Bushes::SpawnImgui(Graphics& gfx)
{
	ImGui::Text("Bush Settings:");
	if (ImGui::SmallButton("Add##Bush"))
	{
		std::string name = "Bush " + std::to_string(_bushVector.size());
		_bushVector.emplace_back(std::make_unique<Bush>(gfx, 4, _basePlaneTexture, name.c_str()));
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove##Bush"))
	{
		if (_bushVector.size() > 0)
		{
			_bushVector.pop_back();
		}
	}
	for (auto& bush : _bushVector)
	{
		if (ImGui::CollapsingHeader(bush->GetName()))
		{
			bush->SpawnImGui(gfx);
		}
	}
}

void Bushes::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
}
