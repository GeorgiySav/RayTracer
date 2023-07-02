#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace engine {

	class Camera
	{
	public:
		Camera();
		~Camera();

		void rotate(double xOffset, double yOffset);

		void update();

		void setHwAspect(float hwAspect) { m_hwAspect = hwAspect; m_update_plane = true; }

		const float* getPlaneDimsPointer() const { return glm::value_ptr(m_plane_dimensions); }
		const float* getCameraMatrixPointer() const { return glm::value_ptr(m_cam_matrix); }
		const float* getPositionPointer() const { return glm::value_ptr(m_position); }
		float getNear() const { return m_near; }
		float getFar() const { return m_far; }


	private:
		void updatePlane();
		void updateMat();

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
		glm::mat4 m_cam_matrix;
		bool m_update_mat;
		glm::vec2 m_plane_dimensions;
		bool m_update_plane;
	};

}
