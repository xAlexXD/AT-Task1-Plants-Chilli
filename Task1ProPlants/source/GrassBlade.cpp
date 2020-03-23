#include "..\includes\GrassBlade.h"
#include <sstream>
#include "BindableBase.h"
#include "BladePrim.h"

GrassBlade::GrassBlade(Graphics& gfx, const char* name, const char* textureName, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta, DirectX::XMFLOAT3 worldRot, DirectX::XMFLOAT3 worldDelta) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta, worldRot, worldDelta)), _name(name), _texName(textureName)
{
	//Grab the base form of the model and apply some starting transforms to get it how we want the initial object to be
	auto model = BladePrim::MakeTextured<TexturedVertex>();
	model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	model.SetNormalsIndependentFlat();

	_vertCount = model._vertices.size();
	_indCount = model._indices.size();

	_vertOut.reserve(_vertCount);
	_indexOut.reserve(_indCount);

	_vertOut.assign(model._vertices.begin(), model._vertices.end());
	_indexOut.assign(model._indices.begin(), model._indices.end());

	AddBind(std::make_unique<DynamicVertexBuffer<TexturedVertex>>(gfx, model._vertices));
	_vertexBuffer = reinterpret_cast<DynamicVertexBuffer<TexturedVertex>*>(GetPointerToLastBindable());

	std::ostringstream path;
	path << "./textures/" << textureName;

	AddBind(std::make_unique<Texture>(gfx, path.str().c_str()));
	_textureBindable = dynamic_cast<Texture*>(GetPointerToLastBindable());
	AddBind(std::make_unique<Sampler>(gfx));

	auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedPhongVertexShader.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"TexturedPhongPixelShader.cso"));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//Create transform constant buffer bind -- this one slighly special, need a reference to this class to be able to call the transform function and use that data
	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));

	//Adjust this to change the per instance scaling
	//DirectX::XMStoreFloat3x3(&_transform->GetModelTransform(), DirectX::XMMatrixScaling(0.9f, 1.5f, 0.9f));
}

void GrassBlade::Update(float dt) noexcept
{
	//Initial Vert offsets
	CalcOffsetsAndSetBuffer();

	_transform->SetWorldRotation(_rotation);
	_transform->SetPosition(_position);
	_transform->SetScale(_scale);
	_transform->Update(dt);
}

DirectX::XMMATRIX GrassBlade::GetTransformXM() const noexcept
{
	return _transform->GetTransformWithPivotOffsetXM(_pivotPos);
}

GameObjectTransform* GrassBlade::GetFaceTransform() noexcept
{
	return _transform.get();
}

void GrassBlade::UpdateOutVertices(Graphics& gfx)
{
	//Get buffer from dynamic vertex buffer
	std::vector<TexturedVertex> vert = _vertexBuffer->GetVerts();
	_vertOut.clear();
	_vertOut.reserve(vert.size());
	for (size_t i = 0; i < vert.size(); i++)
	{
		_vertOut.push_back(vert[i]);
	}

	const auto modelView = GetTransformXM() * gfx.GetCamera();
	const auto modelViewProj = modelView * gfx.GetProjection();

	//Apply the transforms the vertex shader would
	for (auto& vertex : _vertOut)
	{
		//Multiply the positions by the model view projection
		DirectX::XMFLOAT4 temp = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f };
		auto tempVec = DirectX::XMLoadFloat4(&temp);
		tempVec = DirectX::XMVector4Transform(tempVec, modelViewProj);
		DirectX::XMStoreFloat4(&temp, tempVec);
		vertex.pos.x = -temp.x;
		vertex.pos.y = temp.y;
		vertex.pos.z = temp.z;

		//Adjust the normals based on the modelView
		temp = { vertex.n.x, vertex.n.y, vertex.n.z, 1.0f };
		tempVec = DirectX::XMLoadFloat4(&temp);
		tempVec = DirectX::XMVector4Transform(tempVec, modelView);
		DirectX::XMStoreFloat4(&temp, tempVec);
		vertex.n.x = temp.x;
		vertex.n.y = temp.y;
		vertex.n.z = temp.z;
	}
}

