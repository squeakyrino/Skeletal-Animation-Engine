#include "SkeletalAnimationApp.h"

#include <Matrix\MatrixFactory.h>
#include <Graphics\Mesh\SkinnedMesh.h>
#include <Graphics\Animation\Animation.h>
#include <Graphics\Animation\Animator.h>
#include <Engine Systems\ModelLoader.h>
#include <EngineHelperFunctions.h>

#include <iostream>

SkeletalAnimationApp::SkeletalAnimationApp(int width, int height, const std::string& windowName, bool rezisable)
	: EngineApp(width, height, windowName, rezisable), isWireframe(false)
{

}

bool SkeletalAnimationApp::onCreate(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " modelPath." << std::endl
			<< "Ex: " << argv[0] << " ../model/Tokisada/scale.fbx";
		return false;
	}
	std::string modelPath = argv[1];

	animationShader = std::make_shared<Shader>("animation", "./shaders/skeletalAnimationTex.vs", "./shaders/deferred/animatedModelDef.fs");

	ModelLoader modelLoader = ModelLoader(modelPath);
	
	tokiModelSkinned = std::unique_ptr<SkinnedMesh>(new SkinnedMesh(modelLoader, { 1, 1, 1 }));
	
	
	//animDepthMap creation
	auto animDepthMapShader = std::make_shared<Shader>("anim depth map", "./shaders/shadows/depthShaderAnim.vs", "./shaders/shadows/depthShader.fs");
	animDepthMap = std::make_shared<Material>(animDepthMapShader);
	animDepthMap->CullFace = GL_FRONT;
	
	animationMaterial = std::make_shared<Material>(animationShader);
	
	//Texture
	std::string diffTexName = getDirectoryFromPath(modelPath) + "/" + modelLoader.getDiffuseTextureName();

	//TODO: this works for now because we're not destroying the texture
	//on destructor. Technically a leak but wtv for now
	auto diffTexture = std::make_unique<Texture>(diffTexName, true);

	animationMaterial->setTexture("texMap", diffTexture.get(), 0);

	//Material properties
	animationMaterial->setUniformVec3("mat.ambient", modelLoader.ambient);
	animationMaterial->setUniformVec3("mat.diffuse", modelLoader.diffuse);
	animationMaterial->setUniformVec3("mat.specular", modelLoader.specular);
	animationMaterial->setUniformFloat("mat.shininess", modelLoader.shininess);

	//Normal texture
	std::string diffNormalTexName = getDirectoryFromPath(modelPath) + "/" + modelLoader.getNormalTextureName();
	auto diffNormalTexture = std::make_unique<Texture>(diffNormalTexName);
	animationMaterial->setTexture("normalMap", diffNormalTexture.get(), 1);

	//Get the animations
	animations = modelLoader.getAnimations();
	Animation idleAnimation = animations[0];

	animator = std::make_unique<Animator>(idleAnimation, modelLoader.getRootJoint());

	renderer->activeCamera->pos = Vec3(0, 0, 1);

	//Cube mesh	
	std::string cubeModelPath = "models/Normal Cube/cube.fbx";
	ModelLoader cubeLoader = ModelLoader(cubeModelPath);
	normalMapShader = std::make_shared<Shader>("uniformGlobal", "./shaders/normalMap.vs", "./shaders/deferred/animatedModelDef.fs");
	normalMapMat = std::make_shared<Material>(normalMapShader);
	cube = std::unique_ptr<Mesh>(new Mesh(cubeLoader, Vec3(1, 1, 0.5)));

	//Diffuse texture for cube
	auto diffTextureCube = std::make_unique<Texture>("models/Normal Cube/diffMap.jpg", true);
	normalMapMat->setTexture("texMap", diffTextureCube.get(), 0);
	//Normal texture for cube
	auto normalTextureCube = std::make_unique<Texture>("models/Normal Cube/normalMap.jpg");
	normalMapMat->setTexture("normalMap", normalTextureCube.get(), 1);

	//Material properties
	normalMapMat->setUniformVec3("mat.ambient", Vec3(0.2f));
	normalMapMat->setUniformVec3("mat.diffuse", cubeLoader.diffuse);
	normalMapMat->setUniformVec3("mat.specular", cubeLoader.specular);
	normalMapMat->setUniformFloat("mat.shininess", cubeLoader.shininess);

	renderer->setClearColor({ 0.05f, 0.05f, 0.05f });

	Vec3 sunDir = Vec3(0) - Vec3(0.25f, 0.25f, -0.3f);

	//Set up lights
	DirectionalLight sun = DirectionalLight(sunDir, Vec3(0.2f), Vec3(1.0f), Vec3(2));
	//DirectionalLight sun = DirectionalLight(sunDir, Vec3(10), Vec3(10), Vec3(10));
	renderer->addDirLight(sun);

	sun = DirectionalLight({ -0.25f, -0.25f, 0 }, Vec3(0.1f), Vec3(0.8f), Vec3(0.5f));
	renderer->addDirLight(sun);

	sun = DirectionalLight({ 0.25, -0.25f, 0.25f }, Vec3(0.1f), Vec3(0.8f), Vec3(0.5f));
	//renderer->addDirLight(sun);

	Vec3 redLightPos = { 0, 0, 0.5f };
	PointLight point = PointLight(redLightPos, 15, Vec3(0.2f, 0, 0),
		Vec3(1, 0, 0), Vec3(1, 0, 0));
	renderer->addPointLight(point);

	Vec3 blueLightPos = { 0.5f, 0, 0 };
	point = PointLight(blueLightPos, 15, Vec3(0, 0, 0.2f),
		Vec3(0, 0, 1), Vec3(0, 0, 1));
	renderer->addPointLight(point);

	Vec3 greenLightPos = { -0.5f, 0, 0 };
	point = PointLight(greenLightPos, 15, Vec3(0, 0.2f, 0),
		Vec3(0, 0.2f, 0), Vec3(0, 0.2f, 0));
	renderer->addPointLight(point);

	lightShader = std::make_shared<Shader>("lightShader", "./shaders/simpleShader.vs", "./shaders/simpleShader.fs");
	lightCubesMat = std::make_shared<Material>(lightShader);

	lightCubes.push_back(std::make_pair(redLightPos, std::make_unique<Mesh>(cubeModelPath, Vec3(1, 0, 0))));
	lightCubes.push_back(std::make_pair(blueLightPos, std::make_unique<Mesh>(cubeModelPath, Vec3(0, 0, 1))));
	lightCubes.push_back(std::make_pair(greenLightPos, std::make_unique<Mesh>(cubeModelPath, Vec3(0, 1, 0))));


	/*Extra models*/
	//ModelLoader& modelLoader, const Vec3& color, const std::string& modelPath, std::shared_ptr<Shader> animationShader,
	//std::shared_ptr<Shader> animDepthShader
	std::string mothPath = "Models/Shiwanna/scale.fbx";
	ModelLoader mothmanLoad = ModelLoader(mothPath);
	mothman = std::make_unique<Model>(mothmanLoad, Vec3(1), mothPath, animationShader, animDepthMapShader);

	std::string alicePath = "Models/Buraq/scale.fbx";
	ModelLoader aliceLoad = ModelLoader(alicePath);
	alice = std::make_unique<Model>(aliceLoad, Vec3(1), alicePath, animationShader, animDepthMapShader);

	return true;
}

