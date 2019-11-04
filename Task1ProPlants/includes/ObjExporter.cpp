#include "ObjExporter.h"
#include <fstream>

void ObjExporter::ExportToObj(std::vector<TexturedVertex> verts, std::vector<int> inds) noexcept
{
	std::ofstream outFile("./exports/Flower.obj");
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
		std::vector<int> objModifedInds;
		objModifedInds.reserve(inds.size());
		
		//Obj inds start at 1 instead of zero
		for (auto& index : inds)
		{
			objModifedInds.push_back(index + 1);
		}

		//Add extra triangles to draw the backfaces too
		for (size_t i = 0; i < objModifedInds.size(); i+=3)
		{
			outFile << "f" << " "
				<< objModifedInds[i] << "/" << objModifedInds[i] << "/" << objModifedInds[i] << " "
				<< objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << " "
				<< objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << "\n";

			//outFile << "f" << " "
			//	<< objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << "/" << objModifedInds[i + 2] << " "
			//	<< objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << "/" << objModifedInds[i + 1] << " "
			//	<< objModifedInds[i] << "/" << objModifedInds[i] << "/" << objModifedInds[i] << "\n";
		}

		outFile.close();
	}
}