#pragma once
#include <Vectors\Vec2.h>
#include <Graphics\RenderCommand.h>
#include <Graphics\Camera\Camera.h>
#include <Scene\SceneNode.h>
#include <Graphics\Light\DirectionalLight.h>
#include <Graphics\Light\PointLight.h>
#include <Graphics\Renderer\RenderTarget.h>
#include <Graphics\Mesh\ScreenQuadMesh.h>

#include <vector>

namespace Engine
{
	class Renderer
	{
	private:

#pragma region Renderer variables
		//Clear color for the light accumulation pass
		Vec3 clearColor;

		//Current render size
		unsigned int renderHeight;
		unsigned int renderWidth;

		//Render commands
		std::vector<RenderCommand> renderCommandBuffer;

		//Global uniform block for shaders
		unsigned int guboID;

#pragma endregion

#pragma region Lights
		//Allow a max of 4 directional ligts.
		//TODO: technically we can allow for more because we now use deferred
		//It's just that we have to cap the amount of depth maps
		const int MAX_DIR_LIGHTS = 4;
		std::vector<DirectionalLight> dirLights;

		//Directional light, render target with the depth texture
		//And lightviewproj matrix
		std::vector<std::tuple<DirectionalLight,
			std::unique_ptr<RenderTarget>, Mat4>> dirLightsT;

		//TODO: Technically no longer needed
		int DIR_LIGHT_STRUCT_SIZE;

		//Allow a max of 8 point ligts.
		//TODO: Not really needed anymore since we don't have point light shadows
		//So whatever
		const int MAX_POINT_LIGHTS = 8;
		std::vector<PointLight> pointLights;

		//TODO: Technically no longer needed
		int POINT_LIGHT_STRUCT_SIZE;

#pragma endregion

#pragma region Render Passes
		//Gbuffer
		std::unique_ptr<RenderTarget> gbuffer;

		//Default shadow depth material
		std::unique_ptr<Material> depthMat;
		const int DEPTH_MAP_WIDTH = 1024;
		const int DEPTH_MAP_HEIGHT = 1024;

		//Default directional light material
		std::unique_ptr<Material> deferredDirLight;

		//Render Target for lightDeferredPass
		std::unique_ptr<RenderTarget> lightDeferredRT;

		//Default point light material
		std::unique_ptr<Material> deferredPointLight;

		//Material for HDR and gamma correction
		std::unique_ptr<Material> finalHDRMat;
#pragma endregion

	public:
		std::shared_ptr<Camera> activeCamera;

		std::unique_ptr<ScreenQuadMesh> screenQuadMesh;
		std::unique_ptr<Material> screenQuadMat;

	public:		
		Renderer(unsigned int width, unsigned int height);

		/* Pushes a node and its children
		* Note: does not handle duplicates so
		* TODO: Should this make a copy of the material? So it stores the uniforms at push time and then allow material modification
		* After being pushed. This could increase re-usage of the same material. Still eh
		*/
		void pushRenderCommand(SceneNode* node, Material* customDepthMaterial = nullptr);

		void pushRenderCommand(const Mat4& transform, BaseMesh* mesh, Material* mat, Material* customDepthMaterial = nullptr);

		/// <summary>
		/// Renders all pushed commands
		/// </summary>
		void renderPushedCommands();

		void setClearColor(const Vec3& color);

		void addDirLight(const DirectionalLight& dirLight);

		void addPointLight(const PointLight& pointLight);

		PointLight& getPointLight(int index);

	private:
		void sort();

		void renderCommand(const RenderCommand& rc);

		void renderMesh(BaseMesh* mesh);

		void updateGlobalUniforms();

		//Returns the extra offset
		int sendMatrixToGlobalUniform(int currentOffset, const Mat4& m);
		int sendVec3ToGlobalUniform(int currentOffset, const Vec3& v);		
		int sendFloatToGlobalUniform(int currentOffset, const float& v);

		void updateGLState(Material* mat);

		//Renders a depth map
		void renderShadowCommand(const RenderCommand& rc, const Mat4& lightViewProj);

#pragma region Render passes functions
		//Render passes
		//1 - deferred pass to fill gbuffer
		void deferredPass();

		//2 - light depth pass to get depth maps
		void lightDepthPass();

		//3 - deferred shading on lights
		void deferredLightRender();
#pragma endregion
	};
}