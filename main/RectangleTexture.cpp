#include <fstream>
#include <iostream>
#include <sstream>
#include "gl/glew.h"
#include "Error.h"

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace gpr5300
{
	class RectangleTexture final : public Scene
	{
	public:
		

		void Begin() override;
		void End() override;
		void Update(float dt) override;

		float vertices[12] =
		{
			 -0.5f, -0.5f, 0.0f,
			 -0.5f, 0.5f, 0.0f,
			 0.5f, 0.5f, 0.0f,

			0.5f, -0.5f, 0.0f
		};

		float texCoords[8] =
		{
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
		};

		unsigned int indices[6] =
		{
			0, 1, 3,
			1, 2, 3
		};

	private:
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;
		GLuint program_ = 0;
		GLuint vao_ = 0;
		GLuint ebo_ = 0;
		GLuint vbo_[2] = {};
		unsigned int texture_ = 0;
		int texWidth_ = 0;
		int texHeight_ = 0;
		int nrChannels_ = 0;
		unsigned char* data{};
	};

	void RectangleTexture::Begin()
	{
		glCheckError();
		data = stbi_load("data/Textures/fox.jpg", &texWidth_, &texHeight_, &nrChannels_, 0);
		glGenTextures(1, &texture_);
		glCheckError();
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth_, texHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		std::free(data);
		glCheckError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glCheckError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glCheckError();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glCheckError();
		glGenerateMipmap(GL_TEXTURE_2D);
		glCheckError();
		
		

		//VAO
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);


		//VBO
		glGenBuffers(1, &vbo_[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &vbo_[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		//EBO
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glCheckError();


		//Load shaders
		const auto vertexContent = LoadFile("data/shaders/hello_triangle/RectangleTexture.vert");
		const auto* ptr = vertexContent.data();
		vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader_, 1, &ptr, nullptr);
		glCompileShader(vertexShader_);
		//Check success status of shader compilation
		GLint success;
		glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cerr << "Error while loading vertex shader\n";
		}
		const auto fragmentContent = LoadFile("data/shaders/hello_triangle/RectangleTexture.frag");
		ptr = fragmentContent.data();
		fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader_, 1, &ptr, nullptr);
		glCompileShader(fragmentShader_);
		//Check success status of shader compilation

		glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cerr << "Error while loading fragment shader\n";
		}
		//Load program/pipeline
		program_ = glCreateProgram();
		glAttachShader(program_, vertexShader_);
		glAttachShader(program_, fragmentShader_);
		glLinkProgram(program_);
		//Check if shader program was linked correctly

		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cerr << "Error while linking shader program\n";
		}
		glCheckError();
	}

	void RectangleTexture::End()
	{
		//Unload program/pipeline
		glDeleteProgram(program_);

		glDeleteShader(vertexShader_);
		glDeleteShader(fragmentShader_);
		glDeleteTextures(1, &texture_);
		glDeleteVertexArrays(1, &vao_);

	}

	void RectangleTexture::Update(float dt)
	{
	
		glCheckError();
		//Draw program

		glUseProgram(program_);
		glUniform1i(glGetUniformLocation(program_, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glCheckError();
	}


}

int main(int argc, char** argv)
{
	gpr5300::RectangleTexture scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}