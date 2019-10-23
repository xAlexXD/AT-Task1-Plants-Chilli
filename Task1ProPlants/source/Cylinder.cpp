#include "..\includes\Cylinder.h"
#include "BindableBase.h"
#include "PrismPrim.h"

Cylinder::Cylinder(Graphics& gfx,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3 rot,
	DirectX::XMFLOAT3 posDelta,
	DirectX::XMFLOAT3 rotDelta,
	DirectX::XMFLOAT3 matCol,
	int tessalation) :
	_transform(std::make_unique<GameObjectTransform>(pos, rot, posDelta, rotDelta))
{
	if (!IsStaticInitialized())
	{
		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"IndexedPhongPixelShader.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3A colors[6] = {
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,1.0f,0.0f},
				{1.0f,0.0f,1.0f},
				{0.0f,1.0f,1.0f},
			};
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
		} matConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, matConst, 1u));
	}

	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
	};
	auto model = PrismPrim::MakeTesselatedIndependentCapNormals<Vertex>(tessalation);

	AddBind(std::make_unique<VertexBuffer>(gfx, model._vertices));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));
}

Cylinder::~Cylinder()
{
}

void Cylinder::Update(float dt) noexcept
{
	_transform->Update(dt);
}

DirectX::XMMATRIX Cylinder::GetTransformXM() const noexcept
{
	return _transform->GetGameObjectTransformXM();
}
