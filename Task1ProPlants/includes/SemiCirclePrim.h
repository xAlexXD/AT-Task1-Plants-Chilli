#pragma once
#include "IndexedTriangleList.h"
#include <vector>
#include <array>
#include "MathClass.h"

class SemiCirclePrim
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		std::vector<V> vertices(6);
		vertices[0].pos = DirectX::XMFLOAT3(0.000000, 0.000000, 0.000000);
		vertices[1].pos = DirectX::XMFLOAT3(1.333333, 0.000000, 0.000000);
		vertices[2].pos = DirectX::XMFLOAT3(-1.333333, 0.000000, 0.000000);
		vertices[3].pos = DirectX::XMFLOAT3(0.000000, 0.000000, -1.333333);
		vertices[4].pos = DirectX::XMFLOAT3(-1.000000, 0.000000, -1.000000);
		vertices[5].pos = DirectX::XMFLOAT3(1.000000, 0.000000, -1.000000);

		std::vector<unsigned short> indices = {
			0,2,4,
			0,3,4,
			0,5,3,
			0,1,5,
		};

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTextured()
	{
		std::vector<V> vertices(6);
		vertices[0].pos = DirectX::XMFLOAT3(0.000000, 0.000000, 0.000000);
		vertices[0].tc = DirectX::XMFLOAT2(0.500000, 0.000000);

		vertices[1].pos = DirectX::XMFLOAT3(1.333333, 0.000000, 0.000000);
		vertices[1].tc = DirectX::XMFLOAT2(1.000000, 0.000000);

		vertices[2].pos = DirectX::XMFLOAT3(-1.333333, 0.000000, 0.000000);
		vertices[2].tc = DirectX::XMFLOAT2(0.000000, 0.000000);

		vertices[3].pos = DirectX::XMFLOAT3(0.000000, 0.000000, -1.333333);
		vertices[3].tc = DirectX::XMFLOAT2(0.000000, 1.000000);

		vertices[4].pos = DirectX::XMFLOAT3(-1.000000, 0.000000, -1.000000);
		vertices[3].tc = DirectX::XMFLOAT2(0.150000, 0.700000);

		vertices[5].pos = DirectX::XMFLOAT3(1.000000, 0.000000, -1.000000);
		vertices[3].tc = DirectX::XMFLOAT2(0.850000, 0.700000);

		std::vector<unsigned short> indices = {
			0,2,4,
			0,3,4,
			0,5,3,
			0,1,5,
		};

		return { std::move(vertices), std::move(indices) };
	}
};

