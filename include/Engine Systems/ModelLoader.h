#pragma once
#include <Graphics\Animation\Joint.h>
#include <Graphics\Animation\Animation.h>

#include <set>
#include <map>
#include <string>
#include <vector>

#include <assimp\scene.h>

namespace Engine
{
	class ModelLoader
	{
	private:
		std::string pathName;

		//Output variables
		//Animation related vars
		std::vector<Animation> animations;

		//Model related vars
		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;
		std::vector<Vec2> texcoords;
		std::vector<unsigned int> indices;
		std::vector<Vec3> tangents;
		std::vector<Vec3> bitangents;

		//Already ordered by boneIndex
		std::vector<std::vector<unsigned int>> boneIds;
		std::vector<Vec4> boneWeigts;

		std::string rootJointName;

		int numBones;

		//Material properties
		std::string diffuseTexName;
		std::string normalTexName;

		//Temporary variables because there's no easy way to do it without

		/*
		This exists to retrieve the relevant skinning info from assimp.
		jointData - a map that maps vertex ID -> boneID/weights
		*/
		std::map<unsigned int, std::vector<std::pair<unsigned int, float>>> jointData;

		//TODO: Try to move this map out the way or something
		//Name -> Joint
		std::map<std::string, Joint> joints;

	private:

		//Main node processing function
		void processNode(aiNode* node, const aiScene* scene);
		
		//--Mesh processing--
		//Vertices
		std::vector<Vec3> processMeshVertices(aiMesh* mesh);
		//Normals
		std::vector<Vec3> processMeshNormals(aiMesh* mesh);
		//Tex coords
		//This only gets the first set of UV coordinates
		std::vector<Vec2> processMeshTex(aiMesh* mesh);
		//Tangents
		std::vector<Vec3> processMeshTangents(aiMesh* mesh);
		//BitTangents
		std::vector<Vec3> processMeshBitangents(aiMesh* mesh);

		//Indices
		std::vector<unsigned int> processIndices(aiMesh* mesh, const unsigned int& baseVertexIndex);

		//Bone Indexes and weights
		void processBoneIndexWeight();

		/*
		Processes the bones.		
		*/
		void processJoint(aiMesh* mesh, unsigned int startVerticeIndex);

		//Builds the bones' hierarchy given the root skeleton bone
		void buildBoneHierarchy(aiNode* rootSkeletonNode, Joint& parentJoint);


		//Animation function
		std::vector<Animation> processAnimations(const aiScene* scene);

		/*
		Marks all nodes necessary for the skeleton hierarchy for later user
		Might break if the skeleton has a fake root bone which the mesh doesn't use. Too bad!s
		*/
		std::set<std::string> getNodesNecessaryForSkeleton(const aiScene* scene);

		aiNode* findRootJoint(const std::set<std::string>& boneNames, aiNode* rootNode);

		//Converters 
		Vec3 convertVec(const aiVector3D& aiVec);
		Quaternion convertQuat(const aiQuaternion& aiQ);
		Mat4 assimpMatConvert(const aiMatrix4x4& mat);

		void getTexturesName(const aiScene* scene);
		void getMaterialProperties(const aiScene* scene);

	public:
		//Functions
		ModelLoader(const std::string& modelPath);

		//Relevant mesh data
		std::vector<Vec3> getVertices() const;
		std::vector<Vec3> getNormals() const;
		std::vector<Vec2> getTexCoords() const;
		std::vector<unsigned int> getIndices() const;
		std::vector<Vec3> getTangents() const;
		std::vector<Vec3> getBitTangents() const;

	
		//Bone relevant data
		std::vector<std::vector<unsigned int>> getBoneIds() const;
		std::vector<Vec4> getBoneWeights() const;
		int getNumBones() const;

		//Get the root joint with the bone hierarchy built
		Joint getRootJoint() const;

		//Animations
		std::vector<Animation> getAnimations() const;

		//Material properties
		Vec3 ambient;
		Vec3 diffuse;
		Vec3 specular;
		float shininess;

		//Texture
		//TODO: Like the model loader itself it only supports 1 material
		//and 1 diffuse texture
		std::string getDiffuseTextureName() const;

		//Inconsistent because assimp is inconsistent
		std::string getNormalTextureName() const;
	};
}