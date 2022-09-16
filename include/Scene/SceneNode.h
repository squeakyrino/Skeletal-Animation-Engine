#pragma once
#include <Vectors\Vec3.h>
#include <Quaternion\Quaternion.h>
#include <Graphics\Mesh\Mesh.h>
#include <Graphics\Material\Material.h>

namespace Engine
{
	class SceneNode : public std::enable_shared_from_this<SceneNode>
	{
	private:
		Vec3 pos;
		Quaternion rotation;
		Vec3 scale;

		Mat4 transform;

		std::weak_ptr<SceneNode> parent;
		std::vector<std::shared_ptr<SceneNode>> children;

		bool dirty;

	public:
		bool active;

		static std::shared_ptr<SceneNode> root;
	
		//Probably find being public
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> mat;

	private:
		void updateTransform();

	public:
		/* Container node
		* Does not assign a parent
		*/
		SceneNode();

		/* 
		Note: assumes the root as default parent
		*/
		SceneNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat);

		void setPosition(const Vec3& v);
		void setRotation(const Quaternion& q);
		void setScale(const Vec3& v);
		void setScale(const float& s);

		Vec3 getPosition();
		Quaternion getRotation();
		Vec3 getScale();

		/*
		Note: if the node is not active the transform will not be in a valid state.
		Shouldn't be a problem since we check in the renderer but outside use is iffy
		*/
		Mat4 getTransform();

		void addChild(std::shared_ptr<SceneNode> node);

		std::vector<std::shared_ptr<SceneNode>>& getChildren();
	};
}