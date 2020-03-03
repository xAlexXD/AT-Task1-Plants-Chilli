#include "..\includes\Leaves.h"
#include "MathClass.h"
#include <string>

Leaves::Leaves(Graphics& gfx, 
	unsigned int leafCount,
	const char* textureName,
	const char* bunchName) :
	_leafCount(leafCount), _bunchName(bunchName), _texName(textureName)
{
	_leafVector.reserve(30);
	DirectX::XMFLOAT3 zero = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < _leafLimit; i++)
	{
		_leafVector.emplace_back(std::make_unique<Leaf>(gfx, textureName, zero, zero, zero, zero, zero, zero));
	}
}

void Leaves::Update(float dt) noexcept
{
	const float divisions = (2 * PI) / _leafCount;

	for (int i = 0; i < _leafCount; i++)
	{
		auto leafTransform = _leafVector[i]->GetLeafTransform();
		DirectX::XMFLOAT3 f3 = {};
		//Tilt the leaf
		f3 = leafTransform->GetLocalRotation();
		f3.y = _leafTilt;
		f3.x = _leafXRot;
		f3.z = _leafYRot;
		leafTransform->SetLocalRotation(f3);
		//Rotate the leaf around the center
		f3 = leafTransform->GetWorldRotation();
		f3.x = divisions * i + _leavesXRot;
		f3.y = _leavesYRot;
		f3.z = _leavesZRot;
		leafTransform->SetWorldRotation(f3);
		//Position the leaf
		f3 = leafTransform->GetPosition();
		f3.x = _x;
		f3.y = _y;
		f3.z = _z;
		leafTransform->SetPosition(f3);
	}
}

void Leaves::DrawLeaves(Graphics& gfx) noexcept
{
	if (_leafCount > 0 && _leafCount <= _leafLimit)
	{
		for (int i = 0; i < _leafCount; i++)
		{
			_leafVector[i]->Draw(gfx);
		}
	}
}

void Leaves::SpawnImGuiWindow(Graphics& gfx) noexcept
{
	std::string stringForNames = "";
	stringForNames = "Leaf count: " + std::to_string(_leafCount) + "/" + std::to_string(_leafLimit);
	ImGui::Text(stringForNames.c_str());

	if (ImGui::SmallButton("Add"))
	{
		_leafCount = _leafCount == _leafLimit ? _leafCount : _leafCount + 1;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove"))
	{
		_leafCount = _leafCount == 0 ? _leafCount : _leafCount - 1;
	}

	if (ImGui::TreeNode("Single Leaf Parameters:"))
	{
		ImGui::SliderAngle("Tilt", &_leafTilt, -180.0f, 180.0f);
		ImGui::SliderAngle("XRot", &_leafXRot, -180.0f, 180.0f);
		ImGui::SliderAngle("zRot", &_leafYRot, -180.0f, 180.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Grouped Leaf Parameters:"))
	{
		ImGui::SliderFloat("XPos", &_x, -10.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("YPos", &_y, -10.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("ZPos", &_z, -10.0f, 10.0f, "%.2f");
		ImGui::SliderAngle("XRot", &_leavesXRot, -180.0f, 180.0f);
		ImGui::SliderAngle("YRot", &_leavesYRot, -180.0f, 180.0f);
		ImGui::SliderAngle("ZRot", &_leavesZRot, -180.0f, 180.0f);
		ImGui::TreePop();
	}

	if (ImGui::SmallButton("Reset"))
	{
		_leafTilt = 0.0f;
		_leafXRot = 0.0f;
		_leafYRot = 0.0f;
		_leavesXRot = 0.0f;
		_leavesYRot = 0.0f;
		_leavesZRot = 0.0f;
		_x = 0.0f;
		_y = 0.0f;
		_z = 0.0f;
	}
}

void Leaves::UpdateLocalVertAndInd(Graphics& gfx)
{
	//For each leaf in the bunch
		//Take the current size of verts and cache it for this run
			//For each vert push it onto the local backup
			//For each index push it onto the local backup but add the offset of the current size of verts

	//Reserve the size required as the leafs wont ever get more of less verts
	_leafVerts.clear();
	_leafIndices.clear();
	_leafVerts.reserve(_leafCount * 4u);
	_leafIndices.reserve(_leafCount * 6u);

	unsigned int currentRunSize = 0;
	for (int i = 0; i < _leafCount; i++)
	{
		_leafVector[i]->UpdateOutVertices(gfx);

		for (size_t j = 0; j < _leafVector[i]->_vertOut.size(); j++)
		{
			_leafVerts.push_back(_leafVector[i]->_vertOut[j]);
		}

		for (size_t j = 0; j < _leafVector[i]->_indexOut.size(); j++)
		{
			_leafIndices.push_back(_leafVector[i]->_indexOut[j] + currentRunSize);
		}

		currentRunSize = _leafVerts.size();
	}
}
