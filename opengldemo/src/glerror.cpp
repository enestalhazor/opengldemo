#include "glerror.hpp"


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] -> " << error << function << " " << std::endl << file << std::endl << "error line: " << line << std::endl;
		return false;
	}
	return true;

}
