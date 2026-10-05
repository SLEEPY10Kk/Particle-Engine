#include "render/renderer2D.h"
#include "graphics/shaderProgram.h"
#include "graphics/textureManager.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <vector>
#include <cmath>

namespace Eng
{
	struct QuadVertex
	{
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t MaxQuads = 20000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 16;

		GLuint QuadVAO = 0;
		GLuint QuadVBO = 0;
		GLuint QuadEBO = 0;

		std::shared_ptr<ShaderProgram> QuadShader = nullptr;
		std::shared_ptr<Texture2D> WhiteTexture = nullptr;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		BlendMode CurrentBlendMode = BlendMode::Alpha;
		RenderStats Stats;

		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
	};

	static Renderer2DData s_Data;

	static const char* s_VertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec4 aColor;
		layout (location = 2) in vec2 aTexCoord;
		layout (location = 3) in float aTexIndex;

		uniform mat4 uViewProjection;

		out vec4 vColor;
		out vec2 vTexCoord;
		out float vTexIndex;

		void main()
		{
			vColor = aColor;
			vTexCoord = aTexCoord;
			vTexIndex = aTexIndex;
			gl_Position = uViewProjection * vec4(aPos, 0.0, 1.0);
		}
	)";

	static const char* s_FragmentShaderSource = R"(
		#version 330 core
		in vec4 vColor;
		in vec2 vTexCoord;
		in float vTexIndex;

		uniform sampler2D uTextures[16];

		out vec4 FragColor;

		void main()
		{
			int index = int(round(vTexIndex));
			vec4 texColor = vec4(1.0);

			switch (index)
			{
				case 0:  texColor = texture(uTextures[0],  vTexCoord); break;
				case 1:  texColor = texture(uTextures[1],  vTexCoord); break;
				case 2:  texColor = texture(uTextures[2],  vTexCoord); break;
				case 3:  texColor = texture(uTextures[3],  vTexCoord); break;
				case 4:  texColor = texture(uTextures[4],  vTexCoord); break;
				case 5:  texColor = texture(uTextures[5],  vTexCoord); break;
				case 6:  texColor = texture(uTextures[6],  vTexCoord); break;
				case 7:  texColor = texture(uTextures[7],  vTexCoord); break;
				case 8:  texColor = texture(uTextures[8],  vTexCoord); break;
				case 9:  texColor = texture(uTextures[9],  vTexCoord); break;
				case 10: texColor = texture(uTextures[10], vTexCoord); break;
				case 11: texColor = texture(uTextures[11], vTexCoord); break;
				case 12: texColor = texture(uTextures[12], vTexCoord); break;
				case 13: texColor = texture(uTextures[13], vTexCoord); break;
				case 14: texColor = texture(uTextures[14], vTexCoord); break;
				case 15: texColor = texture(uTextures[15], vTexCoord); break;
				default: texColor = vec4(1.0); break;
			}

			FragColor = texColor * vColor;
		}
	)";

	static std::shared_ptr<ShaderProgram> CompileInternalShader(const char* vSource, const char* fSource)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vSource, nullptr);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fSource, nullptr);
		glCompileShader(fragmentShader);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return std::make_shared<ShaderProgram>(program);
	}

	void Renderer2D::Init()
	{
		s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];

		glGenVertexArrays(1, &s_Data.QuadVAO);
		glBindVertexArray(s_Data.QuadVAO);

		glGenBuffers(1, &s_Data.QuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVBO);
		glBufferData(GL_ARRAY_BUFFER, Renderer2DData::MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoord));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));

		std::vector<uint32_t> quadIndices(Renderer2DData::MaxIndices);
		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		glGenBuffers(1, &s_Data.QuadEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(uint32_t), quadIndices.data(), GL_STATIC_DRAW);

		s_Data.WhiteTexture = TextureManager::Get().GetWhiteTexture();
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadShader = CompileInternalShader(s_VertexShaderSource, s_FragmentShaderSource);
		s_Data.QuadShader->Bind();

		int samplers[Renderer2DData::MaxTextureSlots];
		for (int i = 0; i < static_cast<int>(Renderer2DData::MaxTextureSlots); ++i)
		{
			samplers[i] = i;
		}
		s_Data.QuadShader->SetUniformIntArray("uTextures", samplers, Renderer2DData::MaxTextureSlots);

		glBindVertexArray(0);

		SetBlendMode(BlendMode::Alpha);
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBufferBase = nullptr;

		glDeleteVertexArrays(1, &s_Data.QuadVAO);
		glDeleteBuffers(1, &s_Data.QuadVBO);
		glDeleteBuffers(1, &s_Data.QuadEBO);

		s_Data.QuadShader.reset();
		s_Data.WhiteTexture.reset();
	}

	void Renderer2D::Clear(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer2D::Begin(const Camera2D& camera)
	{
		s_Data.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetUniformMat4("uViewProjection", s_Data.ViewProjectionMatrix);

		StartBatch();
	}

	void Renderer2D::End()
	{
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
		{
			return;
		}

		uint32_t dataSize = static_cast<uint32_t>((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data.QuadVertexBufferBase);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
		{
			if (s_Data.TextureSlots[i])
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
		}

		glBindVertexArray(s_Data.QuadVAO);
		glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::SetBlendMode(BlendMode mode)
	{
		if (s_Data.CurrentBlendMode == mode)
		{
			return;
		}

		Flush();
		s_Data.CurrentBlendMode = mode;

		switch (mode)
		{
		case BlendMode::Alpha:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BlendMode::Additive:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case BlendMode::Multiply:
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			break;
		case BlendMode::None:
			glDisable(GL_BLEND);
			break;
		}
	}

	BlendMode Renderer2D::GetBlendMode()
	{
		return s_Data.CurrentBlendMode;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, 0.0f, s_Data.WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRadians, const glm::vec4& color)
	{
		DrawQuad(position, size, rotationRadians, s_Data.WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tint)
	{
		DrawQuad(position, size, 0.0f, texture, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRadians, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tint)
	{
		constexpr glm::vec2 texCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		std::shared_ptr<Texture2D> currentTexture = texture ? texture : s_Data.WhiteTexture;

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (s_Data.TextureSlots[i]->GetID() == currentTexture->GetID())
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f && currentTexture->GetID() != s_Data.WhiteTexture->GetID())
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				NextBatch();
			}
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = currentTexture;
			s_Data.TextureSlotIndex++;
		}

		glm::vec2 halfSize = size * 0.5f;
		glm::vec2 corners[4] = {
			{ -halfSize.x, -halfSize.y },
			{  halfSize.x, -halfSize.y },
			{  halfSize.x,  halfSize.y },
			{ -halfSize.x,  halfSize.y }
		};

		if (rotationRadians != 0.0f)
		{
			float cosTheta = std::cos(rotationRadians);
			float sinTheta = std::sin(rotationRadians);
			for (int i = 0; i < 4; ++i)
			{
				float x = corners[i].x;
				float y = corners[i].y;
				corners[i].x = x * cosTheta - y * sinTheta + position.x;
				corners[i].y = x * sinTheta + y * cosTheta + position.y;
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				corners[i] += position;
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			s_Data.QuadVertexBufferPtr->Position = corners[i];
			s_Data.QuadVertexBufferPtr->Color = tint;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawParticle(
		const glm::vec2& position,
		const glm::vec2& size,
		float rotationRadians,
		const glm::vec4& color,
		const std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad(position, size, rotationRadians, texture, color);
	}

	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color, float thickness)
	{
		glm::vec2 diff = p1 - p0;
		float length = glm::length(diff);
		if (length < 0.0001f) return;

		float angle = std::atan2(diff.y, diff.x);
		glm::vec2 center = (p0 + p1) * 0.5f;
		DrawQuad(center, { length, thickness }, angle, color);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness)
	{
		glm::vec2 half = size * 0.5f;
		glm::vec2 p0 = position + glm::vec2(-half.x, -half.y);
		glm::vec2 p1 = position + glm::vec2( half.x, -half.y);
		glm::vec2 p2 = position + glm::vec2( half.x,  half.y);
		glm::vec2 p3 = position + glm::vec2(-half.x,  half.y);

		DrawLine(p0, p1, color, thickness);
		DrawLine(p1, p2, color, thickness);
		DrawLine(p2, p3, color, thickness);
		DrawLine(p3, p0, color, thickness);
	}

	void Renderer2D::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, int segments, float thickness)
	{
		if (segments < 3) segments = 3;
		float step = (2.0f * 3.14159265f) / static_cast<float>(segments);

		glm::vec2 prev = center + glm::vec2(std::cos(0.0f) * radius, std::sin(0.0f) * radius);
		for (int i = 1; i <= segments; ++i)
		{
			float angle = i * step;
			glm::vec2 next = center + glm::vec2(std::cos(angle) * radius, std::sin(angle) * radius);
			DrawLine(prev, next, color, thickness);
			prev = next;
		}
	}

	const RenderStats& Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		s_Data.Stats.DrawCalls = 0;
		s_Data.Stats.QuadCount = 0;
	}
}
