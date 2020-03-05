#include "..\includes\Bush.h"
#include "MathClass.h"
#include <string>

Bush::Bush(Graphics& gfx, unsigned int faceCount, const char* textureName, const char* bushName) :
	_faceCount(faceCount), _bushName(bushName), _texName(textureName)
{
	_faceVector.reserve(_faceLimit);
	DirectX::XMFLOAT3 zero = {};

	for (int i = 0; i < _faceLimit; ++i)
	{
		_faceVector.emplace_back(std::make_unique<BushFace>(gfx, textureName, zero, zero, zero, zero, zero, zero));
	}
}

void Bush::Update(float dt) noexcept
{
	const float divisions = (PI) / _faceCount;

	for (int i = 0; i < _faceCount; i++)
	{
		auto leafTransform = _faceVector[i]->GetFaceTransform();
		DirectX::XMFLOAT3 f3 = {};
		//Rotate the leaf around the center
		f3 = leafTransform->GetWorldRotation();
		f3.x = divisions * i + 1.0f;
		f3.y = 0.0f;
		f3.z = 0.0f;
		leafTransform->SetWorldRotation(f3);
		//Position the leaf
		f3 = leafTransform->GetPosition();
		f3.x = _x;
		f3.y = _y;
		f3.z = _z;
		leafTransform->SetPosition(f3);
		//TODO add the scale
	}
}

void Bush::Draw(Graphics& gfx) noexcept
{
	if (_faceCount > 0 && _faceCount <= _faceLimit)
	{
		for (int i = 0; i < _faceCount; ++i)
		{
			_faceVector[i]->Draw(gfx);
		}
	}
}

void Bush::SpawnImGui(Graphics& gfx) noexcept
{
	ImGui::Indent();
	std::string stringForNames = "";
	stringForNames = "Face count: " + std::to_string(_faceCount) + "/" + std::to_string(_faceLimit);
	ImGui::Text(stringForNames.c_str());

	stringForNames = "Add##" + _bushName;
	if (ImGui::SmallButton(stringForNames.c_str()))
	{
		_faceCount = _faceCount == _faceLimit ? _faceCount : _faceCount + 1;
	}
	ImGui::SameLine();
	stringForNames = "Remove##" + _bushName;
	if (ImGui::SmallButton(stringForNames.c_str()))
	{
		_faceCount = _faceCount == 0 ? _faceCount : _faceCount - 1;
	}

	stringForNames = "Bush Position:##" + _bushName;
	if (ImGui::TreeNode(stringForNames.c_str()))
	{
		ImGui::Text("X Position");
		stringForNames = "##XPos" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_x, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Y Position");
		stringForNames = "##YPos" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_y, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Z Position");
		stringForNames = "##ZPos" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_z, -10.0f, 10.0f, "%.2f");
		stringForNames = "Reset to Default##Single" + _bushName;
		if (ImGui::SmallButton(stringForNames.c_str()))
		{
			_x = 0.0f;
			_y = 0.0f;
			_z = 0.0f;
		}
		ImGui::TreePop();
	}
	stringForNames = "Bush Scale:##" + _bushName;
	if (ImGui::TreeNode(stringForNames.c_str()))
	{
		ImGui::Text("X Scale");
		stringForNames = "##XScale" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_xScale, 0.1f, 10.0f, "%.2f");
		ImGui::Text("Y Scale");
		stringForNames = "##YScale" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_yScale, 0.1f, 10.0f, "%.2f");
		ImGui::Text("Z Scale");
		stringForNames = "##ZScale" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_zScale, -0.1f, 10.0f, "%.2f");
		stringForNames = "Reset to Default##Grouped" + _bushName;
		if (ImGui::SmallButton("Reset to Default"))
		{
			_xScale = 1.0f;
			_yScale = 1.0f;
			_zScale = 1.0f;
		}
		ImGui::TreePop();
	}
	ImGui::Unindent();
}

void Bush::UpdateLocalVertAndInd(Graphics& gfx)
{
	_faceVerts.clear();
	_faceInds.clear();
	_faceVerts.reserve(_faceCount * 6u);
	_faceInds.reserve(_faceCount * 24u);

	unsigned int currentRunSize = 0;
	for (int i = 0; i < _faceCount; i++)
	{
		_faceVector[i]->UpdateOutVertices(gfx);

		for (size_t j = 0; j < _faceVector[i]->_vertOut.size(); j++)
		{
			_faceVerts.push_back(_faceVector[i]->_vertOut[j]);
		}

		for (size_t j = 0; j < _faceVector[i]->_indexOut.size(); j++)
		{
			_faceInds.push_back(_faceVector[i]->_indexOut[j] + currentRunSize);
		}

		currentRunSize = _faceVerts.size();
	}
}
