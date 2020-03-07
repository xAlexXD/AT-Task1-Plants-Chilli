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
		std::vector<V> vertices(9);
		////Front Face
		//layer 0
		vertices[0].pos = DirectX::XMFLOAT3(-0.010000, 0.000001, 0.000000);
		vertices[1].pos = DirectX::XMFLOAT3(0.010000, 0.000001, 0.000000);
		//Layer 1
		vertices[2].pos = DirectX::XMFLOAT3(-0.010000, 0.000001, 0.100000);
		vertices[3].pos = DirectX::XMFLOAT3(0.010000, 0.000001, 0.100000);
		//Layer 2
		vertices[4].pos = DirectX::XMFLOAT3(-0.010000, 0.000001, 0.200000);
		vertices[5].pos = DirectX::XMFLOAT3(0.010000, 0.000001, 0.200000);
		//Layer 3
		vertices[6].pos = DirectX::XMFLOAT3(-0.010000, 0.000001, 0.300000);
		vertices[7].pos = DirectX::XMFLOAT3(0.010000, 0.000001, 0.300000);
		//Point
		vertices[8].pos = DirectX::XMFLOAT3(0.000000, 0.000001, 0.400000);

		////Back Face
		vertices[9].pos = DirectX::XMFLOAT3(-0.010000, -0.000001, 0.000000);
		vertices[10].pos = DirectX::XMFLOAT3(0.010000, -0.000001, 0.000000);
		//Layer 1
		vertices[11].pos = DirectX::XMFLOAT3(-0.010000, -0.000001, 0.100000);
		vertices[12].pos = DirectX::XMFLOAT3(0.010000, -0.000001, 0.100000);
		//Layer 2
		vertices[13].pos = DirectX::XMFLOAT3(-0.010000, -0.000001, 0.200000);
		vertices[14].pos = DirectX::XMFLOAT3(0.010000, -0.000001, 0.200000);
		//Layer 3
		vertices[15].pos = DirectX::XMFLOAT3(-0.010000, -0.000001, 0.300000);
		vertices[16].pos = DirectX::XMFLOAT3(0.010000, -0.000001, 0.300000);
		//Point
		vertices[17].pos = DirectX::XMFLOAT3(0.000000, -0.000001, 0.400000);
	}

	template<class V>
	static IndexedTriangleList<V> MakeTextured()
	{

	}
};