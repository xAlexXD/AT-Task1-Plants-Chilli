#pragma once
#include "Leaves.h"
#include "Stem.h"

class ObjExporter;

class Flower
{
public:
	Flower() = delete;
	Flower(Graphics& gfx, const char* leafTexture, const char* petalTexture);
	~Flower();

	void Update(float dt);
	void Draw(Graphics& gfx);
	void SpawnImgui(Graphics& gfx);
	void ExportFlower(Graphics& gfx, ObjExporter& exporter, const char* outDir);

private:
	const char* _baseLeafTexture;
	const char* _basePetalTexture;

	std::vector<std::unique_ptr<Leaves>> _petalBunches;
	std::vector<std::unique_ptr<Leaves>> _leafBunches;
	std::unique_ptr<Stem> _stem;
};

