#include "..\includes\Flower.h"
#include "ObjExporter.h"



Flower::Flower(Graphics& gfx, const char* leafTexture, const char* petalTexture) : _baseLeafTexture(leafTexture), _basePetalTexture(petalTexture)
{
	//Initialise with a single bunch of leaves and a single bunch of petals
	DirectX::XMFLOAT3 zero = {};
	_leafBunches.emplace_back(std::make_unique<Leaves>(gfx, 4, leafTexture, "Leaf Bunch 0"));
	_petalBunches.emplace_back(std::make_unique<Leaves>(gfx, 4, petalTexture, "Petal Bunch 0"));
	_stem = std::make_unique<Stem>(gfx, DirectX::XMFLOAT3(0.0f, 0.0f, 1.75f), zero, zero, zero, zero, zero);
}

Flower::~Flower()
{
}

void Flower::Update(float dt)
{
	for (auto& leaves : _leafBunches)
	{
		leaves->Update(dt);
	}
	for (auto& petals : _petalBunches)
	{
		petals->Update(dt);
	}
	_stem->Update(dt);
}

void Flower::Draw(Graphics& gfx)
{
	for (auto& leaves : _leafBunches)
	{
		leaves->DrawLeaves(gfx);
	}
	for (auto& petals : _petalBunches)
	{
		petals->DrawLeaves(gfx);
	}
	_stem->Draw(gfx);
}

void Flower::SpawnImgui(Graphics& gfx)
{
	//Have a window with submenus
	//Each menu contains the settings for each of the bunches so will have to loop through both vectors and spawn their appropirate customisations
	//TreeNode for each leaf and bunch settings
	ImGui::Text("Leaf Bunch Settings:");
	if (ImGui::SmallButton("Add Leaves"))
	{
		std::string name = "Leaf Bunch " + std::to_string(_leafBunches.size());
		_leafBunches.emplace_back(std::make_unique<Leaves>(gfx, 4, _baseLeafTexture, name.c_str()));
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove Leaves"))
	{
		if (_leafBunches.size() > 0)
		{
			_leafBunches.pop_back();
		}
	}
	for (const auto& leaves : _leafBunches)
	{
		if (ImGui::CollapsingHeader(leaves->GetName()))
		{
			leaves->SpawnImGuiWindow(gfx);
		}
	}

	ImGui::Text("Petal Bunch Settings:");
	if (ImGui::SmallButton("Add Petals"))
	{
		std::string name = "Petal Bunch " + std::to_string(_petalBunches.size());
		_petalBunches.emplace_back(std::make_unique<Leaves>(gfx, 4, _baseLeafTexture, name.c_str()));
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove Petals"))
	{
		if (_petalBunches.size() > 0)
		{
			_petalBunches.pop_back();
		}
	}
	for (const auto& petals : _petalBunches)
	{
		if (ImGui::CollapsingHeader(petals->GetName()))
		{
			petals->SpawnImGuiWindow(gfx);
		}
	}
}

void Flower::ExportFlower(Graphics& gfx, ObjExporter& exporter)
{
	//Create a vector of vert vectors, ind vectors and texture string vectors
	std::vector<std::vector<TexturedVertex>> vecOfVertVecs(3, std::vector<TexturedVertex>());
	std::vector<std::vector<int>> vecOfIndVecs(3, std::vector<int>());
	std::vector<std::string> vecOfTexNames;

	//Calc how many it needs to reserve and reserve that many in each
	int totalReserve = _leafBunches.size() + _petalBunches.size() + 1;	//+1 for the stem
	vecOfVertVecs.reserve(totalReserve);
	vecOfIndVecs.reserve(totalReserve);
	vecOfTexNames.reserve(totalReserve);

	int currentOffsetVert = 0;
	int currentVecOffset = 0;

	//Stem
	_stem->UpdateLocalVertsAndInds(gfx);
	vecOfTexNames.push_back(_stem->_texName);

	for (size_t i = 0; i < _stem->_vertOut.size(); ++i)
	{
		vecOfVertVecs[currentVecOffset].push_back(_stem->_vertOut[i]);
	}

	for (size_t i = 0; i < _stem->_indexOut.size(); ++i)
	{
		vecOfIndVecs[currentVecOffset].push_back(_stem->_indexOut[i] + currentOffsetVert);
	}

	currentOffsetVert = 0;
	for (const auto& vec : vecOfVertVecs)
	{
		currentOffsetVert += vec.size();
	}
	++currentVecOffset;

	//Leaves
	for (int i = 0; i < _leafBunches.size(); ++i)
	{
		//for each bunch, add the verts and inds to the grouped vectors
		_leafBunches[i]->UpdateLocalVertAndInd(gfx);
		vecOfTexNames.push_back(_leafBunches[i]->_texName);

		for (size_t j = 0; j < _leafBunches[i]->_leafVerts.size(); ++j)
		{
			vecOfVertVecs[i + currentVecOffset].push_back(_leafBunches[i]->_leafVerts[j]);
		}

		for (size_t j = 0; j < _leafBunches[i]->_leafIndices.size(); ++j)
		{
			vecOfIndVecs[i + currentVecOffset].push_back(_leafBunches[i]->_leafIndices[j] + currentOffsetVert);
		}

		currentOffsetVert = 0;
		for (const auto& vec : vecOfVertVecs)
		{
			currentOffsetVert += vec.size();
		}
	}

	currentVecOffset += _leafBunches.size();

	//Petals
	for (int i = 0; i < _petalBunches.size(); ++i)
	{
		//for each bunch, add the verts and inds to the grouped vectors
		_petalBunches[i]->UpdateLocalVertAndInd(gfx);
		vecOfTexNames.push_back(_petalBunches[i]->_texName);

		for (size_t j = 0; j < _petalBunches[i]->_leafVerts.size(); ++j)
		{
			vecOfVertVecs[i + currentVecOffset].push_back(_petalBunches[i]->_leafVerts[j]);
		}

		for (size_t j = 0; j < _petalBunches[i]->_leafIndices.size(); ++j)
		{
			vecOfIndVecs[i + currentVecOffset].push_back(_petalBunches[i]->_leafIndices[j] + currentOffsetVert);
		}

		currentOffsetVert = 0;
		for (const auto& vec : vecOfVertVecs)
		{
			currentOffsetVert += vec.size();
		}
	}

	//Push them into the obj exporter
	exporter.ExportToObj("Flower", std::move(vecOfVertVecs), std::move(vecOfIndVecs), std::move(vecOfTexNames));
}
