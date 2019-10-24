#include "..\includes\DynamicSolid.h"
#include "CubePrim.h"
#include "BindableBase.h"

DynamicSolid::DynamicSolid(Graphics& gfx, 
	DirectX::XMFLOAT3 pos, 
	DirectX::XMFLOAT3 rot, 
	DirectX::XMFLOAT3 posDelta, 
	DirectX::XMFLOAT3 rotDelta, 
	DirectX::XMFLOAT3 matCol) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta))
{
	auto model = CubePrim::MakeIndependent<Vertex>();
	model.SetNormalsIndependentFlat();

	//READD LINE BELOW IF YOU WANT TO DO DEFORMATIONS ON A BASE OBJECT TO APPLY TO ALL INSTACES
	//model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));

	AddBind(std::make_unique<DynamicVertexBuffer<Vertex>>(gfx, model._vertices));
	_vertexBuffer = reinterpret_cast<DynamicVertexBuffer<Vertex>*>(GetPointerToLastBindable());

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

	//Create and bind the vertex shader
	auto pVs = std::make_unique<VertexShader>(gfx, L"PhongVertexShader.cso");
	auto pVsBc = pVs->GetBytecode();
	AddBind(std::move(pVs));

	//Create and bind the pixel shader
	AddBind(std::make_unique<PixelShader>(gfx, L"PhongPixelShader.cso"));

	//Create and bind Input layout -- uses bytecode from vertex shader
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pVsBc));

	//Create primitive topology
	AddBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	////This gets initialised normally because this needs to be unique per cube, for different transforms
	//Create transform constant buffer bind -- this one slighly special, need a reference to this class to be able to call the transform function and use that data
	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));

	//Add per instance colouring
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];

	} colorConst;
	colorConst.color = matCol;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));

	//Adjust this to change the per instance scaling
	DirectX::XMStoreFloat3x3(&_transform->GetModelTransform(), DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f));
}

void DynamicSolid::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX DynamicSolid::GetTransformXM() const noexcept
{
	return DirectX::XMMATRIX();
}
