#pragma once
#include <vector>
#include <DirectXMath.h>


//Templated so it can accept multiple types of vertex structs
//ALWAYS ASSUMES THE STRUCT WILL AT LEAST CONTAIN A "pos" XMFLOAT3
template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in) : _vertices(std::move(verts_in)), _indices(std::move(indices_in))
	{
		//Add asserts to make sure you cant at least form a single triangle
		assert(_vertices.size() > 2);
		assert(_indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& v : _vertices)
		{
			//Loads in the float 3 from the vert and uses a pointer back to it to modify the matrix by the functions parameter
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(&v.pos, DirectX::XMVector3Transform(pos, matrix));
		}
	}


public:
	std::vector<T> _vertices;
	std::vector<unsigned short> _indices;
};
