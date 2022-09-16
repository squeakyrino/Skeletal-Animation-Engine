#pragma once
#include <Graphics/Mesh/BaseMesh.h>
#include <Vectors\Vec3.h>
#include <Matrix/Mat4.h>
#include <Graphics/Shader/Shader.h>
#include <Engine Systems/OBJLoader.h>
#include <Engine Systems/ModelLoader.h>

#include <vector>

#include <GL/glew.h>
namespace Engine
{
	/*Right now we'll have several VBOs per attribute for simplicity.
	* TODO: Specify topology maybe?
	*/

	class Mesh : public BaseMesh
	{
	private:

		std::vector<Vec3> positions;
		std::vector<unsigned int> indices;

		//Create the opengl buffers and send the data over to the GPU
		void CreateBuffers(const std::vector<Vec3>& positions,
			const std::vector<Vec3>& colors,
			const std::vector<unsigned int>& indices,
			const std::vector<Vec2>& texCoords,
			const std::vector<Vec3>& normals,
			const std::vector<Vec3>& tangents,
			const std::vector<Vec3>& bitangents);

	public:

		//TODO: These are legacy contructors from the first application
		Mesh(const std::string& objPath, const Vec3& color, bool compress = true);
		Mesh(const std::string& objPath, const std::vector<Vec3>& colors, bool compress = true);


		Mesh(const ModelLoader& modelLoader, const Vec3& color, bool compress = true);
		Mesh(const ModelLoader& modelLoader, const std::vector<Vec3>& colors, bool compress = true);


		/*Note: we'll assume if no indices are given then the triangles are as a list of vertices.
		 * If indices are specified than we create an EBO to hold them. This is better since we don't have duplicate vertices.
		 */
		Mesh(const std::vector<Vec3>& positions, const std::vector<Vec3>& colors, 
			const std::vector<unsigned int>& indices = std::vector<unsigned int>());

		// Inherited via BaseMesh
		virtual size_t getNumberVerts() override;

		virtual size_t getSizeIndices() override;

	};
}