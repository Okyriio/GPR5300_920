#include "Error.h"

#include <iostream>
#include <GL/glew.h>



namespace gpr5300
{

void CheckError(std::string_view file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        // Process/log the error.
        switch (err)
        {
        case GL_INVALID_ENUM:
                std::cout << "File: {} Line: {} OpenGL: GL_INVALID_ENUM" << file<< line;
                break;
            case GL_INVALID_VALUE:
                std::cout << "File : {} Line: {} OpenGL: GL_INVALID_VALUE" << file << line;
                break;
            case GL_INVALID_OPERATION:
                std::cout << "File: {} Line: {} OpenGL: GL_INVALID_OPERATION" << file << line;
                break;
            case GL_STACK_OVERFLOW:
                std::cout << "File: {} Line: {} OpenGL: GL_STACK_OVERFLOW" << file << line;
                break;
            case GL_STACK_UNDERFLOW:
                std::cout << "File: {} Line: {} OpenGL: GL_STACK_UNDERFLOW" << file << line;
                break;
            case GL_OUT_OF_MEMORY:
                std::cout << "File: {} Line: {} OpenGL: GL_OUT_OF_MEMORY"<<  file << line;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cout << "File: {} Line: {} OpenGL: GL_INVALID_FRAMEBUFFER_OPERATION" << file << line;
                break;
            case GL_CONTEXT_LOST:
                std::cout << "File: {} Line: {} OpenGL: GL_CONTEXT_LOST" << file << line;
                break;
            case GL_TABLE_TOO_LARGE:
                std::cout << "File: {} Line: {} OpenGL: GL_TABLE_TOO_LARGE" << file << line;
                break;
            default:
                break;
        }
    }
}
}