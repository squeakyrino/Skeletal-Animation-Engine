#pragma once

namespace Engine
{
	class BaseMesh
	{
	protected:

		unsigned int vaoID;
	public:

		/// <summary>
		/// Creates a VAO and binds it.
		/// </summary>
		/// <returns></returns>
		BaseMesh();

		void bindVAO();
		void unbindVAO();


		//Virtual methods

		/// <summary>
		/// Returns the underlying id of the VAO created by OpenGL
		/// </summary>
		/// <returns>The VAO id associated with this mesh</returns>
		unsigned int getVAOID();

		/// <summary>
		/// Returns the number of vertices of this mesh.
		/// </summary>
		/// <returns>Returns the number of total vertices if if getSizeIndices() returns 0.
		/// Otherwise, returns the number of unique vertices of the mesh. </returns>
		size_t virtual getNumberVerts() = 0;

		/// <summary>
		/// Returns the numbers of indices of this mesh.
		/// </summary>
		/// <returns>Returns 0 if the mesh does not have an EBO associated with it. 		
		/// Otherwise returns how many indices there are.</returns>
		size_t virtual getSizeIndices() = 0;
	};
}