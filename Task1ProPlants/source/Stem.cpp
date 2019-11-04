#include "..\includes\Stem.h"
#include "BindableBase.h"
#include "PrismPrim.h"

Stem::Stem(Graphics& gfx, 
	DirectX::XMFLOAT3 pos, 
	DirectX::XMFLOAT3 rot, 
	DirectX::XMFLOAT3 posDelta, 
	DirectX::XMFLOAT3 rotDelta, 
	DirectX::XMFLOAT3 worldRot, 
	DirectX::XMFLOAT3 worldDelta) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta, worldRot, worldDelta))
{
	auto model = PrismPrim::MakeTesselatedIndependentCapNormals<TexturedVertex>(5);
	model.Transform(DirectX::XMMatrixScaling(0.1f, 0.1f, 1.75f));

	for (int i = 0; i < model._vertices.size(); i++)
	{
		model._vertices[i].tc = { 0.0f, 0.0f };
	}
	model.SetNormalsIndependentFlat();

	_indexOut.reserve(model._indices.size());
	for (auto& index : model._indices)
	{
		_indexOut.push_back(index);
	}

	AddBind(std::make_unique<Texture>(gfx, "./textures/green.tga"));
	_texName = "green.tga";
	AddBind(std::make_unique<Sampler>(gfx));

	AddBind(std::make_unique<DynamicVertexBuffer<TexturedVertex>>(gfx, model._vertices));
	_vertexBuffer = reinterpret_cast<DynamicVertexBuffer<TexturedVertex>*>(GetPointerToLastBindable());

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedPhongVertexShader.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"TexturedPhongPixelShader.cso"));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	struct PSMaterialConstant
	{
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} colorConst;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));

	////This gets initialised normally because this needs to be unique per cube, for different transforms
	//Create transform constant buffer bind -- this one slighly special, need a reference to this class to be able to call the transform function and use that data
	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));

	//Adjust this to change the per instance scaling
	//DirectX::XMStoreFloat3x3(&_transform->GetModelTransform(), DirectX::XMMatrixScaling(0.9f, 1.5f, 0.9f));
}

void Stem::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX Stem::GetTransformXM() const noexcept
{
	return _transform->GetTransformXM();
}

void Stem::SpawnImGuiWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Stem Window"))
	{
		if (ImGui::Button("Update Local Data"))
		{
			UpdateLocalVertsAndInds(gfx);
		}
	}
	ImGui::End();
}

void Stem::UpdateLocalVertsAndInds(Graphics& gfx)
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
		DirectX::XMFLOAT4 temp = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 0.0f };
		auto tempVec = DirectX::XMLoadFloat4(&temp);
		tempVec = DirectX::XMVector4Transform(tempVec, modelViewProj);
		DirectX::XMStoreFloat4(&temp, tempVec);
		vertex.pos.x = temp.x;
		vertex.pos.y = temp.y;
		vertex.pos.z = temp.z;

		//Adjust the normals based on the modelView
		temp = { vertex.n.x, vertex.n.y, vertex.n.z, 0.0f };
		tempVec = DirectX::XMLoadFloat4(&temp);
		tempVec = DirectX::XMVector4Transform(tempVec, modelView);
		DirectX::XMStoreFloat4(&temp, tempVec);
		vertex.n.x = temp.x;
		vertex.n.y = temp.y;
		vertex.n.z = temp.z;
	}
}
