#pragma once
#include "frameBuffer.h"
#include "ModelObject.h"
#include "scene.h"
#include "shader.h"
#include <CubeMap.h>

#include "Camera.h"
#include "ShadowBuffer.h"


namespace gpr5300
{
	class Model final : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;
		void ProcessInput(float dt);
		
		void OnEvent(const SDL_Event& event) override;
		

	private:

	
		glm::mat4* modelMatrices;
		unsigned int amount = 5;
		float tt_ = 0;
		int numOfModels = 0;
		Shader skyShader_;
		Shader sceneShader_;
		Shader simpleColorShader_;
		Shader shadowShader_;
		std::vector<ModelObject> models_;
		
		ModelObject model_;
		FrameBuffer frameBuffer_;
		ShadowBuffer shadowBuffer_;
		CubeMap sky_;
		Camera cam_ {glm::vec3(0.0f,0.0f,3.0f)};
		
	};


}
