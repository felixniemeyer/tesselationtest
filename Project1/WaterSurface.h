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
	float tessLevel;

	unsigned int vaoId;
	unsigned int vertexBuffer, uvBuffer;
	unsigned int uniform_mvp, uniform_tessLevel;
	unsigned int texSurface, texHeight;

	unsigned int shaderProgramId, wireframeShaderProgramId;

	void initGraphics();
	void destructGraphics();

	void createShader();
	void createSquare();
	void createTextures();

	void drawArray();
};