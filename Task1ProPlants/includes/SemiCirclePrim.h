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
		std::vector<V> vertices(12);
		vertices[0].pos = DirectX::XMFLOAT3(0.000000, 0.000001, 0.000000);
		vertices[1].pos = DirectX::XMFLOAT3(1.333333, 0.000001, 0.000000);
		vertices[2].pos = DirectX::XMFLOAT3(-1.333333, 0.000001, 0.000000);
		vertices[3].pos = DirectX::XMFLOAT3(0.000000, 0.000001, -1.333333);
		vertices[4].pos = DirectX::XMFLOAT3(-1.000000, 0.000001, -1.000000);
		vertices[5].pos = DirectX::XMFLOAT3(1.000000, 0.000001, -1.000000);
		vertices[6].pos = DirectX::XMFLOAT3(0.000000, -0.000001, 0.000000);
		vertices[7].pos = DirectX::XMFLOAT3(1.333333, -0.000001, 0.000000);
		vertices[8].pos = DirectX::XMFLOAT3(-1.333333, -0.000001, 0.000000);
		vertices[9].pos = DirectX::XMFLOAT3(0.000000, -0.000001, -1.333333);
		vertices[10].pos = DirectX::XMFLOAT3(-1.000000, -0.000001, -1.000000);
		vertices[11].pos = DirectX::XMFLOAT3(1.000000, -0.000001, -1.000000);

		std::vector<unsigned short> indices = {
			0,2,4,
			0,3,4,
			0,5,3,
			0,1,5,
			6,10,8,
			6,10,9,
			6,9,11,
			6,11,7
		};

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTextured()
	{
		std::vector<V> vertices(12);
		vertices[0].pos = DirectX::XMFLOAT3(0.000000, 0.000100, 0.000000);
		vertices[0].tc = DirectX::XMFLOAT2(0.500000, 0.000000);

		vertices[1].pos = DirectX::XMFLOAT3(1.333333, 0.000100, 0.000000);
		vertices[1].tc = DirectX::XMFLOAT2(1.000000, 0.000000);

		vertices[2].pos = DirectX::XMFLOAT3(-1.333333, 0.000100, 0.000000);
		vertices[2].tc = DirectX::XMFLOAT2(0.000000, 0.000000);

		vertices[3].pos = DirectX::XMFLOAT3(0.000000, 0.000100, -1.333333);
		vertices[3].tc = DirectX::XMFLOAT2(0.000000, 1.000000);

		vertices[4].pos = DirectX::XMFLOAT3(-1.000000, 0.000100, -1.000000);
		vertices[4].tc = DirectX::XMFLOAT2(0.850000, 0.700000);

		vertices[5].pos = DirectX::XMFLOAT3(1.000000, 0.000100, -1.000000);
		vertices[5].tc = DirectX::XMFLOAT2(0.850000, 0.700000);

		vertices[6].pos = DirectX::XMFLOAT3(0.000000, -0.000100, 0.000000);
		vertices[6].tc = DirectX::XMFLOAT2(0.500000, 1.000000);

		vertices[7].pos = DirectX::XMFLOAT3(1.333333, -0.000100, 0.000000);
		vertices[7].tc = DirectX::XMFLOAT2(0.000000, 1.000000);

		vertices[8].pos = DirectX::XMFLOAT3(-1.333333, -0.000100, 0.000000);
		vertices[8].tc = DirectX::XMFLOAT2(1.000000, 1.000000);

		vertices[9].pos = DirectX::XMFLOAT3(0.000000, -0.000100, -1.333333);
		vertices[9].tc = DirectX::XMFLOAT2(0.000000, 0.000000);

		vertices[10].pos = DirectX::XMFLOAT3(-1.000000, -0.000100, -1.000000);
		vertices[10].tc = DirectX::XMFLOAT2(0.850000, 0.300000);

		vertices[11].pos = DirectX::XMFLOAT3(1.000000, -0.000100, -1.000000);
		vertices[11].tc = DirectX::XMFLOAT2(0.850000, 0.300000);

		std::vector<unsigned short> indices = {
			0,2,4,
			0,3,4,
			0,5,3,
			0,1,5,
			6,10,8,
			6,10,9,
			6,9,11,
			6,11,7
		};

		return { std::move(vertices), std::move(indices) };
	}
};

