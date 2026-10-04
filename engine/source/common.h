#ifndef ENG_COMMON_H
#define ENG_COMMON_H

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

namespace Eng
{
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;
	using Color = glm::vec4;

	struct CameraData
	{
		glm::mat3 ViewMatrix = glm::mat3(1.0f);
		glm::mat3 ProjectionMatrix = glm::mat3(1.0f);
	};

	struct LightData
	{
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec3 Color = glm::vec3(1.0f);
		float Radius = 100.0f;
		float Intensity = 1.0f;
	};
}

#endif
