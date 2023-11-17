#pragma once
#include <vector>
#include "Scene.h"
#include "Shader.h"
#include "VAOManager.h"

#include "World/Chunk.h"
#include "World/ChunkManager.h"

#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"

#include "Objects/StandardObject.h"
#include "Objects/StandardCube.h"
#include "Objects/StandardPlane.h"

#include "Textures/TextureManager.h"

class MyScene : public Scene {
public:
	MyScene(GLFWwindow* window, InputHandler* H);
	~MyScene();

	void update(float dt) override;
private:
	// Shaders
	Shader* m_chunkShader;
	Shader* m_stdObjectShader;
	Shader* m_skyboxShader;

	// Camera properties
	glm::mat4 m_view, m_projection;
	glm::vec3 m_cameraPos;

	// Renderer and chunk manager
	VAOManager m_VAOManager;
	ChunkManager m_chunkManager;
#pragma region Lights
	// Directional Lights
	//																colour,				direction,					ambient
	DirectionalLight m_directionalLight = DirectionalLight(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.3f, -1.0f, 0.6f), 0.2f);

	// Point Lights
	int m_numPointLights = 5;
	std::vector<PointLight> m_pointLights = {
		//					colour,			   position,						constants
		PointLight(glm::vec3(0.7f, 0.9f, 0.0f), glm::vec3(16.0f, 73.0f, 20.0f), glm::vec3(1.0f, 0.027f, 0.0028f)),
		PointLight(glm::vec3(0.7f, 0.9f, 0.0f), glm::vec3(16.0f, 73.0f, 40.0f), glm::vec3(1.0f, 0.027f, 0.0028f)),
		PointLight(glm::vec3(0.7f, 0.9f, 0.0f), glm::vec3(16.0f, 73.0f, 60.0f), glm::vec3(1.0f, 0.027f, 0.0028f)),
		PointLight(glm::vec3(0.7f, 0.9f, 0.0f), glm::vec3(16.0f, 73.0f, 80.0f), glm::vec3(1.0f, 0.027f, 0.0028f)),
		PointLight(glm::vec3(0.7f, 0.9f, 0.0f), glm::vec3(16.0f, 73.0f, 100.0f), glm::vec3(1.0f, 0.027f, 0.0028f))
	};

	// Spot Lights
	int m_numSpotLights = 5;
	std::vector<SpotLight> m_spotLights = {
		//					colour,			   position,						constants,						direction,						radii
		SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(36.0f, 80.0f, 20.0f), glm::vec3(1.0f, 0.027f, 0.0028f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(30.5f)))),
		SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(36.0f, 80.0f, 40.0f), glm::vec3(1.0f, 0.027f, 0.0028f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(30.5f)))),
		SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(36.0f, 80.0f, 60.0f), glm::vec3(1.0f, 0.027f, 0.0028f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(30.5f)))),
		SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(36.0f, 80.0f, 80.0f), glm::vec3(1.0f, 0.027f, 0.0028f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(30.5f)))),
		SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(36.0f, 80.0f, 100.0f), glm::vec3(1.0f, 0.027f, 0.0028f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(30.5f))))
	};

	//											colour,			   position,						constants,						direction,						radii
	SpotLight m_flashlight = SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.027f, 0.0028f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(glm::cos(glm::radians(15.5f)), glm::cos(glm::radians(20.5f))));
#pragma endregion

# pragma region Textures
	// load chunk atlas texture, the false will disable linear interpolation (enables nearest neighbour)
	unsigned int m_chunkTextureAtlas = TextureManager::loadTexture("..\\Resources\\borrowed_atlas.png", false);

	// load cube & plane textures
	unsigned int m_cubeDiffuseTexture = TextureManager::loadTexture("..\\Resources\\cw_textures\\diffuseCube.jpg");
	unsigned int m_cubeSpecTexture = TextureManager::loadTexture("..\\Resources\\cw_textures\\specularCube.jpg");
	unsigned int m_cubeNormTexture = TextureManager::loadTexture("..\\Resources\\cw_textures\\normalCube.jpg");

	unsigned int m_planeDiffuseTexture = TextureManager::loadTexture("..\\Resources\\cw_textures\\diffuseFloor.jpg");
	unsigned int m_planeSpecTexture = TextureManager::loadTexture("..\\Resources\\cw_textures\\specularFloor.jpg");
	unsigned int m_planeNormTexture = TextureManager::loadTexture("..\\Resources\\cw_textures\\normalFloor.jpg");

	// load cube map texture
	std::string m_cubeMapTexturePaths[6] = {
		"..\\Resources\\cube_map\\px.png",
		"..\\Resources\\cube_map\\nx.png",
		"..\\Resources\\cube_map\\py.png",
		"..\\Resources\\cube_map\\ny.png",
		"..\\Resources\\cube_map\\pz.png",
		"..\\Resources\\cube_map\\nz.png"
	};
	unsigned int m_cubeMapTexture = TextureManager::loadCubeMapTexture(m_cubeMapTexturePaths);
# pragma endregion

	// array of the standard mesh objects
	std::vector<StandardObject*> m_standardObjects = {
		//						position,         specShine, textures
		new StandardCube(glm::vec3(0.0f, 77.0f, 2.0f), 1024, m_cubeDiffuseTexture, m_cubeSpecTexture, m_cubeNormTexture),
		new StandardCube(glm::vec3(0.0f, 74.0f, 2.0f), 1024, m_cubeDiffuseTexture, m_cubeSpecTexture, m_cubeNormTexture),
		new StandardCube(glm::vec3(5.0f, 74.0f, 4.0f), 1024, m_cubeDiffuseTexture, m_cubeSpecTexture, m_cubeNormTexture),
		new StandardPlane(glm::vec3(0.0f, 73.5f, 0.0f), 512, m_planeDiffuseTexture, m_planeSpecTexture, m_planeNormTexture),

		//							position,			 colour					specStrength specShine
		new StandardCube(glm::vec3(-5.0f, 74.0f, -2.0f), glm::vec3(0.7f, 0.2f, 0.6f), 0.5f, 4096)
	};

	// cooridnates of chunk player is in
	int m_chunkPosX;
	int m_chunkPosY;

	// load the light uniforms
	void loadLights(Shader* shader);

	// update uniforms for flashlight
	void updateFlashLight(Shader* shader);

	// listen for keyboard
	void updateInputs();

	// assign the initial uniforms (Texture units and material propety)
	void assignUniforms();

	// update uniforms (View & Projection matrices)
	void updateUniforms();

	void render();
};