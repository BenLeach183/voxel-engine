#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Objects/StandardObjectsData.h"

	/*
		Class to store data on any
		standard object, standard objects
		have 4 float vertex attributes
		position, normals, uv, tangent
	*/

class StandardObject {
public:
	// constructor for just setting position and specular shine
	StandardObject(glm::vec3 position, int specularShine) :
		m_position(position), m_specularShine(specularShine) {};

	// constructor for setting position, albedo, specular strength and specular shine
	StandardObject(glm::vec3 position, glm::vec3 albedo, float specularStrength, int specularShine) :
		m_position(position), m_albedo(albedo), m_specularStrength(specularStrength), m_specularShine(specularShine) {};

	// constructor for setting position, specular shine and diffuse, specular and normal maps.
	StandardObject(glm::vec3 position, int specularShine,
		unsigned int diffuseTexture, unsigned int specularTexture, unsigned int normalTexture) : 
		m_position(position), m_specularShine(specularShine), 
		m_diffuseTexture(diffuseTexture), m_specularTexture(specularTexture), m_normalTexture(normalTexture) {
		m_textured = true;
	}

	// set necessary object properties
	void setPosition(glm::vec3 position) { m_position = position; }
	void setSpecularShine(int specularShine) { m_specularShine = specularShine; }

	// set optional object properties (if none provided and no texture, default values are used)
	void setAlbedo(glm::vec3 albedo) { m_albedo = albedo; }
	void setSpecularStrength(float specularStrength) { m_specularStrength = specularStrength; }

	// set the object rotation
	void setRotation(float rotateValue, glm::vec3 rotateAxis) { m_rotateValue = rotateValue; m_rotateAxis = rotateAxis; }

	// set object textures
	void setTextures(unsigned int diffuseTexture, unsigned int specularTexture, unsigned int normalTexture)
	{
		m_diffuseTexture = diffuseTexture;
		m_specularTexture = specularTexture;
		m_normalTexture = normalTexture;

		// set the is textured flag to true
		m_textured = true;
	}

	// get if texture is assigned to object
	bool isTextured() { return m_textured; }

	// get object properties
	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getAlbedo() { return m_albedo; }
	float getSpecularStrength() { return m_specularStrength; }
	int getSpecularShine() { return m_specularShine;  }
	glm::mat4 getRotation(glm::mat4 matrix) { return glm::rotate(matrix, m_rotateValue, m_rotateAxis); }

	// get object textures
	unsigned int getDiffuseTexture() { return m_diffuseTexture; }
	unsigned int getSpecularTexture() { return m_specularTexture; }
	unsigned int getNormalTexture() { return m_normalTexture; }

	// get pointers to vertex and index data
	std::vector<float>* getVertexData() { return m_vertexData; }
	std::vector<unsigned int>* getIndices() { return m_indices; }
protected:
	// pointers to vertex and index data
	std::vector<float>* m_vertexData;
	std::vector<unsigned int>* m_indices;

	// roation values
	glm::vec3 m_rotateAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	float m_rotateValue = 0.0f;

	// textures
	unsigned int m_diffuseTexture;
	unsigned int m_specularTexture;
	unsigned int m_normalTexture;

	// flag for if the object has a texture
	bool m_textured = false;

	// object properties
	glm::vec3 m_position;
	glm::vec3 m_albedo;
	float m_specularStrength;
	int m_specularShine;
};