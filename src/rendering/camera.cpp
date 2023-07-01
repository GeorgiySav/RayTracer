#include "Camera.h"

#include <iostream>

#include <glm/ext.hpp>

Camera::Camera()
{
	m_position = { 0.f, 1.f, 0.f };
	m_rotation = { 0.f, 0.f, 0.f };
	m_near = 0.01f;
	m_far = 100.f;
	m_fov = 45.f;

	m_sensitivity = 0.1f;
}

Camera::~Camera()
{
}

void Camera::rotate(double xOffset, double yOffset)
{
	xOffset *= m_sensitivity;
	yOffset *= m_sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	m_pitch = std::min(90.0f, m_pitch);
	m_pitch = std::max(-90.f, m_pitch);
	m_yaw = std::fmod(m_yaw, 360.f);
}

glm::vec2 Camera::getPlaneDims(float hwAspect)
{
	glm::vec2 dims = {};
	dims.x = 2 * m_near * glm::tan(glm::radians(m_fov) * 0.5f);
	dims.y = dims.x * hwAspect;
	return dims;
}

glm::mat4 Camera::getCameraMatrix()
{
	glm::mat4 mat(1.0f);
	mat = glm::translate(mat, m_position);
	mat = glm::rotate(mat, m_yaw, glm::vec3{ 1.0f, 0.0f, 0.0f });
	mat = glm::rotate(mat, m_pitch, glm::vec3{ 0.0f, 1.0f, 0.0f });
	mat = glm::rotate(mat, m_roll, glm::vec3{ 0.0f, 0.0f, 1.0f });
	return mat;
}
