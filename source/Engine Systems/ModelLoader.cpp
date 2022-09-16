#include <Engine Systems/ModelLoader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <queue>


namespace Engine
{
	void ModelLoader::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			/*
			The model we're loading can be split in several meshes. Indices reset back to 0 when a new mesh is loaded,
			and because we have all vertices in the same place, we need to offset the indices to their correct place
			*/
			unsigned int baseVertexIndex = (unsigned int)vertices.size();

			//Process functions
			std::vector<Vec3> tempPos = processMeshVertices(mesh);
			std::vector<Vec3> tempNormal = processMeshNormals(mesh);
			std::vector<Vec2> tempUV = processMeshTex(mesh);
			std::vector<Vec3> tempTan = processMeshTangents(mesh);
			std::vector<Vec3> tempBitan = processMeshBitangents(mesh);

			std::vector<unsigned int> tempIndices = processIndices(mesh, baseVertexIndex);

			//Process the bone data
			processJoint(mesh, baseVertexIndex);

			//Add to the output vectors
			vertices.insert(vertices.end(), tempPos.begin(), tempPos.end());
			normals.insert(normals.end(), tempNormal.begin(), tempNormal.end());
			texcoords.insert(texcoords.end(), tempUV.begin(), tempUV.end());
			tangents.insert(tangents.end(), tempTan.begin(), tempTan.end());
			bitangents.insert(bitangents.end(), tempBitan.begin(), tempBitan.end());

			indices.insert(indices.end(), tempIndices.begin(), tempIndices.end());

		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	std::vector<Vec3> ModelLoader::processMeshVertices(aiMesh* mesh)
	{
		std::vector<Vec3> result;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			//Vertices
			Vec3 vertTemp = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			result.push_back(vertTemp);
		}

