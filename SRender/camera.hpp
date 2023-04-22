#pragma once
#include "transform.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	/** The camera is the perspective where the virtual world is rendered from
	 * @todo Perhaps derive from entity
	 */
	class camera : public transform
	{
	private:
		// The field of view of the camera in degrees, horizontal & vertical
		float m_fovH = 0, m_fovV = 0;
		float m_aspectRatio = 0;
		glm::mat4 m_view;
		glm::mat4 m_projection;

	public:
		camera(float _aspectRatio) noexcept;
		camera(
			float _aspectRatio,
			float _fovH
		) noexcept;
		camera(
			float _aspectRatio,
			glm::mat4 _transform
		) noexcept;
		camera(
			float _aspectRatio,
			float _fovH,
			glm::mat4 _transform
		) noexcept;
		camera(
			float _aspectRatio,
			glm::vec3 _from,
			glm::vec3 _to,
			glm::vec3 _up = { 0, 1, 0 }
		) noexcept;
		camera(
			float _aspectRatio,
			float _fovH,
			glm::vec3 _from,
			glm::vec3 _to,
			glm::vec3 _up = { 0, 1, 0 }
		) noexcept;

		void lookAt(
			glm::vec3 _from,
			glm::vec3 _to,
			glm::vec3 _up = { 0, 1, 0 }
		) noexcept;

		void modifyFovH(float _value) noexcept;
		void modifyFovV(float _value) noexcept;
		void updateFovH() noexcept;
		void updateFovV() noexcept;

		void setTransform(const glm::mat4 *_value) noexcept override;
		void setView(glm::mat4 *_value) noexcept;
		void setProjection(float _fovV) noexcept;

		void setProjection(glm::mat4 *_value) noexcept;
		void setAspectRatio(float _aspectRatio) noexcept;

		void setPosition(glm::vec3 _value) noexcept override;
		void translate(glm::vec3 _value) noexcept override;
		void setAxes(
			glm::vec3 _right,
			glm::vec3 _up,
			glm::vec3 _forward
		) noexcept override;

		void setFovH(float _fovH) noexcept;
		void setFovV(float _fovV) noexcept;

		_NODISCARD glm::mat4 getWorldToCameraMatrix() const noexcept;
		_NODISCARD constexpr glm::mat4 getView() const noexcept;
		_NODISCARD constexpr glm::mat4 getProjection() const noexcept;
	};
}
