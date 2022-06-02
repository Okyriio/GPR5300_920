#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "windows.h"
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
		void CreateTexture(const std::string& file_path, auto TextureNumber)
		{
			data_ = stbi_load(file_path.c_str(),
				&texWidth_, &texHeight_, &nrChannels_, 0);
			glActiveTexture(TextureNumber);
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
		unsigned int border_ = 0;
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(normals_), normals_, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);

			glGenBuffers(1, &vbo_[2]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords_), texCoords_, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(2);

		

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
			/*const int objectColor = glGetUniformLocation(program, "objectColor");*/
			const int lightColor = glGetUniformLocation(program, "lightColor");
			const int lightPos = glGetUniformLocation(program, "lightPos");
			const int viewPos = glGetUniformLocation(program, "viewPos");
			const int diffuse = glGetUniformLocation(program, "material.diffuse");
			const int specular = glGetUniformLocation(program, "material.specular");
			const int shininess = glGetUniformLocation(program, "material.shininess");
			const int ambientL = glGetUniformLocation(program, "light.ambientL");
			const int diffuseL = glGetUniformLocation(program, "light.diffuseL");
			const int specularL = glGetUniformLocation(program, "light.specularL");
			glUniform3f(lightPos, 0.0f, 0.0f, 1.0f);
			/*glUniform3f(objectColor, abs(cos(t)), abs(sin(t)), abs(tan(t)));*/
			glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
			glUniform3f(viewPos, 0.0f, 0.0f, 2.0f);
			glUniform1i(diffuse, 0);
			glUniform1i(specular, 1);
			glUniform1f(shininess,32);
			glUniform3f(ambientL, 0.2f, 0.2f, 0.2f);
			glUniform3f(diffuseL, 0.5f, 0.5f, 0.5f);
			glUniform3f(specularL, 1.0f, 1.0f, 1.0f);


			model_ = rotate(model_, glm::radians(0.2f), glm::vec3(0.2f, 1.0f, 0.0f));
			projection_ = glm::perspective(glm::radians(45.f), (float)1280 / (float)720, 0.1f, 100.0f);
			// retrieve the matrix uniform locations
			const unsigned int modelLoc = glGetUniformLocation(program, "model");
			const unsigned int viewLoc = glGetUniformLocation(program, "view");
			// pass them to the shaders (3 different ways)
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view_[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection_[0][0]);

			glBindVertexArray(vao_);
			glActiveTexture(GL_TEXTURE0);

			
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		}
		glm::mat4 view_ = glm::mat4(1.0f);
		GLuint program = 0;
		GLuint vbo_[3] = {};
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
			  -0.5f, -0.5f, 0.5f

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


		float normals_[72] = {
			//Front
			 0.0f, 0.0f, -1.0f,
			 0.0f, 0.0f, -1.0f,
			 0.0f, 0.0f, -1.0f,
			 0.0f, 0.0f, -1.0f,

			 //Back
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,

			 //Right
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			//Left
			-1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,

		   //Top
		   0.0f, 1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,

		  //Bottom
		  0.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		};

		float texCoords_[48] =
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
		
		glm::mat4 projection_ = glm::mat4(1.0f);
		
		glm::mat4 model_ = glm::mat4(1.0f);
	
		
	};

	class Light
	{
	public:
		void Generate(Mesh mesh)
		{
			glGenVertexArrays(1, &vao_);
			glBindVertexArray(vao_);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_[0]);
			// note that we update the lamp's position attribute's stride to reflect the updated buffer data
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_[1]);
			// normal attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

	private:
		GLuint vao_ = 0;
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

		void processInput(float dt)
		{
			const float cameraSpeed = 1.00f * dt; // adjust accordingly
			if (GetKeyState('W') & 0x8000)
				cameraPos += cameraSpeed * cameraFront;
			if (GetKeyState('S') & 0x8000)
				cameraPos -= cameraSpeed * cameraFront;
			if (GetKeyState('A') & 0x8000)
				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			if (GetKeyState('D') & 0x8000)
				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

	private:
		Pipeline pipeline_;
		Texture texture_;
		Texture specular_;
	
		Mesh mesh_;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		
		float t_ = 0.0f;
	};

	void LightCube::Begin()
	{
		glEnable(GL_DEPTH_TEST);
		texture_.CreateTexture("data/Textures/iceice.jpg", GL_TEXTURE0);
		specular_.CreateTexture("data/Textures/Ice_SPEC.jpg", GL_TEXTURE1);
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
		t_ += dt;
		processInput(dt);
		mesh_.view_ = glm::lookAt(cameraPos, cameraFront,cameraUp );
		mesh_.Draw(t_);
	}


}

int main(int argc, char** argv)
{
	gpr5300::LightCube scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}