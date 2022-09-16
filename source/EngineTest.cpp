#include <EngineApp.h>
#include <iostream>
#include <Graphics\Mesh\Mesh.h>
#include <Graphics\Shader\Shader.h>
#include <Graphics\Camera\Camera.h>
#include <Matrix\MatrixFactory.h>
#include <Quaternion\Quaternion.h>
#include <EngineHelperFunctions.h>
#include <Graphics\Renderer.h>
#include <Scene\SceneNode.h>
#include "../SkeletalAnimationApp.h"

using namespace Engine;

#pragma region Errors

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
}

static void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

#pragma endregion

class EngineTest : public EngineApp
{
	std::shared_ptr<Shader> globalUniformShader;
	std::shared_ptr<Material> material;

	//Animation holder variables
	int direction = 1;

	std::shared_ptr<SceneNode> frame;
	std::shared_ptr<SceneNode> cubes;

	std::vector<Vec3> cubePos;

	bool rotateFrame = false;
	bool cubeAnim = false;
	bool stopAnim = false;

	float rotationDir = -1;
	int rotationSpood = 50;
	float rotationPercentage = 0;

	bool goY = false;
	bool goZ = false;
	float move = 0;
	float moveSpeed = 2;
	int animSpood = 80;

	std::shared_ptr<SceneNode> FeaturingDanteFromTheDevilMayCrySeriesNode;
	bool FeaturingDanteFromTheDevilMayCrySeriesBool = false;
public:
	EngineTest(int width, int height, const std::string& windowName, bool rezisable)
		: EngineApp(width, height, windowName, rezisable)
	{
	}