bool SkeletalAnimationApp::update(float elapsedTime)
{
#pragma region Keybind
	if (inputSys->keyPressed(GLFW_KEY_ESCAPE))
		return false;

	if (inputSys->keyPressed(GLFW_KEY_RIGHT))
	{
		//Increase animation slot
		currAnimationIndex++;
		currAnimationIndex = currAnimationIndex % animations.size();

		animator = std::make_unique<Animator>(animations[currAnimationIndex], animator->rootJoint);

		std::cout << "Playing animation: " << currAnimationIndex << ": " << animations[currAnimationIndex].getName() << std::endl;

		mothman->animator = std::make_unique<Animator>(mothman->animations[currAnimationIndex], mothman->animator->rootJoint);
		alice->animator = std::make_unique<Animator>(alice->animations[currAnimationIndex], alice->animator->rootJoint);
	}

	if (inputSys->keyPressed(GLFW_KEY_LEFT))
	{
		//Decrease animation slot
		currAnimationIndex--;
		if (currAnimationIndex < 0)
			currAnimationIndex = (int)animations.size() - 1;

		animator = std::make_unique<Animator>(animations[currAnimationIndex], animator->rootJoint);

		mothman->animator = std::make_unique<Animator>(mothman->animations[currAnimationIndex], mothman->animator->rootJoint);
		alice->animator = std::make_unique<Animator>(alice->animations[currAnimationIndex], alice->animator->rootJoint);

		std::cout << "Playing animation: " << currAnimationIndex << ": " << animations[currAnimationIndex].getName() << std::endl;
	}

	if (inputSys->keyPressed(GLFW_KEY_P))
	{
		//Pause/unpause the animation
		bool newStopChange = !animator->stop;
		animator->stop = newStopChange;

		std::cout << "Animation " << (newStopChange ? "stopped" : "resumed") << "." << std::endl;
	}

	if (inputSys->keyPressed(GLFW_KEY_UP))
	{
		//Color by ids
		animationMaterial->setUniformInt("mode", 0);

		std::cout << "Vertices colored by boneId." << std::endl;
	}
	if (inputSys->keyPressed(GLFW_KEY_DOWN))
	{
		//Color by weights
		animationMaterial->setUniformInt("mode", 1);
		std::cout << "Vertices colored by weights." << std::endl;
	}

	if (inputSys->keyPressed(GLFW_KEY_O))
	{
		isWireframe = !isWireframe;

		if (isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}	

		std::cout << "Wireframe mode: " << (isWireframe ? "ON" : "OFF") << std::endl;
	}
#pragma endregion

	renderer->activeCamera->updateCamera(elapsedTime, inputSys.get());

	renderer->pushRenderCommand(
		MatrixFactory::translationMatrix(0, -0.2f, 0) * MatrixFactory::rotationByAxis({1, 1, 0}, 0.0f) *
		MatrixFactory::scalingMatrix(10, 0.1f, 10), cube.get(), normalMapMat.get());

	//Animation stuff
	animator->update(elapsedTime * 1);

	/*Get the joint transforms*/
	std::vector<Mat4> transformsFromAnimator = animator->getAnimatedBoneTransforms();
	std::vector<Mat4> identityTransforms = std::vector<Mat4>(transformsFromAnimator.size(), MatrixFactory::identityMat4());
	
	animationMaterial->setUniformArrayMatrix4("boneTransform", transformsFromAnimator);
	animationMaterial->setUniformInt("numBones", (int)transformsFromAnimator.size());
	animDepthMap->setUniformArrayMatrix4("boneTransform", transformsFromAnimator);

	
	renderer->pushRenderCommand(MatrixFactory::translationMatrix(0, 0, 0), tokiModelSkinned.get(), animationMaterial.get(), animDepthMap.get());

	mothman->update(elapsedTime);
	alice->update(elapsedTime);
	renderer->pushRenderCommand(MatrixFactory::translationMatrix(0.5f, 0, 0), mothman.get(), mothman->animMaterial.get(), mothman->animDepthMap.get());
	renderer->pushRenderCommand(MatrixFactory::translationMatrix(-0.5f, 0, 0), alice.get(), alice->animMaterial.get(), alice->animDepthMap.get());


	//Add the light cubes
	Mat4 scale = MatrixFactory::scalingMatrix(0.02f);

	for (auto& it : lightCubes)
	{
		Mat4 finalTransform = MatrixFactory::translationMatrix(it.first) * scale;
		renderer->pushRenderCommand(finalTransform, it.second.get(), lightCubesMat.get());
	}

	renderer->renderPushedCommands();

	return true;
}
