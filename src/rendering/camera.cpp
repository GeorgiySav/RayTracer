#include "Camera.h"

#include <iostream>

#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace engine {

	const glm::vec3 Camera::m_world_up = { 0.0f, 1.0f, 0.0f };

	Camera::Camera() {
		m_position = { 0.f, 0.f, 0.f };
		m_rotation = { 0.f, 0.f, 0.f };
		m_near = 0.05f;
		m_far = 100.f;
		m_fov = 90.f;

		m_sensitivity = 0.001f;
	}

	Camera::~Camera() {
	}

	void Camera::move(const glm::vec3& offset) {
		m_position += offset;
		//m_update_mat = true;
	}

	void Camera::move(Direction dir, float offset) {
		glm::vec3 direction = {0.0f, 0.0f, -1.0f};
		direction = glm::vec3(m_cam_matrix * glm::vec4(direction, 1.0));
		direction = glm::normalize(direction);
		glm::vec3 right_direction = glm::normalize(glm::cross(direction, m_world_up));
		switch (dir) {
		case Direction::FRONT:
			m_position += direction * offset;
			break;
		case Direction::RIGHT:
			m_position += right_direction * offset;
			break;
		case Direction::BACK:
			m_position -= direction * offset;
			break;
		case Direction::LEFT:
			m_position -= right_direction * offset;
			break;
		case Direction::UP:
			m_position += m_world_up * offset;
			break;
		case Direction::DOWN:
			m_position -= m_world_up * offset;
			break;
		default:
			break;
		}
	}

	void Camera::rotate(double xOffset, double yOffset)
	{
		xOffset *= m_sensitivity;
		yOffset *= m_sensitivity;

		m_yaw += yOffset;
		m_pitch -= xOffset;

		m_yaw = std::min(glm::pi<float>() * 0.5f, m_yaw);
		m_yaw = std::max(-glm::pi<float>() * 0.5f, m_yaw);
		m_pitch = std::fmod(m_pitch, glm::pi<float>() * 2.0f);//2.0 * glm::pi<float>());

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
		m_cam_matrix = glm::rotate(m_cam_matrix, m_rotation.x, { 1.0f, 0.0f, 0.0f });
		m_cam_matrix = glm::rotate(m_cam_matrix, m_rotation.y, { 0.0f, 1.0f, 0.0f });
		m_cam_matrix = glm::rotate(m_cam_matrix, m_rotation.z + glm::pi<float>(), {0.0f, 0.0f, 1.0f});
		m_update_mat = false;
	}

	void Camera::updatePlane() {
		m_plane_dimensions.x = 2 * m_near * glm::tan(glm::radians(m_fov) * 0.5f);
		m_plane_dimensions.y = m_plane_dimensions.x * m_hwAspect;
		m_lower_left_corner = m_position - glm::vec3(0.5f * m_plane_dimensions, 0) - glm::vec3(0, 0, m_near);
		m_update_plane = false;
	}

}