	bool onCreate(int argc, char** argv) override
	{
		//TODO: This is kinda debuggy at the moment. Probably replaced by a logging system later on?
		setupErrorCallback();
		globalUniformShader = std::make_shared<Shader>("uniformGlobal", "./shaders/testGlobal.vs", "./shaders/test.fs");
		material = std::make_shared<Material>(globalUniformShader);

		std::shared_ptr<Material> matNoDepth = std::make_shared<Material>(globalUniformShader);
		matNoDepth->DepthTest = false;

		Vec3 red(1.0f, 0, 0);
		Vec3 green(0, 1.0f, 0);
		Vec3 blue(0, 0, 1.0f);
		Vec3 grey(0.5f, 0.5f, 0.5f);


		std::shared_ptr<Mesh> backMesh = std::make_shared<Mesh>("models/back.obj", Vec3(0.2f, 0.2f, 0.2f));
		std::shared_ptr<Mesh> frameMesh = std::make_shared<Mesh>("models/frame.obj", Vec3(0.5f, 0, 0));
	
		std::shared_ptr<Mesh> FeaturingDanteFromTheDevilMayCrySeriesMesh = std::make_shared<Mesh>("models/Hole PlanetFixed.obj", Vec3(0.5f, 0, 0));
		FeaturingDanteFromTheDevilMayCrySeriesNode = std::make_shared<SceneNode>(FeaturingDanteFromTheDevilMayCrySeriesMesh, material);
		FeaturingDanteFromTheDevilMayCrySeriesNode->setScale(0.1f);
		FeaturingDanteFromTheDevilMayCrySeriesNode->setPosition({ 3, 0, 0 });
		FeaturingDanteFromTheDevilMayCrySeriesNode->active = FeaturingDanteFromTheDevilMayCrySeriesBool;
		SceneNode::root->addChild(FeaturingDanteFromTheDevilMayCrySeriesNode);

		frame = std::make_shared<SceneNode>(frameMesh, material);
		std::shared_ptr<SceneNode> back = std::make_shared<SceneNode>(backMesh, material);
		cubes = std::make_shared<SceneNode>();

		frame->setScale(0.8f);
		frame->setPosition(Vec3(0, 0, -1));
		frame->setRotation(Quaternion(90, Vec3(0, 1, 0)));

		back->setScale(0.95f);
		back->setPosition(Vec3(0.1f,0,0));

		Vec3 crossedVec = Vec3(1, 1, 1).normalize().cross(Vec3(0,0,1));
		float dotAngle = Vec3(1, 1, 1).normalize().dot(Vec3(0,0,1));
		float angle = std::acos(dotAngle) * 180.0f / 3.141592653589793238463f;

		cubes->setRotation(Quaternion(90,Vec3(0,1,0))*Quaternion(75, Vec3(0, 0, 1))* Quaternion(angle,crossedVec));
		cubes->setScale(0.1f);
		cubes->setPosition(Vec3(-0.25f, -0.25f, -0.25f));

		SceneNode::root->addChild(frame);
		frame->addChild(back);
		frame->addChild(cubes);

		std::vector<Vec3> customColors;
		std::vector<Vec3> colorsInit;
		
		for (int i = 0; i < 6; i++) {
			colorsInit.push_back(red);
			colorsInit.push_back(red);
			colorsInit.push_back(blue);
			colorsInit.push_back(green);
			colorsInit.push_back(blue);
			colorsInit.push_back(green);
		}
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 3; j++) {
				customColors.push_back(colorsInit[i%6]);
			}
		}

		cubePos.push_back(Vec3(-9, -3, -9));
		cubePos.push_back(Vec3(-9, -6, -9));
		cubePos.push_back(Vec3(0, 0, 0));
		cubePos.push_back(Vec3(0, 0, -3));
		cubePos.push_back(Vec3(0, 0, -6));
		cubePos.push_back(Vec3(0, 0, -9));
		cubePos.push_back(Vec3(-3, 0, -9));
		cubePos.push_back(Vec3(-6, 0, -9));
		cubePos.push_back(Vec3(-9, 0, -9));
		

		//Order matters
		std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("models/cube.obj", customColors, false);
		std::shared_ptr<SceneNode> cube = std::make_shared<SceneNode>(cubeMesh, material);
		cube->setPosition(cubePos[0]);
		cubes->addChild(cube);

		cubeMesh = std::make_shared<Mesh>("models/cube.obj", customColors, false);
		cube = std::make_shared<SceneNode>(cubeMesh, matNoDepth);
		cube->setPosition(cubePos[1]);
		cubes->addChild(cube);


		for (int i = 2; i < 9; i++) {
			std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("models/cube.obj", customColors, false);
			std::shared_ptr<SceneNode> cube = std::make_shared<SceneNode>(cubeMesh, material);
			cube->setPosition(cubePos[i]);

			cubes->addChild(cube);
		}

		renderer->activeCamera->pos = Vec3(0, 0, 2);
		return true;
	}

	bool update(float elapsedTime) override
	{
		renderer->activeCamera->updateCamera(elapsedTime, inputSys.get());
		
		if (inputSys->keyPressed(GLFW_KEY_ESCAPE))
		{
			return false;
		}

		if (inputSys->keyPressed(GLFW_KEY_P))
		{
			renderer->activeCamera->toggleProjection();
		}

		if (inputSys->keyPressed(GLFW_KEY_C)) {
			cubeAnim = cubeAnim ^ 1;
			stopAnim = true;
		}

		if (inputSys->keyPressed(GLFW_KEY_F)) {
			rotateFrame = true;
			rotationDir = -rotationDir;
		}
	
		if (inputSys->keyPressed(GLFW_KEY_Q))
		{
			FeaturingDanteFromTheDevilMayCrySeriesBool = !FeaturingDanteFromTheDevilMayCrySeriesBool;
			FeaturingDanteFromTheDevilMayCrySeriesNode->active = FeaturingDanteFromTheDevilMayCrySeriesBool;
		}

		if (cubeAnim) {
			if (move > 0.8f) direction = -1;
			else if (move < -0.8f) {
				direction = 1;
				if (!goY) {
					goY = true;
				}
				else if(!goZ){
					goZ = true;
					goY = false;
				}
				else {
					goZ = false;
					goY = false;
				}
			}
			direction = move > 0.8f ? -1 : move < -0.8f ? 1 : direction;
			move += direction * elapsedTime * moveSpeed;
			std::vector<std::shared_ptr<SceneNode>> cubeStruct = cubes->getChildren();
			for (int i = 0; i < cubeStruct.size(); i++) {
				cubeStruct[i]->setRotation(cubeStruct[i]->getRotation() * Quaternion(animSpood * elapsedTime, Vec3(0, 1, 0)));
				if (goY)
					cubeStruct[i]->setPosition(cubeStruct[i]->getPosition() + Vec3(0, -direction * elapsedTime * moveSpeed, 0));
				if (goZ)
					cubeStruct[i]->setPosition(cubeStruct[i]->getPosition() + Vec3(0,0, -direction * elapsedTime * moveSpeed));
				else if(!goY && !goZ)
					cubeStruct[i]->setPosition(cubeStruct[i]->getPosition() + Vec3(-direction * elapsedTime * moveSpeed,0,0));
			}
		}
		else if (stopAnim) {
			std::vector<std::shared_ptr<SceneNode>> cubeStruct = cubes->getChildren();
			for (int i = 0; i < cubeStruct.size(); i++) {
				cubeStruct[i]->setRotation(Quaternion());
				cubeStruct[i]->setPosition(cubePos[i]);
			}
		}
		

		if (rotateFrame) {

			rotationPercentage += rotationSpood * elapsedTime * rotationDir;

			if (360 <= rotationPercentage || rotationPercentage <= 0) {
				rotateFrame = false;
			}
			else {
				frame->setRotation(Quaternion(90, Vec3(0, 1, 0)) * Quaternion(rotationPercentage, { 0,1,0 }));
			}
		}

		renderer->pushRenderCommand(SceneNode::root.get());

		//Render all the commands
		renderer->renderPushedCommands();		

		//std::cout << "FPS: " << 1.0f / elapsedTime << std::endl;
		return true;
	}
};

int main(int argc, char* argv[])
{
	try
	{
		//EngineTest test(1920, 1080, "Press Q for surprise :)", false);
		//test.start();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		SkeletalAnimationApp test2(1920, 1080, "Skeletal Animation", false);
		setupErrorCallback();
		test2.start(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}