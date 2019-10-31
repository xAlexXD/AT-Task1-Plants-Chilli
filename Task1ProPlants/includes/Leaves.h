#pragma once
#include "DrawablesBase.h"
#include "Leaf.h"


class Leaves
{
public:
	Leaves(Graphics& gfx,
		unsigned int leafCount,
		const char* textureName,
		const char* bunchName
	);

	void Update(float dt) noexcept;
	void DrawLeaves(Graphics& gfx) noexcept;
	void SpawnImGuiWindow(Graphics& gfx) noexcept;

	void UpdateLocalVertAndInd(Graphics& gfx);

private:
	std::vector<std::unique_ptr<Leaf>> _leafVector;
	unsigned int _leafLimit = 30u;
	unsigned int _leafCount = 0u;
	float _leafTilt = 0.0f;
	float _leafXRot = 0.0f;
	float _leafYRot = 0.0f;
	float _leavesXRot = 0.0f;
	float _leavesYRot = 0.0f;
	float _leavesZRot = 0.0f;
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
	const std::string _bunchName;

	std::vector<Leaf::TexturedVertex> _leafVerts;
	std::vector<int> _leafIndices;
};
/*
YLrot to tilt the Leaf

XWRot to rotate around the stem

Z to move it up and down

X to space it from the center
*/