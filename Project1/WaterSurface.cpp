#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>


#include "WaterSurface.h"
#include "ShaderProgram.h"

WaterSurface::WaterSurface()
{
	initGraphics();
}

void 
WaterSurface::initGraphics()
{
	createSquare();
	createShader();
	createTextures();
	uniform_mvp = glGetUniformLocation(shaderProgramId, "mvp");
	uniform_tessLevel = glGetUniformLocation(shaderProgramId, "tessLevel");
}

void
WaterSurface::destructGraphics()
{
	glDeleteProgram(shaderProgramId);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vaoId);
}


namespace helpers
{
	void checkForGlError()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << gluErrorString(err) << "\n";
		}
	}

	void readShaderCodeFromFile(std::string &code, const char* filename)
	{
		std::cout << "reading source from " << filename << "\n";
		std::ifstream codeStream(filename, std::ios::in);
		if (codeStream.is_open())
		{
			std::string Line = "";
			while (getline(codeStream, Line))
				code += "\n" + Line;
			codeStream.close();
		}
		std::cout << "source =\n" << code << "\n";
	}

	void compileShader(GLuint shaderId, const char* codePointer)
	{
		std::cout << "compiling shader program\n";
		
		GLint result = GL_FALSE;
		int logLength;

		glShaderSource(shaderId, 1, &codePointer, NULL);
		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorMessage(logLength);
		glGetShaderInfoLog(shaderId, logLength, NULL, &errorMessage[0]);
		std::cout << &errorMessage[0];
	}

	void linkProgram(GLuint shaderProgramId, GLuint shaderIds[], GLuint numberOfShaders)
	{
		GLint result = GL_FALSE;
		int logLength;

		std::cout << "linking shader program\n";

		for (int i = 0; i < numberOfShaders; i++)
			glAttachShader(shaderProgramId, shaderIds[i]);
		glLinkProgram(shaderProgramId);

		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
		glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorMessage(logLength ? logLength : int(1));
		glGetProgramInfoLog(shaderProgramId, logLength, NULL, &errorMessage[0]);
		std::cout << &errorMessage[0];
		checkForGlError();
	}
}

void 
WaterSurface::createShader()
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);

	std::string vsCode, fsCode, tcsCode, tesCode;
	helpers::readShaderCodeFromFile(vsCode, "./shader/water_surface.vert");
	helpers::compileShader(vs, vsCode.c_str());
	helpers::readShaderCodeFromFile(fsCode, "./shader/water_surface.frag");
	helpers::compileShader(fs, fsCode.c_str());
	helpers::readShaderCodeFromFile(tcsCode, "./shader/water_surface.tcs");
	helpers::compileShader(tcs, tcsCode.c_str());
	helpers::readShaderCodeFromFile(tesCode, "./shader/water_surface.tes");
	helpers::compileShader(tes, tesCode.c_str());


	shaderProgramId = glCreateProgram();
	GLuint shaderIds[] = { vs, fs, tcs, tes }, shaderCount = 4;
	helpers::linkProgram(shaderProgramId,  shaderIds, shaderCount);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(tcs);
	glDeleteShader(tes);
	helpers::checkForGlError();
}

void
WaterSurface::createSquare()
{
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	float vertices[] = {
		-1, -1, 0,
		-1, 1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, -1, 0,
		1, 1, 0
	};
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); //reminder for me: first parameter defines the attribute index: shader: "(layout = index) in vec3 ..."
	glEnableVertexAttribArray(0);

	float verticeUVs[] = {
		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 0,
		1, 1
	};
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), verticeUVs, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(0);

}

void
WaterSurface::createTextures()
{
	texSurface = SOIL_load_OGL_texture(
		"./textures/water.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glBindTexture(GL_TEXTURE_2D, texSurface);
}

void 
WaterSurface::draw(double time, glm::mat4x4 viewProjection)
{
	mvp = viewProjection;// glm::rotate(viewProjection, (float)time*0.4f, glm::tvec3<float>(glm::cos(time*0.2123), glm::sin(time*0.2123), 0));
	GLfloat tl = glm::sin(time*0.4f) * 30 + 31;
	glUseProgram(shaderProgramId);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, &mvp[0][0]);
	glUniform1f(uniform_tessLevel, tl);

	glBindVertexArray(vaoId);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawArrays(GL_PATCHES, 0, 6);
//	helpers::checkForGlError();
	glBindVertexArray(0);

}

WaterSurface::~WaterSurface()
{
	destructGraphics();
}