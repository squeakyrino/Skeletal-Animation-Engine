#pragma once
#include <Graphics/Mesh/SkinnedMesh.h>
#include <Graphics\Material\Material.h>
#include <Graphics\Animation\Animator.h>

namespace Engine
{
	class Model : public SkinnedMesh
	{
	public:
		std::shared_ptr<Material> animMaterial;
		std::shared_ptr<Material> animDepthMap;
		std::vector<Animation> animations;
		std::unique_ptr<Animator> animator;

		Model(ModelLoader& modelLoader, const Vec3& color, const std::string& modelPath, std::shared_ptr<Shader> animationShader,
			std::shared_ptr<Shader> animDepthShader);

		void update(float elapstedTime);

	};
}