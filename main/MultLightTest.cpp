#include <fstream>
#include <iostream>
#include <sstream>
#include <gl/glew.h>
#include <Windows.h>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/mat4x4.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace gpr5300
{
    class TextureM
    {
    public:

        int width, height, nrChannels;
        unsigned int texture;
        unsigned char* data;
        void LoadTexture(const char* texturePath)
        {

            data = stbi_load(texturePath,
                &width,
                &height,
                &nrChannels,
                0);

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

            glGenerateMipmap(GL_TEXTURE_2D);

            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    };

    class MeshM
    {
    public:
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;

        GLuint vao_ = 0;
        GLuint vbo_[3] = {};
        GLuint ebo_ = 0;

        void BindMesh(auto& vertices, auto& normals, auto& indices)
        {
            //VAO
            glGenVertexArrays(1, &vao_);
            glBindVertexArray(vao_);

            //VBO
            glGenBuffers(1, &vbo_[0]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glGenBuffers(1, &vbo_[1]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);

            glGenBuffers(1, &vbo_[2]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(2);

            //EBO
            glGenBuffers(1, &ebo_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
        }
        float vertices[72] =
        {
            //Front
            -0.5f, -0.5f, -0.5f, //0
            -0.5f, 0.5f, -0.5f,
             0.5f, 0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

             //Back
             -0.5f, -0.5f, 0.5, //4
             -0.5f, 0.5f, 0.5,
              0.5f, 0.5f, 0.5,
              0.5f, -0.5f, 0.5,

              //Right
              0.5f, -0.5f, -0.5f, //8
              0.5f, 0.5f, -0.5f,
              0.5f, 0.5f, 0.5,
              0.5f, -0.5f, 0.5,

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
        float normals[72] =
        {
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, -1.0f,0.0f,
            0.0f, -1.0f,0.0f,
            0.0f, -1.0f,0.0f,
            0.0f, -1.0f,0.0f,

        };

        //pas d'indices entre les faces
        unsigned int indices[36] =
        {

            /* 0, 1, 3,
             1, 2, 3*/

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
        float texCoords[8 * 6] =
        {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

           1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
        };
    };

    class PipelineM
    {
    public:
        GLuint vertexShader_ = 0;
        GLuint fragmentShader_ = 0;
        GLuint pipeline_ = 0;

        void SetMatrix4(std::string_view uniform, glm::mat4& matrix4) const //automatise les uniform matrix
        {
            const int uniformLocation = glGetUniformLocation(pipeline_, uniform.data());
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix4));
        }

        void SetVector3(std::string_view uniform, float x, float y, float z)
        {
            int uniformLocation = glGetUniformLocation(pipeline_, uniform.data());
            glUniform3f(uniformLocation, x, y, z);
        }
        void SetVector3(const std::string& uniform, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(pipeline_, uniform.c_str()), 1, &value[0]);
        }

        void SetFloat(const std::string& uniform, float value) const
        {
            glUniform1f(glGetUniformLocation(pipeline_, uniform.c_str()), value);
        }

        void setInt(const std::string& uniform, int value) const
        {
            glUniform1i(glGetUniformLocation(pipeline_, uniform.c_str()), value);
        }

        void CreateShader(const char* vertexPath, const char* fragmentPath)
        {
            //Load shaders
            const auto vertexContent = LoadFile(vertexPath);
            const auto* ptr = vertexContent.data();
            vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader_, 1, &ptr, nullptr);
            glCompileShader(vertexShader_);
            //Check success status of cubePipeline compilation
            GLint success;
            glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                std::cerr << "Error while loading vertex cubePipeline\n";
            }
            const auto fragmentContent = LoadFile(fragmentPath);
            ptr = fragmentContent.data();
            fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader_, 1, &ptr, nullptr);
            glCompileShader(fragmentShader_);
            //Check success status of cubePipeline compilation

            glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                std::cerr << "Error while loading fragment cubePipeline\n";
            }
            //Load program/cubePipeline
            pipeline_ = glCreateProgram();
            glAttachShader(pipeline_, vertexShader_);
            glAttachShader(pipeline_, fragmentShader_);
            glLinkProgram(pipeline_);
            //Check if cubePipeline meshprogram was linked correctly

            glGetProgramiv(pipeline_, GL_LINK_STATUS, &success);
            if (!success)
            {
                std::cerr << "Error while linking cubePipeline program\n";
            }
        }
    };

    class Mulight final : public Scene
    {
    public:
        void Begin() override;
        void End() override;
        void Update(float dt) override;

        constexpr static glm::vec3 Cross(glm::vec3 v1, glm::vec3 v2)
        {
            return { (v1.y * v2.z) - (v1.z * v2.y),(v1.z * v2.x) - (v1.x * v2.z),
                (v1.x * v2.y) - (v1.y * v2.x) };
        }

        TextureM texture1;
        TextureM texture2;
        MeshM cubeMesh;
        PipelineM cubePipeline;

        int mouseX, mouseY;
        float yaw, pitch;

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraLeft = glm::cross(cameraUp, cameraFront);
        glm::vec3 direction = { 0.0f,0.0f,0.0f };
        const float sensitivity = 0.01f;

        glm::vec3 cubePositions[10] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        void processCameraInput(float dt)
        {
          

            const float cameraSpeed = 1.00f * dt; // adjust accordingly
            constexpr int keyDown = 0x8000;
            if (GetKeyState('W') & keyDown)
                cameraPos += cameraSpeed * cameraFront;
            if (GetKeyState('S') & keyDown)
                cameraPos -= cameraSpeed * cameraFront;
            if (GetKeyState('A') & keyDown)
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (GetKeyState('D') & keyDown)
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
    private:
        float tt = 0.0f;
    };

    void Mulight::Begin()
    {
        glEnable(GL_DEPTH_TEST);
        //Texture
        texture1.LoadTexture("data/Textures/iceice.jpg");
        texture2.LoadTexture("data/Textures/Ice_SPEC.jpg");

        //Mesh
        cubeMesh.BindMesh(cubeMesh.vertices, cubeMesh.normals, cubeMesh.indices);

        //Shaders
        cubePipeline.CreateShader("data/shaders/hello_triangle/MultLightTest.vert", "data/shaders/hello_triangle/MultLightTest.frag");
    }

    void Mulight::End()
    {
        //Unload program/cubePipeline
        glDeleteProgram(cubePipeline.pipeline_);

        glDeleteShader(cubePipeline.vertexShader_);
        glDeleteShader(cubePipeline.fragmentShader_);
        glDeleteTextures(1, &texture1.texture);

        glDeleteVertexArrays(1, &cubeMesh.vao_);

    }

    void Mulight::Update(float dt)
    {
        tt += dt;
        processCameraInput(dt);

        //Draw program
        glUseProgram(cubePipeline.pipeline_);
        cubePipeline.setInt("material.diffuse", 0);
        cubePipeline.setInt("material.specular", 0.5);//Begin?

        cubePipeline.SetVector3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z); //for specular
        cubePipeline.SetFloat("material.shininess", 32.0f);

        // directional light
        cubePipeline.SetVector3("dirLight.direction", -0.0f, -1.0f, -0.0f);
        cubePipeline.SetVector3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        cubePipeline.SetVector3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        cubePipeline.SetVector3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        //point light 1
        cubePipeline.SetVector3("pointLights[0].position", 0.0f, 0.0f, 1.0f);
        cubePipeline.SetVector3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubePipeline.SetVector3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f); // darken diffuse light a bit
        cubePipeline.SetVector3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubePipeline.SetFloat("pointLights[0].constant", 1.0f);
        cubePipeline.SetFloat("pointLights[0].linear", 0.09f);
        cubePipeline.SetFloat("pointLights[0].quadratic", 0.032f);
        //add multiple point light here

        //cameraFront = glm::cross(cameraLeft, cameraUp);
        
        // spotLight camera
        cubePipeline.SetVector3("spotLight.position", cameraPos);
        cubePipeline.SetVector3("spotLight.direction", cameraFront);
        cubePipeline.SetVector3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cubePipeline.SetVector3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cubePipeline.SetVector3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubePipeline.SetFloat("spotLight.constant", 1.0f);
        cubePipeline.SetFloat("spotLight.linear", 0.09f);
        cubePipeline.SetFloat("spotLight.quadratic", 0.032f);
        cubePipeline.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        cubePipeline.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        //add multiple spotlight here !! add the same as for point lights

        cubeMesh.model = glm::rotate(cubeMesh.model, dt * glm::radians(-50.0f), glm::vec3(1.0f, 1.0f, 1.0f)); //locale coordinate
        cubeMesh.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //camera
        cubeMesh.projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

        cubePipeline.SetMatrix4("model", cubeMesh.model);
        cubePipeline.SetMatrix4("view", cubeMesh.view);
        cubePipeline.SetMatrix4("projection", cubeMesh.projection);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1.texture);

        //bind specular map
        glActiveTexture(GL_TEXTURE1);

        glBindTexture(GL_TEXTURE_2D, texture2.texture);

        glBindVertexArray(cubeMesh.vao_);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubePipeline.SetMatrix4("model", model);

            glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0); //draw cube
        }
    }
}
int main(int argc, char** argv)
{
    gpr5300::Mulight scene;
    gpr5300::Engine engine(&scene);
    engine.Run();

    return EXIT_SUCCESS;
}