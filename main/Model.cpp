#include <fstream>
#include <sstream>
#include <SDL.h>
#include <stb_image.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "engine.h"
#include "ModelScene.h"
#include <glm/ext/matrix_transform.hpp>




namespace gpr5300
{

	void Model::Begin()
	{
		
		
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		
		/*baseTexture_.CreateTexture("data/Textures/iceice.jpg", GL_TEXTURE0);
		specularTexture_.CreateTexture("data/Textures/Ice_SPEC.jpg", GL_TEXTURE1);*/
		

		/*for (int fill = 0; fill < numOfModels ; fill++)
		{
			ModelObject model;
			models_.emplace_back(model);
		}

		for (int i = 0; i < models_.size(); i++)
		{
			
		}*/
		
		
		
		frameBuffer_.InitFB();
		sky_.cubemapTexture = sky_.loadCubemap(sky_.cubeMapfaces);
		sky_.BindSky();
		skyShader_.Load("data/shaders/hello_triangle/CubeMap.vert", "data/shaders/hello_triangle/CubeMap.frag");
		sceneShader_.Load("data/shaders/hello_triangle/Model.vert" , "data/shaders/hello_triangle/Model.frag");
		screenShader_.Load("data/shaders/hello_triangle/FrameBuffer.vert", "data/shaders/hello_triangle/FrameBuffer.frag");
		//simpleColorShader_.Load("data/shaders/hello_triangle/shader_single_color.vert", "data/shaders/hello_triangle/shader_single_color.frag");
		model_.InitModel("data/Models/backpack/backpack.obj");
	}

	void Model::End()
	{
		//Unload program/pipeline
		
		/*shader_.Delete();*/
	/*	baseTexture_.Delete();
		specularTexture_.Delete();*/
	}

	


	void Model::Update(float dt)
	{

		//Draw program
		tt_ += dt;
		ProcessInput(dt);
				////FirstPass FrameBuffer
		//frameBuffer_.bindFrameBuffer();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = cam_.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(cam_.Zoom), 1280.0f/ 720.0f, 0.1f, 100.0f);


		


		//Set uniforms
		sceneShader_.Use();
		sceneShader_.SetInt("skybox", 0);
		sceneShader_.SetVector3("viewPos", cam_.Position);
		sceneShader_.SetVector3("dirLight.direction", cam_.Front);
		sceneShader_.SetVector3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		sceneShader_.SetVector3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		sceneShader_.SetVector3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		sceneShader_.SetInt("material.ambient", 0);
		sceneShader_.SetInt("material.specular", 1);
		sceneShader_.SetFloat("material.shininess", 64.0f);
		sceneShader_.SetMatrix4("view", view);
		sceneShader_.SetMatrix4("projection", projection);


		 //model_.model = rotate(model_.model, dt * glm::radians(20.0f), glm::vec3(0.2f, 1.0f, 3.0f));
		 model = scale(model_.model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		
		sceneShader_.SetMatrix4("model", model);



		////Stencil
		/*glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);*/

		
		model_.Draw(sceneShader_);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
		skyShader_.Use();
		skyShader_.SetInt("skybox", 0);
		view = glm::mat4(glm::mat3(cam_.GetViewMatrix())); //camera
		skyShader_.SetMatrix4("view", view);
		skyShader_.SetMatrix4("projection", projection);
		// skybox cube
		glBindVertexArray(sky_.skyvao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, sky_.cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		
		////Stencil
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//simpleColorShader_.Use();
		//simpleColorShader_.SetMatrix4("view", model_.view);
		//simpleColorShader_.SetMatrix4("projection", model_.projection);
		//model = scale(model, glm::vec3(1.04f, 1.04f, 1.04f));	// it's a bit too big for our scene, so scale it down
		//simpleColorShader_.SetMatrix4("model", model);
		//model_.Draw(simpleColorShader_);
		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glEnable(GL_DEPTH_TEST);

		////SecondPass FrameBuffer
		//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//frameBuffer_.bindDefaultBuffer();
		//screenShader_.Use();
		//glBindVertexArray(frameBuffer_.vao_);
		//glDisable(GL_DEPTH_TEST);
		//glBindTexture(GL_TEXTURE_2D,frameBuffer_.textureColorbuffer_ );
		//glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Model::mouse_callback( double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (cam_.firstMouse)
		{
			cam_.lastX = xpos;
			cam_.lastY = ypos;
			cam_.firstMouse = false;
		}

		float xoffset = xpos - cam_.lastX;
		float yoffset = cam_.lastY - ypos; // reversed since y-coordinates go from bottom to top

		cam_.lastX = xpos;
		cam_.lastY = ypos;

		
	}

	void Model::ProcessInput(float dt)
	{
		
		SDL_Event event;
		SDL_PollEvent(&event);
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		
		SDL_GetMouseState(&cam_.mousePosx, &cam_.mousPosy);
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			mouse_callback(cam_.mousePosx, cam_.mousPosy);
			cam_.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		default:
			break;
		}

		if (state[SDL_SCANCODE_W]) {
			cam_.ProcessKeyboard(FORWARD, dt);
		}
		if (state[SDL_SCANCODE_A]) {
			cam_.ProcessKeyboard(LEFT, dt);
		}
		if (state[SDL_SCANCODE_S]) {
			cam_.ProcessKeyboard(BACKWARD, dt);
		}
		if (state[SDL_SCANCODE_D]) {
			cam_.ProcessKeyboard(RIGHT, dt);
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