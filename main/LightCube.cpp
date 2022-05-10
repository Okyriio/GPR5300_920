#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/glew.h"
#include "glm/mat4x4.hpp"
#include "engine.h"
#include "file_utility.h"
#include "scene.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace gpr5300
{

	class Texture
	{
	public:
		void CreateTexture(const std::string& file_path)
		{
			data_ = stbi_load(file_path.c_str(),
				&texWidth_, &texHeight_, &nrChannels_, 0);
			glGenTextures(1, &texture_);
			glBindTexture(GL_TEXTURE_2D, texture_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth_, texHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		void Delete() const
		{
			glDeleteTextures(1, &texture_);
		}
	private:
		unsigned char* data_{};
		unsigned int texture_ = 0;
		int texWidth_ = 0;
		int texHeight_ = 0;
		int nrChannels_ = 0;
	};

	class Mesh
	{
	public:
		void Generate()
		{
			// VAO
			glGenVertexArrays(1, &vao_);
			glBindVertexArray(vao_);

			//VBO
			glGenBuffers(1, &vbo_[0]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
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
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);
			SetViewMatrix();
		}

		void Delete() const
		{
			glDeleteProgram(program);
			glDeleteVertexArrays(1, &vao_);
		}

		void SetViewMatrix()
		{
			view_ = translate(view_, glm::vec3(0.0f, 0.0f, -3.f));
		}

		void Draw(float t)
		{
			glUseProgram(program);
			glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
			model_ = rotate(model_, glm::radians(-1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			projection_ = glm::perspective(glm::radians(45.f), (float)1280 / (float)720, 0.1f, 100.0f);
			// retrieve the matrix uniform locations
			unsigned int modelLoc = glGetUniformLocation(program, "model");
			unsigned int viewLoc = glGetUniformLocation(program, "view");
			// pass them to the shaders (3 different ways)
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view_[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection_[0][0]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));
			glBindVertexArray(vao_);
			glActiveTexture(GL_TEXTURE0);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		}

		GLuint program = 0;
	private:
		float vertices_[72] =
		{
			//Front
			-0.5f, -0.5f, -0.5f, //0
			-0.5f, 0.5f, -0.5f,
			 0.5f, 0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,

			 //Back
			 -0.5f, -0.5f, 0.5f, //4
			 -0.5f, 0.5f, 0.5f,
			  0.5f, 0.5f, 0.5f,
			  0.5f, -0.5f, 0.5f,

			  //Right
			  0.5f, -0.5f, -0.5f, //8
			  0.5f, 0.5f, -0.5f,
			  0.5f, 0.5f, 0.5f,
			  0.5f, -0.5f, 0.5f,

			  //Left
			  -0.5f, -0.5f, -0.5f, //12
			  -0.5f, 0.5f, -0.5f,
			  -0.5f, 0.5f, 0.5f,
			  -0.5f, -0.5f, 0.5f,

			  //Top
			  -0.5f, 0.5f, -0.5f, //16
			   0.5f, 0.5f, -0.5f,
			   0.5f, 0.5f, 0.5f,
			  -0.5f, 0.5f, 0.5f,

			  //Bottom
			  -0.5f, -0.5f, -0.5f, //20
			   0.5f, -0.5f, -0.5f,
			   0.5f, -0.5f, 0.5f,
			  -0.5f, -0.5f, 0.5f,

		};

		unsigned int indices_[36] =
		{
			//Front
			0, 1, 2,
			0, 2, 3,

			//Back
			4, 5, 7,
			5, 6, 7,

			//Right
			8, 9, 10,
			8, 10, 11,

			//Left
			12, 13, 14,
			12, 14, 15,

			//Top
			16, 17, 18,
			16, 18, 19,

			//Bottom
			20, 21, 22,
			20, 22, 23
		};

		float texCoords[48] =
		{
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
		};

		GLuint vao_ = 0;
		GLuint ebo_ = 0;
		GLuint vbo_[2] = {};

		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 view_ = glm::mat4(1.0f);
		glm::mat4 projection_ = glm::mat4(1.0f);
	};

	class Pipeline
	{
	public:
		void Load(Mesh& mesh)
		{
			//Load shaders
			const auto vertexContent = LoadFile("data/shaders/hello_triangle/LightCube.vert");
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
			const auto fragmentContent = LoadFile("data/shaders/hello_triangle/LightCube.frag");
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
			//Load program/mesh
			mesh.program = glCreateProgram();
			glAttachShader(mesh.program, vertexShader_);
			glAttachShader(mesh.program, fragmentShader_);
			glLinkProgram(mesh.program);
			//Check if shader program was linked correctly

			glGetProgramiv(mesh.program, GL_LINK_STATUS, &success);
			if (!success)
			{
				std::cerr << "Error while linking shader program\n";
			}

		}

		void Delete() const
		{
			glDeleteShader(vertexShader_);
			glDeleteShader(fragmentShader_);
		}
	private:
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;
	};

	class LightCube final : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;
	private:
		Pipeline pipeline_;
		Texture texture_;
		Mesh mesh_;
		float t_;
	};

	void LightCube::Begin()
	{
		glEnable(GL_DEPTH_TEST);
		texture_.CreateTexture("data/Textures/fox.jpg");
		mesh_.Generate();
		pipeline_.Load(mesh_);
	}

	void LightCube::End()
	{
		//Unload program/pipeline
		mesh_.Delete();
		pipeline_.Delete();
		texture_.Delete();
	}

	void LightCube::Update(float dt)
	{
		//Draw program
		mesh_.Draw(dt);
	}


}

int main(int argc, char** argv)
{
	gpr5300::LightCube scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}