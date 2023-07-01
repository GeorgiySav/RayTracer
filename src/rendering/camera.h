#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void rotate(double xOffset, double yOffset);

	glm::vec2 getPlaneDims(float hwAspect);
	glm::mat4 getCameraMatrix();

	glm::vec3 getPosition() { return m_position; }
	float getNear() { return m_near; }
	float getFar() { return m_far; }

private:
	glm::vec3 m_position;

	glm::vec3 m_rotation;
	float& m_yaw = m_rotation.x;
	float& m_pitch = m_rotation.y;
	float& m_roll = m_rotation.z;

	float m_near;
	float m_far;

	float m_fov;

	float m_sensitivity;
};
