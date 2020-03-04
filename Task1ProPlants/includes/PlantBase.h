#pragma once

class Graphics;
class ObjExporter;

class PlantBase
{
public:
	PlantBase();
	virtual ~PlantBase();

	virtual void Update(float dt) = 0;
	virtual void Draw(Graphics& gfx) = 0;
	virtual void SpawnImgui(Graphics& gfx) = 0;
	virtual void Export(Graphics& gfx, ObjExporter& exporter, const char* outDir) = 0;

private:
};

