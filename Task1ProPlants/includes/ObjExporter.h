#include <sstream>
#include <vector>
#include <stdio.h>
#include "StructDefs.h"

class ObjExporter
{
public:
	ObjExporter() = default;
	~ObjExporter() = default;


	void ExportToObj(std::vector<TexturedVertex> verts, std::vector<int> inds) noexcept;

private:
};