#include <Graphics/Renderer.h>
#include <Graphics/OpenGLUtils.h>
#include <Matrix/MatrixFactory.h>

#include <algorithm>

namespace Engine
{
	Renderer::Renderer(unsigned int width, unsigned int height) : clearColor(Vec3(0, 0, 0)),
		renderWidth(width), renderHeight(height)
	{
		{
			//TODO: Refactor this a bit, in particular the size of uniform buffer
			//And if we change the global uniform we'll have to do it here too
			glGenBuffers(1, &guboID);
			glBindBuffer(GL_UNIFORM_BUFFER, guboID);

			GLsizeiptr bufferSize = 0;

			const int matSize = 64;
			const int vec4Size = 16;
			const int floatSize = 4;

			//Two matrices
			bufferSize += matSize * 2;
			//1 vec 4
			bufferSize += vec4Size;

			{
				DIR_LIGHT_STRUCT_SIZE = 0;

				DIR_LIGHT_STRUCT_SIZE += vec4Size; //direction
				DIR_LIGHT_STRUCT_SIZE += vec4Size; //ambient
				DIR_LIGHT_STRUCT_SIZE += vec4Size; //diffuse
				DIR_LIGHT_STRUCT_SIZE += vec4Size; //specular

				//No padding needed

				//4 x DirectionalLight
				bufferSize += DIR_LIGHT_STRUCT_SIZE * MAX_DIR_LIGHTS;
			}

			//PointLights
			{
				POINT_LIGHT_STRUCT_SIZE = 0;

				POINT_LIGHT_STRUCT_SIZE += vec4Size; //Position
				POINT_LIGHT_STRUCT_SIZE += vec4Size; //ambient
				POINT_LIGHT_STRUCT_SIZE += vec4Size; //diffuse
				POINT_LIGHT_STRUCT_SIZE += vec4Size; //specular

				POINT_LIGHT_STRUCT_SIZE += floatSize; //linear
				POINT_LIGHT_STRUCT_SIZE += floatSize; //quadratic

				//Needs 2 more floats of padding because structs are aligned at 16 bytes
				POINT_LIGHT_STRUCT_SIZE += floatSize * 2; //padding

				//8 x PointLights
				bufferSize += POINT_LIGHT_STRUCT_SIZE * MAX_POINT_LIGHTS;
			}

			glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STREAM_DRAW);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, guboID, 0, bufferSize);
		}


		//Gbuffer
		//1 - position
		//2 - normal
		//3 - texColor
		//Material properties
		//4 - ambient
		//5 - diffuse
		//6 - specular and shininess on alpha
		gbuffer = std::make_unique<RenderTarget>(renderWidth, renderHeight, GL_FLOAT, 6, true);

		//Default depth material for lights
		auto depthShader = std::make_shared<Shader>("depth", "./shaders/shadows/depthShader.vs", "./shaders/shadows/depthShader.fs");
		depthMat = std::make_unique<Material>(depthShader);
		depthMat->CullFace = GL_FRONT;

		//Deferred light shader
		//Blend must be enabled because we will add the contributions of each light and their shadows to one texture.
		//So that we can then apply gamma correction and HDR

		//TODO: move this to another class like material library for default things
		auto deferredDirShader = std::make_shared<Shader>("test", "./shaders/deferred/deferredDirLight.vs", "./shaders/deferred/deferredDirLight.fs");
		deferredDirLight = std::make_unique<Material>(deferredDirShader);
		deferredDirLight->Blend = true;
		deferredDirLight->BlendDst = GL_ONE;

		//Deferred light pass RT
		//Don't need depth, only 1 color needed
		lightDeferredRT = std::make_unique<RenderTarget>(renderWidth, renderHeight, GL_FLOAT, 1, true);

		//Default point light material
		auto deferredPointShader = std::make_shared<Shader>("def point light", "./shaders/deferred/deferredDirLight.vs", "./shaders/deferred/deferredPointLight.fs");
		deferredPointLight = std::make_unique<Material>(deferredPointShader);
		deferredPointLight->Blend = true;
		deferredPointLight->BlendDst = GL_ONE;

		//Final render pass material
		auto hdrGammaShader = std::make_shared<Shader>("screen quad HDR", "./shaders/screenQuad.vs", "./shaders/deferred/hdrGammaRender.fs");
		finalHDRMat = std::make_unique<Material>(hdrGammaShader);

		//TODO: This is for testing prett much
		auto  screenQuadShader = std::make_shared<Shader>("screen quad", "./shaders/screenQuad.vs", "./shaders/screenQuad.fs");
		screenQuadMat = std::make_unique<Material>(screenQuadShader);
		screenQuadMat->DepthTest = false;

		screenQuadMesh = std::make_unique<ScreenQuadMesh>();
	}

	void Renderer::pushRenderCommand(SceneNode* node, Material* customDepthMaterial)
	{
		//If it's not a container node push it to the queue
		if (node->mesh != nullptr)
		{
			Material* depthMatOnRender = customDepthMaterial != nullptr ? customDepthMaterial : depthMat.get();
		
			RenderCommand temp(node->getTransform(), node->mesh.get(), node->mat.get(), depthMatOnRender);
			
			renderCommandBuffer.push_back(temp);
		}		

		for (auto& child : node->getChildren())
		{
			if (child->active)
				pushRenderCommand(child.get());
		}
	}
	
	void Renderer::pushRenderCommand(const Mat4& transform, BaseMesh* mesh, Material* mat, Material* customDepthMaterial)
	{
		//Create a render command and put it on the list
		Material* depthMatOnRender = customDepthMaterial != nullptr ? customDepthMaterial : depthMat.get();

		RenderCommand temp(transform, mesh, mat, depthMatOnRender);
		renderCommandBuffer.push_back(temp);		
	}

	void Renderer::renderPushedCommands()
	{
		/*
		General outline of all the render steps/passes:
		   - Clear default buffer
		   - Sort the render commands to avoid big state changes (like shader program)
		   - 1st pass: fill the gbuffer with information
		   - 2nd pass: create the depth map for the lights
		   - 3rd pass: accumulate the light's contribution to the pixels
		   - 4th pass: render to screen quad applying HDR and gamma correction
	   */
		sort();

		//Send global uniforms here
		updateGlobalUniforms();

		//First pass - deferred fill Gbuffer
		deferredPass();

		//Second pass - Depth 
		lightDepthPass();

		//Third pass: light deferred shading
		deferredLightRender();

		//Render each buffer just to be sure
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/** /
		//Render positions in top left
		unsigned int renderHalfWidth = renderWidth / 2;
		unsigned int renderHalfHeight = renderHeight / 2;

		glViewport(0, renderHalfHeight, renderHalfWidth, renderHalfHeight);
		screenQuadMat->setTexture("screenTexture", gbuffer->textures[0].get(), 0);
		RenderCommand screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), screenQuadMat.get(), nullptr);		
		renderCommand(screenQuad);

		//Normals in top right
		glViewport(renderHalfWidth, renderHalfHeight, renderHalfWidth, renderHalfHeight);
		screenQuadMat->setTexture("screenTexture", gbuffer->textures[1].get(), 0);
		screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), screenQuadMat.get(), nullptr);
		renderCommand(screenQuad);

		//Tex coords lower left
		glViewport(0, 0, renderHalfWidth, renderHalfHeight);
		screenQuadMat->setTexture("screenTexture", gbuffer->textures[2].get(), 0);
		screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), screenQuadMat.get(), nullptr);
		renderCommand(screenQuad);

		/** /
		//Diffuse on lower right
		glViewport(renderHalfWidth, 0, renderHalfWidth, renderHalfHeight);
		screenQuadMat->setTexture("screenTexture", gbuffer->textures[5].get(), 0);
		screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), screenQuadMat.get(), nullptr);
		renderCommand(screenQuad);
		/**/

		/** /
		//LightDepth on lower right
		glViewport(renderHalfWidth, 0, renderHalfWidth, renderHalfHeight);

		auto& rt = std::get<1>(dirLightsT[0]);
		screenQuadMat->setTexture("screenTexture", rt->textures[0].get(), 0);
		screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), screenQuadMat.get(), nullptr);
		renderCommand(screenQuad);
		/**/

		/**/
		//Bind default, clear it and resize viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, renderWidth, renderHeight);
		
		//Final pass: Render HDR quad
		finalHDRMat->setTexture("screenTexture", lightDeferredRT->textures[0].get(), 0);
		RenderCommand screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), finalHDRMat.get(), nullptr);
		
		renderCommand(screenQuad);
		/**/

		renderCommandBuffer.clear();
	}

	void Renderer::setClearColor(const Vec3& color)
	{
		this->clearColor = color;
	}

	void Renderer::addDirLight(const DirectionalLight& dirLight)
	{
		//If we already have 4 lights then delete the first one and place the new light at the end
		if (dirLightsT.size() >= MAX_DIR_LIGHTS)
		{
			//TODO: Destruct the framebuffer on the destructor so we don't leak stuff
			dirLightsT.erase(dirLightsT.begin());
		}
		
		auto rt = std::make_unique<RenderTarget>(DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT, true);

		dirLightsT.emplace_back(dirLight, std::move(rt),
			MatrixFactory::identityMat4());
	}

	void Renderer::addPointLight(const PointLight& pointLight)
	{
		//If we already have 8 lights then delete the first one and place the new light at the end
		if (pointLights.size() >= MAX_POINT_LIGHTS)
		{
			pointLights.erase(pointLights.begin());
		}

		pointLights.push_back(pointLight);
	}

	PointLight& Renderer::getPointLight(int index)
	{
		return pointLights[index];
	}

	bool sortShader(const RenderCommand& a, const RenderCommand& b)
	{
		return a.mat->getShader()->id < b.mat->getShader()->id;
	}

	void Renderer::sort()
	{
		std::sort(renderCommandBuffer.begin(), renderCommandBuffer.end(), sortShader);
	}

	void Renderer::renderCommand(const RenderCommand& rc)
	{
		//Bind shader
		auto comShader = rc.mat->getShader();
		comShader->use();

		auto comMat = rc.mat;

		//Change GLState based on material		
		updateGLState(comMat);

		//Set transform uniform
		comMat->setUniformMatrix4("transform", rc.transform);

		//Send any other material specific uniforms
		comMat->sendUniforms();
		
		renderMesh(rc.mesh);
	}

	void Renderer::renderMesh(BaseMesh* mesh)
	{
		//Bind VAO
		glBindVertexArray(mesh->getVAOID());

		//If we have indices then use them, otherwise revert to no indices
		//TODO: Maybe change topology if specified?
		if (mesh->getSizeIndices() > 0)
		{
			glDrawElements(GL_TRIANGLES, (GLsizei)mesh->getSizeIndices(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->getNumberVerts());
		}
	}

	void Renderer::updateGlobalUniforms()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, guboID);

		int currentOffset = 0;
		
		//View
		currentOffset += sendMatrixToGlobalUniform(currentOffset, activeCamera->getViewMat());
		
		//Projection
		currentOffset += sendMatrixToGlobalUniform(currentOffset, activeCamera->getProjMat());

		//Camera Pos
		currentOffset += sendVec3ToGlobalUniform(currentOffset, activeCamera->pos);

		//Directional ligts
		for (int i = 0; i < MAX_DIR_LIGHTS; i++)
		{
			if (i < dirLights.size())
			{
				//Send the lights
				DirectionalLight& it = dirLights[i];

				//Direction
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.direction);

				//Ambient
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.ambient);

				//Diffuse
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.diffuse);

				//Specular
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.specular);

				//No padding
			}
			else
			{
				//No more lights to send, pad data out
				currentOffset += DIR_LIGHT_STRUCT_SIZE;
			}
		}	

		//PointLights
		for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			if (i < pointLights.size())
			{
				//Send the lights
				PointLight& it = pointLights[i];

				//Position
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.position);

				//Ambient
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.ambient);

				//Diffuse
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.diffuse);

				//Specular
				currentOffset += sendVec3ToGlobalUniform(currentOffset, it.specular);

				//Linear factor
				currentOffset += sendFloatToGlobalUniform(currentOffset, it.linearFac);

				//Quadratic factor
				currentOffset += sendFloatToGlobalUniform(currentOffset, it.quadraticFac);

				//Needs 2 more floats of padding because structs are aligned at 16 bytes
				currentOffset += 4 * 2; //padding
			}
			else
			{
				//Pad data
				currentOffset += POINT_LIGHT_STRUCT_SIZE;
			}
		}
	}

	int Renderer::sendMatrixToGlobalUniform(int currentOffset, const Mat4& m)
	{
		int sizeOfMat4 = 64;

		glBufferSubData(GL_UNIFORM_BUFFER, currentOffset, sizeOfMat4, m.transpose().flatten().data());

		return sizeOfMat4;
	}

	int Renderer::sendVec3ToGlobalUniform(int currentOffset, const Vec3& v)
	{
		//Vec3 are aligned to vec4 so
		int sizeOfVec4 = 16;
		
		std::vector<GLfloat> data = { (GLfloat)v.x,
									(GLfloat)v.y,
									(GLfloat)v.z,
									0 };

		glBufferSubData(GL_UNIFORM_BUFFER, currentOffset, sizeOfVec4, data.data());

		return sizeOfVec4;
	}

	int Renderer::sendFloatToGlobalUniform(int currentOffset, const float& v)
	{
		int sizeOfFloat = 4;

		glBufferSubData(GL_UNIFORM_BUFFER, currentOffset, sizeOfFloat, &v);

		return sizeOfFloat;
	}

	void Renderer::updateGLState(Material* mat)
	{
		//TODO: This is unfinished

		//Depth test
		if (mat->DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}			
		else
		{
			glDisable(GL_DEPTH_TEST);
		}			

		//Cull face
		if (mat->Cull)
		{
			glCullFace(mat->CullFace);
		}

		//Blend stuff
		if (mat->Blend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(mat->BlendSrc, mat->BlendDst);
			glBlendEquation(mat->BlendEquation);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}
	
	void Renderer::renderShadowCommand(const RenderCommand& rc, const Mat4 & lightProjView)
	{
		//Shadow material
		auto comDepthMat = rc.shadowMaterial;

		auto comDepthShader = comDepthMat->getShader();
		comDepthShader->use();

		//Change GLState based on material		
		updateGLState(comDepthMat);

		//Set transform uniform
		comDepthMat->setUniformMatrix4("transform", rc.transform);

		//Set lightViewProj
		comDepthMat->setUniformMatrix4("lightProjView", lightProjView);

		//Send any other material specific uniforms
		comDepthMat->sendUniforms();

		renderMesh(rc.mesh);
	}

	void Renderer::deferredPass()
	{
		//First pass, deferred pass
		//Bind gbuffer
		gbuffer->bind();
		glViewport(0, 0, gbuffer->getWidth(), gbuffer->getHeight());
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const GLenum attachments[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
										GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
		glDrawBuffers(6, attachments);

		//Render into Gbuffer
		for (unsigned int i = 0; i < renderCommandBuffer.size(); ++i)
		{
			renderCommand(renderCommandBuffer[i]);
		}
	}

	void Renderer::lightDepthPass()
	{
		//Directional lights
		for (unsigned int i = 0; i < dirLightsT.size(); ++i)
		{
			auto& tuple = dirLightsT[i];

			//Bind the light's render target
			auto& rt = std::get<1>(tuple);

			rt->bind();
			glClear(GL_DEPTH_BUFFER_BIT);
			//Resize the viewport
			glViewport(0, 0, rt->getWidth(), rt->getHeight());
			
			DirectionalLight& light = std::get<0>(tuple);

			//Create light proj view matrix
			//TODO: There are smarter ways to create the matrices but whatever
			//But this works for now so idc

			Mat4 lightProj = MatrixFactory::ortho(-2.0f, 2.0f, 2.0f, -2.0f, 1.0f, 10.0f);

			Vec3 lightPos = activeCamera->pos  + -light.direction.normalize() * 2;

			Mat4 lightView = MatrixFactory::viewMatrix(lightPos, activeCamera->pos, Vec3::yAxis());

			Mat4 lightProjView = lightProj * lightView;

			//Change lightProjView matrix on tuple
			std::get<2>(tuple) = lightProjView;

			for(int j = 0; j < renderCommandBuffer.size(); j++)
				renderShadowCommand(renderCommandBuffer[j], lightProjView);
		}

		//TODO: Point lights would go here but no point lights shadows :)
	}

	void Renderer::deferredLightRender()
	{
		//Bind the deferred light pass RT
		lightDeferredRT->bind();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, lightDeferredRT->getWidth(), lightDeferredRT->getHeight());

		for (int i = 0; i < dirLightsT.size(); i++)
		{
			//Send gbuffer textures
			//1 - posTex;
			deferredDirLight->setTexture("posTex", gbuffer->textures[0].get(), 0);

			//2 - normalTex;
			deferredDirLight->setTexture("normalTex", gbuffer->textures[1].get(), 1);

			//3 - colorTex;
			deferredDirLight->setTexture("colorTex", gbuffer->textures[2].get(), 2);

			//3 - matAmbTex;
			deferredDirLight->setTexture("matAmbTex", gbuffer->textures[3].get(), 3);

			//4 - matDiffTex;
			deferredDirLight->setTexture("matDiffTex", gbuffer->textures[4].get(), 4);

			//5 - matSpecShinyTex;
			deferredDirLight->setTexture("matSpecShinyTex", gbuffer->textures[5].get(), 5);

			//Now send light stuff
			auto& tuple = dirLightsT[i];
			DirectionalLight& dirLight = std::get<0>(tuple);

			//6 - Depth map
			deferredDirLight->setTexture("depthMap", std::get<1>(tuple)->textures[0].get(), 6);

			//7 - lightDir
			deferredDirLight->setUniformVec3("lightDir", dirLight.direction);

			//8 - lightProjView
			deferredDirLight->setUniformMatrix4("lightProjView", std::get<2>(tuple));

			//9 - lightAmb;
			deferredDirLight->setUniformVec3("lightAmb", dirLight.ambient);

			//10 - lightDiff;
			deferredDirLight->setUniformVec3("lightDiff", dirLight.diffuse);

			//11 - lightSpec;
			deferredDirLight->setUniformVec3("lightSpec", dirLight.specular);

			RenderCommand screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), deferredDirLight.get(), nullptr);
			renderCommand(screenQuad);
		}
		
		//Point Lights
		for (int i = 0; i < pointLights.size(); i++)
		{
			//Send gbuffer textures
			//1 - posTex;
			deferredPointLight->setTexture("posTex", gbuffer->textures[0].get(), 0);

			//2 - normalTex;
			deferredPointLight->setTexture("normalTex", gbuffer->textures[1].get(), 1);

			//3 - colorTex;
			deferredPointLight->setTexture("colorTex", gbuffer->textures[2].get(), 2);

			//3 - matAmbTex;
			deferredPointLight->setTexture("matAmbTex", gbuffer->textures[3].get(), 3);

			//4 - matDiffTex;
			deferredPointLight->setTexture("matDiffTex", gbuffer->textures[4].get(), 4);

			//5 - matSpecShinyTex;
			deferredPointLight->setTexture("matSpecShinyTex", gbuffer->textures[5].get(), 5);

			//Light stuff
			PointLight& pointLight = pointLights[i];

			//6 - radius
			//TODO: Cache this or use the light radius. idk?
			float lightMax = std::fmaxf(std::fmaxf(pointLight.diffuse.x, pointLight.diffuse.y), pointLight.diffuse.z);
			float radius =
				(-pointLight.linearFac + 
					std::sqrtf(pointLight.linearFac * pointLight.linearFac - 4 * pointLight.quadraticFac * (1 - (256.0f / 5.0f) * lightMax)))
				/ (2 * pointLight.quadraticFac);

			deferredPointLight->setUniformFloat("radius", radius);

			//7 - lightPos
			deferredPointLight->setUniformVec3("lightPos", pointLight.position);

			//8 - lightAmb;
			deferredPointLight->setUniformVec3("lightAmb", pointLight.ambient);

			//9 - lightDiff;
			deferredPointLight->setUniformVec3("lightDiff", pointLight.diffuse);

			//10 - lightSpec;
			deferredPointLight->setUniformVec3("lightSpec", pointLight.specular);

			RenderCommand screenQuad = RenderCommand(Mat4(), screenQuadMesh.get(), deferredPointLight.get(), nullptr);
			renderCommand(screenQuad);
		}
	}
}