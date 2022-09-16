#pragma once
#include <EngineApp.h>
#include <Graphics/Mesh/SkinnedMesh.h>
#include <Graphics/Animation/Animator.h>
#include <Graphics\Mesh\Model.h>

using namespace Engine;

class SkeletalAnimationApp : public EngineApp
{
	//Extra models
	std::unique_ptr<Model> mothman;
	std::unique_ptr<Model> alice;

	std::unique_ptr<SkinnedMesh> tokiModelSkinned;

	std::shared_ptr<Shader> animationShader;
	std::shared_ptr<Material> animationMaterial;
	
	//Custom depth shader for animation
	std::shared_ptr<Material> animDepthMap;

	//Normal mapping cube
	std::unique_ptr<Mesh> cube;
	std::shared_ptr<Shader> normalMapShader;
	std::shared_ptr<Material> normalMapMat;

	//Light cube shader
	std::vector<std::pair<Vec3, std::unique_ptr<Mesh>>> lightCubes;
	std::shared_ptr<Shader> lightShader;
	std::shared_ptr<Material> lightCubesMat;


	//TEST: ANIMATION
	int currAnimationIndex = 0;
	std::vector<Animation> animations;
	std::unique_ptr<Animator> animator;
	std::unique_ptr<Animator> animatorCowboy;

	//Wireframe or not
	bool isWireframe;

public:
	SkeletalAnimationApp(int width, int height, const std::string& windowName, bool rezisable);

	bool onCreate(int argc, char** argv) override;

	bool update(float elapsedTime) override;
};