#include <Graphics/Mesh/Model.h>
#include <EngineHelperFunctions.h>

namespace Engine
{
		Model::Model(ModelLoader& modelLoader, const Vec3& color, const std::string& modelPath, std::shared_ptr<Shader> animationShader,
			std::shared_ptr<Shader> animDepthShader) :
		SkinnedMesh(modelLoader, color)
	{
			//Model loader
			//Textures
			std::string diffTexName = getDirectoryFromPath(modelPath) + "/" + modelLoader.getDiffuseTextureName();
			auto diffTexture = std::make_unique<Texture>(diffTexName, true);

			std::string normalTexName = getDirectoryFromPath(modelPath) + "/" + modelLoader.getNormalTextureName();
			auto normalTexture = std::make_unique<Texture>(diffTexName, true);

			//animDepthMap creation
			animDepthMap = std::make_shared<Material>(animDepthShader);
			animDepthMap->CullFace = GL_FRONT;

			animMaterial = std::make_shared<Material>(animationShader);

			//Material prop
			
			//Material properties
			animMaterial->setUniformVec3("mat.ambient", modelLoader.ambient);
			animMaterial->setUniformVec3("mat.diffuse", modelLoader.diffuse);
			animMaterial->setUniformVec3("mat.specular", modelLoader.specular);
			animMaterial->setUniformFloat("mat.shininess", modelLoader.shininess);

			//Adding textures to materials
			animMaterial->setTexture("texMap", diffTexture.get(), 0);
			animMaterial->setTexture("normalMap", diffTexture.get(), 1);


			//Get the animations
			animations = modelLoader.getAnimations();
			Animation idleAnimation = animations[0];

			animator = std::make_unique<Animator>(idleAnimation, modelLoader.getRootJoint());
	}
		void Model::update(float elapstedTime)
		{
			/*Get the joint transforms*/
			animator->update(elapstedTime);
			std::vector<Mat4> transformsFromAnimator = animator->getAnimatedBoneTransforms();

			animMaterial->setUniformArrayMatrix4("boneTransform", transformsFromAnimator);
			animMaterial->setUniformInt("numBones", (int)transformsFromAnimator.size());
			animDepthMap->setUniformArrayMatrix4("boneTransform", transformsFromAnimator);
		}
}