#pragma once
#include <Matrix\Mat4.h>
#include <Graphics\Material\Material.h>
#include <Graphics/Mesh/BaseMesh.h>

namespace Engine
{
	class RenderCommand
	{
	public:
		Mat4 transform;
		BaseMesh* mesh;
		Material* mat;
		Material* shadowMaterial;

		RenderCommand(const Mat4& transform, BaseMesh* mesh, Material* mat, Material* matShadow) :
			transform(transform), mesh(mesh), mat(mat), shadowMaterial(matShadow)
		{
		}
	};
}