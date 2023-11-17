#pragma once
#include "Objects/StandardObject.h"

/*
	Class for the default plane object
	stores reference to vertex and
	index data.

	Stores properties of the
	individual plane.
*/

class StandardPlane : public StandardObject {
public:
	// constructor for just setting position and specular shine
	StandardPlane(glm::vec3 position, int specularShine) :
		StandardObject(position, specularShine) {
			// store reference to the plane vertex and index data
			m_vertexData = &StandardMeshData::planeVertexData;
			m_indices = &StandardMeshData::planeIndices;
	};

	// constructor for setting position, albedo, specular strength and specular shine
	StandardPlane(glm::vec3 position, glm::vec3 albedo, float specularStrength, int specularShine) :
		StandardObject(position, albedo, specularStrength, specularShine) {
			// store reference to the plane vertex and index data
			m_vertexData = &StandardMeshData::planeVertexData;
			m_indices = &StandardMeshData::planeIndices;
	};

	// constructor for setting position, specular shine and diffuse, specular and normal maps.
	StandardPlane(glm::vec3 position, int specularShine,
		unsigned int diffuseTexture, unsigned int specularTexture, unsigned int normalTexture) : 
		StandardObject(position, specularShine, diffuseTexture, specularTexture, normalTexture) { 
			// store reference to the plane vertex and index data
			m_vertexData = &StandardMeshData::planeVertexData;
			m_indices = &StandardMeshData::planeIndices;
	};
};