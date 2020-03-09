#include "..\includes\Grass.h"
#include "ObjExporter.h"
#include "imgui.h"

Grass::Grass(Graphics& gfx, const char* grassTexture) : _baseGrassTexture(grassTexture)
{
	DirectX::XMFLOAT3 zero = {};
	_bladeVector.emplace_back(std::make_unique<GrassBlade>(gfx, "Blade 0", grassTexture, zero, zero, zero, zero, zero, zero));
}

Grass::~Grass()
{
}

void Grass::Update(float dt)
{
	for (const auto& blade : _bladeVector)
	{
		blade->Update(dt);
	}
}

void Grass::Draw(Graphics& gfx)
{
	for (const auto& blade : _bladeVector)
	{
		blade->Draw(gfx);
	}
}

void Grass::SpawnImgui(Graphics& gfx)
{
	ImGui::Text("Grass Blade Settings:");
	if (ImGui::SmallButton("Add##Blade"))
	{
		std::string name = "Blade " + std::to_string(_bladeVector.size());
		DirectX::XMFLOAT3 zero = {};
		_bladeVector.emplace_back(std::make_unique<GrassBlade>(gfx, name.c_str(), _baseGrassTexture, zero, zero, zero, zero, zero, zero));
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove##Blade"))
	{
		if (_bladeVector.size() > 0)
		{
			_bladeVector.pop_back();
		}
	}
	for (auto& blade : _bladeVector)
	{
		if (ImGui::CollapsingHeader(blade->GetName()))
		{
			blade->SpawnImGui(gfx);
		}
	}
}

void Grass::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir)
{
	//Create a vector of vert vectors, ind vectors and texture string vectors
	int totalReserve = _bladeVector.size();
	std::vector<std::vector<TexturedVertex>> vecOfVertVecs(totalReserve, std::vector<TexturedVertex>());
	std::vector<std::vector<int>> vecOfIndVecs(totalReserve, std::vector<int>());
	std::vector<std::string> vecOfTexNames;

	//Calc how many it needs to reserve and reserve that many in each
	vecOfVertVecs.reserve(totalReserve);
	vecOfIndVecs.reserve(totalReserve);
	vecOfTexNames.reserve(totalReserve);

	int currentOffsetVert = 0;

	//Bushes
	for (int i = 0; i < _bladeVector.size(); ++i)
	{
		//for each bunch, add the verts and inds to the grouped vectors
		_bladeVector[i]->UpdateOutVertices(gfx);
		vecOfTexNames.push_back(_bladeVector[i]->GetTexName());

		for (size_t j = 0; j < _bladeVector[i]->_vertOut.size(); ++j)
		{
			vecOfVertVecs[i].push_back(_bladeVector[i]->_vertOut[j]);
		}

		for (size_t j = 0; j < _bladeVector[i]->_indexOut.size(); ++j)
		{
			vecOfIndVecs[i].push_back(_bladeVector[i]->_indexOut[j] + currentOffsetVert);
		}

		currentOffsetVert = 0;
		for (const auto& vec : vecOfVertVecs)
		{
			currentOffsetVert += vec.size();
		}
	}

	//Push them into the obj exporter
	exporter.ExportToObj("Grass", outDir, std::move(vecOfVertVecs), std::move(vecOfIndVecs), std::move(vecOfTexNames));
}
