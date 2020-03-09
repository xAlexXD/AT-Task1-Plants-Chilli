#pragma once
#include "IndexedTriangleList.h"
#include <vector>
#include <array>
#include "MathClass.h"

class BladePrim
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		std::vector<V> vertices(18);
		////Front Face
		//layer 0
		vertices[0].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, 0.000000);
		vertices[1].pos = DirectX::XMFLOAT3(0.200000, 0.000100, 0.000000);
		//Layer 1
		vertices[2].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, -0.400000);
		vertices[3].pos = DirectX::XMFLOAT3(0.200000, 0.000100, -0.400000);
		//Layer 2
		vertices[4].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, -0.800000);
		vertices[5].pos = DirectX::XMFLOAT3(0.200000, 0.000100, -0.800000);
		//Layer 3
		vertices[6].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, -1.200000);
		vertices[7].pos = DirectX::XMFLOAT3(0.200000, 0.000100, -1.200000);
		//Point
		vertices[8].pos = DirectX::XMFLOAT3(0.000000, 0.000100, -1.600000);

		////Back Face
		vertices[9].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, 0.000000);
		vertices[10].pos = DirectX::XMFLOAT3(0.200000, -0.000100, 0.000000);
		//Layer 1
		vertices[11].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, -0.400000);
		vertices[12].pos = DirectX::XMFLOAT3(0.200000, -0.000100, -0.400000);
		//Layer 2
		vertices[13].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, -0.800000);
		vertices[14].pos = DirectX::XMFLOAT3(0.200000, -0.000100, -0.800000);
		//Layer 3
		vertices[15].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, -1.200000);
		vertices[16].pos = DirectX::XMFLOAT3(0.200000, -0.000100, -1.200000);
		//Point
		vertices[17].pos = DirectX::XMFLOAT3(0.000000, -0.000100, -1.600000);

		std::vector<unsigned short> indices = {
			0,1,3,
			0,3,2,
			2,3,5,
			2,5,4,
			4,5,7,
			4,7,6,
			6,7,8,
			9,12,10,
			9,11,12,
			11,14,12,
			11,13,14,
			13,16,14,
			13,15,16,
			15,17,16
		};
		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTextured()
	{
		std::vector<V> vertices(18);
		////Front Face
		//layer 0
		vertices[0].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, 0.000000);
		vertices[0].tc = DirectX::XMFLOAT2(0.000000, 0.000000);

		vertices[1].pos = DirectX::XMFLOAT3(0.200000, 0.000100, 0.000000);
		vertices[1].tc = DirectX::XMFLOAT2(1.000000, 0.000000);
		//Layer 1
		vertices[2].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, -0.400000);
		vertices[2].tc = DirectX::XMFLOAT2(0.000000, 0.250000);

		vertices[3].pos = DirectX::XMFLOAT3(0.200000, 0.000100, -0.400000);
		vertices[3].tc = DirectX::XMFLOAT2(1.000000, 0.250000);
		//Layer 2
		vertices[4].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, -0.800000);
		vertices[4].tc = DirectX::XMFLOAT2(0.000000, 0.500000);

		vertices[5].pos = DirectX::XMFLOAT3(0.200000, 0.000100, -0.800000);
		vertices[5].tc = DirectX::XMFLOAT2(1.000000, 0.500000);
		//Layer 3
		vertices[6].pos = DirectX::XMFLOAT3(-0.200000, 0.000100, -1.200000);
		vertices[6].tc = DirectX::XMFLOAT2(0.000000, 0.750000);

		vertices[7].pos = DirectX::XMFLOAT3(0.200000, 0.000100, -1.200000);
		vertices[7].tc = DirectX::XMFLOAT2(1.000000, 0.750000);
		//Point
		vertices[8].pos = DirectX::XMFLOAT3(0.000000, 0.000100, -1.600000);
		vertices[8].tc = DirectX::XMFLOAT2(0.500000, 1.000000);

		////Back Face
		//layer 0
		vertices[9].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, 0.000000);
		vertices[9].tc = DirectX::XMFLOAT2(0.000000, 0.000000);

		vertices[10].pos = DirectX::XMFLOAT3(0.200000, -0.000100, 0.000000);
		vertices[10].tc = DirectX::XMFLOAT2(1.000000, 0.000000);
		//Layer 1
		vertices[11].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, -0.400000);
		vertices[11].tc = DirectX::XMFLOAT2(0.000000, 0.250000);

		vertices[12].pos = DirectX::XMFLOAT3(0.200000, -0.000100, -0.400000);
		vertices[12].tc = DirectX::XMFLOAT2(1.000000, 0.250000);
		//Layer 2
		vertices[13].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, -0.800000);
		vertices[13].tc = DirectX::XMFLOAT2(0.000000, 0.500000);

		vertices[14].pos = DirectX::XMFLOAT3(0.200000, -0.000100, -0.800000);
		vertices[14].tc = DirectX::XMFLOAT2(1.000000, 0.500000);
		//Layer 3
		vertices[15].pos = DirectX::XMFLOAT3(-0.200000, -0.000100, -1.200000);
		vertices[15].tc = DirectX::XMFLOAT2(0.000000, 0.750000);

		vertices[16].pos = DirectX::XMFLOAT3(0.200000, -0.000100, -1.200000);
		vertices[16].tc = DirectX::XMFLOAT2(1.000000, 0.750000);
		//Point
		vertices[17].pos = DirectX::XMFLOAT3(0.000000, -0.000100, -1.600000);
		vertices[16].tc = DirectX::XMFLOAT2(0.500000, 1.000000);

		std::vector<unsigned short> indices = {
			0,1,3,
			0,3,2,
			2,3,5,
			2,5,4,
			4,5,7,
			4,7,6,
			6,7,8,
			9,12,10,
			9,11,12,
			11,14,12,
			11,13,14,
			13,16,14,
			13,15,16,
			15,17,16
		};

		return { std::move(vertices), std::move(indices) };
	}
};