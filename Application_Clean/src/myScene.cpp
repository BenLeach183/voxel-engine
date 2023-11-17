#pragma once
#include "myScene.h"

MyScene::~MyScene() {
	// delete all standard objects
	for (StandardObject* obj : m_standardObjects) { delete obj; }

	// delete the shaders
	delete m_chunkShader;
	delete m_stdObjectShader;
	delete m_skyboxShader;
}

MyScene::MyScene(GLFWwindow* window, InputHandler* H) : Scene(window, H), m_chunkManager() {
	m_camera = new FirstPersonCamera();
	m_camera->attachHandler(m_window, m_handler);

	// enable back face culling
	glEnable(GL_CULL_FACE);

	// load and compile shaders
	m_chunkShader = new Shader("..\\shaders\\chunk.vs", "..\\shaders\\chunk.fs");
	m_stdObjectShader = new Shader("..\\shaders\\std_object.vs", "..\\shaders\\std_object.fs");
	m_skyboxShader = new Shader("..\\shaders\\skybox.vs", "..\\shaders\\skybox.fs");

	// get reference to the chunk array and width from the chunk manager
	std::vector<Chunk>* chunks = m_chunkManager.getChunkArray();
	unsigned char* chunksWidth = m_chunkManager.getChunksWidth();

	// send the chunk and standard cubes array reference to the VAO manager
	m_VAOManager.initializeChunkArray(chunks);
	m_VAOManager.initializeCubeArray(&m_standardObjects);

	// start generating terrain centred around 0, 0
	m_chunkManager.loadChunksAt(0, 0);

	// create the VAO's
	m_VAOManager.makeChunkVAO();
	m_VAOManager.makeStdVAO();
	m_VAOManager.makeCubeMapVAO();

	// load light values into the shader
	loadLights(m_chunkShader);
	loadLights(m_stdObjectShader);

	// set static uniforms
	assignUniforms();
}

void MyScene::update(float dt) {
	m_camera->update(dt);

	// get the camera properties
	m_cameraPos = m_camera->getPosition();
	m_view = m_camera->getViewMatrix();
	m_projection = m_camera->getProjectionMatrix();

	// get chunk player is in
	m_chunkPosX = floor(m_cameraPos.x / 16.0f);
	m_chunkPosY = floor(m_cameraPos.z / 16.0f);

	// update flashlight position
	updateFlashLight(m_chunkShader);
	updateFlashLight(m_stdObjectShader);

	// rotate cubes around Y-Axis over time
	(m_standardObjects[0])->setRotation((float)(glfwGetTime() * 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	(m_standardObjects[4])->setRotation((float)(glfwGetTime() * 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

	// test for key inputs
	updateInputs();

	// run update loop on chunk manager
	m_chunkManager.update(m_chunkPosX, m_chunkPosY);

	render();
}

void MyScene::render() {
	// update the camera uniforms that change each frame
	updateUniforms();
	
	// draw the chunks
	m_VAOManager.drawChunks(m_chunkShader, m_chunkTextureAtlas);

	// draw std objects
	m_VAOManager.drawStdObjects(m_stdObjectShader);

	// draw the cube map
	m_VAOManager.drawCubeMap(m_skyboxShader, m_cubeMapTexture);	
}

void MyScene::updateUniforms() {
	m_chunkShader->use();
	// Camera uniforms
	m_chunkShader->setMat4("View", m_view);
	m_chunkShader->setMat4("Projection", m_projection);
	m_chunkShader->setVec3("viewPos", m_cameraPos);

	m_stdObjectShader->use();
	// Camera uniforms
	m_stdObjectShader->setMat4("View", m_view);
	m_stdObjectShader->setMat4("Projection", m_projection);
	m_stdObjectShader->setVec3("viewPos", m_cameraPos);

	m_skyboxShader->use();
	// Camera uniforms
	m_skyboxShader->setMat4("View", glm::mat4(glm::mat3(m_view)));
	m_skyboxShader->setMat4("Projection", m_projection);
}

void MyScene::assignUniforms() {
	m_chunkShader->use();
	// assign the object material uniforms for the chunks
	m_chunkShader->setFloat("specularStrength", 0.2f);
	m_chunkShader->setInt("specularShine", 64);

	// assign texture units
	m_chunkShader->setInt("textureMap", 0);

	m_stdObjectShader->use();
	m_stdObjectShader->setInt("diffuseTexture", 0);
	m_stdObjectShader->setInt("specularTexture", 1);
	m_stdObjectShader->setInt("normalTexture", 2);

	m_skyboxShader->use();
	m_skyboxShader->setInt("skybox", 0);
}

void MyScene::updateInputs() {
	// test for keyboard input
	if (m_handler->keyHasBeenPressed()) {
		// "Q" toggle normal maps with
		if (m_handler->isKeyPressed(GLFW_KEY_Q)) {
			m_VAOManager.toggleNormalMaps();
			m_handler->processedKey(GLFW_KEY_Q);
		}

		// "E" toggle flashlight
		if (m_handler->isKeyPressed(GLFW_KEY_E)) {
			m_flashlight.toggleEnabled();
			m_handler->processedKey(GLFW_KEY_E);
		}

		// "R" toggle directional light
		if (m_handler->isKeyPressed(GLFW_KEY_R)) {
			m_directionalLight.toggleEnabled();
			m_directionalLight.setLightUniforms(m_chunkShader);
			m_directionalLight.setLightUniforms(m_stdObjectShader);
			m_handler->processedKey(GLFW_KEY_R);
		}

		// "T" toggle point lights
		if (m_handler->isKeyPressed(GLFW_KEY_T)) {
			for (int i = 0; i < m_numPointLights; i++)
			{
				m_pointLights[i].toggleEnabled();
				m_pointLights[i].setLightUniforms(m_chunkShader, i);
				m_pointLights[i].setLightUniforms(m_stdObjectShader, i);
			}
			m_handler->processedKey(GLFW_KEY_T);
		}

		// "Y" toggle spot lights
		if (m_handler->isKeyPressed(GLFW_KEY_Y)) {
			for (int i = 1; i <= m_numSpotLights; i++)
			{
				m_spotLights[i - 1].toggleEnabled();
				m_spotLights[i - 1].setLightUniforms(m_chunkShader, i);
				m_spotLights[i - 1].setLightUniforms(m_stdObjectShader, i);
			}
			m_handler->processedKey(GLFW_KEY_Y);
		}
	}
}

void MyScene::loadLights(Shader* shader) {
	// set directional light uniforms
	m_directionalLight.setLightUniforms(shader);

	// set point light uniforms
	for (int i = 0; i < m_numPointLights; i++)
	{
		m_pointLights[i].setLightUniforms(shader, i);
	}

	// set spot light uniforms
	for (int i = 1; i <= m_numSpotLights; i++)
	{
		m_spotLights[i - 1].setLightUniforms(shader, i);
	}

	// set flash light position and uniform
	m_flashlight.setPosition(m_cameraPos);
	m_flashlight.setDirection(m_camera->getFront());
	m_flashlight.setLightUniforms(shader, 0);
}

void MyScene::updateFlashLight(Shader* shader) {
	// update flashlight position
	m_flashlight.setPosition(m_cameraPos);
	m_flashlight.setDirection(m_camera->getFront());
	m_flashlight.setLightUniforms(shader, 0);
}