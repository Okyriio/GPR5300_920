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
		
	

		//frameBuffer_.InitFB();
		sky_.cubemapTexture = sky_.loadCubemap(sky_.cubeMapfaces);
		sky_.BindSky();
		skyShader_.Load("data/shaders/hello_triangle/CubeMap.vert", "data/shaders/hello_triangle/CubeMap.frag");
		sceneShader_.Load("data/shaders/hello_triangle/Model.vert", "data/shaders/hello_triangle/Model.frag");
		screenShader_.Load("data/shaders/hello_triangle/FrameBuffer.vert", "data/shaders/hello_triangle/FrameBuffer.frag");
		//simpleColorShader_.Load("data/shaders/hello_triangle/shader_single_color.vert", "data/shaders/hello_triangle/shader_single_color.frag");
		model_.InitModel("data/Models/backpack/backpack.obj");
	
		sceneShader_.Use();
		modelMatrices = new glm::mat4[amount];
		for (unsigned int i = 0; i < amount; i++)
		{
			constexpr float offset = 3.0f;
			glm::mat4 model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(offset * i, 0.0, 0.0));
			modelMatrices[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < model_.meshes.size(); i++)
		{
			unsigned int VAO = model_.meshes[i].vao_;
			glBindVertexArray(VAO);
			// vertex attributes
			std::size_t vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindVertexArray(0);
		}

		
	}

	void Model::End()
	{
		/*Unload program/pipeline*/
		
		/*shader_.Delete();
		baseTexture_.Delete();
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
		sceneShader_.SetVector3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		sceneShader_.SetVector3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		sceneShader_.SetVector3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		
		

		//point light 1
		sceneShader_.SetVector3("pointLights[0].position", 0.0f, 0.0f, 1.0f);
		sceneShader_.SetVector3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		sceneShader_.SetVector3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f); //darken diffuse light a bit
			sceneShader_.SetVector3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		sceneShader_.SetFloat("pointLights[0].constant", 1.0f);
		sceneShader_.SetFloat("pointLights[0].linear", 0.09f);
		sceneShader_.SetFloat("pointLights[0].quadratic", 0.032f);
	

		// spotLight camera
		sceneShader_.SetVector3("spotLight.position", cam_.Position);
		sceneShader_.SetVector3("spotLight.direction", cam_.Front);
		sceneShader_.SetVector3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		sceneShader_.SetVector3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		sceneShader_.SetVector3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		sceneShader_.SetFloat("spotLight.constant", 1.0f);
		sceneShader_.SetFloat("spotLight.linear", 0.09f);
		sceneShader_.SetFloat("spotLight.quadratic", 0.032f);
		sceneShader_.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		sceneShader_.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		//sceneShader_.SetInt("material.texture_diffuse1", 0);
		//sceneShader_.SetInt("material.texture_specular1", 1);
		//sceneShader_.SetInt("material.texture_normal1", 1);
		sceneShader_.SetFloat("material.shininess", 32.0f);

		sceneShader_.SetMatrix4("view", view);
		sceneShader_.SetMatrix4("projection", projection);


		 
		 model = scale(model_.model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		
		


		////Stencil
		/*glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);*/

		
		model_.MultipleDraw(sceneShader_, amount);

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

	void Model::OnEvent(const SDL_Event& event)
	{
		cam_.onEvent(event);
	}

	void Model::ProcessInput(float dt)
	{
		
		SDL_Event event;
		SDL_PollEvent(&event);
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		

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