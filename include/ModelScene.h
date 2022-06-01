#pragma once
#include "mesh.h"
#include "scene.h"
#include "pipeline.h"
#include "texture.h"



namespace gpr5300
{
	class Model final : public Scene
	{
	public:

		void Begin() override;
		void End() override;
		void Update(float dt) override;
		void ProcessInput(float dt);
	private:
		Pipeline pipeline_;
		Texture baseTexture_;
		Texture specularTexture_;
		Mesh mesh_;
		float tt_ = 0.0f;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}