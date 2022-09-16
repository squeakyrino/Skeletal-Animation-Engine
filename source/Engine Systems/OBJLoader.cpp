#include <Engine Systems/OBJLoader.h>
#include <map>


namespace Engine
{
	OBJLoader::OBJLoader(const std::string& objPath, bool compress)
	{
		loadMeshData(objPath);
		processMeshData();
		
		if (compress)
		{
			createIndices();
		}
		else
		{
			outvertices = vertices;
			outnormals = normals;
			outtexcoords = texcoords;
			outindices = std::vector<unsigned int>();
		}
	}

	std::vector<Vec3> OBJLoader::getVertices()
	{
		return outvertices;
	}

	std::vector<Vec3> OBJLoader::getNormals()
	{
		return outnormals;
	}

	std::vector<Vec2> OBJLoader::getTexCoords()
	{
		return outtexcoords;
	}

	std::vector<unsigned int> OBJLoader::getIndices()
	{
		return outindices;
	}

	void OBJLoader::parseVertex(std::stringstream& sin)
	{
		float x, y, z;
		sin >> x >> y >> z;
		Vec3 v(x, y, z);
		vertexData.push_back(v);
	}

	void OBJLoader::parseTexcoord(std::stringstream& sin)
	{
		float u, v;
		sin >> u >> v;
		Vec2 t(u, v);
		texcoordData.push_back(t);
	}

	void OBJLoader::parseNormal(std::stringstream& sin)
	{
		float x, y, z;
		sin >> x >> y >> z;
		Vec3 v(x, y, z);
		normalData.push_back(v);
	}

	void OBJLoader::parseFace(std::stringstream& sin)
	{
		std::string token;
		if (normalData.empty() && texcoordData.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				sin >> token;
				vertexIdx.push_back(std::stoi(token));
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				std::getline(sin, token, '/');
				if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
				std::getline(sin, token, '/');
				if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
				std::getline(sin, token, ' ');
				if (token.size() > 0) normalIdx.push_back(std::stoi(token));
			}
		}
	}

	void OBJLoader::parseLine(std::stringstream& sin)
	{
		std::string s;
		sin >> s;
		if (s.compare("v") == 0) parseVertex(sin);
		else if (s.compare("vt") == 0) parseTexcoord(sin);
		else if (s.compare("vn") == 0) parseNormal(sin);
		else if (s.compare("f") == 0) parseFace(sin);
	}
	
	void OBJLoader::loadMeshData(const std::string& filename)
	{
		std::ifstream ifile(filename);
		std::string line;
		while (std::getline(ifile, line)) {
			std::stringstream sline(line);
			parseLine(sline);
		}
	}

	void OBJLoader::processMeshData()
	{
		bool TexcoordsLoaded = texcoordData.empty();
		bool NormalsLoaded = normalData.empty();

		for (unsigned int i = 0; i < vertexIdx.size(); i++) {
			unsigned int vi = vertexIdx[i];
			Vec3 v = vertexData[vi - 1];
			vertices.push_back(v);
			if (!TexcoordsLoaded) {
				unsigned int ti = texcoordIdx[i];
				Vec2 t = texcoordData[ti - 1];
				texcoords.push_back(t);
			}
			if (!NormalsLoaded) {
				unsigned int ni = normalIdx[i];
				Vec3 n = normalData[ni - 1];
				normals.push_back(n);
			}
		}
	}


	void OBJLoader::createIndices() {

		struct PackedVertex
		{
			Vec3 pos;
			Vec2 uv;
			Vec3 normal;

			bool operator<(const PackedVertex that) const {
				return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
			};

		};

		std::map<PackedVertex, unsigned int> vertexToOutIndex;

		for (size_t i = 0; i < vertices.size(); i++)
		{
			//Pack it first
			PackedVertex packed = { vertices[i], texcoords[i], normals[i] };

			//Try to find if it's already in the map or not
			unsigned int index;

			auto it = vertexToOutIndex.find(packed);
			if (it != vertexToOutIndex.end())
			{
				//If we find it cool
				index = it->second;
				//Add it to the indices
				outindices.push_back(index);
			}
			else
			{
				//If not we need to create a new vertex
				outvertices.push_back(packed.pos);
				outnormals.push_back(packed.normal);
				outtexcoords.push_back(packed.uv);
				
				index = (unsigned int)outvertices.size() - 1;
				outindices.push_back(index);

				vertexToOutIndex[packed] = index;
			}

		}

	}

}