#pragma once
#include <Graphics/Mesh/BaseMesh.h>
#include <Vectors\Vec3.h>

#include <vector>



namespace Engine
{
	class ScreenQuadMesh : public BaseMesh
	{
	private:
		std::vector<Vec2> positions;
		std::vector<unsigned int> indices;


		void createBuffers();
	public:
		ScreenQuadMesh();

		// Inherited via BaseMesh
		virtual size_t getNumberVerts() override;
		virtual size_t getSizeIndices() override;
	};
}