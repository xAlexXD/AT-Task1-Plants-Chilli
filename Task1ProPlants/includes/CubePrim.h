#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class CubePrim
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<DirectX::XMFLOAT3> verts;
		verts.emplace_back(-side, -side, -side); // 0
		verts.emplace_back(side, -side, -side); // 1
		verts.emplace_back(-side, side, -side); // 2
		verts.emplace_back(side, side, -side); // 3
		verts.emplace_back(-side, -side, side); // 4
		verts.emplace_back(side, -side, side); // 5
		verts.emplace_back(-side, side, side); // 6
		verts.emplace_back(side, side, side); // 7

		//Intialise a vector with the space allocation in length of the verts above
		std::vector<V> vertices(verts.size());
		for (size_t i = 0; i < verts.size(); i++)
		{
			vertices[i].pos = verts[i];
		}

		//return a indextrianglelist thats constructed using the return rather than making a temporary object and returning it
		return
		{
			std::move(vertices),
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}
};