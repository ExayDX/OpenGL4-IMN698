#include "SSSSTestLevel.h"

#include "Sphere.h"
#include "Light.h"
#include "Quad.h"
#include <iostream>
#include <string>
#include "ModelLoader.h"
#include "FrameBuffer.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

SSSSTestLevel::SSSSTestLevel()
	: Scene()
{

}


// LOOP
void SSSSTestLevel::draw(int currentFrame)
{
	FrameBuffer* mainFBO = m_frameBuffers["mainFBO"];
	mainFBO->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glActiveTexture(GL_TEXTURE1); // To assure that the second buffer of mainFBO be cleared

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for each (Object* obj in m_objects)
	{
		if (!obj->isVisible())
			continue;

		GLuint shaderProgramID = obj->getShaderProgramId();
		glUseProgram(shaderProgramID);

		// Compute/Get information to pass to uniforms
		glm::mat4 modelMatrix = obj->getModelMatrix(currentFrame);
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_viewMatrix * modelMatrix)));
		const Material* objectMaterial = obj->getMaterial();

		// Get Uniforms location
		// Display matrixes
		GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
		GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
		GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");
		GLuint normalMatrixLoc = glGetUniformLocation(shaderProgramID, "normalMatrix");
		GLuint nbLightLoc = glGetUniformLocation(shaderProgramID, "nbLights");

		// Object information
		GLuint objectColorLoc = glGetUniformLocation(shaderProgramID, "objectColor");
		GLuint objectAmbientLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.ambientCoefs");
		GLuint objectDiffuseLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.diffuseCoefs");
		GLuint objectSpecularLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.specularCoefs");
		GLuint objectShininessLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.shininess");

		// Brightness threshold
		GLuint brightnessThresholdLoc = glGetUniformLocation(shaderProgramID, "brightnessThreshold"); 

		// Display Matrixes 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniform1f(nbLightLoc, m_lights.size());

		// Object information 
		glUniform3f(objectAmbientLoc, objectMaterial->m_ambientCoefs.x, objectMaterial->m_ambientCoefs.y, objectMaterial->m_ambientCoefs.z);
		glUniform3f(objectDiffuseLoc, objectMaterial->m_diffuseCoefs.x, objectMaterial->m_diffuseCoefs.y, objectMaterial->m_diffuseCoefs.z);
		glUniform3f(objectSpecularLoc, objectMaterial->m_specularCoefs.x, objectMaterial->m_specularCoefs.y, objectMaterial->m_specularCoefs.z);
		glUniform1f(objectShininessLoc, objectMaterial->m_shininess);

		// Brightness threshold
		glUniform3f(brightnessThresholdLoc, 0.7f, 0.7f, 0.7f); 

		// Lights information
		for (int i = 0; i < m_lights.size(); ++i)
		{
			glm::vec3 lightPosition = m_lights[i]->getPosition();
			const Material* lightMaterial = m_lights[i]->getMaterial();
			Light::AttenuationProperties lightProperties = m_lights[i]->getAttenuationProperties();

			GLuint lightPositionLoc  = glGetUniformLocation(shaderProgramID, ("lightsPositions["  + std::to_string(i) + "]").c_str());
			GLuint lightAmbientLoc   = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].material.ambientCoefs").c_str());
			GLuint lightDiffuseLoc   = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].material.diffuseCoefs").c_str());
			GLuint lightSpecularLoc  = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].material.specularCoefs").c_str());
			GLuint lightShininessLoc = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].material.shininess").c_str());
			GLuint lightAttConstant	 = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].constant").c_str());
			GLuint lightAttLinear	 = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].linear").c_str());
			GLuint lightAttQuadratic = glGetUniformLocation(shaderProgramID, ("lightsProperties[" + std::to_string(i) + "].quadratic").c_str());

			glUniform3f(lightPositionLoc,  lightPosition.x,                  lightPosition.y,                  lightPosition.z);
			glUniform3f(lightAmbientLoc,   lightMaterial->m_ambientCoefs.x,  lightMaterial->m_ambientCoefs.y,  lightMaterial->m_ambientCoefs.z);
			glUniform3f(lightDiffuseLoc,   lightMaterial->m_diffuseCoefs.x,  lightMaterial->m_diffuseCoefs.y,  lightMaterial->m_diffuseCoefs.z);
			glUniform3f(lightSpecularLoc,  lightMaterial->m_specularCoefs.x, lightMaterial->m_specularCoefs.y, lightMaterial->m_specularCoefs.z);
			glUniform1f(lightShininessLoc, lightMaterial->m_shininess);
			glUniform1f(lightAttConstant,  lightProperties.m_constant);
			glUniform1f(lightAttLinear,    lightProperties.m_linear);
			glUniform1f(lightAttQuadratic, lightProperties.m_quadratic);
		}

		obj->draw();
	}

	mainFBO->unBind();

	// Post Processes 
	GLboolean bloom = true; 

	// Initialize buffers 
	GLuint mainBuffer, finalBuffer; 
	mainBuffer = mainFBO->getRenderBuffer(FrameBuffer::BufferType::eColor, 0);
	finalBuffer = mainFBO->getRenderBuffer(FrameBuffer::BufferType::eColor, 1);

	if (bloom)
	{
		// Set inital values
		GLboolean horizontal = true;
		GLuint BlurProg = m_shaderPrograms["Blur"]->getId(); // Gaussian Blur PP.

		FrameBuffer* ppFBOs[2] = { m_frameBuffers["ppFBO0"], m_frameBuffers["ppFBO1"] };
		GLuint ppTex[2] = { ppFBOs[0]->getRenderBuffer(FrameBuffer::BufferType::eColor, 0),
							ppFBOs[1]->getRenderBuffer(FrameBuffer::BufferType::eColor, 0) };

		GLuint amount = 10;
		glUseProgram(BlurProg);
	
		for (GLuint i = 0; i < amount; ++i)
		{
			if (i != 0)
				finalBuffer = ppTex[!horizontal];

			glBindFramebuffer(GL_FRAMEBUFFER, ppFBOs[horizontal]->getId());
			glUniform1i(glGetUniformLocation(BlurProg, "horizontal"), horizontal);
			glActiveTexture(GL_TEXTURE0); 
			glBindTexture(GL_TEXTURE_2D, finalBuffer );  // bind texture of other framebuffer (or scene if first iteration)
			m_renderQuad->draw(); 
			horizontal = !horizontal;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	}
	
	//  ...
	GLuint BloomHDRProg = m_shaderPrograms["BloomHDR"]->getId(); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(BloomHDRProg); 
	glUniform1i(glGetUniformLocation(BloomHDRProg, "scene"), 0);
	glUniform1i(glGetUniformLocation(BloomHDRProg, "bloomBlur"), 1);
	glUniform1i(glGetUniformLocation(BloomHDRProg, "exposure"), bloom);
	glUniform1f(glGetUniformLocation(BloomHDRProg, "exposure"), 1.5);
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, mainBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, finalBuffer);
	m_renderQuad->draw();
}

