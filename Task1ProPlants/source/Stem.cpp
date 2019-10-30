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
	auto model = PrismPrim::MakeTesselatedIndependentCapNormals<StemVertex>(24);
	model.Transform(DirectX::XMMatrixScaling(0.1f, 0.1f, 1.75f));
	model.SetNormalsIndependentFlat();

	AddBind(std::make_unique<DynamicVertexBuffer<StemVertex>>(gfx, model._vertices));
	_vertexBuffer = reinterpret_cast<DynamicVertexBuffer<StemVertex>*>(GetPointerToLastBindable());

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVertexShader.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PhongPixelShader.cso"));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} colorConst;
	colorConst.color = DirectX::XMFLOAT3(0.3f, 1.0f, 0.3f);
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

void Stem::SpawnImGuiWindow() noexcept
{
}