void GrassBlade::SpawnImGui(Graphics& gfx)
{
	std::string nameString = "";

	ImGui::Indent();
	ImGui::Text("Change Texture (Must be a .TGA)");
	nameString = "##TextureDir" + _name;
	ImGui::InputTextWithHint(nameString.c_str(), "Dir must be with /!", _textureArray, IM_ARRAYSIZE(_textureArray));
	if (ImGui::SmallButton("Apply Texture"))
	{
		UpdateTexture(gfx);
	}

	nameString = "Blade Position:##" + _name;
	if (ImGui::TreeNode(nameString.c_str()))
	{
		ImGui::Text("X Position");
		nameString = "##XPos" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_position.x, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Y Position");
		nameString = "##YPos" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_position.y, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Z Position");
		nameString = "##ZPos" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_position.z, -10.0f, 10.0f, "%.2f");
		nameString = "Reset to Default##BladeyPos" + _name;
		if (ImGui::SmallButton(nameString.c_str()))
		{
			_position = {};
		}
		ImGui::TreePop();
	}

	nameString = "Blade Rotation:##" + _name;
	if (ImGui::TreeNode(nameString.c_str()))
	{
		ImGui::Text("X Rotation");
		nameString = "##XRot" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_rotation.x, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Y Rotation");
		nameString = "##YRot" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_rotation.y, -10.0f, 10.0f, "%.2f");
		ImGui::Text("Z Rotation");
		nameString = "##ZRot" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_rotation.z, -10.0f, 10.0f, "%.2f");
		nameString = "Reset to Default##BladeyRot" + _name;
		if (ImGui::SmallButton(nameString.c_str()))
		{
			_rotation = {};
		}
		ImGui::TreePop();
	}

	nameString = "Blade Scale:##" + _name;
	if (ImGui::TreeNode(nameString.c_str()))
	{
		ImGui::Text("Width");
		nameString = "##Width" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_scale.x, 0.1f, 3.0f, "%.05f");
		ImGui::Text("Height");
		nameString = "##Height" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_scale.z, -0.1f, 3.0f, "%.05f");
		nameString = "Reset to Default##BladeyScale" + _name;
		if (ImGui::SmallButton("Reset to Default"))
		{
			_scale = { 1.0f, 1.0f, 1.0f };
		}
		ImGui::TreePop();
	}

	nameString = "Blade Layered Offsets:##" + _name;
	if (ImGui::TreeNode(nameString.c_str()))
	{
		ImGui::Text("X Offset");
		nameString = "##LayerXOff" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_layerOffset.x, -0.75f, 0.75f, "%.3f");

		ImGui::Text("Y Offset");
		nameString = "##LayerYOff" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_layerOffset.y, -0.75f, 0.75f, "%.3f");

		ImGui::Text("Z Offset");
		nameString = "##LayerZOff" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_layerOffset.z, -0.75f, 0.75f, "%.3f");
		nameString = "Reset to Default##BladeyOffset" + _name;
		if (ImGui::SmallButton(nameString.c_str()))
		{
			_layerOffset = {};
		}
		ImGui::TreePop();
	}

	nameString = "Blade Curve:##" + _name;
	if (ImGui::TreeNode(nameString.c_str()))
	{
		ImGui::Text("Curve Intensity");
		nameString = "##CurveIntensity" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_curveIntensity, -1.0f, 1.0f, "%.3f");

		ImGui::Text("Curve Height Multiplier");
		nameString = "##CurveHeight" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_curveHeightMultiplier, 0.0f, 3.0f, "%.3f");

		ImGui::Text("Curve Span Offset");
		nameString = "##CurveSpan" + _name;
		ImGui::SliderFloat(nameString.c_str(), &_curveSpanOffset, 0.0f, 3.0f, "%.3f");
		ImGui::TreePop();
	}

	ImGui::Unindent();
}

