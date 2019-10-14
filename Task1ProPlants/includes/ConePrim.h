#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "MathClass.h"

class ConePrim
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		////Check to make sure it can atleast make one triangle
		assert(longDiv >= 3);

		const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		////base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			//Creates an empty slot on the back of vertices for us to fill
			vertices.emplace_back();

			//Create a vert that is based on the base vert thats transformed using the inputted amount of tessalation
			auto v = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iLong));
			DirectX::XMStoreFloat3(&vertices.back().pos, v);
		}

		////The center vert of the base
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		//Store the index of the center for use in the indices
		const auto centerIndex = (unsigned short)(vertices.size() - 1);

		////The tip of the cone
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		//Store the index of the tip for use in the indices 
		const auto tipIndex = (unsigned short)(vertices.size() - 1);

		//Cones base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			//Start at the center of the base
			indices.push_back(centerIndex);
			//Reach out to the start of the next index of the base, make sure its clock wise when looking at the base
			indices.push_back((iLong + 1) % longDiv);
			//Join back up to this calcs index
			indices.push_back(iLong);
		}

		//Cones tip indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			//Start from this index
			indices.push_back(iLong);
			//Draw to the next position on the base
			indices.push_back((iLong + 1) % longDiv);
			//Join it up to the tip
			indices.push_back(tipIndex);
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};