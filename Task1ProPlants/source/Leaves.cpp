#include "..\includes\Leaves.h"
#include "MathClass.h"
#include <string>

Leaves::Leaves(Graphics& gfx, 
	unsigned int leafCount) :
	_leafCount(leafCount)
{
	_leafVector.reserve(30);
	DirectX::XMFLOAT3 zero = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < _leafLimit; i++)
	{
		_leafVector.push_back(std::make_unique<Leaf>(gfx, zero, zero, zero, zero, zero, zero));
	}
}

void Leaves::Update(float dt) noexcept
{
	const float divisions = (2 * PI) / _leafCount;

	for (int i = 0; i < _leafCount; i++)
	{
		auto leaf = _leafVector[i].get();
		auto leafTransform = leaf->GetLeafTransform();
		DirectX::XMFLOAT3 f3 = {};
		//Tilt the leaf
		f3 = leafTransform->GetLocalRotation();
		f3.y = _leafTilt;
		f3.x = _leafXRot;
		f3.z = _leafYRot;
		leafTransform->SetLocalRotation(f3);
		//Rotate the leaf around the center
		f3 = leafTransform->GetWorldRotation();
		f3.x = divisions * i;
		f3.y = _leavesXRot;
		f3.z = _leavesYRot;
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

void Leaves::SpawnImGuiWindow() noexcept
{
	if (ImGui::Begin("Leaves Customisation"))
	{
		std::string leafAmount = "Leaf Amount: " + std::to_string(_leafCount);
		ImGui::Text(leafAmount.c_str());
		leafAmount = "Limit: " + std::to_string(_leafLimit);
		ImGui::Text(leafAmount.c_str());
		if (ImGui::Button("+"))
		{
			_leafCount = _leafCount == _leafLimit ? _leafCount : _leafCount + 1;
		}
		if (ImGui::Button("-"))
		{
			_leafCount = _leafCount == 0 ? _leafCount : _leafCount - 1;
		}

		ImGui::Text("Individual Leaf Options");
		ImGui::SliderAngle("Leaf Tilt", &_leafTilt, -180.0f, 180.0f);
		ImGui::SliderAngle("Leaf Rotation X", &_leafXRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Leaf Rotation Z", &_leafYRot, -180.0f, 180.0f);

		ImGui::Text("Grouped Leaf Options");
		ImGui::SliderFloat("X Pos", &_x, -10.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("Y Pos", &_y, -10.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("Z Pos", &_z, -10.0f, 10.0f, "%.2f");
		ImGui::SliderAngle("Bunch Rotation X", &_leavesXRot, -180.0f, 180.0f);
		ImGui::SliderAngle("Bunch Rotation Y", &_leavesYRot, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
		{
			_leafTilt = 0.0f;
			_leafXRot = 0.0f;
			_leafYRot = 0.0f;
			_leavesXRot = 0.0f;
			_leavesYRot = 0.0f;
			_x = 0.0f;
			_y = 0.0f;
			_z = 0.0f;
		}
	}
	ImGui::End();
}