void GrassBlade::CalcOffsetsAndSetBuffer()
{
	auto verts = _vertexBuffer->GetOriginalVerts();

	DirectX::XMFLOAT3 temp;

	//for curve the offset will be based upon the y axis AKA the z and Z axis AKA the Y.

	////Apply cascading offsets to each layer, then add the curvature values
	//Layer 1 -- 1 x Offset
	temp = verts[2].pos;
	temp = DirectX::XMFLOAT3(temp.x + _layerOffset.x, temp.y + _layerOffset.y, temp.z + _layerOffset.z);
	verts[2].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 0.25 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.25));
	temp = verts[3].pos;
	temp = DirectX::XMFLOAT3(temp.x + _layerOffset.x, temp.y + _layerOffset.y, temp.z + _layerOffset.z);
	verts[3].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 0.25 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.25));
	temp = verts[11].pos;
	temp = DirectX::XMFLOAT3(temp.x + _layerOffset.x, temp.y + _layerOffset.y, temp.z + _layerOffset.z);
	verts[11].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 0.25 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.25));
	temp = verts[12].pos;
	temp = DirectX::XMFLOAT3(temp.x + _layerOffset.x, temp.y + _layerOffset.y, temp.z + _layerOffset.z);
	verts[12].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 0.25 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.25));

	//Layer 2 -- 2 x Offset
	temp = verts[4].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 2.0), temp.y + (_layerOffset.y * 2.0), temp.z + (_layerOffset.z * 2.0));
	verts[4].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 1.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.5));
	temp = verts[5].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 2.0), temp.y + (_layerOffset.y * 2.0), temp.z + (_layerOffset.z * 2.0));
	verts[5].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 1.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.5));
	temp = verts[13].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 2.0), temp.y + (_layerOffset.y * 2.0), temp.z + (_layerOffset.z * 2.0));
	verts[13].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 1.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.5));
	temp = verts[14].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 2.0), temp.y + (_layerOffset.y * 2.0), temp.z + (_layerOffset.z * 2.0));
	verts[14].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 1.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 0.5));

	//Layer 3 -- 3 x Offset
	temp = verts[6].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 3.0), temp.y + (_layerOffset.y * 3.0), temp.z + (_layerOffset.z * 3.0));
	verts[6].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 2.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 1.5));
	temp = verts[7].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 3.0), temp.y + (_layerOffset.y * 3.0), temp.z + (_layerOffset.z * 3.0));
	verts[7].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 2.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 1.5));
	temp = verts[15].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 3.0), temp.y + (_layerOffset.y * 3.0), temp.z + (_layerOffset.z * 3.0));
	verts[15].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 2.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 1.5));
	temp = verts[16].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 3.0), temp.y + (_layerOffset.y * 3.0), temp.z + (_layerOffset.z * 3.0));
	verts[16].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 2.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 1.5));

	//Point -- 4 x Offset
	temp = verts[8].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 4.0), temp.y + (_layerOffset.y * 4.0), temp.z + (_layerOffset.z * 4.0));
	verts[8].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 3.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 3.0));
	temp = verts[17].pos;
	temp = DirectX::XMFLOAT3(temp.x + (_layerOffset.x * 4.0), temp.y + (_layerOffset.y * 4.0), temp.z + (_layerOffset.z * 4.0));
	verts[17].pos = DirectX::XMFLOAT3(temp.x, temp.y + (_curveSpanOffset * 3.0 * _curveIntensity), temp.z + (_curveHeightMultiplier * 3.0));

	////Write the changes back to the vertex buffer
	_vertexBuffer->UpdateVerts(verts);
}

void GrassBlade::UpdateTexture(Graphics& gfx)
{
	std::string dir(_textureArray);
	_textureBindable->ChangeTextureView(gfx, dir.c_str());
}
