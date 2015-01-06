#include <glm/glm.hpp>

class ShaderProgram;


class WaterSurface
{
public:
	WaterSurface();
	~WaterSurface();

	void draw(double time, glm::mat4x4);

private:
	glm::mat4x4 mvp;

	unsigned int vaoId;
	unsigned int vertexBuffer, uvBuffer;
	unsigned int uniform_mvp;
	unsigned int texSurface, texHeight;

	unsigned int shaderProgramId;

	void initGraphics();
	void destructGraphics();

	void createShader();
	void createSquare();
	void createTextures();
};