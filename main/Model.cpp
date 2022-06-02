#include <fstream>
#include <sstream>
#include <SDL.h>
#include <glm/ext/matrix_clip_space.hpp>

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
		/*baseTexture_.CreateTexture("data/Textures/iceice.jpg", GL_TEXTURE0);
		specularTexture_.CreateTexture("data/Textures/Ice_SPEC.jpg", GL_TEXTURE1);*/
		model_.InitModel("data/Models/backpack/backpack.obj");
		pipeline_.Load();
	}

	void Model::End()
	{
		//Unload program/pipeline
		
		pipeline_.Delete();
	/*	baseTexture_.Delete();
		specularTexture_.Delete();*/
	}

	void Model::Update(float dt)
	{
		//Draw program
		tt_ += dt;
		ProcessInput(dt);
		pipeline_.Use();

		pipeline_.SetInt("material.diffuse", 0);
		pipeline_.SetInt("material.specular", 1);//Begin?pipeline_.
		pipeline_.SetVector3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z); //for specular
		pipeline_.SetFloat("material.shininess", 32.0f);


		pipeline_.SetVector3("dirLight.direction", 0.0f, -1.0f, -1.0f);
		pipeline_.SetVector3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		pipeline_.SetVector3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		pipeline_.SetVector3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		pipeline_.SetVector3("pointLights[0].position", 0.0f, 0.0f, -1.0f);
		pipeline_.SetVector3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
		pipeline_.SetVector3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f); // darken diffuse light a bit
		pipeline_.SetVector3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		pipeline_.SetFloat("pointLights[0].constant", 1.0f);
		pipeline_.SetFloat("pointLights[0].linear", 0.09f);
		pipeline_.SetFloat("pointLights[0].quadratic", 0.032f);

		pipeline_.SetVector3("spotLight.position", cameraPos);
		pipeline_.SetVector3("spotLight.direction", cameraFront);
		pipeline_.SetVector3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		pipeline_.SetVector3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		pipeline_.SetVector3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		pipeline_.SetFloat("spotLight.constant", 1.0f);
		pipeline_.SetFloat("spotLight.linear", 0.09f);
		pipeline_.SetFloat("spotLight.quadratic", 0.032f);
		pipeline_.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		pipeline_.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		//add multiple spotlight here !! add the same as for point lights
		mesh_.model = glm::rotate(mesh_.model, dt * glm::radians(-0.0f), glm::vec3(1.0f, 1.0f, 1.0f)); //locale coordonate
		mesh_.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //camera
		mesh_.projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

		pipeline_.SetMatrix4("model", mesh_.model);
		pipeline_.SetMatrix4("view", mesh_.view);
		pipeline_.SetMatrix4("projection", mesh_.projection);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(mesh_.model, glm::vec3(0.0f, 0.0f, -3.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(mesh_.model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		pipeline_.SetMatrix4("model", model);
		model_.Draw(pipeline_);
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