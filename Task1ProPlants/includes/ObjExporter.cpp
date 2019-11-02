#include "ObjExporter.h"
#include <fstream>

void ObjExporter::ExportToObj(std::vector<TexturedVertex> verts, std::vector<int> inds) noexcept
{
	std::ofstream outFile("Flower.obj");
	if (outFile.is_open())
	{
		//Start with verts
		outFile << "# Vertices: \n";
		for (auto& v : verts)
		{
			outFile << "v" << " " << v.pos.x << " " << v.pos.y << " " << v.pos.z << "\n";
		}

		//Next the Tex cords
		outFile << "# Tex Cords: \n";
		for (auto& tc : verts)
		{
			outFile << "vt" << " " << tc.tc.x << " " << -tc.tc.y << "\n";
		}

		//Next the normals
		outFile << "# Normals: \n";
		for (auto& n : verts)
		{
			outFile << "vn" << " " << n.n.x << " " << n.n.y << " " << n.n.z << " \n";
		}

		//Finally the faces For some reason it starts at 1 instead of zero
		outFile << "# Faces: \n";
		for (size_t i = 0; i < inds.size(); i+=3)
		{
			outFile << "f" << " "
				<< inds[i] + 1 << "/" << inds[i] + 1 << "/" << inds[i] + 1 << " "
				<< inds[i + 1] + 1 << "/" << inds[i + 1] + 1 << "/" << inds[i + 1] + 1 << " "
				<< inds[i + 2] + 1 << "/" << inds[i + 2] + 1 << "/" << inds[i + 2] + 1 << "\n";
		}

		outFile.close();
	}
}