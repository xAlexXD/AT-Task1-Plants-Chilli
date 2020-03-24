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

void Bushes::Export(Graphics& gfx, ObjExporter& exporter, const char* outDir, const char* outName)
{
	//Create a vector of vert vectors, ind vectors and texture string vectors
	int totalReserve = _bushVector.size();	//+1 for the stem
	std::vector<std::vector<TexturedVertex>> vecOfVertVecs(totalReserve, std::vector<TexturedVertex>());
	std::vector<std::vector<int>> vecOfIndVecs(totalReserve, std::vector<int>());
	std::vector<std::string> vecOfTexNames;

	//Calc how many it needs to reserve and reserve that many in each
	vecOfVertVecs.reserve(totalReserve);
	vecOfIndVecs.reserve(totalReserve);
	vecOfTexNames.reserve(totalReserve);

	int currentOffsetVert = 0;

	//Bushes
	for (int i = 0; i < _bushVector.size(); ++i)
	{
		//for each bunch, add the verts and inds to the grouped vectors
		_bushVector[i]->UpdateLocalVertAndInd(gfx);
		vecOfTexNames.push_back(_bushVector[i]->_texName);

		for (size_t j = 0; j < _bushVector[i]->_faceVerts.size(); ++j)
		{
			vecOfVertVecs[i].push_back(_bushVector[i]->_faceVerts[j]);
		}

		for (size_t j = 0; j < _bushVector[i]->_faceInds.size(); ++j)
		{
			vecOfIndVecs[i].push_back(_bushVector[i]->_faceInds[j] + currentOffsetVert);
		}

		currentOffsetVert = 0;
		for (const auto& vec : vecOfVertVecs)
		{
			currentOffsetVert += vec.size();
		}
	}

	//Push them into the obj exporter
	exporter.ExportToObj(outName, outDir, std::move(vecOfVertVecs), std::move(vecOfIndVecs), std::move(vecOfTexNames));
}
