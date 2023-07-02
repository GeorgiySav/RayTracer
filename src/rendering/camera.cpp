#include "Camera.h"

#include <iostream>

#include <glm/ext.hpp>

namespace engine {

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

		m_update_mat = true;
	}

	void Camera::update() {
		if (m_update_mat)
			updateMat();
		if (m_update_plane)
			updatePlane();
	}

	void Camera::updateMat() {
		m_cam_matrix = glm::mat4(1.0f);
		m_cam_matrix = glm::translate(m_cam_matrix, m_position);
		m_cam_matrix = glm::rotate(m_cam_matrix, m_yaw, glm::vec3{ 1.0f, 0.0f, 0.0f });
		m_cam_matrix = glm::rotate(m_cam_matrix, m_pitch, glm::vec3{ 0.0f, 1.0f, 0.0f });
		m_cam_matrix = glm::rotate(m_cam_matrix, m_roll, glm::vec3{ 0.0f, 0.0f, 1.0f });
		m_update_mat = false;
	}

	void Camera::updatePlane() {
		m_plane_dimensions.x = 2 * m_near * glm::tan(glm::radians(m_fov) * 0.5f);
		m_plane_dimensions.y = m_plane_dimensions.x * m_hwAspect;
		m_update_plane = false;
	}

}