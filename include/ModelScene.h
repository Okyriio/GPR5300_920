#pragma once
#include "frameBuffer.h"
#include "ModelObject.h"
#include "scene.h"
#include "shader.h"
#include <CubeMap.h>

#include "Camera.h"


namespace gpr5300
{
	class Model final : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;
		void ProcessInput(float dt);
		void mouse_callback( double xposIn, double yposIn);
		
	private:
		glm::mat4* modelMatrices;
		unsigned int amount = 100;
		float tt_ = 0;
		int numOfModels = 0;
		Shader skyShader_;
		Shader sceneShader_;
		Shader simpleColorShader_;
		Shader screenShader_;
		std::vector<ModelObject> models_;
		ModelObject model_;
		FrameBuffer frameBuffer_;
		CubeMap sky_;
		Camera cam_ {glm::vec3(0.0f,0.0f,3.0f)};
		
	};


}
