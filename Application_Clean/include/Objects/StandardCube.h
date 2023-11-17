#pragma once
#include "Objects/StandardObject.h"

	/*
		Class for the default cube object
		stores reference to vertex and
		index data.

		Stores properties of the
		individual cube.
	*/

class StandardCube : public StandardObject {
public:
	// constructor for just setting position and specular shine
	StandardCube(glm::vec3 position, int specularShine) :
		StandardObject(position, specularShine) {
			// store reference to the cube vertex and index data
			m_vertexData = &StandardMeshData::cubeVertexData;
			m_indices = &StandardMeshData::cubeIndices;
	};

	// constructor for setting position, albedo, specular strength and specular shine
	StandardCube(glm::vec3 position, glm::vec3 albedo, float specularStrength, int specularShine) :
		StandardObject(position, albedo, specularStrength, specularShine) {
			// store reference to the cube vertex and index data
			m_vertexData = &StandardMeshData::cubeVertexData;
			m_indices = &StandardMeshData::cubeIndices;
	};

	// constructor for setting position, specular shine and diffuse, specular and normal maps.
	StandardCube(glm::vec3 position, int specularShine, 
		unsigned int diffuseTexture, unsigned int specularTexture, unsigned int normalTexture) : 
		StandardObject(position, specularShine, diffuseTexture, specularTexture, normalTexture) {
			// store reference to the cube vertex and index data
			m_vertexData = &StandardMeshData::cubeVertexData; 
			m_indices = &StandardMeshData::cubeIndices; 
	};
};