#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>
#include <assimp/types.h>

#include "shader.h"


namespace gpr5300
{

	class FrameBuffer
	{
	public:
		void InitFB();
		void bindFrameBuffer();

		void bindDefaultBuffer();

		void createScreenVAO();
		unsigned int texture_;
		
		unsigned int textureColorbuffer_ = 0;
		unsigned int vao_{}, vbo_{}, fbo_{}, rbo_{};
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	private:
		
		
		float quadVertices[4 * 6] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		
	};
}