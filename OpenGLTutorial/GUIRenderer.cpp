#include "GUIRenderer.h"
#include "Util.cpp"

GUIRenderer::GUIRenderer() {}

GUIRenderer::~GUIRenderer() {}

void GUIRenderer::init()
{
	Util util;

	programId = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	string fileText = util.readFile("fontVertex.shader");
	const GLchar * shaderSource = fileText.c_str();

	glShaderSource(vertexShader, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);

	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		Util::printShaderLog(vertexShader);
		return;
	}

	glAttachShader(programId, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	fileText = util.readFile("fontFragment.shader");
	shaderSource = fileText.c_str();

	glShaderSource(fragmentShader, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		return;
	}

	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", programId);
		return;
	}

	vertexPositionLocation = glGetAttribLocation(programId, "position");

	if (vertexPositionLocation == -1)
	{
		printf("position is not a valid glsl program variable!\n");
		return;
	}

	uvLocation = glGetAttribLocation(programId, "textureCoords");

	if (uvLocation == -1)
	{
		printf("textureCoords is not a valid glsl program variable!\n");
		return;
	}

	textureSamplerLocation = glGetUniformLocation(programId, "textureSampler");

	if (textureSamplerLocation == -1)
	{
		printf("textureSampler is not a valid glsl variable!\n");
		return;
	}
}

void GUIRenderer::renderText(GUIText * t, glm::vec3 textPosition)
{
	glUseProgram(programId);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glm::vec3 translation = textPosition;

	glUniformMatrix4fv(translationLocation, 1, GL_FALSE, glm::value_ptr(translation));
	glEnableVertexAttribArray(vertexPositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, t->vertexBufferObject);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(uvLocation);
	glBindBuffer(GL_ARRAY_BUFFER, t->uvBufferObject);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, t->font->texture->textureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t->indexBufferObject);

	glDrawElements(GL_TRIANGLES, t->indexCount, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(vertexPositionLocation);

	glUseProgram(NULL);
}