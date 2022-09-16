#include <Scene/SceneNode.h>
#include <Matrix/MatrixFactory.h>

namespace Engine
{
	std::shared_ptr<SceneNode> SceneNode::root = std::make_shared<SceneNode>();

	void SceneNode::updateTransform()
	{
		//No need to update something that isn't active
		if (!active)
			return;

		if (dirty)
		{
			Mat4 tempTransform = MatrixFactory::translationMatrix(pos) * MatrixFactory::rotationByAxis(rotation) * MatrixFactory::scalingMatrix(scale);

			//If we have a parent
			if (auto weakParent = parent.lock())
			{
				transform = weakParent->getTransform() * tempTransform;
			}
			else
			{
				transform = tempTransform;
			}

			dirty = false;
			//All the children
			for (auto& child : children)
			{
				child->dirty = true;
				child->updateTransform();
			}
		}	

	}

	SceneNode::SceneNode()
	{
		scale = Vec3(1, 1, 1);

		dirty = false;
		active = true;

		transform = MatrixFactory::translationMatrix(pos) * MatrixFactory::rotationByAxis(rotation) * MatrixFactory::scalingMatrix(scale);	
	}

	SceneNode::SceneNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) : SceneNode()
	{
		this->mesh = mesh;
		this->mat = mat;
		parent = SceneNode::root;
	}

	void SceneNode::setPosition(const Vec3& v)
	{
		pos = v;
		dirty = true;
	}

	void SceneNode::setRotation(const Quaternion& q)
	{
		rotation = q;
		dirty = true;
	}

	void SceneNode::setScale(const Vec3& v)
	{
		scale = v;
		dirty = true;
	}

	void SceneNode::setScale(const float& s)
	{
		setScale({ s,s,s });
	}

	Vec3 SceneNode::getPosition()
	{
		return pos;
	}

	Quaternion SceneNode::getRotation()
	{
		return rotation;
	}

	Vec3 SceneNode::getScale()
	{
		return scale;
	}

	Mat4 SceneNode::getTransform()
	{
		if (dirty)
		{
			updateTransform();
		}

		return transform;
	}

	void SceneNode::addChild(std::shared_ptr<SceneNode> node)
	{
		node->parent = weak_from_this();
		children.push_back(node);
	}
	
	std::vector<std::shared_ptr<SceneNode>>& SceneNode::getChildren()
	{
		return children;
	}
}