#include "pipeline.h"
#include <iostream>
#include "file_utility.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>


namespace gpr5300
{
	void Pipeline::Load()
	{
		//Load shaders
		const auto vertexContent = gpr5300::LoadFile("data/shaders/hello_triangle/Model.vert");
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
		const auto fragmentContent = gpr5300::LoadFile("data/shaders/hello_triangle/Model.frag");
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
		program = glCreateProgram();
		glAttachShader(program, vertexShader_);
		glAttachShader(program, fragmentShader_);
		glLinkProgram(program);
		//Check if shader program was linked correctly
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cerr << "Error while linking shader program\n";
		}
	}

	void Pipeline::Use() const
	{
		glUseProgram(program);
	}

	void Pipeline::Delete() const
	{
		glDeleteProgram(program);
		glDeleteShader(vertexShader_);
		glDeleteShader(fragmentShader_);
	}

	void Pipeline::SetMatrix4(const std::string_view uniform, glm::mat4& matrix4) const
	{
		const int uniformLocation = glGetUniformLocation(program, uniform.data());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value_ptr(matrix4));
	}

	void Pipeline::SetVector3(const std::string_view uniform, float x, float y, float z) const
	{
		int uniformLocation = glGetUniformLocation(program, uniform.data());
		glUniform3f(uniformLocation, x, y, z);
	}
	void Pipeline::SetVector3(const std::string& uniform, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(program, uniform.c_str()), 1, &value[0]);
	}

	void Pipeline::SetFloat(const std::string& uniform, float value) const
	{
		glUniform1f(glGetUniformLocation(program, uniform.c_str()), value);
	}

	void Pipeline::SetInt(const std::string& uniform, int value) const
	{
		glUniform1i(glGetUniformLocation(program, uniform.c_str()), value);
	}

	void Pipeline::ActivateLight() const
	{
		

	}
}