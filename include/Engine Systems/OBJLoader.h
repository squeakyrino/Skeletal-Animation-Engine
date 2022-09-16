#pragma once

#include <fstream>
#include <vector>
#include <Vectors/Vec3.h>
#include <sstream>

namespace Engine
{
	class OBJLoader
	{
		/*TODO: These shouldn't be part of the class anymore since they're just temporary while creating the data necessary
		* to fill the out vectors
		*/
		std::vector<Vec3> vertices, vertexData;
		std::vector<Vec2> texcoords, texcoordData;
		std::vector<Vec3> normals, normalData;

		std::vector<unsigned int> vertexIdx, texcoordIdx, normalIdx;

		std::vector<Vec3> outvertices;
		std::vector<Vec2> outtexcoords;
		std::vector<Vec3> outnormals;
		std::vector<unsigned int> outindices;

		void parseVertex(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseFace(std::stringstream& sin);
		void parseLine(std::stringstream& sin);

		void processMeshData();

		void loadMeshData(const std::string& filename);


		void createIndices();

	public:
		OBJLoader(const std::string& objPath, bool compress);

		std::vector<Vec3> getVertices();
		std::vector<Vec3> getNormals();
		std::vector<Vec2> getTexCoords();
		std::vector<unsigned int> getIndices();
	};
}