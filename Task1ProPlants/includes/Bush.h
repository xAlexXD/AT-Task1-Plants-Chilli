#pragma once
#include "DrawablesBase.h"
#include "BushFace.h"
#include "StructDefs.h"

class Bush
{
public:
	Bush(Graphics& gfx,
		unsigned int faceCount,
		const char* textureName,
		const char* bushName
	);

	void Update(float dt) noexcept;
	void Draw(Graphics& gfx) noexcept;
	void SpawnImGui(Graphics& gfx) noexcept;

	void UpdateLocalVertAndInd(Graphics& gfx);

	std::vector<TexturedVertex> _faceVerts;
	std::vector<int> _faceInds;
	std::string _texName = "";

	const char* GetName()
	{
		return _bushName.c_str();
	}

private:
	void UpdateTexture(Graphics& gfx);

	std::vector<std::unique_ptr<BushFace>> _faceVector;
	unsigned int _faceLimit = 12u;
	unsigned int _faceCount = 0u;
	unsigned int _perFaceVerts = 0u;
	unsigned int _perFaceInds = 0u;
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
	float _xScale = 1.0f;
	float _yScale = 1.0f;
	float _zScale = 1.0f;
	float _xRot = 0.0f;
	float _yRot = 0.0f;
	float _zRot = 0.0f;
	const std::string _bushName;

	char _textureArray[128] = "";
};

