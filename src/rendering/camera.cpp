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

		m_forward = { 0.0f, 0.0f, -1.0f };
		m_right = glm::cross(m_forward, m_world_up);

		m_sensitivity = 0.001f;
	}

	Camera::~Camera() {
	}

	void Camera::move(const glm::vec3& offset) {
		m_position += offset;
		//m_update_mat = true;
		m_update_mat = true;
	}

	void Camera::move(Direction dir, float offset) {
		switch (dir) {
		case Direction::FRONT:
			m_position += m_forward * offset;
			break;
		case Direction::RIGHT:
			m_position -= m_right * offset;
			break;
		case Direction::BACK:
			m_position -= m_forward * offset;
			break;
		case Direction::LEFT:
			m_position += m_right * offset;
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
		m_update_mat = true;
	}

	void Camera::rotate(double xOffset, double yOffset)
	{
		xOffset *= m_sensitivity;
		yOffset *= m_sensitivity;

		m_pitch -= yOffset;
		m_yaw += xOffset;

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-(float)yOffset, m_right),
			glm::angleAxis((float)xOffset, glm::vec3(0.f, 1.0f, 0.0f))));
		m_forward = glm::rotate(q, m_forward);
		m_right = glm::cross(m_forward, m_world_up);
		//m_yaw = std::min(glm::pi<float>() * 0.5f, m_yaw);
		//m_yaw = std::max(-glm::pi<float>() * 0.5f, m_yaw);
		//m_pitch = std::fmod(m_pitch, glm::pi<float>() * 2.0f);//2.0 * glm::pi<float>());

		m_update_mat = true;
	}

	void Camera::update() {
		if (m_update_plane)
			updatePlane();
		if (m_update_mat)
			updateMat();
	}

	void Camera::updateMat() {
		m_cam_matrix = glm::mat4(1.0f);
		m_cam_matrix = glm::rotate(m_cam_matrix, m_rotation.x, { 1.0f, 0.0f, 0.0f });
		m_cam_matrix = glm::rotate(m_cam_matrix, m_rotation.y, { 0.0f, 1.0f, 0.0f });
		m_cam_matrix = glm::rotate(m_cam_matrix, m_rotation.z + glm::pi<float>(), {0.0f, 0.0f, 1.0f});

		m_projection = glm::perspectiveFov(glm::radians(m_fov), (float)m_plane_dimensions.x, (float)m_plane_dimensions.y, m_near, m_far);
		m_inverse_projection = glm::inverse(m_projection);

		m_view = glm::lookAt(m_position, m_position + m_forward, m_world_up);
		m_inverse_view = glm::inverse(m_view);

		m_update_mat = false;
	}

	void Camera::updatePlane() {
		m_plane_dimensions.x = 2 * m_near * glm::tan(glm::radians(m_fov) * 0.5f);
		m_plane_dimensions.y = m_plane_dimensions.x * m_hwAspect;
		m_update_plane = false;
	}

}