#include <DirectXMath.h>

#ifndef STRUCTDEFS_H
#define STRUCTDEFS_H

struct TexturedVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 n;
	DirectX::XMFLOAT2 tc;
};

#endif