		return result;
	}

	std::vector<Vec3> ModelLoader::processMeshNormals(aiMesh* mesh)
	{
		std::vector<Vec3> result;

		if (!mesh->HasNormals())
		{
			std::cout << "Mesh: " << mesh->mName.data << " has no normals" << std::endl;
			return result;
		}

		//The array is mNumVertices in size
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			//Vertices
			Vec3 vertTemp = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			result.push_back(vertTemp);
		}

		return result;
	}

	std::vector<Vec2> ModelLoader::processMeshTex(aiMesh* mesh)
	{
		std::vector<Vec2> result;

		if (mesh->HasTextureCoords(0))
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				result.push_back(vec);
			}
		}

		return result;
	}

	std::vector<Vec3> ModelLoader::processMeshTangents(aiMesh* mesh)
	{
		std::vector<Vec3> result;

		if (mesh->HasTangentsAndBitangents())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				//Tangents
				Vec3 tanTemp = Vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				result.push_back(tanTemp);
			}
		}
		
		return result;
	}

	std::vector<Vec3> ModelLoader::processMeshBitangents(aiMesh* mesh)
	{
		std::vector<Vec3> result;

		if (mesh->HasTangentsAndBitangents())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				//Bitangents
				Vec3 bitanTemp = Vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
				result.push_back(bitanTemp);
			}
		}

		return result;
	}

	std::vector<unsigned int> ModelLoader::processIndices(aiMesh* mesh, const unsigned int& baseVertexIndex)
	{
		std::vector<unsigned int> result;

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				result.push_back(face.mIndices[j] + baseVertexIndex);
		}

		return result;
	}

	void ModelLoader::processBoneIndexWeight()
	{
		/*
		For the bone ids:
			- Resize the array for the amount of vertices
			- Every vertex's weights must be init to a vector of 4 zeros (and then might be replaced by the data we have)
		*/

		boneIds.resize(vertices.size());

		for (int i = 0; i < vertices.size(); i++)
		{
			boneIds[i] = std::vector<unsigned int>(4, 0);
		}

		//Sorted by vertex indice already because of map
		for (auto const& it : jointData)
		{
			auto boneIndexWeightsPairs = it.second;

			Vec4 resultWeight;

			//Little hack to pack ids in vectors
			switch (boneIndexWeightsPairs.size())
			{
			case 4:
			{
				boneIds[it.first][3] = boneIndexWeightsPairs[3].first;
				resultWeight.w = boneIndexWeightsPairs[3].second;
			}
			case 3:
			{
				boneIds[it.first][2] = boneIndexWeightsPairs[2].first;
				resultWeight.z = boneIndexWeightsPairs[2].second;
			}
			case 2:
			{
				boneIds[it.first][1] = boneIndexWeightsPairs[1].first;
				resultWeight.y = boneIndexWeightsPairs[1].second;
			}
			case 1:
			{
				boneIds[it.first][0] = boneIndexWeightsPairs[0].first;
				resultWeight.x = boneIndexWeightsPairs[0].second;
			}
			default:
				break;
			}
			boneWeigts.push_back(resultWeight);
		}
	}

	void ModelLoader::processJoint(aiMesh* mesh, unsigned int startVerticeIndex)
	{
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* boneTemp = mesh->mBones[i];

			Joint jointTemp = Joint(i, boneTemp->mName.data, assimpMatConvert(boneTemp->mOffsetMatrix));
			
			joints.insert(std::map<std::string, Joint>::value_type(jointTemp.getName(), jointTemp));

			for (unsigned int j = 0; j < boneTemp->mNumWeights; j++)
			{
				auto vertexTemp = boneTemp->mWeights[j];

				//boneId/weight
				auto pair = std::make_pair(jointTemp.getIndex(), vertexTemp.mWeight);

				//Needs this correction
				//See: processNode()
				unsigned int correctedVertexId = vertexTemp.mVertexId + startVerticeIndex;
				jointData[correctedVertexId].push_back(pair);
			}
		}
	}

	void ModelLoader::buildBoneHierarchy(aiNode* rootSkeletonNode, Joint& parentJoint)
	{
		//Go through the children now
		for (unsigned int i = 0; i < rootSkeletonNode->mNumChildren; i++)
		{
			aiNode* currChild = rootSkeletonNode->mChildren[i];

			auto jointChildIt = joints.find(currChild->mName.data);

			if (jointChildIt != joints.end())
			{
				Joint& childJoint = jointChildIt->second;
				parentJoint.addChild(childJoint);

				//Child joint came from the map, not the parent join children
				//so fix this
				//TODO: Pointers later or something idk
				Joint& referenceChildJoint = parentJoint.getChildren()[parentJoint.getChildren().size() - 1];
				buildBoneHierarchy(currChild, referenceChildJoint);
			}
			else
			{
				//std::cout << "BONE: ERROR: Bone name " << currChild->mName.data << " not in map." << std::endl;
				buildBoneHierarchy(currChild, parentJoint);
			}
		}
	}

	std::vector<Animation> ModelLoader::processAnimations(const aiScene* scene)
	{
		std::vector<Animation> result;

		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			//For each animation
			// - Get duration
			// - Get a JointAnimation for each joint

			float TicksPerSecond = (float)scene->mAnimations[i]->mTicksPerSecond;
			float animationDuration = (float)scene->mAnimations[i]->mDuration / TicksPerSecond;

			std::string animationName = scene->mAnimations[i]->mName.data;

			std::vector<JointAnimation> jointAnimations;

			for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				//For each joint
				// - Get the name
				// - Get the keyframes (ASSIMP guarantees they're in order) for the joint
				std::string jointName = scene->mAnimations[i]->mChannels[j]->mNodeName.data;

				std::vector<KeyFrame> kreyframes;

				for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
				{
					//For each keyframe
					// - Get the timestamp (IMPORTANT: Divide by ticksPerSecond)
					// - Get the value

					aiVector3D aiPos = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
					aiQuaternion aiRot = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;

					aiVector3D aiScale = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue;

					double timeStamp = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime / TicksPerSecond;

					//Start with the transform first
					Vec3 localBonePos = convertVec(aiPos);
					Quaternion localBoneRot = convertQuat(aiRot);
					Vec3 localBoneScale = convertVec(aiScale);

					JointTransform jointTransform(localBonePos, localBoneRot, localBoneScale);

					KeyFrame resultKeyframe((float)timeStamp, jointTransform);
					kreyframes.push_back(resultKeyframe);
				}

				JointAnimation resultJointAnimtion(kreyframes, jointName);
				jointAnimations.push_back(resultJointAnimtion);
			}

			Animation resultAnimation(animationDuration, jointAnimations, animationName);
			result.push_back(resultAnimation);
		}
		return result;
	}

	std::set<std::string> ModelLoader::getNodesNecessaryForSkeleton(const aiScene* scene)
	{
		std::set<std::string> result;

		/*
		ASSIMP's official recommendation actually sucks.

		So instead we put every bone's name into a set. ASSIMP guarantees that bones have a node with their exact name.
		All we need to do is get every bone's name and put it into a set. This is less efficient than getting the nodes but
		works much better.
		*/

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			if (mesh->HasBones())
			{
				for (unsigned int j = 0; j < mesh->mNumBones; j++)
				{
					aiBone* bone = mesh->mBones[j];
					
					//This works because bones MUST have a node with their name as per ASSIMP spec
					std::string boneName = bone->mName.data;
					
					result.insert(boneName);					
				}
			}
			else
			{
				//No bones on mesh so yeet
			}
		}

		return result;
	}

	aiNode* ModelLoader::findRootJoint(const std::set<std::string>& boneNames, aiNode* rootNode)
	{
		/* The idea is this:
			We already have the set of the necessary bones' names to build the hierarchy so
			do a Breadth-first search on the root node until we find the first node that is necessary
			That is our root node to process the skeleton
		*/

		std::queue<aiNode*> queue;
		
		//Our discovered set is actually the nodeSet

		//Push the root into the queue
		queue.push(rootNode);

		while (!queue.empty())
		{
			aiNode* currNode = queue.front();
			queue.pop();

			//If this node matches any node in the nodeSet then we found it
			if (boneNames.find(currNode->mName.data) != boneNames.end())
			{
				//Found it!
				return currNode;
			}

			//Otherwise, go through the children now
			for (unsigned int i = 0; i < currNode->mNumChildren; i++)
			{
				aiNode* currChild = currNode->mChildren[i];
				queue.push(currChild);				
			}
		}

		//Rip
		return nullptr;
	}

	Vec3 ModelLoader::convertVec(const aiVector3D& aiVec)
	{
		return Vec3(aiVec.x, aiVec.y, aiVec.z);
	}

	Quaternion ModelLoader::convertQuat(const aiQuaternion& aiQ)
	{
		return Quaternion(aiQ.w, aiQ.x, aiQ.y, aiQ.z);
	}

	Mat4 ModelLoader::assimpMatConvert(const aiMatrix4x4& mat)
	{
		std::vector<std::vector<float>> result = std::vector<std::vector<float>>(4, std::vector<float>(4, 0));
		
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[j][k] = mat[j][k];
			}
		}

		Mat4 resultMat = Mat4(result);

		return resultMat;
	}

	void ModelLoader::getTexturesName(const aiScene* scene)
	{
		//Diffuse
		auto mat = scene->mMaterials[0];

		aiString file;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &file);

		//std::cout << "\tDiffuse tex name:" << file.C_Str() << std::endl;

		this->diffuseTexName = std::string(file.C_Str());

		//Normal
		//Note: depends on the model because of assimp limitations
		mat->GetTexture(aiTextureType_HEIGHT, 0, &file);

		//std::cout << "\tNormal tex name:" << file.C_Str() << std::endl;

		this->normalTexName = std::string(file.C_Str());
	}

	void ModelLoader::getMaterialProperties(const aiScene* scene)
	{
		auto mat = scene->mMaterials[0];

		aiColor3D ambColor = aiColor3D();
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambColor);
		this->ambient = Vec3(ambColor.r, ambColor.g, ambColor.b);

		aiColor3D diffColor = aiColor3D();
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor);
		this->diffuse = Vec3(diffColor.r, diffColor.g, diffColor.b);


		aiColor3D specColor = aiColor3D();
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specColor);
		this->specular = Vec3(specColor.r, specColor.g, specColor.b);

		mat->Get(AI_MATKEY_SHININESS, this->shininess);
	}

	ModelLoader::ModelLoader(const std::string& modelPath) :
		pathName(modelPath), numBones(0),
		shininess(0)
	{
		Assimp::Importer import;

		//Flip UVs because of OpenGL
		const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_JoinIdenticalVertices
			| aiProcess_OptimizeMeshes
			| aiProcess_ValidateDataStructure
		);

		if (!scene || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		animations = processAnimations(scene);

		std::set<std::string> nodesNecessaryForSkeleton = 
			getNodesNecessaryForSkeleton(scene);

		//nodesNecessaryForSkeleton now has all the necessary bone names

		//Process the tree to find the meshes with their vertices, normals, etc...
		processNode(scene->mRootNode, scene);

		//After we get the meshing information and filled the jointData, process the per vertex bone info
		processBoneIndexWeight();

		
		//Only do the following if we actually have bones
		if (nodesNecessaryForSkeleton.size() != 0)
		{
			//Find the root skeleton node
			aiNode* rootBoneNode = findRootJoint(nodesNecessaryForSkeleton, scene->mRootNode);

			rootJointName = rootBoneNode->mName.data;

			//Get the joint associated with the root node
			auto rootJointIt = joints.find(rootJointName);

			//Build the skeleton now
			buildBoneHierarchy(rootBoneNode, rootJointIt->second);

			this->numBones = (int)joints.size();
		}

		//Material properties
		
		//TODO: Only 1 material supported. Several materials would be a massive re-write
		getTexturesName(scene);
		
		//Mat properties
		getMaterialProperties(scene);
	}

	std::vector<Vec3> ModelLoader::getVertices() const
	{
		return vertices;
	}

	std::vector<Vec3> ModelLoader::getNormals() const
	{
		return normals;
	}
	
	std::vector<Vec2> ModelLoader::getTexCoords() const
	{
		return texcoords;
	}

	std::vector<unsigned int> ModelLoader::getIndices() const
	{
		return indices;
	}

	std::vector<Vec3> ModelLoader::getTangents() const
	{
		return tangents;
	}

	std::vector<Vec3> ModelLoader::getBitTangents() const
	{
		return bitangents;
	}

	std::vector<std::vector<unsigned int>> ModelLoader::getBoneIds() const
	{
		return boneIds;
	}

	std::vector<Vec4> ModelLoader::getBoneWeights() const
	{
		return boneWeigts;
	}

	int ModelLoader::getNumBones() const
	{
		return numBones;
	}

	std::vector<Animation> ModelLoader::getAnimations() const
	{
		return animations;
	}

	std::string ModelLoader::getDiffuseTextureName() const
	{
		return diffuseTexName;
	}

	std::string ModelLoader::getNormalTextureName() const
	{
		return normalTexName;
	}
	
	Joint ModelLoader::getRootJoint() const
	{
		return joints.find(rootJointName)->second;
	}
}