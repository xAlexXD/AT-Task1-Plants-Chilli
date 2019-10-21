#include "SolidSphere.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "SpherePrim.h"

SolidSphere::SolidSphere(Graphics & gfx, float radius) : _transform(std::make_unique<GameObjectTransform>())
{

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};
		auto model = SpherePrim::Make<Vertex>();
		model.Transform(DirectX::XMMatrixScaling(radius, radius, radius));
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model._vertices));
		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model._indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SolidVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPixelShader.cso"));

		struct PSColorConstant
		{
			DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;	//Need to add padding of 8 bits to make it match shader colour definition
		} colorConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransConstBuffer>(gfx, *this));
}

void SolidSphere::Update(float dt) noexcept
{
	_transform->Update(dt);
}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	_transform->SetPosition(pos);
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return _transform->GetGameObjectTransformXM();
}