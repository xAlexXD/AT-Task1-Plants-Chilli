#include "..\includes\GrassBlade.h"
#include <sstream>
#include "BindableBase.h"
#include "BladePrim.h"

GrassBlade::GrassBlade(Graphics& gfx, const char* textureName, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 posDelta, DirectX::XMFLOAT3 rotDelta, DirectX::XMFLOAT3 worldRot, DirectX::XMFLOAT3 worldDelta) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta, worldRot, worldDelta))
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
	CalcOffsetsAndSetBuffer();
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
	std::vector<TexturedVertex> vert = _vertexBuffer->GetOriginalVerts();
	_vertOut.clear();
	_vertOut.reserve(vert.size());
	for (size_t i = 0; i < vert.size(); i++)
	{
		_vertOut.push_back(vert[i]);
	}

	const auto modelView = GetTransformXM();
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

void GrassBlade::TempGui(Graphics& gfx)
{
	if (ImGui::TreeNode("Grass Layered Offsets##Blade"))
	{
		ImGui::Text("X Offset");
		ImGui::SliderFloat("##XOff", &_layerOffsetX, -2.0f, 2.0f, "%.3f");
		ImGui::Text("Y Offset");
		ImGui::SliderFloat("##YOff", &_layerOffsetY, -2.0f, 2.0f, "%.3f");
		ImGui::Text("Z Offset");
		ImGui::SliderFloat("##ZOff", &_layerOffsetZ, -2.0f, 2.0f, "%.3f");
		ImGui::TreePop();
	}
}

void GrassBlade::CalcOffsetsAndSetBuffer()
{
	auto verts = _vertexBuffer->GetOriginalVerts();

	DirectX::XMFLOAT3 temp;

	////Apply cascading offsets to each layer
	//Layer 1 -- 1 x Offset
	temp = verts[2].pos;
	verts[2].pos = DirectX::XMFLOAT3(temp.x + _layerOffsetX, temp.y + _layerOffsetY, temp.z + _layerOffsetZ);
	temp = verts[3].pos;
	verts[3].pos = DirectX::XMFLOAT3(temp.x + _layerOffsetX, temp.y + _layerOffsetY, temp.z + _layerOffsetZ);
	temp = verts[11].pos;
	verts[11].pos = DirectX::XMFLOAT3(temp.x + _layerOffsetX, temp.y + _layerOffsetY, temp.z + _layerOffsetZ);
	temp = verts[12].pos;
	verts[12].pos = DirectX::XMFLOAT3(temp.x + _layerOffsetX, temp.y + _layerOffsetY, temp.z + _layerOffsetZ);

	//Layer 2 -- 2 x Offset
	temp = verts[4].pos;
	verts[4].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 2.0), temp.y + (_layerOffsetY * 2.0), temp.z + (_layerOffsetZ * 2.0));
	temp = verts[5].pos;
	verts[5].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 2.0), temp.y + (_layerOffsetY * 2.0), temp.z + (_layerOffsetZ * 2.0));
	temp = verts[13].pos;
	verts[13].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 2.0), temp.y + (_layerOffsetY * 2.0), temp.z + (_layerOffsetZ * 2.0));
	temp = verts[14].pos;
	verts[14].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 2.0), temp.y + (_layerOffsetY * 2.0), temp.z + (_layerOffsetZ * 2.0));

	//Layer 3 -- 3 x Offset
	temp = verts[6].pos;
	verts[6].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 3.0), temp.y + (_layerOffsetY * 3.0), temp.z + (_layerOffsetZ * 3.0));
	temp = verts[7].pos;
	verts[7].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 3.0), temp.y + (_layerOffsetY * 3.0), temp.z + (_layerOffsetZ * 3.0));
	temp = verts[15].pos;
	verts[15].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 3.0), temp.y + (_layerOffsetY * 3.0), temp.z + (_layerOffsetZ * 3.0));
	temp = verts[16].pos;
	verts[16].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 3.0), temp.y + (_layerOffsetY * 3.0), temp.z + (_layerOffsetZ * 3.0));

	//Point -- 4 x Offset
	temp = verts[8].pos;
	verts[8].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 4.0), temp.y + (_layerOffsetY * 4.0), temp.z + (_layerOffsetZ * 4.0));
	temp = verts[17].pos;
	verts[17].pos = DirectX::XMFLOAT3(temp.x + (_layerOffsetX * 4.0), temp.y + (_layerOffsetY * 4.0), temp.z + (_layerOffsetZ * 4.0));

	////Write the changes back to the vertex buffer
	_vertexBuffer->UpdateVerts(verts);
}
