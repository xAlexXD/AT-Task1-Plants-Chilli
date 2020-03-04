#pragma once
#include "PlantBase.h"
#include "Leaves.h"
#include "Stem.h"

class Flower : public PlantBase
{
public:
	Flower() = delete;
	Flower(Graphics& gfx, const char* leafTexture, const char* petalTexture);
	~Flower() override;

	void Update(float dt) override;
	void Draw(Graphics& gfx) override;
	void SpawnImgui(Graphics& gfx) override;
	void Export(Graphics& gfx, ObjExporter& exporter, const char* outDir) override;

private:
	const char* _baseLeafTexture;
	const char* _basePetalTexture;

	std::vector<std::unique_ptr<Leaves>> _petalBunches;
	std::vector<std::unique_ptr<Leaves>> _leafBunches;
	std::unique_ptr<Stem> _stem;
};

