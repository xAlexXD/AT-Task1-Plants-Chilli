#pragma once
#include "DrawablesBase.h"
#include "Leaf.h"


class Leaves
{
public:
	Leaves(Graphics& gfx,
		float yOffset,
		unsigned int leafCount
	);

	void Update(float dt) noexcept;
	void DrawLeaves(Graphics& gfx) noexcept;
	void SpawnImGuiWindow() noexcept;

private:
	std::vector<std::unique_ptr<Leaf>> _leafVector;
	unsigned int _leafLimit = 30u;
	unsigned int _leafCount = 0u;
	float _leafTilt = 0.0f;
	float _leafHeight = 0.0f;
	float _leafOffset = 0.0f;
};
/*
YLrot to tilt the Leaf

XWRot to rotate around the stem

Z to move it up and down

X to space it from the center
*/