#include "VAOManager.h"

// stores reference to the chunk array in chunk manager
void VAOManager::initializeChunkArray(std::vector<Chunk>* chunkArray) {
	m_chunks = chunkArray;
}

// stores reference to the std objects
void VAOManager::initializeCubeArray(std::vector<StandardObject*>* stdObjectsArray) {
	m_standardObjects = stdObjectsArray;

	// resize the VBO and EBO
	m_stdVBOs.resize((*m_standardObjects).size());
	m_stdEBOs.resize((*m_standardObjects).size());
}

// create the vao for the voxel terrain chunks
void VAOManager::makeChunkVAO() {
	// create the VAO
	glCreateVertexArrays(1, &m_chunkVAO);

	// the vertex only has one attribute
	glEnableVertexArrayAttrib(m_chunkVAO, 0);

	// change glVertexArrayAttribFormat to glVertexArrayAttribIFormat as I'm now 
	// passing integer as vertex attribute
	glVertexArrayAttribIFormat(m_chunkVAO, 0, 1, GL_UNSIGNED_INT, 0);

	// binds the attribute
	glVertexArrayAttribBinding(m_chunkVAO, 0, 0);


}

// create the vao for non-voxel objects
void VAOManager::makeStdVAO() {
	// create the VAO
	glCreateVertexArrays(1, &m_stdVAO);

	// loop over all the objects
	for (int i = 0; i < (*m_standardObjects).size(); i++)
	{
		// get reference to the vertex and index data
		std::vector<float>* vertexData = (*(*m_standardObjects)[i]).getVertexData();
		std::vector<unsigned int>* indexData = (*(*m_standardObjects)[i]).getIndices();

		// create the VBO's and EBO's for the object
		glCreateBuffers(1, &m_stdVBOs[i]);
		glNamedBufferStorage(m_stdVBOs[i], sizeof(float) * (*vertexData).size(), (*vertexData).data(), GL_DYNAMIC_STORAGE_BIT);

		glCreateBuffers(1, &m_stdEBOs[i]);
		glNamedBufferStorage(m_stdEBOs[i], sizeof(unsigned int) * (*indexData).size(), (*indexData).data(), GL_DYNAMIC_STORAGE_BIT);
	}

	// enable the 4 vertext attributes position, normal, uv and tangent
	glEnableVertexArrayAttrib(m_stdVAO, 0);
	glEnableVertexArrayAttrib(m_stdVAO, 1);
	glEnableVertexArrayAttrib(m_stdVAO, 2);
	glEnableVertexArrayAttrib(m_stdVAO, 3);

	// assign the size and offset of the attributes
	glVertexArrayAttribFormat(m_stdVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(m_stdVAO, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
	glVertexArrayAttribFormat(m_stdVAO, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6);
	glVertexArrayAttribFormat(m_stdVAO, 3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8);

	// bind the attributes
	glVertexArrayAttribBinding(m_stdVAO, 0, 0);
	glVertexArrayAttribBinding(m_stdVAO, 1, 0);
	glVertexArrayAttribBinding(m_stdVAO, 2, 0);
	glVertexArrayAttribBinding(m_stdVAO, 3, 0);
}

void VAOManager::makeCubeMapVAO() {
	// create the VAO
	glCreateVertexArrays(1, &m_cubeMapVAO);

	// create the vbo buffer with vertex data
	glCreateBuffers(1, &m_cubeMapVBO);
	glNamedBufferStorage(m_cubeMapVBO, sizeof(float) * StandardMeshData::cubemapVertexData.size(), StandardMeshData::cubemapVertexData.data(), GL_DYNAMIC_STORAGE_BIT);

	// create the ebo buffer with index data
	glCreateBuffers(1, &m_cubeMapEBO);
	glNamedBufferStorage(m_cubeMapEBO, sizeof(unsigned int) * StandardMeshData::cubemapIndices.size(), StandardMeshData::cubemapIndices.data(), GL_DYNAMIC_STORAGE_BIT);

	// enable the vertex attribute, only stores position
	glEnableVertexArrayAttrib(m_cubeMapVAO, 0);
	glVertexArrayAttribFormat(m_cubeMapVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_cubeMapVAO, 0, 0);

	// bind the vbo and ebo to vao
	glVertexArrayVertexBuffer(m_cubeMapVAO, 0, m_cubeMapVBO, 0, sizeof(float) * 3);
	glVertexArrayElementBuffer(m_cubeMapVAO, m_cubeMapEBO);
}

// draws all the chunks with a shader and a tetxure atlas
void VAOManager::drawChunks(Shader* shader, unsigned int texture) {
	// bind the chunk shader
	shader->use();

	// bind the texture atlas
	glBindTextureUnit(0, texture);

	// bind the VAO
	glBindVertexArray(m_chunkVAO);

	// loop over chunk array
	for (auto& chunk : *m_chunks) {
		// if chunk isn't visible don't draw
		if (!chunk.getVisible()) continue;

		// set the chunks position as the model matrix translation
		// use render pos as pos is reserved for calculations on another thread
		glm::mat4 m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(chunk.getRenderPosX() * 16.0f, 0.0f, chunk.getRenderPosY() * 16.0f));
		shader->setMat4("Model", m_model);

		// bind the vbo and ebo for the chunk to the vao
		// GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride
		glVertexArrayVertexBuffer(m_chunkVAO, 0, chunk.getVBO(), 0, sizeof(unsigned int));
		glVertexArrayElementBuffer(m_chunkVAO, chunk.getEBO());

		// draw the chunk
		glDrawElements(GL_TRIANGLES, chunk.getIndices()->size(), GL_UNSIGNED_INT, 0);
	}
}

// draw all of the standard objects
void VAOManager::drawStdObjects(Shader* shader) {
	shader->use();

	// bind std objects vao
	glBindVertexArray(m_stdVAO);

	// enable or disable normal maps
	shader->setBool("nMapEnabled", m_normalMapsEnabled);

	// loop through the standard objects array
	for (int i = 0; i < (*m_standardObjects).size(); i++)
	{
		StandardObject* object = (*m_standardObjects)[i];

		// bind the VBO and EBO for current object
		glVertexArrayVertexBuffer(m_stdVAO, 0, m_stdVBOs[i], 0, sizeof(float) * 11);
		glVertexArrayElementBuffer(m_stdVAO, m_stdEBOs[i]);

		// set the model matrix position
		glm::mat4 m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, (*object).getPosition());

		// set the rotation
		m_model = object->getRotation(m_model);

		shader->setMat4("Model", m_model);

		// set the uniforms for object specific properties
 		shader->setInt("specularShine", (*object).getSpecularShine());
		shader->setBool("isTextured", (*object).isTextured());

		// if object has a texture set a default albedo and specular strength property
		(!(*object).isTextured()) ? shader->setVec3("staticAlbedo", (*object).getAlbedo()) : shader->setVec3("albedo", glm::vec3(0.5f, 0.5f, 0.5f));
		(!(*object).isTextured()) ? shader->setFloat("staticSpecularStrength", (*object).getSpecularStrength()) : shader->setFloat("staticSpecularStrength", 0.0f);

		// bind the object textures
		if ((*object).isTextured()) {
			glBindTextureUnit(0, (*object).getDiffuseTexture());
			glBindTextureUnit(1, (*object).getSpecularTexture());
			glBindTextureUnit(2, (*object).getNormalTexture());
		}

		// draw the mesh
		glDrawElements(GL_TRIANGLES, (*(*object).getIndices()).size(), GL_UNSIGNED_INT, 0);
	}
}

void VAOManager::drawCubeMap(Shader* shader, unsigned int texture) {
	shader->use();

	// bind the cube map texture
	glBindTextureUnit(0, texture);

	// bind the cube map vao
	glBindVertexArray(m_cubeMapVAO);

	// set depth function to less then or equal (as cube map is positioned at the far plane)
	glDepthFunc(GL_LEQUAL);

	// draw the cube map
	glDrawElements(GL_TRIANGLES, StandardMeshData::cubemapIndices.size(), GL_UNSIGNED_INT, 0);

	// set depth function back
	glDepthFunc(GL_LESS);
}

void VAOManager::toggleNormalMaps() {
	m_normalMapsEnabled = !m_normalMapsEnabled;
}