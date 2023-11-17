#pragma once
#pragma once
#include "Controls/Camera.h"

/*

Very Basic First Person Camera
WSAD and mouse for rotate/zoom

*/
class FirstPersonCamera : public Camera {

public:
	FirstPersonCamera(glm::vec3 pos = glm::vec3(0.0f, 75.5f, 0.0f));

	glm::mat4 getViewMatrix()                                          override;
	glm::mat4 getProjectionMatrix()                                    override;
	void update(float dt)                                              override;
	void attachHandler(GLFWwindow* W, InputHandler* H) override;
private:

	float m_speed;       // camera speed
	float m_sensitivity; // mouse sensitivity
	float m_pitchContrain; // don't allow pitch to go above/below 90 otherwise screen flips

	float m_lastX; // previous mouse position
	float m_lastY;

	GLFWwindow* m_window;
	InputHandler* m_handler;

	void updateCameraVectors();
	void zoom(float offsetY); // mouse scroll
	void look(float offsetX, float offsetY); // move camera towards direction 



};

