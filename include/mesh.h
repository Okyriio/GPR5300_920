#pragma once
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include "pipeline.h"
#include <vector>
#include <iostream>
#include <assimp/types.h>


namespace gpr5300
{

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	
	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		// mesh data
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture>      textures;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection;

		void InitMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
		void Draw(Pipeline& pipeline);
	private:
		
		//  render data
		unsigned int VAO{}, VBO{}, EBO{};

		void setupMesh();
	
	};
}