void SSSSTestLevel::createShaderPrograms()
{
	// Create Shader programs
	// -- Render Shaders
	ShaderProgram* BlinnPhongProgram = new ShaderProgram("BlinnPhong.vs", "BlinnPhong.fg");
	ShaderProgram* BumpColorMapsProgram = new ShaderProgram("BumpColorMaps.vs", "BumpColorMaps.fg");

	// -- PostProcess Shaders
	ShaderProgram* BlurProgram = new ShaderProgram("basicPPVS.vs", "GaussianBlur.fg");
	ShaderProgram* SSSProgram = new ShaderProgram("basicPPVS.vs", "SSS.fg");
	ShaderProgram* BloomHDRProgram = new ShaderProgram("basicPPVS.vs", "BloomHDR.fg");
	ShaderProgram* HDRProgram = new ShaderProgram("basicPPVS.vs", "hdr.fg");
	ShaderProgram* DebugProgram = new ShaderProgram("Debug.vs", "Debug.fg"); 

	// Insert ShaderProgram in the list
	// -- Render Shaders
	m_shaderPrograms["BlinnPhong"] =  BlinnPhongProgram;
	m_shaderPrograms["BumpColorMaps"] = BumpColorMapsProgram;

	// -- PostProcess Shaders
	m_shaderPrograms["Blur"] =  BlurProgram;
	m_shaderPrograms["SSS"] = SSSProgram;
	m_shaderPrograms["BloomHDR"] = BloomHDRProgram;
	m_shaderPrograms["HDR"] = HDRProgram; 

	m_shaderPrograms["Debug"] = DebugProgram; 
}

