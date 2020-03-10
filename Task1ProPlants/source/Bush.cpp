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

	//Cache the sizes of a faces inds and verts
	_perFaceInds = _faceVector.front()->GetIndCount();
	_perFaceVerts = _faceVector.front()->GetVertCount();
}

void Bush::Update(float dt) noexcept
{
	const float divisions = (PI) / _faceCount;

	for (int i = 0; i < _faceCount; i++)
	{
		auto bushTransform = _faceVector[i]->GetFaceTransform();
		DirectX::XMFLOAT3 f3 = {};
		//Rotate the Bush around the center
		f3 = bushTransform->GetWorldRotation();
		f3.x = divisions * i + _xRot;
		f3.y = _yRot;
		f3.z = _zRot;
		bushTransform->SetWorldRotation(f3);
		//Position the Bush
		f3 = bushTransform->GetPosition();
		f3.x = _x;
		f3.y = _y;
		f3.z = _z;
		bushTransform->SetPosition(f3);
		//SCALE DE BUSH
		bushTransform->SetScale(DirectX::XMFLOAT3(_xScale, _yScale, _zScale));
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
	ImGui::Text("Change Texture (Must be a .TGA)");
	stringForNames = "##TextureDir" + _bushName;
	ImGui::InputTextWithHint(stringForNames.c_str(), "Dir must be with /!", _textureArray, IM_ARRAYSIZE(_textureArray));
	if (ImGui::SmallButton("Apply Texture"))
	{
		UpdateTexture(gfx);
	}

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
		stringForNames = "Reset to Default##BushyPos" + _bushName;
		if (ImGui::SmallButton(stringForNames.c_str()))
		{
			_x = 0.0f;
			_y = 0.0f;
			_z = 0.0f;
		}
		ImGui::TreePop();
	}
	stringForNames = "Bush Rotation:##" + _bushName;
	if (ImGui::TreeNode(stringForNames.c_str()))
	{
		ImGui::Text("X Rotation");
		stringForNames = "##XRot" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_xRot, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Y Rotation");
		stringForNames = "##YRot" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_yRot, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Z Rotation");
		stringForNames = "##ZRot" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_zRot, -10.0f, 10.0f, "%.2f");
		stringForNames = "Reset to Default##BushyRot" + _bushName;
		if (ImGui::SmallButton(stringForNames.c_str()))
		{
			_xRot = 0.0f;
			_yRot = 0.0f;
			_zRot = 0.0f;
		}
		ImGui::TreePop();
	}
	stringForNames = "Bush Scale:##" + _bushName;
	if (ImGui::TreeNode(stringForNames.c_str()))
	{
		ImGui::Text("Radius");
		stringForNames = "##Radius" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_xScale, 0.1f, 3.0f, "%.05f");
		ImGui::Text("Height");
		stringForNames = "##Height" + _bushName;
		ImGui::SliderFloat(stringForNames.c_str(), &_zScale, -0.1f, 3.0f, "%.05f");
		stringForNames = "Reset to Default##BushyScale" + _bushName;
		if (ImGui::SmallButton(stringForNames.c_str()))
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
	_faceVerts.reserve(_faceCount * _perFaceVerts);
	_faceInds.reserve(_faceCount * _perFaceInds);

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

void Bush::UpdateTexture(Graphics& gfx)
{
	std::string dir(_textureArray);
	for (auto& face : _faceVector)
	{
		face->UpdateTexture(gfx, dir.c_str());
	}
}
