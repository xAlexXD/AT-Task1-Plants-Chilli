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
	ImGui::Indent();
	std::string stringForNames = "";
	stringForNames = "Leaf count: " + std::to_string(_leafCount) + "/" + std::to_string(_leafLimit);
	ImGui::Text(stringForNames.c_str());

	stringForNames = "Add##" + _bunchName;
	if (ImGui::SmallButton(stringForNames.c_str()))
	{
		_leafCount = _leafCount == _leafLimit ? _leafCount : _leafCount + 1;
	}
	ImGui::SameLine();
	stringForNames = "Remove##" + _bunchName;
	if (ImGui::SmallButton(stringForNames.c_str()))
	{
		_leafCount = _leafCount == 0 ? _leafCount : _leafCount - 1;
	}

	stringForNames = "Single Leaf Parameters:##" + _bunchName;
	if (ImGui::TreeNode(stringForNames.c_str()))
	{
		ImGui::Text("Tilt");
		stringForNames = "##Tilt" + _bunchName;
		ImGui::SliderAngle(stringForNames.c_str(), &_leafTilt, -180.0f, 180.0f);
		ImGui::Text("X Rotation");
		stringForNames = "##XRotSingle" + _bunchName;
		ImGui::SliderAngle(stringForNames.c_str(), &_leafXRot, -180.0f, 180.0f);
		ImGui::Text("Z Rotation");
		stringForNames = "##ZRotSingle" + _bunchName;
		ImGui::SliderAngle(stringForNames.c_str(), &_leafYRot, -180.0f, 180.0f);
		stringForNames = "Reset to Default##Single" + _bunchName;
		if (ImGui::SmallButton(stringForNames.c_str()))
		{
			_leafTilt = 0.0f;
			_leafXRot = 0.0f;
			_leafYRot = 0.0f;
		}
		ImGui::TreePop();
	}
	stringForNames = "Grouped Leaf Parameters:##" + _bunchName;
	if (ImGui::TreeNode(stringForNames.c_str()))
	{
		ImGui::Text("X Position");
		stringForNames = "##XPos" + _bunchName;
		ImGui::SliderFloat(stringForNames.c_str(), &_x, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Y Position");
		stringForNames = "##YPos" + _bunchName;
		ImGui::SliderFloat(stringForNames.c_str(), &_y, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Z Position");
		stringForNames = "##ZPos" + _bunchName;
		ImGui::SliderFloat(stringForNames.c_str(), &_z, -10.0f, 10.0f, "%.2f");
		ImGui::Text("X Rotation");
		stringForNames = "##XRotGrouped" + _bunchName;
		ImGui::SliderAngle(stringForNames.c_str(), &_leavesXRot, -180.0f, 180.0f);
		ImGui::Text("Y Rotation");
		stringForNames = "##YRotGrouped" + _bunchName;
		ImGui::SliderAngle(stringForNames.c_str(), &_leavesYRot, -180.0f, 180.0f);
		ImGui::Text("Z Rotation");
		stringForNames = "##ZRotGrouped" + _bunchName;
		ImGui::SliderAngle(stringForNames.c_str(), &_leavesZRot, -180.0f, 180.0f);
		stringForNames = "Reset to Default##Grouped" + _bunchName;
		if (ImGui::SmallButton("Reset to Default"))
		{
			_leavesXRot = 0.0f;
			_leavesYRot = 0.0f;
			_leavesZRot = 0.0f;
			_x = 0.0f;
			_y = 0.0f;
			_z = 0.0f;
		}
		ImGui::TreePop();
	}
	ImGui::Unindent();
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