void SSSSTestLevel::createMaterials()
{
	Material* defaultMaterial = new Material(glm::vec3(0.2f, 0.1f, 0.05f),
		glm::vec3(0.8f, 0.4f, 0.31f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f);

	Material* blueMaterial = new Material(glm::vec3(0.0f, 0.1f, 0.06f),
		glm::vec3(0.0f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		128.0f);

	Material* orangeMaterial = new Material(glm::vec3(0.15f, 0.1f, 0.0f),
		glm::vec3(0.8f, 0.6f, 0.0f),
		glm::vec3(0.6f, 0.6f, 0.5f),
		32);

	Material* defaultLightMaterial = new Material(glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		1.0f);

	m_materials.insert(std::pair<std::string, Material*>("default", defaultMaterial));
	m_materials.insert(std::pair<std::string, Material*>("blue", blueMaterial));
	m_materials.insert(std::pair<std::string, Material*>("orange", orangeMaterial));
	m_materials.insert(std::pair<std::string, Material*>("defaultLight", defaultLightMaterial));
}

void SSSSTestLevel::levelSetup()
{
	Object* sphere1 = new Sphere(glm::vec3(-7, 0, 0), m_materials["default"], 2, 40, 40, m_shaderPrograms["BlinnPhong"]->getId());
	sphere1->setVisible(true);
	m_objects.push_back(sphere1);
	
	Object* sphere2 = new Sphere(glm::vec3(0, 0, 0), m_materials["orange"], 2, 40, 40, m_shaderPrograms["BlinnPhong"]->getId());
	sphere2->setVisible(true);
	m_objects.push_back(sphere2);
	
	Object* sphere3 = new Sphere(glm::vec3(7, 0, 0), m_materials["blue"], 2, 40, 40, m_shaderPrograms["BlinnPhong"]->getId());
	sphere3->setVisible(true);
	sphere3->addPostProcess(m_shaderPrograms["SSS"]->getId()); 
	m_objects.push_back(sphere3);

	ModelContainer* model1 = ModelLoader::loadModel("./HeadModel/head_tri_non_smooth.obj", m_materials["default"], m_shaderPrograms["BlinnPhong"]->getId(), std::vector<Vec3>());
	model1->smoothNormals();
	m_objects.push_back(model1);

	m_renderQuad = new Quad(glm::vec3(0.0f), nullptr, 0); 
}

void SSSSTestLevel::lightSetup()
{
	Light::AttenuationProperties attenuationProp;
	attenuationProp.m_constant = 1.0f;
	attenuationProp.m_linear = 0.02f;
	attenuationProp.m_quadratic = 0.0005f;

	Light* light1 = new Light(glm::vec3(0, 10, 0), m_materials["defaultLight"], attenuationProp, m_shaderPrograms["BlinnPhong"]->getId());
	m_lights.push_back(light1);

	Light* light2 = new Light(glm::vec3(-30, 40, 0), m_materials["defaultLight"], attenuationProp, m_shaderPrograms["BlinnPhong"]->getId());
	m_lights.push_back(light2);
}

void SSSSTestLevel::buffersSetup()
{
	// Create and Frame buffers
	FrameBuffer* mainFBO = new FrameBuffer();
	FrameBuffer* ppFBO0 = new FrameBuffer(); // ping pong FrameBuffer
	FrameBuffer* ppFBO1 = new FrameBuffer(); // ping pong FrameBuffer

	// Define colorBuffers parameters 
	FrameBuffer::ParameterMap colorBufferParam;
	colorBufferParam[GL_TEXTURE_MIN_FILTER] = GL_LINEAR;
	colorBufferParam[GL_TEXTURE_MAG_FILTER] = GL_LINEAR;
	colorBufferParam[GL_TEXTURE_WRAP_S] = GL_CLAMP_TO_EDGE;
	colorBufferParam[GL_TEXTURE_WRAP_T] = GL_CLAMP_TO_EDGE;

	// Create FBO0 RenderBuffers
	mainFBO->bind();
	{
		mainFBO->addBuffer(FrameBuffer::BufferType::eColor, 0, &colorBufferParam); // Main rendering
		mainFBO->addBuffer(FrameBuffer::BufferType::eColor, 1, &colorBufferParam); // Brightness treshold value (for bloom and tone mapping)
		mainFBO->addBuffer(FrameBuffer::BufferType::eDepth, 0); 
		//mainFBO->addBuffer(FrameBuffer::BufferType::eStencil, 0); // Subsurface scattering stencil buffer

		//// Attachments
		GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, attachments); 

		// -- Check if Frame buffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	mainFBO->unBind(); 

	// Create PingPong FrameBuffers
	ppFBO0->bind();
	{
		ppFBO0->addBuffer(FrameBuffer::BufferType::eColor, 0, &colorBufferParam);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	
	}
	ppFBO0->unBind();
	ppFBO1->bind();
	{
		ppFBO1->addBuffer(FrameBuffer::BufferType::eColor, 0, &colorBufferParam);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	ppFBO1->unBind(); 

	// Add all framebuffers 
	m_frameBuffers["mainFBO"] = mainFBO;
	m_frameBuffers["ppFBO0"] = ppFBO0;
	m_frameBuffers["ppFBO1"] = ppFBO1; 
}