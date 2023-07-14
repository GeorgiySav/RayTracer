#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "consts.h"

namespace engine {

	class Camera
	{
	public:
		Camera();
		~Camera();

		void move(const glm::vec3& offset);
		void move(Direction dir, float offset);
		void rotate(double xOffset, double yOffset);

		void update();

		void setHwAspect(float hwAspect) { m_hwAspect = hwAspect; m_update_plane = true; }

		const float* getPlaneDimsPointer() const { return glm::value_ptr(m_plane_dimensions); }
		const float* getInverseProjectionPointer() const { return glm::value_ptr(m_inverse_projection); }
		const float* getInverseViewPointer() const { return glm::value_ptr(m_inverse_view); }
		const float* getPositionPointer() const { return glm::value_ptr(m_position); }
		float getNear() const { return m_near; }
		float getFar() const { return m_far; }
		const glm::vec3& getPosition() const { return m_position; }
		const glm::vec3& getRotation() const { return m_rotation; }

	private:
		void updatePlane();
		void updateMat();

		static const glm::vec3 m_world_up;

		glm::vec3 m_position;

		glm::vec3 m_rotation;
		float& m_yaw = m_rotation.x;
		float& m_pitch = m_rotation.y;
		float& m_roll = m_rotation.z;

		float m_near;
		float m_far;

		float m_fov;

		float m_sensitivity;

		float m_hwAspect;

		// values for ray tracing
		glm::vec3 m_forward;
		glm::vec3 m_right;
		glm::mat4 m_projection;
		glm::mat4 m_inverse_projection;
		glm::mat4 m_view;
		glm::mat4 m_inverse_view;
		bool m_update_mat;

		glm::vec2 m_plane_dimensions;
		glm::vec3 m_lower_left_corner;
		bool m_update_plane;
	};
}
