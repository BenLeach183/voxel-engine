#pragma once
#include <vector>

	/*
		Stores all of the vertex and
		index data for each static
		object.

		For default cube, plane and
		cubemap.
	*/

namespace StandardMeshData {
#pragma region Cube
	// Cube vertices
	static std::vector<float> cubeVertexData = {
		//  xyz, normal,uv, tan
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,1.0f, 0.0f, 0.0f,
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,-1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,-1.0f, 0.0f, 0.0f,
		// left
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,0.0f, 0.0f, 1.0f,
		// right
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,0.0f, 0.0f, -1.0f,
		 // bottom
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,1.0f, 0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,1.0f, 0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,1.0f, 0.0f, 0.0f,
		 // top
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,1.0f, 0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,1.0f, 0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,1.0f, 0.0f, 0.0f
	};

	// Cube indices
	static std::vector<unsigned int> cubeIndices = {
			0, 2, 1, 2, 0, 3,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 14, 13, 14, 12, 15,
			16, 17, 18, 18, 19, 16,
			20, 22, 21, 22, 20, 23
	};
#pragma endregion

#pragma region Plane
	// Plane vertices
	static std::vector<float> planeVertexData = {
	-7.0, 0.0,  -7.0,     0.0, 1.0, 0.0, 0.0,0.0,1.0f, 0.0f, 0.0f,
	 7.0, 0.0,  -7.0,     0.0, 1.0, 0.0, 1.0,0.0,1.0f, 0.0f, 0.0f,
	 7.0, 0.0,   7.0,     0.0, 1.0, 0.0, 1.0,1.0,1.0f, 0.0f, 0.0f,
	-7.0, 0.0,   7.0,     0.0, 1.0, 0.0, 0.0,1.0,1.0f, 0.0f, 0.0f
	};

	// Plane indices
	static std::vector<unsigned int> planeIndices = {
		3,2,1,
		3,1,0
	};
#pragma endregion

#pragma region Cubemap
	// Cubemap vertices
	static std::vector<float> cubemapVertexData = {
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	// Cubemap indices
	static std::vector<unsigned int> cubemapIndices = {
		// Right
		1, 6, 2,
		6, 1, 5,
		// Left
		0, 7, 4,
		7, 0, 3,
		// Top
		4, 6, 5,
		6, 4, 7,
		// Bottom
		0, 2, 3,
		2, 0, 1,
		// Back
		0, 5, 1,
		5, 0, 4,
		// Front
		3, 6, 7,
		6, 3, 2
	};
#pragma endregion

}
