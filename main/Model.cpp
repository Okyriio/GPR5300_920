#include <fstream>
#include <sstream>
#include <SDL.h>

#include "engine.h"
#include "ModelScene.h"

#include <glm/ext/matrix_transform.hpp>



namespace gpr5300
{

	void Model::Begin()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		baseTexture_.CreateTexture("data/Textures/iceice.jpg", GL_TEXTURE0);
		specularTexture_.CreateTexture("data/Textures/Ice_SPEC.jpg", GL_TEXTURE1);
		mesh_.Generate();
		pipeline_.Load();
	}

	void Model::End()
	{
		//Unload program/pipeline
		mesh_.Delete();
		pipeline_.Delete();
		baseTexture_.Delete();
		specularTexture_.Delete();
	}

	void Model::Update(float dt)
	{
		//Draw program
		tt_ += dt;
		ProcessInput(dt);
		mesh_.view_ = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		pipeline_.SetVector3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
		mesh_.Draw(tt_, pipeline_);
	}

	void Model::ProcessInput(float dt)
	{
		const float cameraSpeed = 1.00f * dt; // adjust accordingly
		const Uint8* state = SDL_GetKeyboardState(nullptr);

		if (state[SDL_SCANCODE_W]) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (state[SDL_SCANCODE_A]) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (state[SDL_SCANCODE_S]) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (state[SDL_SCANCODE_D]) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		
	}
}

int main(int argc, char** argv)
{
	gpr5300::Model scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}